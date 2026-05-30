#include "sequence/seq_battle.h"

void seq_battleExit(void) {
    extern void battle_exit(void);
    battle_exit();
}


u8 seq_battleMain(int param_1) {
    return 0;
}


s32 battle_init(void) {
    return 0;
}


void battle_exit(void) {
}


u8 seq_battleInit(void) {
    return 0;
}
