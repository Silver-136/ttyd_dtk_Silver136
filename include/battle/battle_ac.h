#pragma once

#include <dolphin/types.h>
#include "battle/battle_database_common.h"
#include "battle/battle_pad.h"
#include "driver/dispdrv.h"

typedef struct BattleWork BattleWork;

typedef s32 (*BattleACMainCallback)(BattleWork* work);
typedef s32 (*BattleACResultCallback)(BattleWork* work);
typedef void (*BattleACDeleteCallback)(BattleWork* work);

typedef struct BattleACDispParams {
    s32 blinkTimer;                 // 0x0, frame counter for the icon blink (ac_repeatedly_lv)
    u8 pad_04[0xC - 0x4];           // 0x4, no named access
    GXColor color;                  // 0xC, icon colour
    s8 colorStep[4];                // 0x10, per-frame colour delta while the icon blinks
    f32 iconX;                      // 0x14, see actionCommandDisp
    f32 iconY;                      // 0x18, see actionCommandDisp
    s32 reserved_1C;                // 0x1C, never read
    s32 slideTimer;                 // 0x20, frames of the icon slide in (0..20) / out (40..60)
    s32 iconStep;                   // 0x24, step shown by the icon (lst_rc_keep, stick_keep_left), -1 = none
    f32 gaugeRatio;                 // 0x28, power gauge fill, 0..1
} BattleACDispParams;

// FAKE: slideTimer read through a byte offset from a register-held index (i = 4 maps to slideTimer); the plain field schedules the
// address differently (ac_power_gauge_lv 98.86%, (&slideTimer)[i - 4] 78.95%). Used by ac_air_gauge, ac_power_gauge_lv, ac_timing_a.
#define AC_SLIDE_TIMER_AT(disp, i) (*(s32*)((u8*)&(disp)->slideTimer + (i) - 4))

typedef struct BattleActions {
    struct BattleUnit* mAcUnit;      // 0x0
    s32 setupFlags;                  // 0x4, BattleActionCommandSetup a4; bit0: auto-play while confused
    s32 setupParam;                  // 0x8, BattleActionCommandSetup a5
    s32 mAcState;                    // 0xC
    BattleACMainCallback maincb;     // 0x10
    BattleACResultCallback resultcb; // 0x14
    DispCallback dispcb;             // 0x18
    BattleACDeleteCallback deletecb; // 0x1C
    s32 mDefenseResult;              // 0x20
    s32 mResultCount;                // 0x24
    s32 result;                      // 0x28
    s8 mBaseAcDifficulty;            // 0x2C
    u8 mAcDifficulty;                // 0x2D
    u8 pad_2E[2];                    // 0x2E, never accessed
    s32 doneFlags;                   // 0x30, bit 1: AC finished (delete handler forces the end state otherwise)
    u32 acFlags;                     // 0x34, btlevtcmd_AcSetFlag; 0x10 tested in signal timing
    s32 params[8];                   // 0x38, btlevtcmd_AcSetParam/AcSetParamAll/AcGetParam
    s32 outputParams[8];             // 0x58
    s32 gaugeParams[4];              // 0x78, btlevtcmd_AcSetGaugeParam
    s16 countdown;                   // 0x88, per-frame countdown in repeatedly/signal_timing/stick_rotate
    u8 pad_8A[0x8C - 0x8A];          // 0x8A, never accessed
    BattleWorkPad pad;               // 0x8C
    u32 seId;                        // 0x288, power gauge SFX id (battle_ac.c)
    f32 seValue;                     // 0x28C, power gauge SFX value
    union {                          // 0x290, gauge display parameters (ac_*.c use their own views)
        BattleACDispParams disp;
        u8 dispData[0x2BC - 0x290];
    };
    u8 work[0x30C - 0x2BC];          // 0x2BC, per-command work (each ac_*.c casts it to its own struct)
} BattleActions;

void BattleActionCommandManagerInit(BattleWork* wp);
void BattleActionCommandManager(BattleWork* wp);
s32 BattleActionCommandResult(BattleWork* work);
void BattleActionCommandDeclareACResult(BattleWork* work, BattleWeapon* weapon, s32 result);

void BattleActionCommandStart(BattleWork* work);
void BattleActionCommandStop(BattleWork* work);
s32 BattleActionCommandGetDefenceResult(void);
void BattleActionCommandResetDefenceResult(void);
s32 BattleActionCommandGetDifficulty(BattleWork* work);
void BattleAcGaugeSeDelete(void);
void BattleAcGaugeSeInit(void);
void BattleAcGaugeSeUpdate(f32 value);
s32 BattleActionCommandGetPrizeLv(BattleWork* work, BattleUnit* unit, s32 lv);
void BattleActionCommandSetDifficulty(BattleWork* work, BattleUnit* unit, s32 difficulty);
void BattleActionCommandSetup(BattleWork* work, s32 id, BattleUnit* unit, s32 setupFlags, s32 setupParam);
s32 BattleActionCommandCheckDefence(BattleUnit* unit, BattleWeapon* weapon);
