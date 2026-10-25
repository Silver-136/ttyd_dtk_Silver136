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

BattleStageObjectData jon_btlobject_data[] = {
    {"B_0", 2, 2, {0}, 0, 60, {0}},
    {"jon_0", 2, 3, {0}, 0, 60, {0}},
    {"B_1", 2, 2, {0}, 0, 60, {0}},
    {"jon_1", 2, 3, {0}, 0, 60, {0}},
    {"B_2", 2, 2, {0}, 0, 60, {0}},
    {"jon_2", 2, 3, {0}, 0, 60, {0}},
    {"B_6", 2, 2, {0}, 0, 60, {0}},
    {"jon_6", 2, 3, {0}, 0, 60, {0}},
};

BattleStageData jon_btlstage_data[] = {
    {
        "bti_01", // mGlobalStageDataDir
        "stg_00_3", // mSpecificStageDataDir
        2, // mNumObjects
        jon_btlobject_data, // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_00_3", // mSpecificStageDataDir
        2, // mNumObjects
        &jon_btlobject_data[2], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_00_3", // mSpecificStageDataDir
        2, // mNumObjects
        &jon_btlobject_data[4], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
    {
        "bti_01", // mGlobalStageDataDir
        "stg_00_3", // mSpecificStageDataDir
        2, // mNumObjects
        &jon_btlobject_data[6], // mObjects
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, NULL, {0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgAWeapon
        {NULL, 0, {0}, 0, NULL, 100, 0, 0, 2, 0.0f, 0, 0, 0, 0, weaponGetPowerDefault, {1, 0, 0, 0, 0, 0, 0, 0}, NULL, {0}, 0x2100060, 0x20000000, 0, 26, 3, 2, NULL, 0, 0x7FF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}}, // bgBWeapon
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // initEvt..bgCEvt
        0, 0, 0, 0, // reserved_1B0..disableBgBDestroy
    },
};

BattleUnitSetup btlparty_jon_jon_100_01_off_1[] = {
    {
        &unit_boss_zonbaba, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {365.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_zonbaba, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_100_01_off_1 = {
    1, // numEnemies
    btlparty_jon_jon_100_01_off_1, // enemyData
    0, 0, // heldItemWeight..randomItemWeight
    100, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_01_01_off_1[] = {
    {
        &unit_yami_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_kuriboo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_01_01_off_1 = {
    1, // numEnemies
    btlparty_jon_jon_01_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_01_01_off_2[] = {
    {
        &unit_yami_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_kuriboo, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_01_01_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_01_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_01_01_off_3[] = {
    {
        &unit_yami_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_kuriboo, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_01_01_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_01_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_01_02_off_1[] = {
    {
        &unit_hinnya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_01_02_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_01_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_01_02_off_2[] = {
    {
        &unit_hinnya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_yami_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_kuriboo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_01_02_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_01_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_01_02_off_3[] = {
    {
        &unit_hinnya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_01_02_off_3 = {
    5, // numEnemies
    btlparty_jon_jon_01_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_01_03_off_1[] = {
    {
        &unit_hannya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_01_03_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_01_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_01_03_off_2[] = {
    {
        &unit_hannya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_yami_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_kuriboo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_01_03_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_01_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_01_03_off_3[] = {
    {
        &unit_hannya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_hannya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hannya, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_01_03_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_01_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_01_04_off_1[] = {
    {
        &unit_honenoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_01_04_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_01_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_01_04_off_2[] = {
    {
        &unit_honenoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_yami_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_kuriboo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_01_04_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_01_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_01_04_off_3[] = {
    {
        &unit_honenoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
    {
        &unit_hinnya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hinnya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_01_04_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_01_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_01_05_off_1[] = {
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_01_05_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_01_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_01_05_off_2[] = {
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_yami_kuriboo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_kuriboo, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_01_05_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_01_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_01_05_off_3[] = {
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
    {
        &unit_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_chorobon, // itemDropTable
    },
    {
        &unit_honenoko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_honenoko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_01_05_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_01_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_11_01_off_1[] = {
    {
        &unit_yami_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_yami_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_11_01_off_1 = {
    2, // numEnemies
    btlparty_jon_jon_11_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_11_01_off_2[] = {
    {
        &unit_yami_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_piders, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_11_01_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_11_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_11_01_off_3[] = {
    {
        &unit_yami_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_sambo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_sambo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_11_01_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_11_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_11_02_off_1[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_11_02_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_11_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_11_02_off_2[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_yami_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_11_02_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_11_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_11_02_off_3[] = {
    {
        &unit_monochrome_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_piders, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_11_02_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_11_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_11_03_off_1[] = {
    {
        &unit_sambo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_sambo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_sambo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_sambo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_11_03_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_11_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_11_03_off_2[] = {
    {
        &unit_sambo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_11_03_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_11_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_11_03_off_3[] = {
    {
        &unit_sambo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_piders, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_sambo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_11_03_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_11_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_11_04_off_1[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_11_04_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_11_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_11_04_off_2[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_sambo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_11_04_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_11_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_11_04_off_3[] = {
    {
        &unit_monochrome_kurokumorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_monochrome_kurokumorn, // itemDropTable
    },
    {
        &unit_yami_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_11_04_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_11_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_11_05_off_1[] = {
    {
        &unit_piders, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_piders, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_11_05_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_11_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_11_05_off_2[] = {
    {
        &unit_piders, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_yami_patakuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_11_05_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_11_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_11_05_off_3[] = {
    {
        &unit_piders, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_sambo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
    {
        &unit_piders, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_piders, // itemDropTable
    },
    {
        &unit_sambo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_11_05_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_11_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_21_01_off_1[] = {
    {
        &unit_yami_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
    {
        &unit_yami_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
    {
        &unit_yami_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_21_01_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_21_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_21_01_off_2[] = {
    {
        &unit_yami_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
    {
        &unit_yami_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
    {
        &unit_bomhei, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_21_01_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_21_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_21_01_off_3[] = {
    {
        &unit_yami_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
    {
        &unit_yami_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_21_01_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_21_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_21_02_off_1[] = {
    {
        &unit_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_21_02_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_21_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_21_02_off_2[] = {
    {
        &unit_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_yami_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_21_02_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_21_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_21_02_off_3[] = {
    {
        &unit_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_21_02_off_3 = {
    5, // numEnemies
    btlparty_jon_jon_21_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_21_03_off_1[] = {
    {
        &unit_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_jyugem, // itemDropTable
    },
    {
        &unit_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_jyugem, // itemDropTable
    },
    {
        &unit_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_jyugem, // itemDropTable
    },
    {
        &unit_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_jyugem, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_21_03_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_21_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_21_03_off_2[] = {
    {
        &unit_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_jyugem, // itemDropTable
    },
    {
        &unit_yami_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_21_03_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_21_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_21_03_off_3[] = {
    {
        &unit_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_jyugem, // itemDropTable
    },
    {
        &unit_bomhei, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_bomhei, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_jyugem, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_21_03_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_21_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_21_04_off_1[] = {
    {
        &unit_bomhei, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_bomhei, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_bomhei, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_bomhei, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_21_04_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_21_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_21_04_off_2[] = {
    {
        &unit_bomhei, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_yami_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
    {
        &unit_bomhei, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_21_04_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_21_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_21_04_off_3[] = {
    {
        &unit_bomhei, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_bomhei, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bomhei, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_21_04_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_21_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_21_05_off_1[] = {
    {
        &unit_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_21_05_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_21_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_21_05_off_2[] = {
    {
        &unit_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_yami_togekuri, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_togekuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_21_05_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_21_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_21_05_off_3[] = {
    {
        &unit_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_teresa, // itemDropTable
    },
    {
        &unit_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
    {
        &unit_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_borodo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_21_05_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_21_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_31_01_off_1[] = {
    {
        &unit_yami_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_noko, // itemDropTable
    },
    {
        &unit_yami_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_noko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_31_01_off_1 = {
    2, // numEnemies
    btlparty_jon_jon_31_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_31_01_off_2[] = {
    {
        &unit_yami_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_noko, // itemDropTable
    },
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_31_01_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_31_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_31_01_off_3[] = {
    {
        &unit_yami_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_noko, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_31_01_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_31_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_31_02_off_1[] = {
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_31_02_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_31_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_31_02_off_2[] = {
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_ura_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_31_02_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_31_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_31_02_off_3[] = {
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_31_02_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_31_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_31_03_off_1[] = {
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_31_03_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_31_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_31_03_off_2[] = {
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_ura_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_ura_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_31_03_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_31_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_31_03_off_3[] = {
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_31_03_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_31_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_31_04_off_1[] = {
    {
        &unit_ura_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_ura_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_ura_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_ura_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_31_04_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_31_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_31_04_off_2[] = {
    {
        &unit_ura_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_yami_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_noko, // itemDropTable
    },
    {
        &unit_ura_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_31_04_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_31_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_31_04_off_3[] = {
    {
        &unit_ura_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_ura_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ura_noko, // itemDropTable
    },
    {
        &unit_patamet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patamet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_31_04_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_31_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_31_05_off_1[] = {
    {
        &unit_flower_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_31_05_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_31_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_31_05_off_2[] = {
    {
        &unit_flower_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_flower_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_yami_noko, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_noko, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_31_05_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_31_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_31_05_off_3[] = {
    {
        &unit_flower_chorobon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_flower_chorobon, // itemDropTable
    },
    {
        &unit_hyper_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_31_05_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_31_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_41_01_off_1[] = {
    {
        &unit_yami_pata, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_yami_pata, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_yami_pata, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_yami_pata, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_41_01_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_41_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_41_01_off_2[] = {
    {
        &unit_yami_pata, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_yami_pata, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_41_01_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_41_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_41_01_off_3[] = {
    {
        &unit_yami_pata, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_yami_pata, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_41_01_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_41_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_41_02_off_1[] = {
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_41_02_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_41_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_41_02_off_2[] = {
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_41_02_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_41_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_41_02_off_3[] = {
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_41_02_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_41_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_41_03_off_1[] = {
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_41_03_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_41_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_41_03_off_2[] = {
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_yami_pata, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_41_03_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_41_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_41_03_off_3[] = {
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_heavy_bom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_heavy_bom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_41_03_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_41_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_41_04_off_1[] = {
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_41_04_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_41_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_41_04_off_2[] = {
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_41_04_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_41_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_41_04_off_3[] = {
    {
        &unit_sambo_mummy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sambo_mummy, // itemDropTable
    },
    {
        &unit_yami_pata, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yami_patakuri, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_41_04_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_41_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_41_05_off_1[] = {
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_41_05_off_1 = {
    2, // numEnemies
    btlparty_jon_jon_41_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_41_05_off_2[] = {
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_41_05_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_41_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_41_05_off_3[] = {
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_patatogemet, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_patatogemet, // itemDropTable
    },
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
    {
        &unit_bubble, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bubble, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_41_05_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_41_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_51_01_off_1[] = {
    {
        &unit_badge_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
    {
        &unit_badge_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
    {
        &unit_badge_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_51_01_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_51_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_51_01_off_2[] = {
    {
        &unit_badge_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
    {
        &unit_badge_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
    {
        &unit_bllizard, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_51_01_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_51_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_51_01_off_3[] = {
    {
        &unit_badge_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
    {
        &unit_badge_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_51_01_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_51_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_51_02_off_1[] = {
    {
        &unit_bllizard, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_bllizard, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_bllizard, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_bllizard, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_51_02_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_51_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_51_02_off_2[] = {
    {
        &unit_bllizard, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_bllizard, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_51_02_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_51_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_51_02_off_3[] = {
    {
        &unit_bllizard, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_51_02_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_51_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_51_03_off_1[] = {
    {
        &unit_purple_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_51_03_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_51_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_51_03_off_2[] = {
    {
        &unit_purple_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_badge_borodo, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_badge_borodo, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_51_03_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_51_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_51_03_off_3[] = {
    {
        &unit_purple_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_burst_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_51_03_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_51_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_51_04_off_1[] = {
    {
        &unit_burst_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
    {
        &unit_burst_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
    {
        &unit_burst_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_51_04_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_51_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_51_04_off_2[] = {
    {
        &unit_burst_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_burst_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_51_04_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_51_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_51_04_off_3[] = {
    {
        &unit_burst_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
    {
        &unit_bllizard, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
    {
        &unit_bllizard, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_bllizard, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_51_04_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_51_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_51_05_off_1[] = {
    {
        &unit_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_51_05_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_51_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_51_05_off_2[] = {
    {
        &unit_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_burst_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_burst_wanwan, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_51_05_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_51_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_51_05_off_3[] = {
    {
        &unit_sinemon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_sinemon, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
    {
        &unit_purple_teresa, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_purple_teresa, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_51_05_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_51_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_61_01_off_1[] = {
    {
        &unit_hyper_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
    {
        &unit_hyper_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
    {
        &unit_hyper_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_61_01_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_61_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_61_01_off_2[] = {
    {
        &unit_hyper_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
    {
        &unit_hyper_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
    {
        &unit_dark_keeper, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_61_01_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_61_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_61_01_off_3[] = {
    {
        &unit_hyper_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
    {
        &unit_super_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_61_01_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_61_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_61_02_off_1[] = {
    {
        &unit_karon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_61_02_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_61_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_61_02_off_2[] = {
    {
        &unit_karon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_dark_keeper, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_61_02_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_61_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_61_02_off_3[] = {
    {
        &unit_karon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_ice_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_ice_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_61_02_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_61_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_61_03_off_1[] = {
    {
        &unit_super_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
    {
        &unit_super_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
    {
        &unit_super_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_61_03_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_61_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_61_03_off_2[] = {
    {
        &unit_super_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
    {
        &unit_dark_keeper, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
    {
        &unit_super_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_61_03_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_61_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_61_03_off_3[] = {
    {
        &unit_super_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_super_mahorn, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
    {
        &unit_karon, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_karon, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_61_03_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_61_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_61_04_off_1[] = {
    {
        &unit_ice_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_ice_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_ice_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_ice_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_61_04_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_61_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_61_04_off_2[] = {
    {
        &unit_ice_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_hyper_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
    {
        &unit_hyper_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_61_04_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_61_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_61_04_off_3[] = {
    {
        &unit_ice_pakkun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_ice_pakkun, // itemDropTable
    },
    {
        &unit_dark_keeper, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_61_04_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_61_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_61_05_off_1[] = {
    {
        &unit_dark_keeper, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
    {
        &unit_dark_keeper, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
    {
        &unit_dark_keeper, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_61_05_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_61_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_61_05_off_2[] = {
    {
        &unit_dark_keeper, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
    {
        &unit_hyper_jyugem, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hyper_jyugem, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_61_05_off_2 = {
    2, // numEnemies
    btlparty_jon_jon_61_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_61_05_off_3[] = {
    {
        &unit_dark_keeper, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
    {
        &unit_dark_keeper, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dark_keeper, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_61_05_off_3 = {
    2, // numEnemies
    btlparty_jon_jon_61_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_71_01_off_1[] = {
    {
        &unit_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
    {
        &unit_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
    {
        &unit_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_71_01_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_71_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_71_01_off_2[] = {
    {
        &unit_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
    {
        &unit_togenoko_ace, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_71_01_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_71_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_71_01_off_3[] = {
    {
        &unit_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
    {
        &unit_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_71_01_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_71_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_71_02_off_1[] = {
    {
        &unit_togenoko_ace, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_togenoko_ace, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_togenoko_ace, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_71_02_off_1 = {
    3, // numEnemies
    btlparty_jon_jon_71_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_71_02_off_2[] = {
    {
        &unit_togenoko_ace, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_71_02_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_71_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_71_02_off_3[] = {
    {
        &unit_togenoko_ace, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_togenoko_ace, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_71_02_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_71_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default2, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_71_03_off_1[] = {
    {
        &unit_phantom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_71_03_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_71_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_71_03_off_2[] = {
    {
        &unit_phantom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_phantom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_71_03_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_71_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_71_03_off_3[] = {
    {
        &unit_phantom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_phantom, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_togenoko_ace, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_71_03_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_71_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_71_04_off_1[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 130.0f, 20.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_71_04_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_71_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_71_04_off_2[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
    {
        &unit_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_71_04_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_71_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_71_04_off_3[] = {
    {
        &unit_basabasa_chururu, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_togenoko_ace, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_basabasa_chururu, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_chururu, // itemDropTable
    },
    {
        &unit_togenoko_ace, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_71_04_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_71_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default2, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_71_05_off_1[] = {
    {
        &unit_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_71_05_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_71_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_71_05_off_2[] = {
    {
        &unit_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_togenoko_ace, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
    {
        &unit_togenoko_ace, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_togenoko_ace, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_71_05_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_71_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_71_05_off_3[] = {
    {
        &unit_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_wanwan, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_wanwan, // itemDropTable
    },
    {
        &unit_mahorn, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_71_05_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_71_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_81_01_off_1[] = {
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_81_01_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_81_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_81_01_off_2[] = {
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_churantalar, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_churantalar, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_81_01_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_81_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_81_01_off_3[] = {
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_pakkun_flower, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
    {
        &unit_yamitogedaruma, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_81_01_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_81_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_81_02_off_1[] = {
    {
        &unit_yamitogedaruma, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
    {
        &unit_yamitogedaruma, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_81_02_off_1 = {
    2, // numEnemies
    btlparty_jon_jon_81_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_81_02_off_2[] = {
    {
        &unit_yamitogedaruma, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
    {
        &unit_churantalar, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_churantalar, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_churantalar, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_81_02_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_81_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_81_02_off_3[] = {
    {
        &unit_yamitogedaruma, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
    {
        &unit_yamitogedaruma, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_81_02_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_81_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_81_03_off_1[] = {
    {
        &unit_churantalar, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_churantalar, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_churantalar, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_churantalar, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_81_03_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_81_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_81_03_off_2[] = {
    {
        &unit_churantalar, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_pakkun_flower, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
    {
        &unit_churantalar, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_81_03_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_81_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_81_03_off_3[] = {
    {
        &unit_churantalar, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_churantalar, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_churantalar, // itemDropTable
    },
    {
        &unit_yamitogedaruma, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_81_03_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_81_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_81_04_off_1[] = {
    {
        &unit_pakkun_flower, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
    {
        &unit_pakkun_flower, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
    {
        &unit_pakkun_flower, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
    {
        &unit_pakkun_flower, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_81_04_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_81_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default5, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_81_04_off_2[] = {
    {
        &unit_pakkun_flower, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
    {
        &unit_yamitogedaruma, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_yamitogedaruma, // itemDropTable
    },
    {
        &unit_pakkun_flower, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_81_04_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_81_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default5, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_81_04_off_3[] = {
    {
        &unit_pakkun_flower, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_pakkun_flower, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_81_04_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_81_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default, // hpDropTable
    battle_flower_drop_param_default5, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_81_05_off_1[] = {
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_81_05_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_81_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_81_05_off_2[] = {
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_81_05_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_81_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_81_05_off_3[] = {
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
    {
        &unit_hennya, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_hennya, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_81_05_off_3 = {
    5, // numEnemies
    btlparty_jon_jon_81_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_91_01_off_1[] = {
    {
        &unit_mahorn_custom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_mahorn_custom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_mahorn_custom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_mahorn_custom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_mahorn_custom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_91_01_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_91_01_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_91_01_off_2[] = {
    {
        &unit_mahorn_custom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_dokugassun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_dokugassun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_91_01_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_91_01_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_91_01_off_3[] = {
    {
        &unit_mahorn_custom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_mahorn_custom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_basabasa_green, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_91_01_off_3 = {
    3, // numEnemies
    btlparty_jon_jon_91_01_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default4, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_91_02_off_1[] = {
    {
        &unit_dokugassun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_dokugassun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_dokugassun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_dokugassun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_91_02_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_91_02_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_91_02_off_2[] = {
    {
        &unit_dokugassun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_dokugassun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_dokugassun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_giant_bomb, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_91_02_off_2 = {
    4, // numEnemies
    btlparty_jon_jon_91_02_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_91_02_off_3[] = {
    {
        &unit_dokugassun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 40.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_basabasa_green, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
    {
        &unit_dokugassun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_91_02_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_91_02_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default3, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_91_03_off_1[] = {
    {
        &unit_giant_bomb, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_giant_bomb, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_giant_bomb, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_giant_bomb, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_91_03_off_1 = {
    4, // numEnemies
    btlparty_jon_jon_91_03_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default5, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_91_03_off_2[] = {
    {
        &unit_giant_bomb, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_giant_bomb, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_mahorn_custom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_91_03_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_91_03_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default5, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_91_03_off_3[] = {
    {
        &unit_giant_bomb, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_giant_bomb, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_giant_bomb, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_giant_bomb, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_91_03_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_91_03_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default5, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_91_04_off_1[] = {
    {
        &unit_basabasa_green, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 130.0f, -20.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
    {
        &unit_basabasa_green, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
    {
        &unit_basabasa_green, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 130.0f, 0.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
    {
        &unit_basabasa_green, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 130.0f, 10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
    {
        &unit_basabasa_green, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 130.0f, 20.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_91_04_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_91_04_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_91_04_off_2[] = {
    {
        &unit_basabasa_green, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
    {
        &unit_dokugassun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
    {
        &unit_dokugassun, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 40.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_dokugassun, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_91_04_off_2 = {
    3, // numEnemies
    btlparty_jon_jon_91_04_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_91_04_off_3[] = {
    {
        &unit_basabasa_green, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {30.0f, 130.0f, -10.0f}, // position
        0, // hitOffsetX
        {1, 0, 0, 0}, // unitWork
        battle_item_basabasa_green, // itemDropTable
    },
    {
        &unit_mahorn_custom, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {70.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_mahorn_custom, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {110.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_91_04_off_3 = {
    4, // numEnemies
    btlparty_jon_jon_91_04_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default4, // hpDropTable
    battle_flower_drop_param_default, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_91_05_off_1[] = {
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_91_05_off_1 = {
    5, // numEnemies
    btlparty_jon_jon_91_05_off_1, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default5, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_91_05_off_2[] = {
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_91_05_off_2 = {
    5, // numEnemies
    btlparty_jon_jon_91_05_off_2, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default5, // fpDropTable
    5, // field_1C
};

BattleUnitSetup btlparty_jon_jon_91_05_off_3[] = {
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {10.0f, 0.0f, -20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {50.0f, 0.0f, -10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {90.0f, 0.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {130.0f, 0.0f, 10.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
    {
        &unit_twinkling_pansy, // mUnitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {170.0f, 0.0f, 20.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        battle_item_twinkling_pansy, // itemDropTable
    },
};

BattleGroupSetup btlgrp_jon_jon_91_05_off_3 = {
    5, // numEnemies
    btlparty_jon_jon_91_05_off_3, // enemyData
    20, // heldItemWeight
    20, // randomItemWeight
    60, // noItemWeight
    battle_heart_drop_param_default3, // hpDropTable
    battle_flower_drop_param_default5, // fpDropTable
    5, // field_1C
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_test1_off[] = {
    {50, &btlgrp_jon_jon_01_01_off_1, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_01_off[] = {
    {10, &btlgrp_jon_jon_01_01_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_01_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_01_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_02_off[] = {
    {10, &btlgrp_jon_jon_01_02_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_02_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_02_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_03_off[] = {
    {10, &btlgrp_jon_jon_01_03_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_03_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_03_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_04_off[] = {
    {10, &btlgrp_jon_jon_01_04_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_04_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_04_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_05_off[] = {
    {10, &btlgrp_jon_jon_01_05_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_05_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_01_05_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_11_off[] = {
    {10, &btlgrp_jon_jon_11_01_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_01_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_01_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_12_off[] = {
    {10, &btlgrp_jon_jon_11_02_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_02_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_02_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_13_off[] = {
    {10, &btlgrp_jon_jon_11_03_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_03_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_03_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_14_off[] = {
    {10, &btlgrp_jon_jon_11_04_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_04_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_04_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_15_off[] = {
    {10, &btlgrp_jon_jon_11_05_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_05_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_11_05_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_21_off[] = {
    {10, &btlgrp_jon_jon_21_01_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_01_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_01_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_22_off[] = {
    {10, &btlgrp_jon_jon_21_02_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_02_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_02_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_23_off[] = {
    {10, &btlgrp_jon_jon_21_03_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_03_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_03_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_24_off[] = {
    {10, &btlgrp_jon_jon_21_04_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_04_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_04_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_25_off[] = {
    {10, &btlgrp_jon_jon_21_05_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_05_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_21_05_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_31_off[] = {
    {10, &btlgrp_jon_jon_31_01_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_01_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_01_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_32_off[] = {
    {10, &btlgrp_jon_jon_31_02_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_02_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_02_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_33_off[] = {
    {10, &btlgrp_jon_jon_31_03_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_03_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_03_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_34_off[] = {
    {10, &btlgrp_jon_jon_31_04_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_04_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_04_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_35_off[] = {
    {10, &btlgrp_jon_jon_31_05_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_05_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_31_05_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_41_off[] = {
    {10, &btlgrp_jon_jon_41_01_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_01_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_01_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_42_off[] = {
    {10, &btlgrp_jon_jon_41_02_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_02_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_02_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_43_off[] = {
    {10, &btlgrp_jon_jon_41_03_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_03_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_03_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_44_off[] = {
    {10, &btlgrp_jon_jon_41_04_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_04_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_04_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_45_off[] = {
    {10, &btlgrp_jon_jon_41_05_off_1, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_05_off_2, jon_btlstage_data},
    {10, &btlgrp_jon_jon_41_05_off_3, jon_btlstage_data},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_51_off[] = {
    {10, &btlgrp_jon_jon_51_01_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_01_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_01_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_52_off[] = {
    {10, &btlgrp_jon_jon_51_02_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_02_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_02_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_53_off[] = {
    {10, &btlgrp_jon_jon_51_03_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_03_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_03_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_54_off[] = {
    {10, &btlgrp_jon_jon_51_04_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_04_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_04_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_55_off[] = {
    {10, &btlgrp_jon_jon_51_05_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_05_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_51_05_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_61_off[] = {
    {10, &btlgrp_jon_jon_61_01_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_01_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_01_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_62_off[] = {
    {10, &btlgrp_jon_jon_61_02_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_02_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_02_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_63_off[] = {
    {10, &btlgrp_jon_jon_61_03_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_03_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_03_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_64_off[] = {
    {10, &btlgrp_jon_jon_61_04_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_04_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_04_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_65_off[] = {
    {10, &btlgrp_jon_jon_61_05_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_05_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_61_05_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_71_off[] = {
    {10, &btlgrp_jon_jon_71_01_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_01_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_01_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_72_off[] = {
    {10, &btlgrp_jon_jon_71_02_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_02_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_02_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_73_off[] = {
    {10, &btlgrp_jon_jon_71_03_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_03_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_03_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_74_off[] = {
    {10, &btlgrp_jon_jon_71_04_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_04_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_04_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_75_off[] = {
    {10, &btlgrp_jon_jon_71_05_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_05_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_71_05_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_81_off[] = {
    {10, &btlgrp_jon_jon_81_01_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_01_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_01_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_82_off[] = {
    {10, &btlgrp_jon_jon_81_02_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_02_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_02_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_83_off[] = {
    {10, &btlgrp_jon_jon_81_03_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_03_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_03_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_84_off[] = {
    {10, &btlgrp_jon_jon_81_04_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_04_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_04_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_85_off[] = {
    {10, &btlgrp_jon_jon_81_05_off_1, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_05_off_2, &jon_btlstage_data[1]},
    {10, &btlgrp_jon_jon_81_05_off_3, &jon_btlstage_data[1]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_91_off[] = {
    {10, &btlgrp_jon_jon_91_01_off_1, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_01_off_2, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_01_off_3, &jon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_92_off[] = {
    {10, &btlgrp_jon_jon_91_02_off_1, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_02_off_2, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_02_off_3, &jon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_93_off[] = {
    {10, &btlgrp_jon_jon_91_03_off_1, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_03_off_2, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_03_off_3, &jon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_94_off[] = {
    {10, &btlgrp_jon_jon_91_04_off_1, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_04_off_2, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_04_off_3, &jon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_95_off[] = {
    {10, &btlgrp_jon_jon_91_05_off_1, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_05_off_2, &jon_btlstage_data[2]},
    {10, &btlgrp_jon_jon_91_05_off_3, &jon_btlstage_data[2]},
    {0},
};

NpcBattleSelect btlgrpsel_jon_btlno_jon_100_zonbaba_off[] = {
    {10, &btlgrp_jon_jon_100_01_off_1, &jon_btlstage_data[3]},
    {0},
};

NpcSetupBattleInfo btlsetup_jon_tbl[] = {
    {
        /*"マップstg_00_3"*/"\x83\x7D\x83\x62\x83\x76\x73\x74\x67\x5F\x30\x30\x5F\x33", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_test1_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ヤミクリボー"*/"\x83\x84\x83\x7E\x83\x4E\x83\x8A\x83\x7B\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_01_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ヒンニャー"*/"\x83\x71\x83\x93\x83\x6A\x83\x83\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_02_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ハンニャー"*/"\x83\x6E\x83\x93\x83\x6A\x83\x83\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_03_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ホネノコ"*/"\x83\x7A\x83\x6C\x83\x6D\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_04_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{5, 5}, {0}, {0}, {0}, {10, 10}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"チョロボン"*/"\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_05_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{5, 5}, {0}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ヤミパタクリ"*/"\x83\x84\x83\x7E\x83\x70\x83\x5E\x83\x4E\x83\x8A", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_11_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"モノクロシンエモン"*/"\x83\x82\x83\x6D\x83\x4E\x83\x8D\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_12_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"サンボ"*/"\x83\x54\x83\x93\x83\x7B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_13_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"モノクロクロクモーン"*/"\x83\x82\x83\x6D\x83\x4E\x83\x8D\x83\x4E\x83\x8D\x83\x4E\x83\x82\x81\x5B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_14_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"パイダース"*/"\x83\x70\x83\x43\x83\x5F\x81\x5B\x83\x58", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_15_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ヤミトゲクリ"*/"\x83\x84\x83\x7E\x83\x67\x83\x51\x83\x4E\x83\x8A", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_21_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {10, 10}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ボロドー"*/"\x83\x7B\x83\x8D\x83\x68\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_22_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ジュゲム"*/"\x83\x57\x83\x85\x83\x51\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_23_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ボムへい"*/"\x83\x7B\x83\x80\x82\xD6\x82\xA2", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_24_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{5, 5}, {5, 5}, {0}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"テレサ"*/"\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_25_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{5, 5}, {5, 5}, {10, 10}, {0}, {5, 5}, {5, 5}, {0}, {0}, {5, 5}, {0}, {5, 5}, {0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ヤミノコノコ"*/"\x83\x84\x83\x7E\x83\x6D\x83\x52\x83\x6D\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_31_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ハイパーシンエモン"*/"\x83\x6E\x83\x43\x83\x70\x81\x5B\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_32_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"パタメット"*/"\x83\x70\x83\x5E\x83\x81\x83\x62\x83\x67", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_33_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ウラノコ"*/"\x83\x45\x83\x89\x83\x6D\x83\x52", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_34_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"フラワーチョロボン"*/"\x83\x74\x83\x89\x83\x8F\x81\x5B\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_35_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ヤミパタパタ"*/"\x83\x84\x83\x7E\x83\x70\x83\x5E\x83\x70\x83\x5E", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_41_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {10, 10}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ヘビーボム"*/"\x83\x77\x83\x72\x81\x5B\x83\x7B\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_42_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {10, 10}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"バブル"*/"\x83\x6F\x83\x75\x83\x8B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_43_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"サンボマミー"*/"\x83\x54\x83\x93\x83\x7B\x83\x7D\x83\x7E\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_44_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"パタトゲメット"*/"\x83\x70\x83\x5E\x83\x67\x83\x51\x83\x81\x83\x62\x83\x67", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_45_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {4, 4}, {0}, {4, 4}, {4, 4}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"バッジボロドー"*/"\x83\x6F\x83\x62\x83\x57\x83\x7B\x83\x8D\x83\x68\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_51_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ブリザード"*/"\x83\x75\x83\x8A\x83\x55\x81\x5B\x83\x68", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_52_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"パープルテレサ"*/"\x83\x70\x81\x5B\x83\x76\x83\x8B\x83\x65\x83\x8C\x83\x54", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_53_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"バーストワンワン"*/"\x83\x6F\x81\x5B\x83\x58\x83\x67\x83\x8F\x83\x93\x83\x8F\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_54_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"シンエモン"*/"\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_55_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ハイパージュゲム"*/"\x83\x6E\x83\x43\x83\x70\x81\x5B\x83\x57\x83\x85\x83\x51\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_61_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"カロン"*/"\x83\x4A\x83\x8D\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_62_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"スーパーマホーン"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x7D\x83\x7A\x81\x5B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_63_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"アイスパックン"*/"\x83\x41\x83\x43\x83\x58\x83\x70\x83\x62\x83\x4E\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_64_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ダークキーパー"*/"\x83\x5F\x81\x5B\x83\x4E\x83\x4C\x81\x5B\x83\x70\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_65_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マホーン"*/"\x83\x7D\x83\x7A\x81\x5B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_71_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"トゲノコエース"*/"\x83\x67\x83\x51\x83\x6D\x83\x52\x83\x47\x81\x5B\x83\x58", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_72_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ファントム"*/"\x83\x74\x83\x40\x83\x93\x83\x67\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_73_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"バサバサチュルル"*/"\x83\x6F\x83\x54\x83\x6F\x83\x54\x83\x60\x83\x85\x83\x8B\x83\x8B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_74_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ワンワン"*/"\x83\x8F\x83\x93\x83\x8F\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_75_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ヘンニャー"*/"\x83\x77\x83\x93\x83\x6A\x83\x83\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_81_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ヤミトゲダルマー"*/"\x83\x84\x83\x7E\x83\x67\x83\x51\x83\x5F\x83\x8B\x83\x7D\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_82_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"チュランタラー"*/"\x83\x60\x83\x85\x83\x89\x83\x93\x83\x5E\x83\x89\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_83_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"パックンフラワー"*/"\x83\x70\x83\x62\x83\x4E\x83\x93\x83\x74\x83\x89\x83\x8F\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_84_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ヘンニャー"*/"\x83\x77\x83\x93\x83\x6A\x83\x83\x81\x5B", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_85_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"マホーンカスタム"*/"\x83\x7D\x83\x7A\x81\x5B\x83\x93\x83\x4A\x83\x58\x83\x5E\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_91_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ドクガッスン"*/"\x83\x68\x83\x4E\x83\x4B\x83\x62\x83\x58\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_92_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ジャイアントボム"*/"\x83\x57\x83\x83\x83\x43\x83\x41\x83\x93\x83\x67\x83\x7B\x83\x80", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_93_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"バサバサグリーン"*/"\x83\x6F\x83\x54\x83\x6F\x83\x54\x83\x4F\x83\x8A\x81\x5B\x83\x93", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_94_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"きらめくパンジーさん"*/"\x82\xAB\x82\xE7\x82\xDF\x82\xAD\x83\x70\x83\x93\x83\x57\x81\x5B\x82\xB3\x82\xF1", // name
        NULL, 0, // mapName..fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_95_off, // select
        0x10000000, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
        "BGM_ZAKO_BATTLE1", // bgmName
    },
    {
        /*"ゾンババ"*/"\x83\x5D\x83\x93\x83\x6F\x83\x6F", // name
        "jon_06", // mapName
        0, // fixedItem
        -1, // swFlag
        NULL, // altSelect
        btlgrpsel_jon_btlno_jon_100_zonbaba_off, // select
        0x20100010, // battleFlags
        0, // field_1C
        {{3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {3, 3}, {1, 1}, {1, 1}, {1, 1}, {1, 1}}, // field_20
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

