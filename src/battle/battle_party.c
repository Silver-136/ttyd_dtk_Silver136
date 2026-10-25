#include "battle/battle_party.h"
#include "battle/battle_unit.h"
#include "unit/party/unit_party_nokotarou.h"
#include "unit/party/unit_party_yoshi.h"
#include "unit/party/unit_party_clauda.h"
#include "unit/party/unit_party_vivian.h"
#include "unit/party/unit_party_sanders.h"
#include "unit/party/unit_party_chuchurina.h"
#include "unit/party/unit_party_christine.h"


#define PARTY_SETUP(kind) {&kind, 0, {0}, 0x04000002, {1000.0f, 0.0f, 1000.0f}, 0, {0, 0, 0, 0}, NULL}

BattleUnitSetup entryunit_party[7] = {
    PARTY_SETUP(unitdata_Party_Christine),
    PARTY_SETUP(unitdata_Party_Nokotarou),
    PARTY_SETUP(unitdata_Party_Yoshi),
    PARTY_SETUP(unitdata_Party_Clauda),
    PARTY_SETUP(unitdata_Party_Vivian),
    PARTY_SETUP(unitdata_Party_Sanders),
    PARTY_SETUP(unitdata_Party_Chuchurina),
};
