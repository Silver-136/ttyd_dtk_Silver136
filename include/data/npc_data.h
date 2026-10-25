#pragma once

#include "driver/npcdrv.h"
#include <dolphin/types.h>
#include "evt/evt_word.h"

typedef struct NpcAiInfo {
    const char* description; //0x0, used for s/etup.xml tag lookup
    s32 flags; //0x4
    EvtWord* initEvent; //0x8
    EvtWord* moveEvent; //0xC
    EvtWord* deadEvent; //0x10
    EvtWord* findEvent; //0x14
    EvtWord* lostEvent; //0x18
    EvtWord* returnEvent; //0x1C
    EvtWord* blowEvent; //0x20
} NpcAiInfo;

extern NpcTribeInfo npcTribe[];

extern NpcAiInfo npc_ai_type_table[];
extern NpcTerritoryInfo npc_define_territory_type[];
