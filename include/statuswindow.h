#pragma once

#include <dolphin/types.h>

void statusWinInit(void);
void statusWinReInit(void);
void statusWinMain(void);
void statusWinOpen(void);
void statusWinClose(void);
void statusWinForceOpen(void);
void statusWinForceClose(void);
void statusWinForceOff(void);
void statusWinDispOn(void);
void statusWinDispOff(void);
s32 statusWinCheck(void);
s32 statusWinCheckUpdate(void);
void statusWinForceUpdate(void);
void statusWinForceUpdateCoin(void);
void statusGetApPos(f32* out);

void N_statusClearBlink(void);
void statusMarioHPBlink(s32 frames);
void statusPartyHPBlink(s32 frames);
void statusFPBlink(s32 frames);
