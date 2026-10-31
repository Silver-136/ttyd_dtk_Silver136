// Declares the battle controller input structure and its update functions.

#pragma once

#include <dolphin/pad.h>

// Direction bits BtlPad_WorkUpdate adds to the button word (above the PAD_BUTTON_* bits) while the control
// stick or the C-stick is held 30 or more units from the centre
#define BATTLE_PAD_STICK_UP        (1 << 16)
#define BATTLE_PAD_STICK_DOWN      (1 << 17)
#define BATTLE_PAD_STICK_LEFT      (1 << 18)
#define BATTLE_PAD_STICK_RIGHT     (1 << 19)
#define BATTLE_PAD_SUBSTICK_UP     (1 << 20)
#define BATTLE_PAD_SUBSTICK_DOWN   (1 << 21)
#define BATTLE_PAD_SUBSTICK_LEFT   (1 << 22)
#define BATTLE_PAD_SUBSTICK_RIGHT  (1 << 23)

typedef struct BattleWorkPad {
    PADStatus padData[2]; //0x0, current, last

    u32 buttonsPressedHistory[30]; //0x18
    u32 buttonsHeldHistory[30]; //0x90
    u32 buttonsHistory[30]; //0x108
    u32 buttonsReleasedHistory[30]; //0x180

    s8 holdRepeatState; //0x1F8
    s8 holdRepeatFrameCounter; //0x1F9
    u8 pad_0x1FA[2]; //0x1FA
} BattleWorkPad;

void BattlePadInit(void);
void BattlePadManager(void);

void BtlPad_WorkInit(BattleWorkPad* pad);
void BtlPad_WorkUpdate(BattleWorkPad* pad, s32 chan);

u32 BattlePadGetTrigger(void);
u32 BattlePadGetNow(void);

BOOL BattlePadCheckTrigger(s32 mask);
BOOL BattlePadMultiCheckTrigger(s32 chan, s32 mask);

BOOL BattlePadCheckRecordTrigger(s32 frame, s32 mask);
u32 BattlePadMultiCheckRecordTrigger(s32 chan, s32 frame, s32 mask);

s32 BattlePadCheckNow(s32 mask);
BOOL BattlePadMultiCheckNow(s32 chan, s32 mask);

s32 BattlePadCheckUp(s32 mask);
u32 BattlePadMultiCheckUp(s32 chan, s32 mask);

s32 BattlePadCheckRepeat(s32 mask);
u32 BattlePadMultiCheckRepeat(s32 chan, s32 mask);

// Names from LLCOOLT's tree.
#define BTL_PAD_STICK_LEFT (1 << 18)
#define BTL_PAD_STICK_RIGHT (1 << 19)
