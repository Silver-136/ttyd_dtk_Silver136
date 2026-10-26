#pragma once

// Work slots of the zako groups (unit_boss_gundan_zako_group1/2) that their caller unit_boss_kanbu3 reads
#define ZAKO_GROUP1_WORK_DYING 1 // dead_event sets 1; unit_boss_kanbu3 waits longer before its own events go on while set
#define ZAKO_GROUP2_WORK_DYING 2 // dead_event sets 1; unit_boss_kanbu3 waits longer before its exit while set
