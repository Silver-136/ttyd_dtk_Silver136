// eff_peach_fukidashi.c: Peach speech-bubble (fukidashi) effect entry, its main/draw callbacks and effSetPeachFukiBatu.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include "driver/npcdrv.h"
#include <dolphin/types.h>

EffectEntry* effPeachFukidashiEntry(s32 type, NpcEntry* npc, s32 duration, f32 x, f32 y, f32 z,
                                    f32 size, f32 angle);
void effPeachFukidashiMain(EffectEntry* effect);
void effPeachFukidashiDisp(CameraId cameraId, EffectEntry* effect);
void effSetPeachFukiBatu(const char* name);
