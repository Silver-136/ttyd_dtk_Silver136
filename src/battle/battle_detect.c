#include "battle/battle_detect.h"

void BattleSamplingEnemyUpdate(void) {
    extern void _btlSamplingEnemy(void);

    _btlSamplingEnemy();
}


void _btlSamplingEnemy(void) {
}


void BattleChoiceSamplingEnemy(void* targetWork, u32 targetWeightingFlags, int* unitIdx, int* param_4) {
}


s32 BattleSamplingEnemy(void* targetWork, void* weapon, int attackerIdx, int enemyBelong, u32 targetClassFlags, u32 targetPropertyFlags, u8 param_7) {
    return 0;
}


void BattleGetFirstAttackUnit(void* battleWork, void** part, void** unit) {
}
