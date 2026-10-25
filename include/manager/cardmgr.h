#pragma once

#include "evt/evt_badgeshop.h"
#include "evt/evt_johoya.h"
#include "evt/evt_yuugijou.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include <dolphin/card.h>
#include <dolphin/os.h>

typedef struct {
    struct {
        char fileName[0x20]; //0x0
        char dateString[0x20]; //0x20
        u8 bannerTex[0x1800]; //0x40
        u8 iconTex[0x400]; //0x1840
        u8 iconTlut[0x200]; //0x1C40
        u16 rumbleEnabled; //0x1E40
        u16 soundMode; //0x1E42
        u16 lastSlot; //0x1E44
    } data;
    u8 __align[0x1FF8 - 0x1E46]; //0x1E46
    u32 checksum1; //0x1FF8
    u32 checksum2; //0x1FFC
} CardMetadata;

typedef struct {
    struct {
        u16 flags; //0x0
        u8 pad_2[0x8 - 0x2]; //0x2
        GlobalWork global; //0x8
        PouchData pouch; //0x13E0
        NpcDeadInfo deadInfo[64]; //0x19B4
        BadgeShopWork badgeShop; //0x1EB4
        PiantaParlorWork parlor; //0x1FD8
#ifdef VERSION_G8ME01
        InformantUnknown informant; //0x2230 (US parlor work is 0x120 bigger)
        u8 pad_225B[0x2260 - 0x225B]; //0x225B
    } data;
    u8 __align[0x3FF0 - 0x2260]; //0x2260
#else
        InformantUnknown informant; //0x2110
        u8 pad_213B[0x2140 - 0x213B]; //0x213B
    } data;
    u8 __align[0x3FF0 - 0x2140]; //0x2140
#endif
    char version[4]; //0x3FF0
    s32 size; //0x3FF4
    u32 checksum1; //0x3FF8
    u32 checksum2; //0x3FFC
} CardSaveSlot;

typedef struct {
    CardMetadata metadata; //0x0
    CardSaveSlot saves[4]; //0x2000 (+ 0x4000 * save_index)
} CardData;

typedef struct {
    vu16 flags; //0x0
    u8 pad_2[0x4 - 0x2]; //0x2
    void* workArea; //0x4
    s32 chan; //0x8
    s32 fileNo; //0xC
    u64 serialNo; //0x10
    u8 pad_18[0x1C - 0x18]; //0x18
    CARDFileInfo info; //0x1C
    CARDStat status; //0x30
    s32 result; //0x9C
    s32 currSlot; //0xA0
    s32 opSlot; //0xA4, slot an operation reads (memcard_read offsets)
    CardData* data; //0xA8
    void* readBuf; //0xAC, read buffer, holds metadata or a save slot
    CardSaveSlot* writeBuf; //0xB0, save slot staged for writing
    BOOL validSave1; //0xB4, is main save valid?
    BOOL validSave2; //0xB8, is backup save valid?
    s32 writeFirst; //0xBC
    s32 writeSecond; //0xC0
    u8 pad_C4[0xC8 - 0xC4]; //0xC4
    OSTime lastSaveTime1; //0xC8
    OSTime lastSaveTime2; //0xD0
    s32 opStep; //0xD8, step within the current operation
    u8 pad_DC[0xE0 - 0xDC]; //0xDC
    s32 op; //0xE0, current operation (0 = idle, 8 = finishing/error)
    s32 prevInserted; //0xE4, last CARDProbe result; a change sets flag 0x4000
    u8 pad_E8[0xEC - 0xE8]; //0xE8
    s32 busyFrames; //0xEC, counts down; nonzero keeps cardIsExec returning TRUE
} CardWork;

CardData* cardGetFilePtr(void);
void cardBufReset(void);
void cardInit(void);
void cardInsertWatchStart(void); //called by evt_memcard, TODO: rename
void cardInsertWatchEnd(void); //called by evt_memcard, TODO: rename
BOOL cardInsertChanged(void); //called by evt_memcard, TODO: rename
BOOL cardIsExec(void);
s32 cardGetCode(void);
void cardMain(void);
void cardCopy2Main(s32 slot);
void cardErase(s32 slot);
void cardCopy(s32 from, s32 to);
void cardWrite(s32 slot);
void cardWriteHeader(void);
void cardReadAll(void);
void cardCreate(void);
void cardFormat(void);
BOOL cardInsertChanged(void);
void cardInsertWatchEnd(void);
void cardInsertWatchStart(void);
