#pragma once

// Work slots of the battle Kammy (unit_boss_kamec_obaba); Bowser's unit reads whether she is on her broom (GW(9))
#define KAMEC_OBABA_WORK_SKY 0          // 1 while on the broom (airborne, sky poses); dmg_fall_event sets 0; phase_event
                                        // remounts and sets 1
#define KAMEC_OBABA_WORK_SKY_DAMAGE 1   // damage taken while on the broom; above 1 she falls (dmg_fall_event)
#define KAMEC_OBABA_WORK_GROUND_TURNS 2 // turns spent on the ground; phase_event adds 1 and remounts the broom at 3
#define KAMEC_OBABA_WORK_STAFF_EFFECT 3 // EffectEntry of the staff effect (effThunderflareN64Entry) kept on the wand tip
#define KAMEC_OBABA_WORK_MAGIC_EFFECT 4 // EffectEntry of the magic shot (effMagic1N64Entry); eff_magic_get_posX reads its x
