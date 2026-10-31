#pragma once

#include <dolphin/types.h>
#include "evt/evt_word.h"

struct NpcTerritoryInfo;

typedef struct MapData {
    const char* mapName;
    EvtWord* eventAddr;
} MapData;

typedef struct AreaData {
    const char* areaName;
    const char* areaJpName;
    void* btlSetupTbl;   // BattleSetupData table, set by relSetBtlAddr
    struct NpcTerritoryInfo* btlSetupNoTbl; // NpcTerritoryInfo table, set by relSetBtlAddr
    MapData* maps[];
} AreaData;

typedef struct WorldData {
    const char* worldName;
    const char* worldJpName;
    AreaData* areas[64];
} WorldData;

MapData* mapDataPtr(const char* mapName);
AreaData* areaDataPtr(const char* areaName);
void relSetEvtAddr(const char* mapName, EvtWord* eventAddr);
void relSetBtlAddr(const char* areaName, void* btlSetupTbl, void* btlSetupNoTbl);
