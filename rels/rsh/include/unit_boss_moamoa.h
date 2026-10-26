#pragma once

// Work slots of the battle Smorg units (unit_boss_moamoa and its tentacles and mouth), which read each other's slots

// The body (unit_boss_moamoa)
#define MOAMOA_UNIT_WORK_LIMB_HIT 0      // an attacking limb sets 0 at its attack's start and 1 after its hits;
                                         // evt_attack_other_arm waits for it
#define MOAMOA_UNIT_WORK_REGROW_TURNS 1  // turns until the lost limbs grow back (2); phase_event counts down only while
                                         // slots 3-6 are all -1, then resets it to 2 and runs moamoa_tentacl_on or
                                         // moamoa_mouth_on
#define MOAMOA_UNIT_WORK_MOUTH_FORM 2    // 0 tentacle form, 1 mouth form (phase_event sets 1 at regrow time once
                                         // HP <= 50 % of max); picks which limbs regrow and which ones dead_event removes
#define MOAMOA_UNIT_WORK_TENTACLE_C 3    // unit id of tentacle_c (-1 once gone)
#define MOAMOA_UNIT_WORK_TENTACLE_B 4    // unit id of tentacle_b (-1 once gone)
#define MOAMOA_UNIT_WORK_TENTACLE_A 5    // unit id of tentacle_a (-1 once gone)
#define MOAMOA_UNIT_WORK_MOUTH 6         // unit id of the mouth (-1 none; the mouth's dead_event clears it)

// The tentacles and the mouth
#define MOAMOA_LIMB_WORK_BODY 0          // unit id of the body that spawned it
