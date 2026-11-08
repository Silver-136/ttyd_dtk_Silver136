#pragma once

// Work slots of unit_barriern_custom, shared by its copies in the aji and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SATELLITE_COUNT 0   // number of satellites: init_event spawns that many (setup value, 0 in every aji setup); unison_phase_event stores the count after spawning up to 2 more
#define UNIT_WORK_WAWAWA_ATTACK 1     // 0 during wakka_attack_event, 1 during wawawa_attack_event; 1 makes the ring blurred, faster (jump speed 7, scale step 0.25) and hidden after landing
#define UNIT_WORK_RINGS_DONE 2        // rings of wawawa_attack_event that have finished: reset to 0, wawawa_attack_event_sub adds 1, the attack waits until it reaches 5
#define UNIT_WORK_BARRIER_EFFECT 3    // effSyuryouEntry barrier effect shown while any satellite lives (EffectEntry*, 0 none); _check_satellite creates, moves and deletes it
#define UNIT_WORK_SATELLITE 4         // first of four slots (4-7): unit id of satellite n (entry_barriern_satellite), -1 none
#define UNIT_WORK_SATELLITE_SPLINE 10 // first of four slots (10-13): SatelliteSpline* moving satellite n in or out (barriernSatelliteTo_Init allocates; _Main frees it after 60 frames)

// BattleUnitPart.work slots (btlevtcmd_Get/SetPartsWork)
#define PARTS_WORK_TARGET_UNIT 0 // part 1, first of five slots (0-4): target unit of each ring (wakka_attack_event uses PARTS_WORK_TARGET_UNIT only)
#define PARTS_WORK_TARGET_PART 5 // part 1, first of five slots (5-9): target part of each ring
#define PARTS_WORK_TARGET_HIT 10 // part 1, first of five slots (10-14): 1 once a ring has hit that target; the last ring at a target that was hit bypasses the pre-damage check but still calls CheckDamage (wawawa_attack_event_sub_last)
#define PARTS_WORK_RING_SFX 15   // on ring parts 7-11: id of the ring's flight sound (SFX_ENM_BARRI_ATTACK7/8), switched off when the ring lands
