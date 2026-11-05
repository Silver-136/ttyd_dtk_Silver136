#pragma once

// Work slots of unit_atmic_teresa, shared by its copies in the jin and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_FACE_HIDDEN 0 // 1 while it hides its face (kaokakusi_event, pose_table_kaokakusi); surprise_event and breath_event reveal it (0, pose_table); attack_event picks surprise/breath while 1
