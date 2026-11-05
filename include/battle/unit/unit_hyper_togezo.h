#pragma once

// Work slots of unit_hyper_togezo, shared by its copies in the jon, tou2 and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_GUARD 0      // 1 while guarding (guard_event and normal_attack_event set it; guard_cancel_event clears it); attack_event cancels it only after flipped recovery and item-use checks
#define UNIT_WORK_SPINNING 1   // 1 keeps the sibling loop that rolls it during normal_attack_event turning; 0 ends it
#define UNIT_WORK_SPIN_SPEED 2 // degrees per frame of that roll (+-36, or +-18 slowing by 1 every 2-3 frames; times the face direction)
#define UNIT_WORK_CHARGED 3    // 1 when charge is selected; after recovery, item-use and guard checks, attack_event clears it and uses normal_attack_event
