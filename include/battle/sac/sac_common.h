#pragma once

#include "evt/evt_cmd.h"
#include "dolphin/types.h"
#include "dolphin/mtx.h"
#include "driver/effdrv.h"

// The star stone that rises above Mario at the start of a special move (main_star in sac_bakugame, sac_muki and
// sac_suki; sac_deka keeps its own variant)
typedef struct SacStarStone {
    s32 state;        // 0x0, main_star step (1 appear, 2 rise and spin, 3 fly up, then idle)
    s32 timer;        // 0x4, frames in the current step
    Vec pos;          // 0x8
    Vec from;         // 0x14
    Vec to;           // 0x20
    u8 pad_2C[0xC];   // 0x2C, never accessed
    Vec scale;        // 0x38
    Vec rot;          // 0x44
    EffectEntry* eff; // 0x50, effStarStoneEntry
} SacStarStone;

USER_FUNC(sac_ac_help_off);
USER_FUNC(sac_ac_help_on);
USER_FUNC(sac_cheer);
USER_FUNC(sac_cheer_end);
USER_FUNC(sac_enemy_slide_go);
USER_FUNC(sac_enemy_slide_return);
USER_FUNC(sac_handbeat);
USER_FUNC(sac_handbeat_end);
extern EvtWord sac_mario_back_home[];
extern EvtWord sac_mario_item_on_end_evt[];
extern EvtWord sac_mario_item_on_evt[];
extern EvtWord sac_start_camera_evt[];
extern EvtWord sac_start_stardust[];
USER_FUNC(sac_wao);
