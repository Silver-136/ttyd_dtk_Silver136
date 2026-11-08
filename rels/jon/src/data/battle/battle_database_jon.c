// Pit of 100 Trials (jon): battle database: the enemy groups (BattleSetup) and stages of the Pit floors.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "jon.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "driver/npcdrv.h"

extern BattleUnitKind unit_hyper_jyugem;
extern BattleUnitKind unit_jyugem;

BattleStageObjectData jon_btlobject_data[8];
BattleStageData jon_btlstage_data[4];
BattleUnitSetup btlparty_jon_jon_100_01_off_1[1];
BattleGroupSetup btlgrp_jon_jon_100_01_off_1;
BattleUnitSetup btlparty_jon_jon_01_01_off_1[1];
BattleGroupSetup btlgrp_jon_jon_01_01_off_1;
BattleUnitSetup btlparty_jon_jon_01_01_off_2[2];
BattleGroupSetup btlgrp_jon_jon_01_01_off_2;
BattleUnitSetup btlparty_jon_jon_01_01_off_3[2];
BattleGroupSetup btlgrp_jon_jon_01_01_off_3;
BattleUnitSetup btlparty_jon_jon_01_02_off_1[5];
BattleGroupSetup btlgrp_jon_jon_01_02_off_1;
BattleUnitSetup btlparty_jon_jon_01_02_off_2[2];
BattleGroupSetup btlgrp_jon_jon_01_02_off_2;
BattleUnitSetup btlparty_jon_jon_01_02_off_3[5];
BattleGroupSetup btlgrp_jon_jon_01_02_off_3;
BattleUnitSetup btlparty_jon_jon_01_03_off_1[4];
BattleGroupSetup btlgrp_jon_jon_01_03_off_1;
BattleUnitSetup btlparty_jon_jon_01_03_off_2[2];
BattleGroupSetup btlgrp_jon_jon_01_03_off_2;
BattleUnitSetup btlparty_jon_jon_01_03_off_3[4];
BattleGroupSetup btlgrp_jon_jon_01_03_off_3;
BattleUnitSetup btlparty_jon_jon_01_04_off_1[4];
BattleGroupSetup btlgrp_jon_jon_01_04_off_1;
BattleUnitSetup btlparty_jon_jon_01_04_off_2[2];
BattleGroupSetup btlgrp_jon_jon_01_04_off_2;
BattleUnitSetup btlparty_jon_jon_01_04_off_3[3];
BattleGroupSetup btlgrp_jon_jon_01_04_off_3;
BattleUnitSetup btlparty_jon_jon_01_05_off_1[5];
BattleGroupSetup btlgrp_jon_jon_01_05_off_1;
BattleUnitSetup btlparty_jon_jon_01_05_off_2[3];
BattleGroupSetup btlgrp_jon_jon_01_05_off_2;
BattleUnitSetup btlparty_jon_jon_01_05_off_3[4];
BattleGroupSetup btlgrp_jon_jon_01_05_off_3;
BattleUnitSetup btlparty_jon_jon_11_01_off_1[2];
BattleGroupSetup btlgrp_jon_jon_11_01_off_1;
BattleUnitSetup btlparty_jon_jon_11_01_off_2[3];
BattleGroupSetup btlgrp_jon_jon_11_01_off_2;
BattleUnitSetup btlparty_jon_jon_11_01_off_3[3];
BattleGroupSetup btlgrp_jon_jon_11_01_off_3;
BattleUnitSetup btlparty_jon_jon_11_02_off_1[4];
BattleGroupSetup btlgrp_jon_jon_11_02_off_1;
BattleUnitSetup btlparty_jon_jon_11_02_off_2[3];
BattleGroupSetup btlgrp_jon_jon_11_02_off_2;
BattleUnitSetup btlparty_jon_jon_11_02_off_3[3];
BattleGroupSetup btlgrp_jon_jon_11_02_off_3;
BattleUnitSetup btlparty_jon_jon_11_03_off_1[4];
BattleGroupSetup btlgrp_jon_jon_11_03_off_1;
BattleUnitSetup btlparty_jon_jon_11_03_off_2[2];
BattleGroupSetup btlgrp_jon_jon_11_03_off_2;
BattleUnitSetup btlparty_jon_jon_11_03_off_3[4];
BattleGroupSetup btlgrp_jon_jon_11_03_off_3;
BattleUnitSetup btlparty_jon_jon_11_04_off_1[3];
BattleGroupSetup btlgrp_jon_jon_11_04_off_1;
BattleUnitSetup btlparty_jon_jon_11_04_off_2[3];
BattleGroupSetup btlgrp_jon_jon_11_04_off_2;
BattleUnitSetup btlparty_jon_jon_11_04_off_3[2];
BattleGroupSetup btlgrp_jon_jon_11_04_off_3;
BattleUnitSetup btlparty_jon_jon_11_05_off_1[4];
BattleGroupSetup btlgrp_jon_jon_11_05_off_1;
BattleUnitSetup btlparty_jon_jon_11_05_off_2[2];
BattleGroupSetup btlgrp_jon_jon_11_05_off_2;
BattleUnitSetup btlparty_jon_jon_11_05_off_3[4];
BattleGroupSetup btlgrp_jon_jon_11_05_off_3;
BattleUnitSetup btlparty_jon_jon_21_01_off_1[3];
BattleGroupSetup btlgrp_jon_jon_21_01_off_1;
BattleUnitSetup btlparty_jon_jon_21_01_off_2[4];
BattleGroupSetup btlgrp_jon_jon_21_01_off_2;
BattleUnitSetup btlparty_jon_jon_21_01_off_3[4];
BattleGroupSetup btlgrp_jon_jon_21_01_off_3;
BattleUnitSetup btlparty_jon_jon_21_02_off_1[5];
BattleGroupSetup btlgrp_jon_jon_21_02_off_1;
BattleUnitSetup btlparty_jon_jon_21_02_off_2[2];
BattleGroupSetup btlgrp_jon_jon_21_02_off_2;
BattleUnitSetup btlparty_jon_jon_21_02_off_3[5];
BattleGroupSetup btlgrp_jon_jon_21_02_off_3;
BattleUnitSetup btlparty_jon_jon_21_03_off_1[4];
BattleGroupSetup btlgrp_jon_jon_21_03_off_1;
BattleUnitSetup btlparty_jon_jon_21_03_off_2[2];
BattleGroupSetup btlgrp_jon_jon_21_03_off_2;
BattleUnitSetup btlparty_jon_jon_21_03_off_3[4];
BattleGroupSetup btlgrp_jon_jon_21_03_off_3;
BattleUnitSetup btlparty_jon_jon_21_04_off_1[4];
BattleGroupSetup btlgrp_jon_jon_21_04_off_1;
BattleUnitSetup btlparty_jon_jon_21_04_off_2[3];
BattleGroupSetup btlgrp_jon_jon_21_04_off_2;
BattleUnitSetup btlparty_jon_jon_21_04_off_3[4];
BattleGroupSetup btlgrp_jon_jon_21_04_off_3;
BattleUnitSetup btlparty_jon_jon_21_05_off_1[4];
BattleGroupSetup btlgrp_jon_jon_21_05_off_1;
BattleUnitSetup btlparty_jon_jon_21_05_off_2[3];
BattleGroupSetup btlgrp_jon_jon_21_05_off_2;
BattleUnitSetup btlparty_jon_jon_21_05_off_3[4];
BattleGroupSetup btlgrp_jon_jon_21_05_off_3;
BattleUnitSetup btlparty_jon_jon_31_01_off_1[2];
BattleGroupSetup btlgrp_jon_jon_31_01_off_1;
BattleUnitSetup btlparty_jon_jon_31_01_off_2[3];
BattleGroupSetup btlgrp_jon_jon_31_01_off_2;
BattleUnitSetup btlparty_jon_jon_31_01_off_3[3];
BattleGroupSetup btlgrp_jon_jon_31_01_off_3;
BattleUnitSetup btlparty_jon_jon_31_02_off_1[3];
BattleGroupSetup btlgrp_jon_jon_31_02_off_1;
BattleUnitSetup btlparty_jon_jon_31_02_off_2[3];
BattleGroupSetup btlgrp_jon_jon_31_02_off_2;
BattleUnitSetup btlparty_jon_jon_31_02_off_3[3];
BattleGroupSetup btlgrp_jon_jon_31_02_off_3;
BattleUnitSetup btlparty_jon_jon_31_03_off_1[4];
BattleGroupSetup btlgrp_jon_jon_31_03_off_1;
BattleUnitSetup btlparty_jon_jon_31_03_off_2[4];
BattleGroupSetup btlgrp_jon_jon_31_03_off_2;
BattleUnitSetup btlparty_jon_jon_31_03_off_3[2];
BattleGroupSetup btlgrp_jon_jon_31_03_off_3;
BattleUnitSetup btlparty_jon_jon_31_04_off_1[4];
BattleGroupSetup btlgrp_jon_jon_31_04_off_1;
BattleUnitSetup btlparty_jon_jon_31_04_off_2[3];
BattleGroupSetup btlgrp_jon_jon_31_04_off_2;
BattleUnitSetup btlparty_jon_jon_31_04_off_3[3];
BattleGroupSetup btlgrp_jon_jon_31_04_off_3;
BattleUnitSetup btlparty_jon_jon_31_05_off_1[5];
BattleGroupSetup btlgrp_jon_jon_31_05_off_1;
BattleUnitSetup btlparty_jon_jon_31_05_off_2[3];
BattleGroupSetup btlgrp_jon_jon_31_05_off_2;
BattleUnitSetup btlparty_jon_jon_31_05_off_3[2];
BattleGroupSetup btlgrp_jon_jon_31_05_off_3;
BattleUnitSetup btlparty_jon_jon_41_01_off_1[4];
BattleGroupSetup btlgrp_jon_jon_41_01_off_1;
BattleUnitSetup btlparty_jon_jon_41_01_off_2[3];
BattleGroupSetup btlgrp_jon_jon_41_01_off_2;
BattleUnitSetup btlparty_jon_jon_41_01_off_3[3];
BattleGroupSetup btlgrp_jon_jon_41_01_off_3;
BattleUnitSetup btlparty_jon_jon_41_02_off_1[4];
BattleGroupSetup btlgrp_jon_jon_41_02_off_1;
BattleUnitSetup btlparty_jon_jon_41_02_off_2[2];
BattleGroupSetup btlgrp_jon_jon_41_02_off_2;
BattleUnitSetup btlparty_jon_jon_41_02_off_3[3];
BattleGroupSetup btlgrp_jon_jon_41_02_off_3;
BattleUnitSetup btlparty_jon_jon_41_03_off_1[3];
BattleGroupSetup btlgrp_jon_jon_41_03_off_1;
BattleUnitSetup btlparty_jon_jon_41_03_off_2[3];
BattleGroupSetup btlgrp_jon_jon_41_03_off_2;
BattleUnitSetup btlparty_jon_jon_41_03_off_3[4];
BattleGroupSetup btlgrp_jon_jon_41_03_off_3;
BattleUnitSetup btlparty_jon_jon_41_04_off_1[4];
BattleGroupSetup btlgrp_jon_jon_41_04_off_1;
BattleUnitSetup btlparty_jon_jon_41_04_off_2[4];
BattleGroupSetup btlgrp_jon_jon_41_04_off_2;
BattleUnitSetup btlparty_jon_jon_41_04_off_3[2];
BattleGroupSetup btlgrp_jon_jon_41_04_off_3;
BattleUnitSetup btlparty_jon_jon_41_05_off_1[2];
BattleGroupSetup btlgrp_jon_jon_41_05_off_1;
BattleUnitSetup btlparty_jon_jon_41_05_off_2[4];
BattleGroupSetup btlgrp_jon_jon_41_05_off_2;
BattleUnitSetup btlparty_jon_jon_41_05_off_3[4];
BattleGroupSetup btlgrp_jon_jon_41_05_off_3;
BattleUnitSetup btlparty_jon_jon_51_01_off_1[3];
BattleGroupSetup btlgrp_jon_jon_51_01_off_1;
BattleUnitSetup btlparty_jon_jon_51_01_off_2[3];
BattleGroupSetup btlgrp_jon_jon_51_01_off_2;
BattleUnitSetup btlparty_jon_jon_51_01_off_3[3];
BattleGroupSetup btlgrp_jon_jon_51_01_off_3;
BattleUnitSetup btlparty_jon_jon_51_02_off_1[4];
BattleGroupSetup btlgrp_jon_jon_51_02_off_1;
BattleUnitSetup btlparty_jon_jon_51_02_off_2[3];
BattleGroupSetup btlgrp_jon_jon_51_02_off_2;
BattleUnitSetup btlparty_jon_jon_51_02_off_3[2];
BattleGroupSetup btlgrp_jon_jon_51_02_off_3;
BattleUnitSetup btlparty_jon_jon_51_03_off_1[4];
BattleGroupSetup btlgrp_jon_jon_51_03_off_1;
BattleUnitSetup btlparty_jon_jon_51_03_off_2[3];
BattleGroupSetup btlgrp_jon_jon_51_03_off_2;
BattleUnitSetup btlparty_jon_jon_51_03_off_3[3];
BattleGroupSetup btlgrp_jon_jon_51_03_off_3;
BattleUnitSetup btlparty_jon_jon_51_04_off_1[3];
BattleGroupSetup btlgrp_jon_jon_51_04_off_1;
BattleUnitSetup btlparty_jon_jon_51_04_off_2[3];
BattleGroupSetup btlgrp_jon_jon_51_04_off_2;
BattleUnitSetup btlparty_jon_jon_51_04_off_3[3];
BattleGroupSetup btlgrp_jon_jon_51_04_off_3;
BattleUnitSetup btlparty_jon_jon_51_05_off_1[4];
BattleGroupSetup btlgrp_jon_jon_51_05_off_1;
BattleUnitSetup btlparty_jon_jon_51_05_off_2[3];
BattleGroupSetup btlgrp_jon_jon_51_05_off_2;
BattleUnitSetup btlparty_jon_jon_51_05_off_3[3];
BattleGroupSetup btlgrp_jon_jon_51_05_off_3;
BattleUnitSetup btlparty_jon_jon_61_01_off_1[3];
BattleGroupSetup btlgrp_jon_jon_61_01_off_1;
BattleUnitSetup btlparty_jon_jon_61_01_off_2[3];
BattleGroupSetup btlgrp_jon_jon_61_01_off_2;
BattleUnitSetup btlparty_jon_jon_61_01_off_3[2];
BattleGroupSetup btlgrp_jon_jon_61_01_off_3;
BattleUnitSetup btlparty_jon_jon_61_02_off_1[4];
BattleGroupSetup btlgrp_jon_jon_61_02_off_1;
BattleUnitSetup btlparty_jon_jon_61_02_off_2[2];
BattleGroupSetup btlgrp_jon_jon_61_02_off_2;
BattleUnitSetup btlparty_jon_jon_61_02_off_3[3];
BattleGroupSetup btlgrp_jon_jon_61_02_off_3;
BattleUnitSetup btlparty_jon_jon_61_03_off_1[3];
BattleGroupSetup btlgrp_jon_jon_61_03_off_1;
BattleUnitSetup btlparty_jon_jon_61_03_off_2[3];
BattleGroupSetup btlgrp_jon_jon_61_03_off_2;
BattleUnitSetup btlparty_jon_jon_61_03_off_3[3];
BattleGroupSetup btlgrp_jon_jon_61_03_off_3;
BattleUnitSetup btlparty_jon_jon_61_04_off_1[4];
BattleGroupSetup btlgrp_jon_jon_61_04_off_1;
BattleUnitSetup btlparty_jon_jon_61_04_off_2[3];
BattleGroupSetup btlgrp_jon_jon_61_04_off_2;
BattleUnitSetup btlparty_jon_jon_61_04_off_3[2];
BattleGroupSetup btlgrp_jon_jon_61_04_off_3;
BattleUnitSetup btlparty_jon_jon_61_05_off_1[3];
BattleGroupSetup btlgrp_jon_jon_61_05_off_1;
BattleUnitSetup btlparty_jon_jon_61_05_off_2[2];
BattleGroupSetup btlgrp_jon_jon_61_05_off_2;
BattleUnitSetup btlparty_jon_jon_61_05_off_3[2];
BattleGroupSetup btlgrp_jon_jon_61_05_off_3;
BattleUnitSetup btlparty_jon_jon_71_01_off_1[3];
BattleGroupSetup btlgrp_jon_jon_71_01_off_1;
BattleUnitSetup btlparty_jon_jon_71_01_off_2[3];
BattleGroupSetup btlgrp_jon_jon_71_01_off_2;
BattleUnitSetup btlparty_jon_jon_71_01_off_3[3];
BattleGroupSetup btlgrp_jon_jon_71_01_off_3;
BattleUnitSetup btlparty_jon_jon_71_02_off_1[3];
BattleGroupSetup btlgrp_jon_jon_71_02_off_1;
BattleUnitSetup btlparty_jon_jon_71_02_off_2[3];
BattleGroupSetup btlgrp_jon_jon_71_02_off_2;
BattleUnitSetup btlparty_jon_jon_71_02_off_3[3];
BattleGroupSetup btlgrp_jon_jon_71_02_off_3;
BattleUnitSetup btlparty_jon_jon_71_03_off_1[5];
BattleGroupSetup btlgrp_jon_jon_71_03_off_1;
BattleUnitSetup btlparty_jon_jon_71_03_off_2[4];
BattleGroupSetup btlgrp_jon_jon_71_03_off_2;
BattleUnitSetup btlparty_jon_jon_71_03_off_3[4];
BattleGroupSetup btlgrp_jon_jon_71_03_off_3;
BattleUnitSetup btlparty_jon_jon_71_04_off_1[4];
BattleGroupSetup btlgrp_jon_jon_71_04_off_1;
BattleUnitSetup btlparty_jon_jon_71_04_off_2[4];
BattleGroupSetup btlgrp_jon_jon_71_04_off_2;
BattleUnitSetup btlparty_jon_jon_71_04_off_3[4];
BattleGroupSetup btlgrp_jon_jon_71_04_off_3;
BattleUnitSetup btlparty_jon_jon_71_05_off_1[4];
BattleGroupSetup btlgrp_jon_jon_71_05_off_1;
BattleUnitSetup btlparty_jon_jon_71_05_off_2[3];
BattleGroupSetup btlgrp_jon_jon_71_05_off_2;
BattleUnitSetup btlparty_jon_jon_71_05_off_3[3];
BattleGroupSetup btlgrp_jon_jon_71_05_off_3;
BattleUnitSetup btlparty_jon_jon_81_01_off_1[4];
BattleGroupSetup btlgrp_jon_jon_81_01_off_1;
BattleUnitSetup btlparty_jon_jon_81_01_off_2[4];
BattleGroupSetup btlgrp_jon_jon_81_01_off_2;
BattleUnitSetup btlparty_jon_jon_81_01_off_3[3];
BattleGroupSetup btlgrp_jon_jon_81_01_off_3;
BattleUnitSetup btlparty_jon_jon_81_02_off_1[2];
BattleGroupSetup btlgrp_jon_jon_81_02_off_1;
BattleUnitSetup btlparty_jon_jon_81_02_off_2[4];
BattleGroupSetup btlgrp_jon_jon_81_02_off_2;
BattleUnitSetup btlparty_jon_jon_81_02_off_3[3];
BattleGroupSetup btlgrp_jon_jon_81_02_off_3;
BattleUnitSetup btlparty_jon_jon_81_03_off_1[4];
BattleGroupSetup btlgrp_jon_jon_81_03_off_1;
BattleUnitSetup btlparty_jon_jon_81_03_off_2[3];
BattleGroupSetup btlgrp_jon_jon_81_03_off_2;
BattleUnitSetup btlparty_jon_jon_81_03_off_3[3];
BattleGroupSetup btlgrp_jon_jon_81_03_off_3;
BattleUnitSetup btlparty_jon_jon_81_04_off_1[4];
BattleGroupSetup btlgrp_jon_jon_81_04_off_1;
BattleUnitSetup btlparty_jon_jon_81_04_off_2[3];
BattleGroupSetup btlgrp_jon_jon_81_04_off_2;
BattleUnitSetup btlparty_jon_jon_81_04_off_3[4];
BattleGroupSetup btlgrp_jon_jon_81_04_off_3;
BattleUnitSetup btlparty_jon_jon_81_05_off_1[4];
BattleGroupSetup btlgrp_jon_jon_81_05_off_1;
BattleUnitSetup btlparty_jon_jon_81_05_off_2[3];
BattleGroupSetup btlgrp_jon_jon_81_05_off_2;
BattleUnitSetup btlparty_jon_jon_81_05_off_3[5];
BattleGroupSetup btlgrp_jon_jon_81_05_off_3;
BattleUnitSetup btlparty_jon_jon_91_01_off_1[5];
BattleGroupSetup btlgrp_jon_jon_91_01_off_1;
BattleUnitSetup btlparty_jon_jon_91_01_off_2[3];
BattleGroupSetup btlgrp_jon_jon_91_01_off_2;
BattleUnitSetup btlparty_jon_jon_91_01_off_3[3];
BattleGroupSetup btlgrp_jon_jon_91_01_off_3;
BattleUnitSetup btlparty_jon_jon_91_02_off_1[4];
BattleGroupSetup btlgrp_jon_jon_91_02_off_1;
BattleUnitSetup btlparty_jon_jon_91_02_off_2[4];
BattleGroupSetup btlgrp_jon_jon_91_02_off_2;
BattleUnitSetup btlparty_jon_jon_91_02_off_3[4];
BattleGroupSetup btlgrp_jon_jon_91_02_off_3;
BattleUnitSetup btlparty_jon_jon_91_03_off_1[4];
BattleGroupSetup btlgrp_jon_jon_91_03_off_1;
BattleUnitSetup btlparty_jon_jon_91_03_off_2[3];
BattleGroupSetup btlgrp_jon_jon_91_03_off_2;
BattleUnitSetup btlparty_jon_jon_91_03_off_3[4];
BattleGroupSetup btlgrp_jon_jon_91_03_off_3;
BattleUnitSetup btlparty_jon_jon_91_04_off_1[5];
BattleGroupSetup btlgrp_jon_jon_91_04_off_1;
BattleUnitSetup btlparty_jon_jon_91_04_off_2[3];
BattleGroupSetup btlgrp_jon_jon_91_04_off_2;
BattleUnitSetup btlparty_jon_jon_91_04_off_3[4];
BattleGroupSetup btlgrp_jon_jon_91_04_off_3;
BattleUnitSetup btlparty_jon_jon_91_05_off_1[5];
BattleGroupSetup btlgrp_jon_jon_91_05_off_1;
BattleUnitSetup btlparty_jon_jon_91_05_off_2[5];
BattleGroupSetup btlgrp_jon_jon_91_05_off_2;
BattleUnitSetup btlparty_jon_jon_91_05_off_3[5];
BattleGroupSetup btlgrp_jon_jon_91_05_off_3;
NpcBattleSelect btlgrpsel_jon_btlno_jon_test1_off[2];
NpcBattleSelect btlgrpsel_jon_btlno_jon_01_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_02_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_03_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_04_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_05_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_11_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_12_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_13_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_14_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_15_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_21_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_22_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_23_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_24_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_25_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_31_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_32_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_33_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_34_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_35_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_41_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_42_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_43_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_44_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_45_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_51_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_52_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_53_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_54_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_55_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_61_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_62_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_63_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_64_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_65_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_71_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_72_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_73_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_74_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_75_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_81_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_82_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_83_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_84_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_85_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_91_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_92_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_93_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_94_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_95_off[4];
NpcBattleSelect btlgrpsel_jon_btlno_jon_100_zonbaba_off[2];
NpcSetupBattleInfo btlsetup_jon_tbl[53];
NpcTerritoryInfo btl_jon_setup_no_tbl[53];

