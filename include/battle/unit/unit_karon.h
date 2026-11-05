#pragma once

// Work slots of unit_karon, shared by its copies in the jon, las and tst RELs.

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SPIN_DONE 0       // 1 ends the sibling loop that spins the thrown bone part
#define UNIT_WORK_REWARD_GIVEN 1    // 1 once coins, exp and star points were paid out; dead_event and fake_dead_event pay only while 0
#define UNIT_WORK_REBIRTH_TURNS 2   // turns until it rebuilds itself: fake_dead_event sets 2-4, rebirth_event counts down and at 0 revives with full HP; attack_event runs rebirth_event while >= 1
#define UNIT_WORK_LINK_HIT 3        // 1 once a bone of the link attack hit; the third bone then hits without the avoid check
#define UNIT_WORK_LINK_BONE_COUNT 4 // bones thrown so far in the link attack (each link_attack_event_sub adds 1); the third sets -1 when done and link_attack_event waits for -1
