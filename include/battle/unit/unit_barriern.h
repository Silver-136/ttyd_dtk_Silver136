#pragma once

// Work slots of unit_barriern, shared by its copies in the aji, mri and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SATELLITE_COUNT 0   // number of satellites: the setup value is how many init_event spawns; the split event stores the new count
#define UNIT_WORK_WAWAWA_ATTACK 1     // 1 during wawawa_attack_event (barrier and satellites attack together), 0 during wakka_attack_event; the hit events pick blur and part speed by it
#define UNIT_WORK_SHOTS_DONE 2        // shots of the wawawa attack that have finished; the attack waits until it reaches 3
#define UNIT_WORK_BARRIER_EFFECT 3    // barrier EffectEntry (effSyuryouEntry) while a satellite is alive; deleted when none is left (SFX_ENM_BARRI_BARRIER_BREAK1)
#define UNIT_WORK_SATELLITE 4         // first of the satellite unit ids (slots 4..5), -1 = none
#define UNIT_WORK_SATELLITE_SPLINE 10 // first of the per-satellite SatelliteSpline pointers (slot 10 + n) of a move in progress; freed after 60 frames

// BattleUnitPart.work slots (btlevtcmd_Get/SetPartsWork)
#define PARTS_WORK_TARGET_UNIT 0 // part 1: first of the target unit ids (slots 0-4) of the current attack
#define PARTS_WORK_TARGET_PART 5 // part 1: first of the target part ids (slots 5-9), paired with PARTS_WORK_TARGET_UNIT
#define PARTS_WORK_TARGET_HIT 10 // part 1: first of the per-target hit flags (slots 10-14): 1 once that target was hit; the last shot then hits it without a pre-damage check
#define PARTS_WORK_SFX 15        // shot parts (7..): id of the SFX_ENM_BARRI_ATTACK7/8 sound, stopped when the shot ends