/**
 * Back and front wall pieces of the Pit of 100 Trials battle stages; each stage in jon_btlstage_data uses a
 * pair of them.
 */
BattleStageObjectData jon_btlobject_data[] = {
    {"B_0", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"jon_0", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"B_1", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"jon_1", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"B_2", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"jon_2", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
    {"B_6", 2, STAGE_LAYER_B, {0}, 0, 60, {0}},
    {"jon_6", 2, STAGE_LAYER_C, {0}, 0, 60, {0}},
};

/**
 * The 4 battle stages of the Pit of 100 Trials, all built from the stage model stg_00_3, each with its own
 * pair of wall pieces from the object list above.
 */
BattleStageData jon_btlstage_data[] = {
    {
        "bti_01", // globalStageDataDir
        "stg_00_3", // specificStageDataDir
        2, // numObjects
        jon_btlobject_data, // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_00_3", // specificStageDataDir
        2, // numObjects
        &jon_btlobject_data[2], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_00_3", // specificStageDataDir
        2, // numObjects
        &jon_btlobject_data[4], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // globalStageDataDir
        "stg_00_3", // specificStageDataDir
        2, // numObjects
        &jon_btlobject_data[6], // objects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

/** Enemy lineup of formation 1 of BTLNO_JON_100_ZONBABA, in the Pit of 100 Trials, floor 100: Bonetail. */
BattleUnitSetup btlparty_jon_jon_100_01_off_1[] = {
    {
        &unit_boss_zonbaba, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {365.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_zonbaba, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_100_ZONBABA: the lineup above with the default heart and flower drops. Item drop
 * weights: 0 held, 0 random, 100 none.
 */
BattleGroupSetup btlgrp_jon_jon_100_01_off_1 = {
    1, // numEnemies
    btlparty_jon_jon_100_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_01, in the Pit of 100 Trials, floors 1, 6: Gloomba. */
BattleUnitSetup btlparty_jon_jon_01_01_off_1[] = {
    {
        &unit_yami_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_kuriboo, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_01: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_01_01_off_1 = {
    1, // numEnemies
    btlparty_jon_jon_01_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_JON_01, in the Pit of 100 Trials, floors 1, 6: Gloomba, Spania. */
BattleUnitSetup btlparty_jon_jon_01_01_off_2[] = {
    {
        &unit_yami_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_kuriboo, // itemDropTable
    },
    {
        &unit_hannya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_01: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_01_01_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_01_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_01, in the Pit of 100 Trials, floors 1, 6: Gloomba, Dull Bones.
 */
BattleUnitSetup btlparty_jon_jon_01_01_off_3[] = {
    {
        &unit_yami_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_kuriboo, // itemDropTable
    },
    {
        &unit_honenoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_01: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_01_01_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_01_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_02, in the Pit of 100 Trials, floors 2, 7: Spinia x5. */
BattleUnitSetup btlparty_jon_jon_01_02_off_1[] = {
    {
        &unit_hinnya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_02: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_01_02_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_01_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_JON_02, in the Pit of 100 Trials, floors 2, 7: Spinia, Gloomba. */
BattleUnitSetup btlparty_jon_jon_01_02_off_2[] = {
    {
        &unit_hinnya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_yami_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_kuriboo, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_02: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_01_02_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_01_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_02, in the Pit of 100 Trials, floors 2, 7: Spinia x3, Spania x2.
 */
BattleUnitSetup btlparty_jon_jon_01_02_off_3[] = {
    {
        &unit_hinnya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hannya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hinnya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hannya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hinnya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_02: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_01_02_off_3 = {
    5, // numEnemies
    btlparty_jon_jon_01_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_03, in the Pit of 100 Trials, floors 3, 8: Spania x4. */
BattleUnitSetup btlparty_jon_jon_01_03_off_1[] = {
    {
        &unit_hannya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_03: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_01_03_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_01_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_JON_03, in the Pit of 100 Trials, floors 3, 8: Spania, Gloomba. */
BattleUnitSetup btlparty_jon_jon_01_03_off_2[] = {
    {
        &unit_hannya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_yami_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_kuriboo, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_03: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_01_03_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_01_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_03, in the Pit of 100 Trials, floors 3, 8: Spania x2, Fuzzy x2.
 */
BattleUnitSetup btlparty_jon_jon_01_03_off_3[] = {
    {
        &unit_hannya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_hannya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_03: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_01_03_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_01_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_04, in the Pit of 100 Trials, floors 4, 9: Dull Bones x4. */
BattleUnitSetup btlparty_jon_jon_01_04_off_1[] = {
    {
        &unit_honenoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_04: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_01_04_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_01_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_04, in the Pit of 100 Trials, floors 4, 9: Dull Bones, Gloomba.
 */
BattleUnitSetup btlparty_jon_jon_01_04_off_2[] = {
    {
        &unit_honenoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_yami_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_kuriboo, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_04: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_01_04_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_01_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_04, in the Pit of 100 Trials, floors 4, 9: Dull Bones, Spinia x2.
 */
BattleUnitSetup btlparty_jon_jon_01_04_off_3[] = {
    {
        &unit_honenoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_hinnya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_04: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_01_04_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_01_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_05, in the Pit of 100 Trials, floors 5, 10: Fuzzy x5. */
BattleUnitSetup btlparty_jon_jon_01_05_off_1[] = {
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_05: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_01_05_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_01_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_JON_05, in the Pit of 100 Trials, floors 5, 10: Fuzzy x2, Gloomba. */
BattleUnitSetup btlparty_jon_jon_01_05_off_2[] = {
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_yami_kuriboo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_kuriboo, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_05: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_01_05_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_01_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_05, in the Pit of 100 Trials, floors 5, 10: Fuzzy x2, Dull Bones
 * x2.
 */
BattleUnitSetup btlparty_jon_jon_01_05_off_3[] = {
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_honenoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_honenoko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_05: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_01_05_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_01_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_11, in the Pit of 100 Trials, floors 11, 16: Paragloomba x2. */
BattleUnitSetup btlparty_jon_jon_11_01_off_1[] = {
    {
        &unit_yami_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_yami_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_11: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_11_01_off_1 = {
    2, // numEnemies
    btlparty_jon_jon_11_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_11, in the Pit of 100 Trials, floors 11, 16: Paragloomba, Pider
 * x2.
 */
BattleUnitSetup btlparty_jon_jon_11_01_off_2[] = {
    {
        &unit_yami_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_11: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_11_01_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_11_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_11, in the Pit of 100 Trials, floors 11, 16: Paragloomba, Pokey
 * x2.
 */
BattleUnitSetup btlparty_jon_jon_11_01_off_3[] = {
    {
        &unit_yami_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_sambo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_sambo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_11: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_11_01_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_11_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_12, in the Pit of 100 Trials, floors 12, 17: Cleft x4. */
BattleUnitSetup btlparty_jon_jon_11_02_off_1[] = {
    {
        &unit_monochrome_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_12: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_11_02_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_11_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_12, in the Pit of 100 Trials, floors 12, 17: Cleft x2,
 * Paragloomba.
 */
BattleUnitSetup btlparty_jon_jon_11_02_off_2[] = {
    {
        &unit_monochrome_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_yami_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_12: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_11_02_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_11_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_12, in the Pit of 100 Trials, floors 12, 17: Cleft, Pider, Dark
 * Puff.
 */
BattleUnitSetup btlparty_jon_jon_11_02_off_3[] = {
    {
        &unit_monochrome_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_12: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_11_02_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_11_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_13, in the Pit of 100 Trials, floors 13, 18: Pokey x4. */
BattleUnitSetup btlparty_jon_jon_11_03_off_1[] = {
    {
        &unit_sambo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_sambo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_sambo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_sambo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_13: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_11_03_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_11_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_JON_13, in the Pit of 100 Trials, floors 13, 18: Pokey, Dark Puff. */
BattleUnitSetup btlparty_jon_jon_11_03_off_2[] = {
    {
        &unit_sambo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_13: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_11_03_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_11_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_13, in the Pit of 100 Trials, floors 13, 18: Pokey x2, Pider x2.
 */
BattleUnitSetup btlparty_jon_jon_11_03_off_3[] = {
    {
        &unit_sambo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_sambo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_13: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_11_03_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_11_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_14, in the Pit of 100 Trials, floors 14, 19: Dark Puff x3. */
BattleUnitSetup btlparty_jon_jon_11_04_off_1[] = {
    {
        &unit_monochrome_kurokumorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_14: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_11_04_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_11_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_14, in the Pit of 100 Trials, floors 14, 19: Dark Puff x2, Pokey.
 */
BattleUnitSetup btlparty_jon_jon_11_04_off_2[] = {
    {
        &unit_monochrome_kurokumorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_sambo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_14: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_11_04_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_11_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_14, in the Pit of 100 Trials, floors 14, 19: Dark Puff,
 * Paragloomba.
 */
BattleUnitSetup btlparty_jon_jon_11_04_off_3[] = {
    {
        &unit_monochrome_kurokumorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_yami_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_14: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_11_04_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_11_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_15, in the Pit of 100 Trials, floors 15, 20: Pider x4. */
BattleUnitSetup btlparty_jon_jon_11_05_off_1[] = {
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_15: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_11_05_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_11_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_15, in the Pit of 100 Trials, floors 15, 20: Pider, Paragloomba.
 */
BattleUnitSetup btlparty_jon_jon_11_05_off_2[] = {
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_yami_patakuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_15: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_11_05_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_11_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_15, in the Pit of 100 Trials, floors 15, 20: Pider x2, Pokey x2.
 */
BattleUnitSetup btlparty_jon_jon_11_05_off_3[] = {
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_sambo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_piders, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_sambo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_15: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_11_05_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_11_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_21, in the Pit of 100 Trials, floors 21, 26: Spiky Gloomba x3. */
BattleUnitSetup btlparty_jon_jon_21_01_off_1[] = {
    {
        &unit_yami_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
    {
        &unit_yami_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
    {
        &unit_yami_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_21: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_21_01_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_21_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_21, in the Pit of 100 Trials, floors 21, 26: Spiky Gloomba x2,
 * Bob-omb, Bandit.
 */
BattleUnitSetup btlparty_jon_jon_21_01_off_2[] = {
    {
        &unit_yami_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
    {
        &unit_yami_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
    {
        &unit_bomhei, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_21: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_21_01_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_21_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_21, in the Pit of 100 Trials, floors 21, 26: Spiky Gloomba x2,
 * Boo x2.
 */
BattleUnitSetup btlparty_jon_jon_21_01_off_3[] = {
    {
        &unit_yami_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
    {
        &unit_yami_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_21: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_21_01_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_21_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_22, in the Pit of 100 Trials, floors 22, 27: Bandit x5. */
BattleUnitSetup btlparty_jon_jon_21_02_off_1[] = {
    {
        &unit_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_22: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_21_02_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_21_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_22, in the Pit of 100 Trials, floors 22, 27: Bandit, Spiky
 * Gloomba.
 */
BattleUnitSetup btlparty_jon_jon_21_02_off_2[] = {
    {
        &unit_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_yami_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_22: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_21_02_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_21_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_22, in the Pit of 100 Trials, floors 22, 27: Bandit x3, Boo x2.
 */
BattleUnitSetup btlparty_jon_jon_21_02_off_3[] = {
    {
        &unit_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_22: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_21_02_off_3 = {
    5, // numEnemies
    btlparty_jon_jon_21_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_23, in the Pit of 100 Trials, floors 23, 28: Lakitu x4. */
BattleUnitSetup btlparty_jon_jon_21_03_off_1[] = {
    {
        &unit_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_jyugem, // itemDropTable
    },
    {
        &unit_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_jyugem, // itemDropTable
    },
    {
        &unit_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_jyugem, // itemDropTable
    },
    {
        &unit_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_jyugem, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_23: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_21_03_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_21_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_23, in the Pit of 100 Trials, floors 23, 28: Lakitu, Spiky
 * Gloomba.
 */
BattleUnitSetup btlparty_jon_jon_21_03_off_2[] = {
    {
        &unit_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_jyugem, // itemDropTable
    },
    {
        &unit_yami_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_23: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_21_03_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_21_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_23, in the Pit of 100 Trials, floors 23, 28: Lakitu x2, Bob-omb
 * x2.
 */
BattleUnitSetup btlparty_jon_jon_21_03_off_3[] = {
    {
        &unit_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_jyugem, // itemDropTable
    },
    {
        &unit_bomhei, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_bomhei, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_jyugem, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_23: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_21_03_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_21_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_24, in the Pit of 100 Trials, floors 24, 29: Bob-omb x4. */
BattleUnitSetup btlparty_jon_jon_21_04_off_1[] = {
    {
        &unit_bomhei, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_bomhei, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_bomhei, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_bomhei, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_24: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_21_04_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_21_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_24, in the Pit of 100 Trials, floors 24, 29: Bob-omb x2, Spiky
 * Gloomba.
 */
BattleUnitSetup btlparty_jon_jon_21_04_off_2[] = {
    {
        &unit_bomhei, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_yami_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
    {
        &unit_bomhei, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_24: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_21_04_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_21_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_24, in the Pit of 100 Trials, floors 24, 29: Bob-omb x2, Boo x2.
 */
BattleUnitSetup btlparty_jon_jon_21_04_off_3[] = {
    {
        &unit_bomhei, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_bomhei, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_24: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_21_04_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_21_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_25, in the Pit of 100 Trials, floors 25, 30: Boo x4. */
BattleUnitSetup btlparty_jon_jon_21_05_off_1[] = {
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_25: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_21_05_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_21_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_25, in the Pit of 100 Trials, floors 25, 30: Boo x2, Spiky
 * Gloomba.
 */
BattleUnitSetup btlparty_jon_jon_21_05_off_2[] = {
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_yami_togekuri, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_25: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_21_05_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_21_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_25, in the Pit of 100 Trials, floors 25, 30: Boo x2, Bandit x2.
 */
BattleUnitSetup btlparty_jon_jon_21_05_off_3[] = {
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_25: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_21_05_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_21_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_31, in the Pit of 100 Trials, floors 31, 36: Dark Koopa x2. */
BattleUnitSetup btlparty_jon_jon_31_01_off_1[] = {
    {
        &unit_yami_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_noko, // itemDropTable
    },
    {
        &unit_yami_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_noko, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_31: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_31_01_off_1 = {
    2, // numEnemies
    btlparty_jon_jon_31_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_31, in the Pit of 100 Trials, floors 31, 36: Dark Koopa,
 * Parabuzzy x2.
 */
BattleUnitSetup btlparty_jon_jon_31_01_off_2[] = {
    {
        &unit_yami_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_noko, // itemDropTable
    },
    {
        &unit_patamet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_patamet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_31: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_31_01_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_31_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_31, in the Pit of 100 Trials, floors 31, 36: Dark Koopa, Flower
 * Fuzzy x2.
 */
BattleUnitSetup btlparty_jon_jon_31_01_off_3[] = {
    {
        &unit_yami_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_noko, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_31: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_31_01_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_31_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_32, in the Pit of 100 Trials, floors 32, 37: Hyper Cleft x3. */
BattleUnitSetup btlparty_jon_jon_31_02_off_1[] = {
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_32: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_31_02_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_31_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_32, in the Pit of 100 Trials, floors 32, 37: Hyper Cleft x2,
 * Shady Koopa.
 */
BattleUnitSetup btlparty_jon_jon_31_02_off_2[] = {
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_ura_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_32: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_31_02_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_31_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_32, in the Pit of 100 Trials, floors 32, 37: Hyper Cleft x2,
 * Parabuzzy.
 */
BattleUnitSetup btlparty_jon_jon_31_02_off_3[] = {
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_patamet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_32: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_31_02_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_31_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_33, in the Pit of 100 Trials, floors 33, 38: Parabuzzy x4. */
BattleUnitSetup btlparty_jon_jon_31_03_off_1[] = {
    {
        &unit_patamet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_patamet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_patamet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_patamet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_33: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_31_03_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_31_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_33, in the Pit of 100 Trials, floors 33, 38: Parabuzzy x2, Shady
 * Koopa x2.
 */
BattleUnitSetup btlparty_jon_jon_31_03_off_2[] = {
    {
        &unit_patamet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_ura_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_patamet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_ura_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_33: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_31_03_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_31_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_33, in the Pit of 100 Trials, floors 33, 38: Parabuzzy, Flower
 * Fuzzy.
 */
BattleUnitSetup btlparty_jon_jon_31_03_off_3[] = {
    {
        &unit_patamet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_33: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_31_03_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_31_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_34, in the Pit of 100 Trials, floors 34, 39: Shady Koopa x4. */
BattleUnitSetup btlparty_jon_jon_31_04_off_1[] = {
    {
        &unit_ura_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_ura_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_ura_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_ura_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_34: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_31_04_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_31_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_34, in the Pit of 100 Trials, floors 34, 39: Shady Koopa x2, Dark
 * Koopa.
 */
BattleUnitSetup btlparty_jon_jon_31_04_off_2[] = {
    {
        &unit_ura_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_yami_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_noko, // itemDropTable
    },
    {
        &unit_ura_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_34: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_31_04_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_31_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_34, in the Pit of 100 Trials, floors 34, 39: Shady Koopa x2,
 * Parabuzzy.
 */
BattleUnitSetup btlparty_jon_jon_31_04_off_3[] = {
    {
        &unit_ura_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_ura_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_patamet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_34: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_31_04_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_31_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_35, in the Pit of 100 Trials, floors 35, 40: Flower Fuzzy x5. */
BattleUnitSetup btlparty_jon_jon_31_05_off_1[] = {
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_35: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_31_05_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_31_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_35, in the Pit of 100 Trials, floors 35, 40: Flower Fuzzy x2,
 * Dark Koopa.
 */
BattleUnitSetup btlparty_jon_jon_31_05_off_2[] = {
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_yami_noko, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_noko, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_35: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_31_05_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_31_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_35, in the Pit of 100 Trials, floors 35, 40: Flower Fuzzy, Hyper
 * Cleft.
 */
BattleUnitSetup btlparty_jon_jon_31_05_off_3[] = {
    {
        &unit_flower_chorobon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_35: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_31_05_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_31_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 1 of BTLNO_JON_41, in the Pit of 100 Trials, floors 41, 46: Dark Paratroopa x4.
 */
BattleUnitSetup btlparty_jon_jon_41_01_off_1[] = {
    {
        &unit_yami_pata, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_yami_pata, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_yami_pata, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_yami_pata, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_41: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_41_01_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_41_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_41, in the Pit of 100 Trials, floors 41, 46: Dark Paratroopa x2,
 * Spiky Parabuzzy.
 */
BattleUnitSetup btlparty_jon_jon_41_01_off_2[] = {
    {
        &unit_yami_pata, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_yami_pata, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_patatogemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_41: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_41_01_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_41_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_41, in the Pit of 100 Trials, floors 41, 46: Dark Paratroopa x2,
 * Bulky Bob-omb.
 */
BattleUnitSetup btlparty_jon_jon_41_01_off_3[] = {
    {
        &unit_yami_pata, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_yami_pata, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_heavy_bom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_41: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_41_01_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_41_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_42, in the Pit of 100 Trials, floors 42, 47: Bulky Bob-omb x4. */
BattleUnitSetup btlparty_jon_jon_41_02_off_1[] = {
    {
        &unit_heavy_bom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_heavy_bom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_heavy_bom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_heavy_bom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_42: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_41_02_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_41_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_JON_42, in the Pit of 100 Trials, floors 42, 47: Bulky Bob-omb x2. */
BattleUnitSetup btlparty_jon_jon_41_02_off_2[] = {
    {
        &unit_heavy_bom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_heavy_bom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_42: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_41_02_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_41_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_42, in the Pit of 100 Trials, floors 42, 47: Bulky Bob-omb,
 * Poison Pokey, Spiky Parabuzzy.
 */
BattleUnitSetup btlparty_jon_jon_41_02_off_3[] = {
    {
        &unit_heavy_bom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_sambo_mummy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_patatogemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_42: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_41_02_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_41_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_43, in the Pit of 100 Trials, floors 43, 48: Lava Bubble x3. */
BattleUnitSetup btlparty_jon_jon_41_03_off_1[] = {
    {
        &unit_bubble, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_bubble, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_bubble, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_43: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_41_03_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_41_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_43, in the Pit of 100 Trials, floors 43, 48: Lava Bubble x2, Dark
 * Paratroopa.
 */
BattleUnitSetup btlparty_jon_jon_41_03_off_2[] = {
    {
        &unit_bubble, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_bubble, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_yami_pata, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_43: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_41_03_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_41_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_43, in the Pit of 100 Trials, floors 43, 48: Lava Bubble x2,
 * Poison Pokey, Bulky Bob-omb.
 */
BattleUnitSetup btlparty_jon_jon_41_03_off_3[] = {
    {
        &unit_bubble, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_sambo_mummy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_bubble, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_heavy_bom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_43: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_41_03_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_41_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_44, in the Pit of 100 Trials, floors 44, 49: Poison Pokey x4. */
BattleUnitSetup btlparty_jon_jon_41_04_off_1[] = {
    {
        &unit_sambo_mummy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_sambo_mummy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_sambo_mummy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_sambo_mummy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_44: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_41_04_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_41_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_44, in the Pit of 100 Trials, floors 44, 49: Poison Pokey x2,
 * Spiky Parabuzzy x2.
 */
BattleUnitSetup btlparty_jon_jon_41_04_off_2[] = {
    {
        &unit_sambo_mummy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_patatogemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_sambo_mummy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_patatogemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_44: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_41_04_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_41_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_44, in the Pit of 100 Trials, floors 44, 49: Poison Pokey, Dark
 * Paratroopa.
 */
BattleUnitSetup btlparty_jon_jon_41_04_off_3[] = {
    {
        &unit_sambo_mummy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_yami_pata, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_44: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_41_04_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_41_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 1 of BTLNO_JON_45, in the Pit of 100 Trials, floors 45, 50: Spiky Parabuzzy x2.
 */
BattleUnitSetup btlparty_jon_jon_41_05_off_1[] = {
    {
        &unit_patatogemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_patatogemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_45: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_41_05_off_1 = {
    2, // numEnemies
    btlparty_jon_jon_41_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_45, in the Pit of 100 Trials, floors 45, 50: Spiky Parabuzzy x4.
 */
BattleUnitSetup btlparty_jon_jon_41_05_off_2[] = {
    {
        &unit_patatogemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_patatogemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_patatogemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_patatogemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_45: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_41_05_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_41_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_45, in the Pit of 100 Trials, floors 45, 50: Spiky Parabuzzy x2,
 * Lava Bubble x2.
 */
BattleUnitSetup btlparty_jon_jon_41_05_off_3[] = {
    {
        &unit_patatogemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_patatogemet, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_bubble, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_bubble, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_45: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_41_05_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_41_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_51, in the Pit of 100 Trials, floors 51, 56: Badge Bandit x3. */
BattleUnitSetup btlparty_jon_jon_51_01_off_1[] = {
    {
        &unit_badge_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
    {
        &unit_badge_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
    {
        &unit_badge_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_51: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_51_01_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_51_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_51, in the Pit of 100 Trials, floors 51, 56: Badge Bandit x2, Ice
 * Puff.
 */
BattleUnitSetup btlparty_jon_jon_51_01_off_2[] = {
    {
        &unit_badge_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
    {
        &unit_badge_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
    {
        &unit_bllizard, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_51: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_51_01_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_51_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_51, in the Pit of 100 Trials, floors 51, 56: Badge Bandit x2,
 * Dark Boo.
 */
BattleUnitSetup btlparty_jon_jon_51_01_off_3[] = {
    {
        &unit_badge_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
    {
        &unit_badge_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
    {
        &unit_purple_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_51: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_51_01_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_51_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_52, in the Pit of 100 Trials, floors 52, 57: Ice Puff x4. */
BattleUnitSetup btlparty_jon_jon_51_02_off_1[] = {
    {
        &unit_bllizard, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_bllizard, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_bllizard, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_bllizard, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_52: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_51_02_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_51_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_52, in the Pit of 100 Trials, floors 52, 57: Ice Puff x2, Dark
 * Boo.
 */
BattleUnitSetup btlparty_jon_jon_51_02_off_2[] = {
    {
        &unit_bllizard, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_purple_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_bllizard, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_52: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_51_02_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_51_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_52, in the Pit of 100 Trials, floors 52, 57: Ice Puff, Moon
 * Cleft.
 */
BattleUnitSetup btlparty_jon_jon_51_02_off_3[] = {
    {
        &unit_bllizard, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_52: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_51_02_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_51_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_53, in the Pit of 100 Trials, floors 53, 58: Dark Boo x4. */
BattleUnitSetup btlparty_jon_jon_51_03_off_1[] = {
    {
        &unit_purple_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_53: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_51_03_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_51_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_53, in the Pit of 100 Trials, floors 53, 58: Dark Boo x2, Badge
 * Bandit.
 */
BattleUnitSetup btlparty_jon_jon_51_03_off_2[] = {
    {
        &unit_purple_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_badge_borodo, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
    {
        &unit_purple_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_53: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_51_03_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_51_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_53, in the Pit of 100 Trials, floors 53, 58: Dark Boo x2, Red
 * Chomp.
 */
BattleUnitSetup btlparty_jon_jon_51_03_off_3[] = {
    {
        &unit_purple_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_burst_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_53: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_51_03_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_51_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_54, in the Pit of 100 Trials, floors 54, 59: Red Chomp x3. */
BattleUnitSetup btlparty_jon_jon_51_04_off_1[] = {
    {
        &unit_burst_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
    {
        &unit_burst_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
    {
        &unit_burst_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_54: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_51_04_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_51_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_54, in the Pit of 100 Trials, floors 54, 59: Red Chomp x2, Dark
 * Boo.
 */
BattleUnitSetup btlparty_jon_jon_51_04_off_2[] = {
    {
        &unit_burst_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
    {
        &unit_purple_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_burst_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_54: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_51_04_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_51_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_54, in the Pit of 100 Trials, floors 54, 59: Red Chomp, Ice Puff
 * x2.
 */
BattleUnitSetup btlparty_jon_jon_51_04_off_3[] = {
    {
        &unit_burst_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
    {
        &unit_bllizard, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_bllizard, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_54: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_51_04_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_51_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_55, in the Pit of 100 Trials, floors 55, 60: Moon Cleft x4. */
BattleUnitSetup btlparty_jon_jon_51_05_off_1[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_55: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_51_05_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_51_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_55, in the Pit of 100 Trials, floors 55, 60: Moon Cleft x2, Red
 * Chomp.
 */
BattleUnitSetup btlparty_jon_jon_51_05_off_2[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_burst_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_55: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_51_05_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_51_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_55, in the Pit of 100 Trials, floors 55, 60: Moon Cleft, Dark Boo
 * x2.
 */
BattleUnitSetup btlparty_jon_jon_51_05_off_3[] = {
    {
        &unit_sinemon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_purple_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_55: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_51_05_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_51_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_61, in the Pit of 100 Trials, floors 61, 66: Dark Lakitu x3. */
BattleUnitSetup btlparty_jon_jon_61_01_off_1[] = {
    {
        &unit_hyper_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
    {
        &unit_hyper_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
    {
        &unit_hyper_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_61: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_61_01_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_61_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_61, in the Pit of 100 Trials, floors 61, 66: Dark Lakitu x2, Dark
 * Craw.
 */
BattleUnitSetup btlparty_jon_jon_61_01_off_2[] = {
    {
        &unit_hyper_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
    {
        &unit_hyper_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
    {
        &unit_dark_keeper, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_61: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_61_01_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_61_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_61, in the Pit of 100 Trials, floors 61, 66: Dark Lakitu, Dark
 * Wizzerd.
 */
BattleUnitSetup btlparty_jon_jon_61_01_off_3[] = {
    {
        &unit_hyper_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
    {
        &unit_super_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_61: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_61_01_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_61_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_62, in the Pit of 100 Trials, floors 62, 67: Dry Bones x4. */
BattleUnitSetup btlparty_jon_jon_61_02_off_1[] = {
    {
        &unit_karon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_62: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_61_02_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_61_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_62, in the Pit of 100 Trials, floors 62, 67: Dry Bones, Dark
 * Craw.
 */
BattleUnitSetup btlparty_jon_jon_61_02_off_2[] = {
    {
        &unit_karon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_dark_keeper, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_62: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_61_02_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_61_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_62, in the Pit of 100 Trials, floors 62, 67: Dry Bones, Frost
 * Piranha x2.
 */
BattleUnitSetup btlparty_jon_jon_61_02_off_3[] = {
    {
        &unit_karon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_ice_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_ice_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_62: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_61_02_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_61_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_63, in the Pit of 100 Trials, floors 63, 68: Dark Wizzerd x3. */
BattleUnitSetup btlparty_jon_jon_61_03_off_1[] = {
    {
        &unit_super_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
    {
        &unit_super_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
    {
        &unit_super_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_63: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_61_03_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_61_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_63, in the Pit of 100 Trials, floors 63, 68: Dark Wizzerd x2,
 * Dark Craw.
 */
BattleUnitSetup btlparty_jon_jon_61_03_off_2[] = {
    {
        &unit_super_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
    {
        &unit_dark_keeper, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
    {
        &unit_super_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_63: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_61_03_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_61_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_63, in the Pit of 100 Trials, floors 63, 68: Dark Wizzerd, Dry
 * Bones x2.
 */
BattleUnitSetup btlparty_jon_jon_61_03_off_3[] = {
    {
        &unit_super_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
    {
        &unit_karon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_63: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_61_03_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_61_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_64, in the Pit of 100 Trials, floors 64, 69: Frost Piranha x4. */
BattleUnitSetup btlparty_jon_jon_61_04_off_1[] = {
    {
        &unit_ice_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_ice_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_ice_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_ice_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_64: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_61_04_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_61_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_64, in the Pit of 100 Trials, floors 64, 69: Frost Piranha, Dark
 * Lakitu x2.
 */
BattleUnitSetup btlparty_jon_jon_61_04_off_2[] = {
    {
        &unit_ice_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_hyper_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
    {
        &unit_hyper_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_64: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_61_04_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_61_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_64, in the Pit of 100 Trials, floors 64, 69: Frost Piranha, Dark
 * Craw.
 */
BattleUnitSetup btlparty_jon_jon_61_04_off_3[] = {
    {
        &unit_ice_pakkun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_dark_keeper, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_64: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_61_04_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_61_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_65, in the Pit of 100 Trials, floors 65, 70: Dark Craw x3. */
BattleUnitSetup btlparty_jon_jon_61_05_off_1[] = {
    {
        &unit_dark_keeper, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
    {
        &unit_dark_keeper, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
    {
        &unit_dark_keeper, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_65: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_61_05_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_61_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_65, in the Pit of 100 Trials, floors 65, 70: Dark Craw, Dark
 * Lakitu.
 */
BattleUnitSetup btlparty_jon_jon_61_05_off_2[] = {
    {
        &unit_dark_keeper, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
    {
        &unit_hyper_jyugem, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_65: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_61_05_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_61_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 3 of BTLNO_JON_65, in the Pit of 100 Trials, floors 65, 70: Dark Craw x2. */
BattleUnitSetup btlparty_jon_jon_61_05_off_3[] = {
    {
        &unit_dark_keeper, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
    {
        &unit_dark_keeper, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_65: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_61_05_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_61_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_71, in the Pit of 100 Trials, floors 71, 76: Wizzerd x3. */
BattleUnitSetup btlparty_jon_jon_71_01_off_1[] = {
    {
        &unit_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
    {
        &unit_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
    {
        &unit_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_71: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_71_01_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_71_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_71, in the Pit of 100 Trials, floors 71, 76: Wizzerd x2, Dark
 * Koopatrol.
 */
BattleUnitSetup btlparty_jon_jon_71_01_off_2[] = {
    {
        &unit_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
    {
        &unit_togenoko_ace, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_71: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_71_01_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_71_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_71, in the Pit of 100 Trials, floors 71, 76: Wizzerd, Chain-Chomp
 * x2.
 */
BattleUnitSetup btlparty_jon_jon_71_01_off_3[] = {
    {
        &unit_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
    {
        &unit_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_71: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_71_01_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_71_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 1 of BTLNO_JON_72, in the Pit of 100 Trials, floors 72, 77: Dark Koopatrol x3.
 */
BattleUnitSetup btlparty_jon_jon_71_02_off_1[] = {
    {
        &unit_togenoko_ace, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_togenoko_ace, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_togenoko_ace, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_72: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_71_02_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_71_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_72, in the Pit of 100 Trials, floors 72, 77: Dark Koopatrol,
 * Chain-Chomp x2.
 */
BattleUnitSetup btlparty_jon_jon_71_02_off_2[] = {
    {
        &unit_togenoko_ace, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_72: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_71_02_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_71_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_72, in the Pit of 100 Trials, floors 72, 77: Dark Koopatrol x2,
 * Wizzerd.
 */
BattleUnitSetup btlparty_jon_jon_71_02_off_3[] = {
    {
        &unit_togenoko_ace, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_togenoko_ace, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_72: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_71_02_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_71_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_73, in the Pit of 100 Trials, floors 73, 78: Phantom Ember x5. */
BattleUnitSetup btlparty_jon_jon_71_03_off_1[] = {
    {
        &unit_phantom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_73: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_71_03_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_71_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_73, in the Pit of 100 Trials, floors 73, 78: Phantom Ember x3,
 * Wizzerd.
 */
BattleUnitSetup btlparty_jon_jon_71_03_off_2[] = {
    {
        &unit_phantom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_73: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_71_03_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_71_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_73, in the Pit of 100 Trials, floors 73, 78: Phantom Ember,
 * Swoopula, Chain-Chomp, Dark Koopatrol.
 */
BattleUnitSetup btlparty_jon_jon_71_03_off_3[] = {
    {
        &unit_phantom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_togenoko_ace, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_73: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_71_03_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_71_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_74, in the Pit of 100 Trials, floors 74, 79: Swoopula x4. */
BattleUnitSetup btlparty_jon_jon_71_04_off_1[] = {
    {
        &unit_basabasa_chururu, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 130.0f, 20.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_74: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_71_04_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_71_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_74, in the Pit of 100 Trials, floors 74, 79: Swoopula x2, Wizzerd
 * x2.
 */
BattleUnitSetup btlparty_jon_jon_71_04_off_2[] = {
    {
        &unit_basabasa_chururu, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
    {
        &unit_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_74: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_71_04_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_71_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_74, in the Pit of 100 Trials, floors 74, 79: Swoopula x2, Dark
 * Koopatrol x2.
 */
BattleUnitSetup btlparty_jon_jon_71_04_off_3[] = {
    {
        &unit_basabasa_chururu, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_togenoko_ace, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_togenoko_ace, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_74: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_71_04_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_71_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_75, in the Pit of 100 Trials, floors 75, 80: Chain-Chomp x4. */
BattleUnitSetup btlparty_jon_jon_71_05_off_1[] = {
    {
        &unit_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_75: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_71_05_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_71_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_75, in the Pit of 100 Trials, floors 75, 80: Chain-Chomp, Dark
 * Koopatrol x2.
 */
BattleUnitSetup btlparty_jon_jon_71_05_off_2[] = {
    {
        &unit_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_togenoko_ace, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_togenoko_ace, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_75: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_71_05_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_71_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_75, in the Pit of 100 Trials, floors 75, 80: Chain-Chomp x2,
 * Wizzerd.
 */
BattleUnitSetup btlparty_jon_jon_71_05_off_3[] = {
    {
        &unit_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_mahorn, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_75: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_71_05_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_71_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_81, in the Pit of 100 Trials, floors 81, 86: Spunia x4. */
BattleUnitSetup btlparty_jon_jon_81_01_off_1[] = {
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_81: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_81_01_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_81_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_81, in the Pit of 100 Trials, floors 81, 86: Spunia x2, Arantula
 * x2.
 */
BattleUnitSetup btlparty_jon_jon_81_01_off_2[] = {
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_churantalar, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_churantalar, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_81: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_81_01_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_81_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_81, in the Pit of 100 Trials, floors 81, 86: Spunia, Piranha
 * Plant, Dark Bristle.
 */
BattleUnitSetup btlparty_jon_jon_81_01_off_3[] = {
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_pakkun_flower, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
    {
        &unit_yamitogedaruma, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_81: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_81_01_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_81_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_82, in the Pit of 100 Trials, floors 82, 87: Dark Bristle x2. */
BattleUnitSetup btlparty_jon_jon_81_02_off_1[] = {
    {
        &unit_yamitogedaruma, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
    {
        &unit_yamitogedaruma, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_82: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_81_02_off_1 = {
    2, // numEnemies
    btlparty_jon_jon_81_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_82, in the Pit of 100 Trials, floors 82, 87: Dark Bristle,
 * Arantula x3.
 */
BattleUnitSetup btlparty_jon_jon_81_02_off_2[] = {
    {
        &unit_yamitogedaruma, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
    {
        &unit_churantalar, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_churantalar, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_churantalar, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_82: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_81_02_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_81_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_82, in the Pit of 100 Trials, floors 82, 87: Dark Bristle x2,
 * Spunia.
 */
BattleUnitSetup btlparty_jon_jon_81_02_off_3[] = {
    {
        &unit_yamitogedaruma, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
    {
        &unit_yamitogedaruma, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_82: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_81_02_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_81_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_83, in the Pit of 100 Trials, floors 83, 88: Arantula x4. */
BattleUnitSetup btlparty_jon_jon_81_03_off_1[] = {
    {
        &unit_churantalar, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_churantalar, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_churantalar, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_churantalar, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_83: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_81_03_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_81_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_83, in the Pit of 100 Trials, floors 83, 88: Arantula x2, Piranha
 * Plant.
 */
BattleUnitSetup btlparty_jon_jon_81_03_off_2[] = {
    {
        &unit_churantalar, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_pakkun_flower, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
    {
        &unit_churantalar, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_83: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_81_03_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_81_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_83, in the Pit of 100 Trials, floors 83, 88: Arantula x2, Dark
 * Bristle.
 */
BattleUnitSetup btlparty_jon_jon_81_03_off_3[] = {
    {
        &unit_churantalar, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_churantalar, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_yamitogedaruma, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_83: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_81_03_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_81_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_84, in the Pit of 100 Trials, floors 84, 89: Piranha Plant x4. */
BattleUnitSetup btlparty_jon_jon_81_04_off_1[] = {
    {
        &unit_pakkun_flower, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
    {
        &unit_pakkun_flower, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
    {
        &unit_pakkun_flower, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
    {
        &unit_pakkun_flower, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_84: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_81_04_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_81_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default5, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_84, in the Pit of 100 Trials, floors 84, 89: Piranha Plant x2,
 * Dark Bristle.
 */
BattleUnitSetup btlparty_jon_jon_81_04_off_2[] = {
    {
        &unit_pakkun_flower, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
    {
        &unit_yamitogedaruma, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
    {
        &unit_pakkun_flower, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_84: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_81_04_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_81_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default5, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_84, in the Pit of 100 Trials, floors 84, 89: Piranha Plant,
 * Spunia x3.
 */
BattleUnitSetup btlparty_jon_jon_81_04_off_3[] = {
    {
        &unit_pakkun_flower, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_84: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_81_04_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_81_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default5, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_85, in the Pit of 100 Trials, floors 85, 90: Spunia x4. */
BattleUnitSetup btlparty_jon_jon_81_05_off_1[] = {
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_85: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_81_05_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_81_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 2 of BTLNO_JON_85, in the Pit of 100 Trials, floors 85, 90: Spunia x3. */
BattleUnitSetup btlparty_jon_jon_81_05_off_2[] = {
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_85: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_81_05_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_81_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 3 of BTLNO_JON_85, in the Pit of 100 Trials, floors 85, 90: Spunia x5. */
BattleUnitSetup btlparty_jon_jon_81_05_off_3[] = {
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_85: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_81_05_off_3 = {
    5, // numEnemies
    btlparty_jon_jon_81_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 1 of BTLNO_JON_91, in the Pit of 100 Trials, floors 91, 95, 99: Elite Wizzerd
 * x5.
 */
BattleUnitSetup btlparty_jon_jon_91_01_off_1[] = {
    {
        &unit_mahorn_custom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_mahorn_custom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_mahorn_custom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_mahorn_custom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_mahorn_custom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_91: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_91_01_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_91_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_91, in the Pit of 100 Trials, floors 91, 95, 99: Elite Wizzerd,
 * Poison Puff x2.
 */
BattleUnitSetup btlparty_jon_jon_91_01_off_2[] = {
    {
        &unit_mahorn_custom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_dokugassun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_dokugassun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_91: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_91_01_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_91_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_91, in the Pit of 100 Trials, floors 91, 95, 99: Elite Wizzerd
 * x2, Swampire.
 */
BattleUnitSetup btlparty_jon_jon_91_01_off_3[] = {
    {
        &unit_mahorn_custom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_mahorn_custom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_basabasa_green, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_91: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_91_01_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_91_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_92, in the Pit of 100 Trials, floors 92, 96: Poison Puff x4. */
BattleUnitSetup btlparty_jon_jon_91_02_off_1[] = {
    {
        &unit_dokugassun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_dokugassun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_dokugassun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_dokugassun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_92: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_91_02_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_91_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_92, in the Pit of 100 Trials, floors 92, 96: Poison Puff x3,
 * Bob-ulk.
 */
BattleUnitSetup btlparty_jon_jon_91_02_off_2[] = {
    {
        &unit_dokugassun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_dokugassun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_dokugassun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_giant_bomb, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_92: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_91_02_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_91_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_92, in the Pit of 100 Trials, floors 92, 96: Poison Puff x2,
 * Swampire, Amazy Dayzee.
 */
BattleUnitSetup btlparty_jon_jon_91_02_off_3[] = {
    {
        &unit_dokugassun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_basabasa_green, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
    {
        &unit_dokugassun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_92: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_91_02_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_91_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_93, in the Pit of 100 Trials, floors 93, 97: Bob-ulk x4. */
BattleUnitSetup btlparty_jon_jon_91_03_off_1[] = {
    {
        &unit_giant_bomb, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_giant_bomb, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_giant_bomb, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_giant_bomb, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_93: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_91_03_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_91_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default5, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_93, in the Pit of 100 Trials, floors 93, 97: Bob-ulk x2, Elite
 * Wizzerd.
 */
BattleUnitSetup btlparty_jon_jon_91_03_off_2[] = {
    {
        &unit_giant_bomb, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_giant_bomb, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_mahorn_custom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_93: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_91_03_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_91_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default5, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_93, in the Pit of 100 Trials, floors 93, 97: Bob-ulk x3, Amazy
 * Dayzee.
 */
BattleUnitSetup btlparty_jon_jon_91_03_off_3[] = {
    {
        &unit_giant_bomb, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_giant_bomb, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_giant_bomb, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_93: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_91_03_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_91_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default5, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/** Enemy lineup of formation 1 of BTLNO_JON_94, in the Pit of 100 Trials, floors 94, 98: Swampire x5. */
BattleUnitSetup btlparty_jon_jon_91_04_off_1[] = {
    {
        &unit_basabasa_green, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 130.0f, -20.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
    {
        &unit_basabasa_green, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
    {
        &unit_basabasa_green, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
    {
        &unit_basabasa_green, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
    {
        &unit_basabasa_green, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {170.0f, 130.0f, 20.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_94: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_91_04_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_91_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_94, in the Pit of 100 Trials, floors 94, 98: Swampire, Poison
 * Puff x2.
 */
BattleUnitSetup btlparty_jon_jon_91_04_off_2[] = {
    {
        &unit_basabasa_green, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
    {
        &unit_dokugassun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_dokugassun, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_94: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_91_04_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_91_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_94, in the Pit of 100 Trials, floors 94, 98: Swampire, Elite
 * Wizzerd, Amazy Dayzee x2.
 */
BattleUnitSetup btlparty_jon_jon_91_04_off_3[] = {
    {
        &unit_basabasa_green, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {30.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
    {
        &unit_mahorn_custom, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_94: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_91_04_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_91_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 1 of BTLNO_JON_95, in the Pit of 100 Trials (no floor uses it): Amazy Dayzee x5.
 */
BattleUnitSetup btlparty_jon_jon_91_05_off_1[] = {
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

/**
 * Formation 1 of BTLNO_JON_95: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_91_05_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_91_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default5, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 2 of BTLNO_JON_95, in the Pit of 100 Trials (no floor uses it): Amazy Dayzee x5.
 */
BattleUnitSetup btlparty_jon_jon_91_05_off_2[] = {
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

/**
 * Formation 2 of BTLNO_JON_95: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_91_05_off_2 = {
    5, // numEnemies
    btlparty_jon_jon_91_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default5, // fpDropTable
    5, // reserved_1C
};

/**
 * Enemy lineup of formation 3 of BTLNO_JON_95, in the Pit of 100 Trials (no floor uses it): Amazy Dayzee x5.
 */
BattleUnitSetup btlparty_jon_jon_91_05_off_3[] = {
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

/**
 * Formation 3 of BTLNO_JON_95: the lineup above with the default heart and flower drops. Item drop weights:
 * 20 held, 20 random, 60 none.
 */
BattleGroupSetup btlgrp_jon_jon_91_05_off_3 = {
    5, // numEnemies
    btlparty_jon_jon_91_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default5, // fpDropTable
    5, // reserved_1C
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_test1_off[] = {
    {50, &btlgrp_jon_jon_01_01_off_1, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_01 picks one of its formations by weight (10:10:10) on stage stg_00_3: Gloomba; Gloomba, Spania;
 * Gloomba, Dull Bones.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_01_off[] = {
    {10, &btlgrp_jon_jon_01_01_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_01_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_01_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_02 picks one of its formations by weight (10:10:10) on stage stg_00_3: Spinia x5; Spinia,
 * Gloomba; Spinia x3, Spania x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_02_off[] = {
    {10, &btlgrp_jon_jon_01_02_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_02_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_02_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_03 picks one of its formations by weight (10:10:10) on stage stg_00_3: Spania x4; Spania,
 * Gloomba; Spania x2, Fuzzy x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_03_off[] = {
    {10, &btlgrp_jon_jon_01_03_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_03_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_03_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_04 picks one of its formations by weight (10:10:10) on stage stg_00_3: Dull Bones x4; Dull
 * Bones, Gloomba; Dull Bones, Spinia x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_04_off[] = {
    {10, &btlgrp_jon_jon_01_04_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_04_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_04_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_05 picks one of its formations by weight (10:10:10) on stage stg_00_3: Fuzzy x5; Fuzzy x2,
 * Gloomba; Fuzzy x2, Dull Bones x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_05_off[] = {
    {10, &btlgrp_jon_jon_01_05_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_05_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_05_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_11 picks one of its formations by weight (10:10:10) on stage stg_00_3: Paragloomba x2;
 * Paragloomba, Pider x2; Paragloomba, Pokey x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_11_off[] = {
    {10, &btlgrp_jon_jon_11_01_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_01_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_01_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_12 picks one of its formations by weight (10:10:10) on stage stg_00_3: Cleft x4; Cleft x2,
 * Paragloomba; Cleft, Pider, Dark Puff.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_12_off[] = {
    {10, &btlgrp_jon_jon_11_02_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_02_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_02_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_13 picks one of its formations by weight (10:10:10) on stage stg_00_3: Pokey x4; Pokey, Dark
 * Puff; Pokey x2, Pider x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_13_off[] = {
    {10, &btlgrp_jon_jon_11_03_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_03_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_03_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_14 picks one of its formations by weight (10:10:10) on stage stg_00_3: Dark Puff x3; Dark Puff
 * x2, Pokey; Dark Puff, Paragloomba.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_14_off[] = {
    {10, &btlgrp_jon_jon_11_04_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_04_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_04_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_15 picks one of its formations by weight (10:10:10) on stage stg_00_3: Pider x4; Pider,
 * Paragloomba; Pider x2, Pokey x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_15_off[] = {
    {10, &btlgrp_jon_jon_11_05_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_05_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_05_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_21 picks one of its formations by weight (10:10:10) on stage stg_00_3: Spiky Gloomba x3; Spiky
 * Gloomba x2, Bob-omb, Bandit; Spiky Gloomba x2, Boo x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_21_off[] = {
    {10, &btlgrp_jon_jon_21_01_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_01_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_01_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_22 picks one of its formations by weight (10:10:10) on stage stg_00_3: Bandit x5; Bandit, Spiky
 * Gloomba; Bandit x3, Boo x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_22_off[] = {
    {10, &btlgrp_jon_jon_21_02_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_02_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_02_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_23 picks one of its formations by weight (10:10:10) on stage stg_00_3: Lakitu x4; Lakitu, Spiky
 * Gloomba; Lakitu x2, Bob-omb x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_23_off[] = {
    {10, &btlgrp_jon_jon_21_03_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_03_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_03_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_24 picks one of its formations by weight (10:10:10) on stage stg_00_3: Bob-omb x4; Bob-omb x2,
 * Spiky Gloomba; Bob-omb x2, Boo x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_24_off[] = {
    {10, &btlgrp_jon_jon_21_04_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_04_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_04_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_25 picks one of its formations by weight (10:10:10) on stage stg_00_3: Boo x4; Boo x2, Spiky
 * Gloomba; Boo x2, Bandit x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_25_off[] = {
    {10, &btlgrp_jon_jon_21_05_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_05_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_05_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_31 picks one of its formations by weight (10:10:10) on stage stg_00_3: Dark Koopa x2; Dark
 * Koopa, Parabuzzy x2; Dark Koopa, Flower Fuzzy x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_31_off[] = {
    {10, &btlgrp_jon_jon_31_01_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_01_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_01_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_32 picks one of its formations by weight (10:10:10) on stage stg_00_3: Hyper Cleft x3; Hyper
 * Cleft x2, Shady Koopa; Hyper Cleft x2, Parabuzzy.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_32_off[] = {
    {10, &btlgrp_jon_jon_31_02_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_02_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_02_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_33 picks one of its formations by weight (10:10:10) on stage stg_00_3: Parabuzzy x4; Parabuzzy
 * x2, Shady Koopa x2; Parabuzzy, Flower Fuzzy.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_33_off[] = {
    {10, &btlgrp_jon_jon_31_03_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_03_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_03_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_34 picks one of its formations by weight (10:10:10) on stage stg_00_3: Shady Koopa x4; Shady
 * Koopa x2, Dark Koopa; Shady Koopa x2, Parabuzzy.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_34_off[] = {
    {10, &btlgrp_jon_jon_31_04_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_04_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_04_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_35 picks one of its formations by weight (10:10:10) on stage stg_00_3: Flower Fuzzy x5; Flower
 * Fuzzy x2, Dark Koopa; Flower Fuzzy, Hyper Cleft.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_35_off[] = {
    {10, &btlgrp_jon_jon_31_05_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_05_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_05_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_41 picks one of its formations by weight (10:10:10) on stage stg_00_3: Dark Paratroopa x4; Dark
 * Paratroopa x2, Spiky Parabuzzy; Dark Paratroopa x2, Bulky Bob-omb.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_41_off[] = {
    {10, &btlgrp_jon_jon_41_01_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_01_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_01_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_42 picks one of its formations by weight (10:10:10) on stage stg_00_3: Bulky Bob-omb x4; Bulky
 * Bob-omb x2; Bulky Bob-omb, Poison Pokey, Spiky Parabuzzy.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_42_off[] = {
    {10, &btlgrp_jon_jon_41_02_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_02_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_02_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_43 picks one of its formations by weight (10:10:10) on stage stg_00_3: Lava Bubble x3; Lava
 * Bubble x2, Dark Paratroopa; Lava Bubble x2, Poison Pokey, Bulky Bob-omb.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_43_off[] = {
    {10, &btlgrp_jon_jon_41_03_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_03_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_03_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_44 picks one of its formations by weight (10:10:10) on stage stg_00_3: Poison Pokey x4; Poison
 * Pokey x2, Spiky Parabuzzy x2; Poison Pokey, Dark Paratroopa.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_44_off[] = {
    {10, &btlgrp_jon_jon_41_04_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_04_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_04_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_45 picks one of its formations by weight (10:10:10) on stage stg_00_3: Spiky Parabuzzy x2; Spiky
 * Parabuzzy x4; Spiky Parabuzzy x2, Lava Bubble x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_45_off[] = {
    {10, &btlgrp_jon_jon_41_05_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_05_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_05_off_3, jon_btlstage_data},
    {0},
};

/**
 * BTLNO_JON_51 picks one of its formations by weight (10:10:10) on stage stg_00_3: Badge Bandit x3; Badge
 * Bandit x2, Ice Puff; Badge Bandit x2, Dark Boo.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_51_off[] = {
    {10, &btlgrp_jon_jon_51_01_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_01_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_01_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_52 picks one of its formations by weight (10:10:10) on stage stg_00_3: Ice Puff x4; Ice Puff x2,
 * Dark Boo; Ice Puff, Moon Cleft.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_52_off[] = {
    {10, &btlgrp_jon_jon_51_02_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_02_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_02_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_53 picks one of its formations by weight (10:10:10) on stage stg_00_3: Dark Boo x4; Dark Boo x2,
 * Badge Bandit; Dark Boo x2, Red Chomp.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_53_off[] = {
    {10, &btlgrp_jon_jon_51_03_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_03_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_03_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_54 picks one of its formations by weight (10:10:10) on stage stg_00_3: Red Chomp x3; Red Chomp
 * x2, Dark Boo; Red Chomp, Ice Puff x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_54_off[] = {
    {10, &btlgrp_jon_jon_51_04_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_04_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_04_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_55 picks one of its formations by weight (10:10:10) on stage stg_00_3: Moon Cleft x4; Moon Cleft
 * x2, Red Chomp; Moon Cleft, Dark Boo x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_55_off[] = {
    {10, &btlgrp_jon_jon_51_05_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_05_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_05_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_61 picks one of its formations by weight (10:10:10) on stage stg_00_3: Dark Lakitu x3; Dark
 * Lakitu x2, Dark Craw; Dark Lakitu, Dark Wizzerd.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_61_off[] = {
    {10, &btlgrp_jon_jon_61_01_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_01_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_01_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_62 picks one of its formations by weight (10:10:10) on stage stg_00_3: Dry Bones x4; Dry Bones,
 * Dark Craw; Dry Bones, Frost Piranha x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_62_off[] = {
    {10, &btlgrp_jon_jon_61_02_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_02_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_02_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_63 picks one of its formations by weight (10:10:10) on stage stg_00_3: Dark Wizzerd x3; Dark
 * Wizzerd x2, Dark Craw; Dark Wizzerd, Dry Bones x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_63_off[] = {
    {10, &btlgrp_jon_jon_61_03_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_03_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_03_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_64 picks one of its formations by weight (10:10:10) on stage stg_00_3: Frost Piranha x4; Frost
 * Piranha, Dark Lakitu x2; Frost Piranha, Dark Craw.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_64_off[] = {
    {10, &btlgrp_jon_jon_61_04_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_04_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_04_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_65 picks one of its formations by weight (10:10:10) on stage stg_00_3: Dark Craw x3; Dark Craw,
 * Dark Lakitu; Dark Craw x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_65_off[] = {
    {10, &btlgrp_jon_jon_61_05_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_05_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_05_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_71 picks one of its formations by weight (10:10:10) on stage stg_00_3: Wizzerd x3; Wizzerd x2,
 * Dark Koopatrol; Wizzerd, Chain-Chomp x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_71_off[] = {
    {10, &btlgrp_jon_jon_71_01_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_01_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_01_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_72 picks one of its formations by weight (10:10:10) on stage stg_00_3: Dark Koopatrol x3; Dark
 * Koopatrol, Chain-Chomp x2; Dark Koopatrol x2, Wizzerd.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_72_off[] = {
    {10, &btlgrp_jon_jon_71_02_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_02_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_02_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_73 picks one of its formations by weight (10:10:10) on stage stg_00_3: Phantom Ember x5; Phantom
 * Ember x3, Wizzerd; Phantom Ember, Swoopula, Chain-Chomp, Dark Koopatrol.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_73_off[] = {
    {10, &btlgrp_jon_jon_71_03_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_03_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_03_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_74 picks one of its formations by weight (10:10:10) on stage stg_00_3: Swoopula x4; Swoopula x2,
 * Wizzerd x2; Swoopula x2, Dark Koopatrol x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_74_off[] = {
    {10, &btlgrp_jon_jon_71_04_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_04_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_04_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_75 picks one of its formations by weight (10:10:10) on stage stg_00_3: Chain-Chomp x4;
 * Chain-Chomp, Dark Koopatrol x2; Chain-Chomp x2, Wizzerd.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_75_off[] = {
    {10, &btlgrp_jon_jon_71_05_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_05_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_05_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_81 picks one of its formations by weight (10:10:10) on stage stg_00_3: Spunia x4; Spunia x2,
 * Arantula x2; Spunia, Piranha Plant, Dark Bristle.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_81_off[] = {
    {10, &btlgrp_jon_jon_81_01_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_01_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_01_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_82 picks one of its formations by weight (10:10:10) on stage stg_00_3: Dark Bristle x2; Dark
 * Bristle, Arantula x3; Dark Bristle x2, Spunia.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_82_off[] = {
    {10, &btlgrp_jon_jon_81_02_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_02_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_02_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_83 picks one of its formations by weight (10:10:10) on stage stg_00_3: Arantula x4; Arantula x2,
 * Piranha Plant; Arantula x2, Dark Bristle.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_83_off[] = {
    {10, &btlgrp_jon_jon_81_03_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_03_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_03_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_84 picks one of its formations by weight (10:10:10) on stage stg_00_3: Piranha Plant x4; Piranha
 * Plant x2, Dark Bristle; Piranha Plant, Spunia x3.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_84_off[] = {
    {10, &btlgrp_jon_jon_81_04_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_04_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_04_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_85 picks one of its formations by weight (10:10:10) on stage stg_00_3: Spunia x4; Spunia x3;
 * Spunia x5.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_85_off[] = {
    {10, &btlgrp_jon_jon_81_05_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_05_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_05_off_3, &jon_btlstage_data[1]},
    {0},
};

/**
 * BTLNO_JON_91 picks one of its formations by weight (10:10:10) on stage stg_00_3: Elite Wizzerd x5; Elite
 * Wizzerd, Poison Puff x2; Elite Wizzerd x2, Swampire.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_91_off[] = {
    {10, &btlgrp_jon_jon_91_01_off_1, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_01_off_2, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_01_off_3, &jon_btlstage_data[2]},
    {0},
};

/**
 * BTLNO_JON_92 picks one of its formations by weight (10:10:10) on stage stg_00_3: Poison Puff x4; Poison
 * Puff x3, Bob-ulk; Poison Puff x2, Swampire, Amazy Dayzee.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_92_off[] = {
    {10, &btlgrp_jon_jon_91_02_off_1, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_02_off_2, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_02_off_3, &jon_btlstage_data[2]},
    {0},
};

/**
 * BTLNO_JON_93 picks one of its formations by weight (10:10:10) on stage stg_00_3: Bob-ulk x4; Bob-ulk x2,
 * Elite Wizzerd; Bob-ulk x3, Amazy Dayzee.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_93_off[] = {
    {10, &btlgrp_jon_jon_91_03_off_1, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_03_off_2, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_03_off_3, &jon_btlstage_data[2]},
    {0},
};

/**
 * BTLNO_JON_94 picks one of its formations by weight (10:10:10) on stage stg_00_3: Swampire x5; Swampire,
 * Poison Puff x2; Swampire, Elite Wizzerd, Amazy Dayzee x2.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_94_off[] = {
    {10, &btlgrp_jon_jon_91_04_off_1, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_04_off_2, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_04_off_3, &jon_btlstage_data[2]},
    {0},
};

/**
 * BTLNO_JON_95 picks one of its formations by weight (10:10:10) on stage stg_00_3: Amazy Dayzee x5; Amazy
 * Dayzee x5; Amazy Dayzee x5.
 */
NpcBattleSelect btlgrpsel_jon_btlno_jon_95_off[] = {
    {10, &btlgrp_jon_jon_91_05_off_1, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_05_off_2, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_05_off_3, &jon_btlstage_data[2]},
    {0},
};

/** BTLNO_JON_100_ZONBABA always fights Bonetail on stage stg_00_3. */
NpcBattleSelect btlgrpsel_jon_btlno_jon_100_zonbaba_off[] = {
    {10, &btlgrp_jon_jon_100_01_off_1, &jon_btlstage_data[3]},
    {0},
};

NpcSetupBattleInfo btlsetup_jon_tbl[] = {
    {
        "マップstg_00_3", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_test1_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ヤミクリボー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_01_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ヒンニャー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_02_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ハンニャー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_03_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ホネノコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_04_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{5, 5}, {0}, {0}, {0}, {10, 10}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "チョロボン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_05_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ヤミパタクリ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_11_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "モノクロシンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_12_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "サンボ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_13_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "モノクロクロクモーン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_14_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "パイダース", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_15_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ヤミトゲクリ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_21_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ボロドー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_22_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ジュゲム", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_23_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ボムへい", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_24_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "テレサ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_25_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{5, 5}, {5, 5}, {10, 10}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ヤミノコノコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_31_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ハイパーシンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_32_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "パタメット", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_33_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ウラノコ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_34_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "フラワーチョロボン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_35_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ヤミパタパタ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_41_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ヘビーボム", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_42_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {10, 10}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "バブル", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_43_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "サンボマミー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_44_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "パタトゲメット", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_45_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "バッジボロドー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_51_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ブリザード", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_52_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "パープルテレサ", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_53_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "バーストワンワン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_54_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "シンエモン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_55_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ハイパージュゲム", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_61_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "カロン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_62_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "スーパーマホーン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_63_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "アイスパックン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_64_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ダークキーパー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_65_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マホーン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_71_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "トゲノコエース", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_72_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ファントム", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_73_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "バサバサチュルル", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_74_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ワンワン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_75_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ヘンニャー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_81_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ヤミトゲダルマー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_82_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "チュランタラー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_83_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "パックンフラワー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_84_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ヘンニャー", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_85_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "マホーンカスタム", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_91_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ドクガッスン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_92_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ジャイアントボム", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_93_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "バサバサグリーン", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_94_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "きらめくパンジーさん", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_95_off, // select
        NPC_BATTLE_NO_BUMP_ATTACK, // battleFlags
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        "ゾンババ", // name
        "jon_06", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_100_zonbaba_off, // select
#if VERSION <= VERSION_DEMO_JP
        NPC_BATTLE_MAX_COUNT | NPC_BATTLE_STORY,
#else
        (NPC_BATTLE_NO_ESCAPE | NPC_BATTLE_NO_PARTY_ENTRY | NPC_BATTLE_STORY), // battleFlags
#endif
        0, // randomMode
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // randomRanges
        "BGM_BOSS_STG1_GONBABA1", // bgmName
    },
    {0},
};

NpcTerritoryInfo btl_jon_setup_no_tbl[] = {
    {"BTLNO_JON_TEST1", 0},
    {"BTLNO_JON_01", 1},
    {"BTLNO_JON_02", 2},
    {"BTLNO_JON_03", 3},
    {"BTLNO_JON_04", 4},
    {"BTLNO_JON_05", 5},
    {"BTLNO_JON_11", 6},
    {"BTLNO_JON_12", 7},
    {"BTLNO_JON_13", 8},
    {"BTLNO_JON_14", 9},
    {"BTLNO_JON_15", 10},
    {"BTLNO_JON_21", 11},
    {"BTLNO_JON_22", 12},
    {"BTLNO_JON_23", 13},
    {"BTLNO_JON_24", 14},
    {"BTLNO_JON_25", 15},
    {"BTLNO_JON_31", 16},
    {"BTLNO_JON_32", 17},
    {"BTLNO_JON_33", 18},
    {"BTLNO_JON_34", 19},
    {"BTLNO_JON_35", 20},
    {"BTLNO_JON_41", 21},
    {"BTLNO_JON_42", 22},
    {"BTLNO_JON_43", 23},
    {"BTLNO_JON_44", 24},
    {"BTLNO_JON_45", 25},
    {"BTLNO_JON_51", 26},
    {"BTLNO_JON_52", 27},
    {"BTLNO_JON_53", 28},
    {"BTLNO_JON_54", 29},
    {"BTLNO_JON_55", 30},
    {"BTLNO_JON_61", 31},
    {"BTLNO_JON_62", 32},
    {"BTLNO_JON_63", 33},
    {"BTLNO_JON_64", 34},
    {"BTLNO_JON_65", 35},
    {"BTLNO_JON_71", 36},
    {"BTLNO_JON_72", 37},
    {"BTLNO_JON_73", 38},
    {"BTLNO_JON_74", 39},
    {"BTLNO_JON_75", 40},
    {"BTLNO_JON_81", 41},
    {"BTLNO_JON_82", 42},
    {"BTLNO_JON_83", 43},
    {"BTLNO_JON_84", 44},
    {"BTLNO_JON_85", 45},
    {"BTLNO_JON_91", 46},
    {"BTLNO_JON_92", 47},
    {"BTLNO_JON_93", 48},
    {"BTLNO_JON_94", 49},
    {"BTLNO_JON_95", 50},
    {"BTLNO_JON_100_ZONBABA", 51},
    {0},
};
