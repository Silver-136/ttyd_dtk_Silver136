// database.c: loading of the per-map setup data (setupDataLoad, setupDataBase) that spawns the field enemies.

#pragma once

#include <dolphin/types.h>

void setupDataLoad(const char* mapName);
void setupDataBase(const char* areaName, const char* mapName);
BOOL setupDataCheck(void);
