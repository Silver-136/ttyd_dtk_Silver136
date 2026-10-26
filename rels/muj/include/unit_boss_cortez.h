#pragma once

// Work slots of the battle Cortez units (unit_boss_cortez and his four weapons), which read each other's slots

// Cortez (unit_boss_cortez)
#define CORTEZ_UNIT_WORK_SWORD 1                 // unit id of the sword (weapon index 0); -1 once it is broken
#define CORTEZ_UNIT_WORK_SABER 2                 // unit id of the saber (index 1); -1 once it is broken
#define CORTEZ_UNIT_WORK_CLAW 3                  // unit id of the claw (index 2); -1 once it is broken
#define CORTEZ_UNIT_WORK_RAPIER 4                // unit id of the rapier (index 3); -1 once it is broken
#define CORTEZ_UNIT_WORK_MODE 5                  // form: 0 mode 1 (pose_table_mode_1), 1/2/3 mode 2 upper/middle/lower,
                                                 // 4 mode 3 (pose_table_mode_3)
#define CORTEZ_UNIT_WORK_ATTACK_CYCLE 6          // mode-1 attack rotation 0-1-2: 0 and 1 run at1_normal_attack,
                                                 // 2 runs at1_midaregiri_attack
#define CORTEZ_UNIT_WORK_HIT_THIS_TURN 7         // 1 once the rib (part 2) or the bone pile (part 4) was hit (init 1);
                                                 // phase_event clears it and, if it was 0, moves mode 2/3 back up to 1
#define CORTEZ_UNIT_WORK_FORMATION 8             // weapon formation 0-2 rolled each turn; cortez_weapon_get_home_height
                                                 // picks each weapon's home height (20 or 60) from it
#define CORTEZ_UNIT_WORK_AUDIENCE_ATTACK_COUNT 9 // times attack_audience_event ran; it runs only while this is 0, at
                                                 // half HP or less, when _cortez_check_drain_audience finds audience
#define CORTEZ_UNIT_WORK_CHARGED 10              // 1 after at2_hone_charge_event: the next mode-2 attack is
                                                 // at2_hone_rain_attack_event (which clears it); selects the *_charge
                                                 // pose tables
#define CORTEZ_UNIT_WORK_RIB_OPEN_TURNS 11       // turns the rib (part 2) stays open: mode2_rib_open_event and a hit on
                                                 // the rib set 2; phase_event counts down and closes it at 0; 0 = closed

// The weapons (unit_boss_cortez_sword / _saber / _claw / _rapier; their shared events are in unit_boss_cortez_sword.c)
#define CORTEZ_WEAPON_WORK_OWNER 0               // unit id of Cortez (set when he spawns it; -1 = none)
#define CORTEZ_WEAPON_WORK_DOWN_TURNS 1          // turns it lies broken: cortez_weapon_dead_event sets 2,
                                                 // cortez_weapon_phase_event counts down and restores it at 0; its attack
                                                 // event does nothing while nonzero
#define CORTEZ_WEAPON_WORK_INDEX 2               // which weapon it is (sword 0, saber 1, claw 2, rapier 3): picks its
                                                 // home height and the Cortez slot (index + 1) cleared when it breaks
