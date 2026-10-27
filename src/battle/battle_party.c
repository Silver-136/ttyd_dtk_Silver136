#include "battle/battle_party.h"
#include "battle/battle_unit.h"
#include "unit/party/unit_party_nokotarou.h"
#include "unit/party/unit_party_yoshi.h"
#include "unit/party/unit_party_clauda.h"
#include "unit/party/unit_party_vivian.h"
#include "unit/party/unit_party_sanders.h"
#include "unit/party/unit_party_chuchurina.h"
#include "unit/party/unit_party_christine.h"

BattleUnitSetup entryunit_party[7] = {
    {&unitdata_Party_Christine, 0, {0}, 0x04000002, {1000.0f, 0.0f, 1000.0f}, 0, {0, 0, 0, 0}, NULL},
    {&unitdata_Party_Nokotarou, 0, {0}, 0x04000002, {1000.0f, 0.0f, 1000.0f}, 0, {0, 0, 0, 0}, NULL},
    {&unitdata_Party_Yoshi, 0, {0}, 0x04000002, {1000.0f, 0.0f, 1000.0f}, 0, {0, 0, 0, 0}, NULL},
    {&unitdata_Party_Clauda, 0, {0}, 0x04000002, {1000.0f, 0.0f, 1000.0f}, 0, {0, 0, 0, 0}, NULL},
    {&unitdata_Party_Vivian, 0, {0}, 0x04000002, {1000.0f, 0.0f, 1000.0f}, 0, {0, 0, 0, 0}, NULL},
    {&unitdata_Party_Sanders, 0, {0}, 0x04000002, {1000.0f, 0.0f, 1000.0f}, 0, {0, 0, 0, 0}, NULL},
    {&unitdata_Party_Chuchurina, 0, {0}, 0x04000002, {1000.0f, 0.0f, 1000.0f}, 0, {0, 0, 0, 0}, NULL}
};
