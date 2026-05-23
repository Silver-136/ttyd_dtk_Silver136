#include "sequence/seq_battle.h"

void seq_battleExit(void) {
    extern void battle_exit(void);
    battle_exit();
}
