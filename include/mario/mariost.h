#pragma once

#include <dolphin/os.h>
#include "driver/fadedrv.h"
#include "driver/npcdrv.h"
#include "evt/evt_lottery.h"
#include "countdown.h"

// Reset code stored with OSResetSystem and read back by OSGetResetCode after the restart:
// bit 0 progressive video mode, bit 1 no video mode was set (the early demos store 0 for that); the language follows from bit 2 (US and PAL).
#define RESET_CODE_INTERLACED 0
#define RESET_CODE_PROGRESSIVE 1
#define RESET_CODE_NO_VIDEO_MODE 2

// GlobalWork.systemLevelFlags: marioStSystemLevel(n) sets the stop bits of levels below n; a stopped level's
// timer (systemLv0Time..systemLv3Time) does not advance
typedef enum SystemLevelFlags {
    SYSTEM_LEVEL_STOP_LV0 = (1 << 0),     // 0x1
    SYSTEM_LEVEL_STOP_LV1 = (1 << 1),     // 0x2
    SYSTEM_LEVEL_STOP_LV2 = (1 << 2),     // 0x4
    SYSTEM_LEVEL_STOP_LV3 = (1 << 3),     // 0x8
    SYSTEM_LEVEL_STOP_ALL = 0xF,
    SYSTEM_LEVEL_NO_FIELD_BATTLE = (1 << 4), // 0x10, no field battle starts (scripted jumps, jump stands)
} SystemLevelFlags;

// GlobalWork.flags (scripts: evt_sub_get_system_flag / evt_sub_system_flag_onoff). 0x40 is only tested and cleared
// by map scripts (no setter in the code) and is not named.
typedef enum GlobalWorkFlags {
    GW_FLAG_SAVE_LOADED = (1 << 0),          // 0x1, a save was loaded: the next map change restores the positions
    GW_FLAG_NO_ENTRANCE = (1 << 1),          // 0x2, map entered without an entrance: story init skipped, first bero
    GW_FLAG_VIDEO_MODE_SET = (1 << 2),       // 0x4, the video mode was chosen (a reset restarts, not shuts down)
    GW_FLAG_PROGRESSIVE = (1 << 3),          // 0x8, progressive scan (kept on the memory card; hot reset)
    GW_FLAG_CURTAIN_OPEN = (1 << 4),         // 0x10, the next map opens with the stage curtain (sys start)
    GW_FLAG_NO_SAVE_BLOCK = (1 << 5),        // 0x20, save blocks are not created (E3 demo)
    GW_FLAG_E3 = (1 << 12),                  // 0x1000, E3/kiosk behaviour: set by seq_e3 and, at start-up, by every demo (marioStInit)
    GW_FLAG_E3_TIMEOUT = (1 << 13),          // 0x2000, kiosk mode went idle: back to the logo
    GW_FLAG_NO_LETTER_BOX_UNUSED = (1 << 15), // 0x8000, the letter box is not drawn; nothing sets it
} GlobalWorkFlags;

// GlobalWork.language: index into the game's language prefix tables ("jp", "us", "ge", "fr", "sp", "it" in
// seq_load, nameent and seq_mapchange; effdrv, seq_mapchange and sys also list "du")
typedef enum Language {
    LANGUAGE_JP = 0,
    LANGUAGE_US = 1,
    LANGUAGE_GE = 2,
    LANGUAGE_FR = 3,
    LANGUAGE_SP = 4,
    LANGUAGE_IT = 5,
    LANGUAGE_DU = 6, // "du": 7th prefix in effdrv, seq_mapchange and sys; cardmgr, evt_msg, muj_02 treat it as US
} Language;

// GlobalWork.soundMode: the surround choice of the sound option (seq_load); mono or stereo follows OSGetSoundMode.
// seq_mapchange selects SND_OUTPUTMODE_SURROUND by it outside mono.
enum {
    GW_SOUND_MODE_NO_SURROUND = 0, // the mono or stereo option
    GW_SOUND_MODE_SURROUND = 1,    // the surround option
};

