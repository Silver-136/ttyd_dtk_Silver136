#include "battle/battle_status_icon.h"

void* memset(void*, int, unsigned long);

void BattleStatusIconDelete(void) {
}

void BattleStatusIconInit(BattleWorkUnit* unit) {
    memset((void*)((s32)unit + 0x348), 0, 0x7A0);
}
