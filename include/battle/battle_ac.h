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

// BattleActions.acFlags (btlevtcmd_AcSetFlag): each action command reads its own bits, so each has its own set.
// The setup call does not clear the word; every script that sets up a command reading it sets the flags first.
// Commands without a set read no flags (timing_a, lst_rc_keep, shot_target, crane_timing, air_gauge,
// power_gauge_lv, gauge_timing2, monosiri_target). _UNUSED: read by the command, but no script sets the bit
// (stick_rotate is never set up at all), or set but read by nothing.
typedef enum AcStickKeepLeftFlags {
    AC_STICK_KEEP_LEFT_FLAG_COMPACT_GAUGE = (1 << 0),         // narrower step gauge
    AC_STICK_KEEP_LEFT_FLAG_NO_EARLY_RELEASE_FAIL = (1 << 1), // releasing too early neither fails nor ends it
} AcStickKeepLeftFlags;

typedef enum AcRepeatedlyFlags {
    AC_REPEATEDLY_FLAG_ESCAPE_UNUSED = (1 << 0),  // set by the Run Away script only; read by nothing
    AC_REPEATEDLY_FLAG_PLAIN_GAUGE = (1 << 1),    // no target-zone split in the gauge colours
} AcRepeatedlyFlags;

typedef enum AcRepeatedlyLvFlags {
    AC_REPEATEDLY_LV_FLAG_NO_FULL_BONUS = (1 << 0),            // a full gauge adds no extra level
    AC_REPEATEDLY_LV_FLAG_SHOW_TARGET_MARK_UNUSED = (1 << 1),  // target marker above the gauge
    AC_REPEATEDLY_LV_FLAG_SHOW_LEVEL_NUMBER_UNUSED = (1 << 2), // level number beside the gauge
    AC_REPEATEDLY_LV_FLAG_FAST_GAUGE_DISP = (1 << 3),          // displayed gauge follows twice as fast
    AC_REPEATEDLY_LV_FLAG_INSTANT_GAUGE_DISP = (1 << 4),       // displayed gauge jumps to the value
    AC_REPEATEDLY_LV_FLAG_HOLD_OVERHEAT = (1 << 5),            // holding counts; a full gauge drains, then refills
    AC_REPEATEDLY_LV_FLAG_MIN_LEVEL_1 = (1 << 6),              // level at least 1 even with an empty gauge
} AcRepeatedlyLvFlags;

typedef enum AcShotFlags {
    AC_SHOT_FLAG_NO_CHARGE = (1 << 0),            // A fires at once with the mean power; no gauge
} AcShotFlags;

typedef enum AcStickRotateFlags {
    AC_STICK_ROTATE_FLAG_STRICT_UNUSED = (1 << 0), // neutral, reversing, skipping or wrong speed ends the command
    AC_STICK_ROTATE_FLAG_GUIDED_UNUSED = (1 << 1), // fixed start, stopping ends it, alternate display
} AcStickRotateFlags;

typedef enum AcButtonDownFlags {
    AC_BUTTON_DOWN_FLAG_REVEAL_ONE_BY_ONE = (1 << 0), // only the pressed buttons and the next one are shown
    AC_BUTTON_DOWN_FLAG_PARTIAL_SUCCESS = (1 << 1),   // a miss after at least one correct press still succeeds
    AC_BUTTON_DOWN_FLAG_ALLOW_REPEATS = (1 << 2),     // a random sequence may repeat a button
} AcButtonDownFlags;

typedef enum AcTimingWideFlags {
    AC_TIMING_WIDE_FLAG_NO_SLIDE_IN = (1 << 0),                 // icon starts at its place
    AC_TIMING_WIDE_FLAG_KEEP_ICON_ON_SUCCESS_UNUSED = (1 << 1), // icon stays after a success
    AC_TIMING_WIDE_FLAG_KEEP_ICON_ON_FAIL_UNUSED = (1 << 2),    // icon stays after a failure
    AC_TIMING_WIDE_FLAG_HOLD_AFTER_RESULT = (1 << 3),           // wait for StopAC instead of ending by itself
    AC_TIMING_WIDE_FLAG_SWAP_RESULT_ICON = (1 << 4),            // pressed/released icons swapped after the input
} AcTimingWideFlags;

typedef enum AcPendulumCraneFlags {
    AC_PENDULUM_CRANE_FLAG_STICK_BRAKE_UNUSED = (1 << 0), // stick against the swing slows the pendulum
} AcPendulumCraneFlags;

typedef enum AcPowerGaugeFlags {
    AC_POWER_GAUGE_FLAG_FILL_WHILE_HELD = (1 << 0), // gauge runs only once the button is down
    AC_POWER_GAUGE_FLAG_PLAIN_GAUGE = (1 << 1),     // no coloured segments
    AC_POWER_GAUGE_FLAG_SHOW_ZONE_LAMP = (1 << 2),  // lamp lit while in the zone, instead of the target marker
    AC_POWER_GAUGE_FLAG_LAMP_SHIFT_LEFT = (1 << 3), // lamp 12 units further left
} AcPowerGaugeFlags;

typedef enum AcPowerGaugeLv2Flags {
    AC_POWER_GAUGE_LV2_FLAG_NO_FULL_BONUS_UNUSED = (1 << 0), // a full gauge adds no extra level
} AcPowerGaugeLv2Flags;

typedef enum AcSignalTimingFlags {
    AC_SIGNAL_TIMING_FLAG_SHOW_BUTTONS = (1 << 0),          // each signal shows its own button
    AC_SIGNAL_TIMING_FLAG_HIDE_UPCOMING_BUTTONS = (1 << 1), // with SHOW_BUTTONS: buttons appear shortly before
    AC_SIGNAL_TIMING_FLAG_HIDE_HIT_COUNT = (1 << 2),
    AC_SIGNAL_TIMING_FLAG_LARGE_HIT_COUNT = (1 << 3),       // hit counter at 1.5x
    AC_SIGNAL_TIMING_FLAG_END_ON_HIT = (1 << 4),            // a correct press ends the command
} AcSignalTimingFlags;

typedef enum AcGaugeTimingFlags {
    AC_GAUGE_TIMING_FLAG_WRAP = (1 << 0),         // sweep mode: the gauge wraps around at 100
    AC_GAUGE_TIMING_FLAG_CYCLE_PAUSE = (1 << 1),  // gauge blanked for 15 frames after each cycle
} AcGaugeTimingFlags;

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
    u32 acFlags;                     // 0x34, btlevtcmd_AcSetFlag; Ac<Command>Flags above
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
