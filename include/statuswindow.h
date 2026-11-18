// statuswindow.c: the field HUD of HP, FP, star power, coins and star points (StatusWindowWork).

#pragma once

#include "version.h"
#include <dolphin/mtx.h>

// StatusWindow blink / pendingBlink: the values that blink (valueCheck, statusMarioHPBlink and the others)
enum {
    STATUS_BLINK_MARIO_HP = 0x1,
    STATUS_BLINK_FP = 0x2,
    STATUS_BLINK_STAR_POWER = 0x4,
    STATUS_BLINK_PARTY_HP = 0x8,
    STATUS_BLINK_COINS = 0x10,
};

typedef struct StatusValue {
    s16 hp;         // 0x0
    s16 maxHp;      // 0x2
    s16 fp;         // 0x4
    s16 maxFp;      // 0x6
    s16 party;      // 0x8
    s16 partyHp;    // 0xA
    s16 partyMaxHp; // 0xC
    s16 starPoints; // 0xE
    s16 coins;      // 0x10
    s16 ap;         // 0x12
    s16 maxAp;      // 0x14
    s16 lastParty;  // 0x16, early demos only: the partner shown while Mario is in bg mode
} StatusValue;

// StatusWindowWork.flags (statusWinOpen/Close, statusWinForce*, statusWinDisp*)
typedef enum StatusWindowFlags {
    STATUS_WINDOW_FLAG_OPEN = (1 << 0),            // 0x1
    STATUS_WINDOW_FLAG_CLOSED = (1 << 1),          // 0x2
    STATUS_WINDOW_FLAG_OPEN_REQ = (1 << 2),        // 0x4
    STATUS_WINDOW_FLAG_CLOSE_REQ = (1 << 3),       // 0x8
    STATUS_WINDOW_FLAG_FORCE_OPEN = (1 << 4),      // 0x10
    STATUS_WINDOW_FLAG_FORCE_CLOSE = (1 << 5),     // 0x20
    STATUS_WINDOW_FLAG_FORCE_OPEN_REQ = (1 << 6),  // 0x40
    STATUS_WINDOW_FLAG_FORCE_CLOSE_REQ = (1 << 7), // 0x80
    STATUS_WINDOW_FLAG_MENU_ICONS = (1 << 8),      // 0x100, draws the pause-menu icons
    STATUS_WINDOW_FLAG_DISP_OFF = (1 << 12),       // 0x1000
} StatusWindowFlags;

typedef struct StatusWindowWork {
    u8 pad_0[0x4];  // 0x0
    u16 flags;          // 0x4, StatusWindowFlags
    f32 timer;          // 0x8, frames (at 60 Hz) into the current slide (winPositionCalc)
    f32 duration;       // 0xC, slide length in frames: 16 to open, 28 to close, divided by speed
    f32 t;              // 0x10, slide position, 0 closed .. 1 open (half-cosine ease)
    f32 tStart;         // 0x14, t when the slide started
    f32 tEnd;           // 0x18, 1 opening, 0 closing
    f32 speed;          // 0x1C, slide speed factor (statusWinInit sets 1)
    f32 x;              // 0x20, current HUD position, between closeX/Y and openX/Y by t
    f32 y;              // 0x24
    u8 pad_28[0x8]; // 0x28
    f32 openX;          // 0x30, position when open (-304, 208)
    f32 openY;          // 0x34
    u8 pad_38[0x8]; // 0x38
    f32 closeX;         // 0x40, position when closed (-304, 334)
    f32 closeY;         // 0x44
    u8 pad_48[0x8]; // 0x48
    StatusValue cur;    // 0x50, values the HUD shows; valueUpdate counts them toward the real values
    Vec prevPos;        // 0x68, Mario's position last frame (idle check)
    s32 idle;           // 0x74, frames Mario has stood still; the HUD opens after 10 s
    s32 forceTimer;     // 0x78, frames a Z press or a value change holds the HUD open/closed
    s32 forcedClose;    // 0x7C, closed with Z; no idle reopen until Mario moves
    s32 blink;          // 0x80, STATUS_BLINK_* values blinking now
    s32 pendingBlink;   // 0x84, STATUS_BLINK_* values that changed; blink takes them once nothing changes
    s32 blinkTimer;     // 0x88, frames of blinking left (2 s)
} StatusWindowWork;

void statusWinInit(void);
void statusWinReInit(void);
void statusWinMain(void);

void statusWinForceOff(void);
void statusWinForceOpen(void);

void statusWinDispOff(void);
void statusWinDispOn(void);
void statusWinForceUpdate(void);
void statusClearBlink(void);
void statusAPBlink(s32 frames);
void statusFPBlink(s32 frames);
void statusMarioHPBlink(s32 frames);
void statusPartyHPBlink(s32 frames);
BOOL statusWinCheck(void);
BOOL statusWinCheckUpdate(void);
void statusWinClose(void);
void statusWinForceClose(void);
void statusWinForceCloseClear(void);
void statusWinForceUpdateCoin(void);
Vec statusGetApPos(void);
void statusWinOpen(void);
void winPositionCalc(void);
