#pragma once

// Work slots of the battle Blooper units (unit_boss_gesso and its two arms), which read each other's slots

// The body (unit_boss_gesso)
#define GESSO_UNIT_WORK_STATE 0         // 0 on the ceiling with both arms; 1 fallen after both arms are gone
                                        // (gesso_fall_check; skips its attacks); 2 risen from the floor after 2 turns
                                        // (gesso_ikasumi_attack); back to 0 (arms respawned) after 2 more turns
#define GESSO_UNIT_WORK_LEFT_ARM 1      // unit id of the left arm; -1 once it dies
#define GESSO_UNIT_WORK_RIGHT_ARM 2     // unit id of the right arm; -1 once it dies
#define GESSO_UNIT_WORK_STATE_TURNS 3   // turns spent in state 1 or 2; at 2 phase_event changes the state and resets it
#define GESSO_UNIT_WORK_BUSY 4          // 1 while its damage event runs (3 when the hit kills it), 2 while the arms'
                                        // gesso_fall_check runs, else 0; the arms restart its wait event only at 0
#define GESSO_UNIT_WORK_ARM_TALK_DONE 5 // 1 once the right arm's first attack on Mario showed the tik_02_gs_04 messages

// The arms (unit_boss_gesso_left_arm / _right_arm)
#define GESSO_ARM_WORK_BODY 0           // unit id of the body that spawned it; -1 none