// GlobalWork.resetType
enum {
    RESET_TYPE_NONE = 0,
    RESET_TYPE_SOFT = 1,
    RESET_TYPE_MENU = 2,  // back to the system menu (evt_memcard)
};

typedef struct GlobalWork {
    s32 flags; // 0x0, GlobalWorkFlags
    s32 framerate; // 0x4, display rate, 60 or 50
    s32 debugFlag; // 0x8, only cleared (cardmgr); nameent takes input only while 0
    s32 debugBattle; // 0xC, seq_battle copies it to debug_battle_flag
    BOOL dvdError; // 0x10, set while the drive reports an error; renderTime stops meanwhile
    s32 inBattle; // 0x14, verified s32
    u32 systemLevelFlags; // 0x18, SystemLevelFlags
    u32 retraceCount; // 0x1C, VI retrace count (viPostCallback); frame timer for blinking etc.
    OSTime playTime; // 0x20, the save file's play time (win_mario, file select), capped at 99999:59:59
    OSTime retraceDeltaTime; // 0x28, time since the last retrace
    OSTime retraceTime; // 0x30, time of the last retrace
    OSTime renderTime; // 0x38, time outside menus; saved with the file, so it adds up across sessions
    OSTime renderFieldTime; // 0x40, the same outside battle
    OSTime systemLv0Time; // 0x48, clock that runs unless system level 0 stops it (SYSTEM_LEVEL_STOP_LV0)
    OSTime systemLv1Time; // 0x50, same for level 1
    OSTime systemLv2Time; // 0x58, same for level 2
    OSTime systemLv3Time; // 0x60, same for level 3
    CountdownWork countdown; // 0x68, countdown timer work (countdown.c)
    lottery_info lotteryInfo; // 0xA8
    u8 pad_E8[0xF8 - 0xE8]; // 0xE8
    // In: run by seq_game when the new map starts. Out: run by seq_mapchange when the old map is left.
    FadeType mapFadeInType; // 0xF8
    s32 mapFadeInDuration; // 0xFC, ms
    FadeType mapFadeOutType; // 0x100
    s32 mapFadeOutDuration; // 0x104, ms
    FadeType areaFadeInType; // 0x108
    s32 areaFadeInDuration; // 0x10C, ms
    FadeType areaFadeOutType; // 0x110
    s32 areaFadeOutDuration; // 0x114, ms
    BOOL didAreaChange; // 0x118, the last map change entered a new area (seq_mapchange)
    char beroEnterName[0x10]; // 0x11C, entrance Mario arrives by; empty = none (evt_bero)
    char currentMapName[0x10]; // 0x12C, map name, e.g. "gor_01"
    char currentAreaName[0x10]; // 0x13C, area name, e.g. "gor"
    char lastMapBeforeTitle[0x10]; // 0x14C, map left when the game last went to the title (only written)
    void* relocationBase; // 0x15C, loaded map REL module (OSLink/OSUnlink in seq_mapchange)
    void* mapAlloc; // 0x160, map heap block (_mapAlloc)
    s32 mapEventId; // 0x164, map init event id (seq_mapchange.c)
    FieldBattleData* fbatData; // 0x168, field battle data of the battle being started (npcdrv)
    u32 language; // 0x16C, Language
    u16 fbWidth; // 0x170, frame buffer width (GXSetScissor)
    u16 efbHeight; // 0x172, EFB height
    s32 gSW0; // 0x174, GSW(0), the story progress, as a full word (swdrv)
    u32 gSWF[0x100]; // 0x178, global saved work flags (GSWF)
    u8 gSW[0x800]; // 0x578, GSW byte values
    u32 lSWF[0x10]; // 0xD78, LSWF flags (cleared on an area change, swReInit)
    u8 lSW[0x400]; // 0xDB8, LSW byte values (cleared on an area change, swReInit)
    s32 lastSlot; // 0x11B8, last used save slot, start cursor on load screen
    u8 playerName[0x11D0 - 0x11BC]; // 0x11BC, file name typed at file creation (seq_load)
    s32 saveSlot; // 0x11D0, memory card slot the game saves to
    Vec savePlayerPos; // 0x11D4, Mario's position when saved (cardmgr)
    s32 savePartyId[2]; // 0x11E0, partners out when saved
    OSTime saveTime; // 0x11E8, OSGetTime of the last save
    s32 saveCount; // 0x11F0, number of saves
    u8 pad_11F4[0x1274 - 0x11F4]; // 0x11F4
    s32 soundMode; // 0x1274, GW_SOUND_MODE_*, kept in the save metadata
    s32 resetType; // 0x1278, 0 = no reset, 1 = soft reset, 2 = menu
    BOOL resetButtonDown; // 0x127C, reset button was down (mariost.c)
    BOOL softResetWait; // 0x1280, B+X+Start is held (viPostCallback)
    OSTime softResetTimeout; // 0x1288, time B+X+Start was first held; 0.5 s soft-resets
    BOOL restartChecked; // 0x1290, restart reset-combo check done
    BOOL rumbleEnabled; // 0x1294, rumble option, 0 = off
    u8 pad_1298[0x12C0 - 0x1298]; // 0x1298
    u8 rumbleStart[4]; // 0x12C0, the motor started rumbling (for the 30 s limit)
    OSTime rumbleStartTime[4]; // 0x12C8, when it started
    u8 rumbleForcedOff[4]; // 0x12E8, rumble cut after 30 s
    u8 rumbleForcedOffLatch[4]; // 0x12EC, rumble cut latch
    OSTime rumbleForcedOffTime[4]; // 0x12F0, time rumble was cut
    u8 rumbleHardStop[4]; // 0x1310, stop motor hard (PADControlMotor 2)
    OSTick deltaGame; // 0x1314, OS ticks from frame start to the end of the update (main.c)
    OSTick deltaRender; // 0x1318, OS ticks from frame start to the end of the render
    u8 pad_131C[0x1324 - 0x131C]; // 0x131C
// controller state kept by the key functions (system.c)
    BOOL keyDataValid; // 0x1324, key data valid (system.c)
    u32 button[4]; // 0x1328, buttons held per controller (makeKey)
    u32 buttonNew[4]; // 0x1338, buttons pressed this frame
    u32 buttonRepeat[4]; // 0x1348, pressed or auto-repeated this frame
    u32 buttonRepeatDelay[4]; // 0x1358,
    u32 buttonUp[4]; // 0x1368, buttons released this frame
    u32 dirs[4]; // 0x1378, DEMOPad direction bits held
    u32 dirsNew[4]; // 0x1388, direction bits new this frame
    u32 dirsRepeat[4]; // 0x1398, new or auto-repeated
    u32 dirsRepeatDelay[4]; // 0x13A8, frames to the next direction repeat
    s8 stickX[4]; // 0x13B8, main stick x
    s8 stickY[4]; // 0x13BC, main stick y
    s8 substickX[4]; // 0x13C0, C-stick x
    s8 substickY[4]; // 0x13C4, C-stick y
    u8 triggerLeft[4]; // 0x13C8, analog L
    u8 triggerRight[4]; // 0x13CC, analog R
    u8 rumbleStatus[4]; // 0x13D0, requested motor state per controller
    u8 prevRumbleStatus[4]; // 0x13D4, motor state last applied
} GlobalWork;
extern GlobalWork* gp;

void marioStInit(void);
void marioStMain(void);
void marioStDisp(void);
void marioStSystemLevel(s32 level);
u32 marioStGetSystemLevel(void);

extern GXRenderModeObj GXNtsc480IntDfMarioSt;
extern GXRenderModeObj GXNtsc480ProgMarioSt;
extern s32 _mariostSystemLevel;
extern OSTime none_key;
extern GXRenderModeObj sRMObjHReso;
extern GXRenderModeObj sRMObjHReso_prog;
