#pragma once

#include <dolphin/mtx.h>

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
    s16 pad16;      // 0x16
} StatusValue;

typedef struct StatusWindowWork {
    u8 pad_0[0x4];  // 0x0
    u16 flags;          // 0x4
    u8 align_0x6[2];    // 0x6
    f32 timer;          // 0x8
    f32 duration;       // 0xC
    f32 t;              // 0x10
    f32 tStart;         // 0x14
    f32 tEnd;           // 0x18
    f32 speed;          // 0x1C
    f32 x;              // 0x20
    f32 y;              // 0x24
    u8 pad_28[0x8]; // 0x28
    f32 openX;          // 0x30
    f32 openY;          // 0x34
    u8 pad_38[0x8]; // 0x38
    f32 closeX;         // 0x40
    f32 closeY;         // 0x44
    u8 pad_48[0x8]; // 0x48
    StatusValue cur;    // 0x50
    Vec prevPos;        // 0x68
    s32 idle;           // 0x74
    s32 forceTimer;     // 0x78
    s32 forcedClose;    // 0x7C
    s32 blink;          // 0x80
    s32 pendingBlink;   // 0x84
    s32 blinkTimer;     // 0x88
} StatusWindowWork;

void statusWinInit(void);
void statusWinReInit(void);
void statusWinMain(void);

void statusWinForceOff(void);
void statusWinForceOpen(void);

void statusWinDispOff(void);
void statusWinDispOn(void);
void statusWinForceUpdate(void);
void N_statusClearBlink(void);
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
