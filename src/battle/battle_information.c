#include "battle/battle_information.h"
s32 BattleInformationGetResult(void* info) {
    return *(s32*)((s32)info + 0x10);
}

void BattleInformationSetResult(void* info, s32 result) {
    *(s32*)((s32)info + 0x10) = result;
}

void BattleInfomationSetBattleSetupInfo(void* info, s32 battleSetupInfo) {
    *(s32*)((s32)info + 0xC) = battleSetupInfo;
}

void BattleInformationSetFirstAttack(void* info, s32 firstAttack) {
    *(s32*)((s32)info + 0x8) = firstAttack;
}

void BattleInformationSetParty(void* info, s32 party) {
    *(s32*)((s32)info + 0x4) = party;
}

void BattleInformationSetMode(void* info, s32 mode) {
    *(s32*)((s32)info + 0x0) = mode;
}

void BattleInformationInit(void* info) {
    BattleInformationSetMode(info, 0);
    BattleInformationSetParty(info, 0);
    BattleInformationSetFirstAttack(info, 0);
    BattleInformationSetResult(info, 0);

    *(s32*)((s32)info + 0xC) = 0;
    *(s32*)((s32)info + 0x14) = 0;
    *(u8*)((s32)info + 0x18) = 0;
    *(u8*)((s32)info + 0x19) = 0;
    *(u8*)((s32)info + 0x1A) = 0;
    *(u8*)((s32)info + 0x1B) = 0;
}