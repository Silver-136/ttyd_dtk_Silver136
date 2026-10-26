#pragma once

#include <dolphin/os.h>
#include "driver/windowdrv.h"
#include "memory.h"

// MsgChar.value of a text character (msgAnalize, read by msgDisp)
typedef enum MsgCharFlags {
    MSG_CHAR_DYNAMIC = (1 << 0),        // 0x1, <dynamic>: drawn rotated by rot
    MSG_CHAR_SHAKE = (1 << 1),          // 0x2, <shake>
    MSG_CHAR_WAVE = (1 << 2),           // 0x4, <wave>
    MSG_CHAR_TALK = (1 << 3),           // 0x8, plays the voice and moves the speaker's mouth
    MSG_CHAR_VOICE = (1 << 4),          // 0x10, plays the voice only (US: the '・' character)
    MSG_CHAR_SEL_ROW_MASK = 0xFF00,     // <select> windows: the row of the character (MSG_CHAR_SEL_ROW)
    MSG_CHAR_SCALE_X = (1 << 16),       // 0x10000, <scaleX> (US): stretched on x only
} MsgCharFlags;
#define MSG_CHAR_SEL_ROW(n) (0x100 << (n))

typedef struct MsgChar {
    union {
        u32 value; //0x0
        GXColor color; //0x0, colour-change entries
        struct NpcEntry* npc; //0x0, paper entries (NULL = the speaker)
    };
    u16 glyph; //0x4
    s16 x; //0x6 (page-break entries: page number)
    s16 y; //0x8 (page-scroll entries: scroll amount)
    s16 iconY; //0xA, icon entries: extra y offset
    u32 time; //0xC
    f32 scale; //0x10
    f32 rot; //0x14, animation entries: rotation; dynamic-scale entries: current scale
} MsgChar;

// MsgWinData.flags (msgdrv, evt_msg). 0x1 (set at creation, never read) is not named.
typedef enum MsgWinFlags {
    MSG_WIN_FLAG_START = (1 << 1),       // 0x2
    MSG_WIN_FLAG_FAST = (1 << 2),        // 0x4
    MSG_WIN_FLAG_BATTLE_UNIT = (1 << 3), // 0x8, speaker is a battle unit
    MSG_WIN_FLAG_SMALL = (1 << 4),       // 0x10
    MSG_WIN_FLAG_SELECT = (1 << 5),      // 0x20
    MSG_WIN_FLAG_REVIEW = (1 << 6),      // 0x40
    MSG_WIN_FLAG_NO_SKIP = (1 << 7),     // 0x80, <dkey>
    MSG_WIN_FLAG_KEY_X = (1 << 8),       // 0x100, <keyxon>: X also advances
    MSG_WIN_FLAG_NO_PAGE_LOG = (1 << 9), // 0x200
} MsgWinFlags;

typedef struct MsgWinData {
    const char* tag; //0x0
    u32 flags; //0x4, MsgWinFlags
    u16 curPage; //0x8, page being shown (page-break entries carry their page number)
    u16 pageState; //0xA, 1: waiting for the key, 2: advance to the next page
    u16 pageAdvanced; //0xC
    u8 pad_0E[0x10 - 0xE]; //0xE
    s32 nextEntry; //0x10, index into entries
    u8 pad_14[0x18 - 0x14]; //0x14
    OSTime openTime; //0x18, set when the window opens; drives the open anim (SPM openTime)
    s32 playTime; //0x20, playback clock (ms) compared with entries[].time
    s32 scrollTarget; //0x24
    s32 scrollPos; //0x28, eases toward scrollTarget
    u8 pad_2C[0x30 - 0x2C]; //0x2C
    OSTime lastUpdateTime; //0x30, reset every msgMain; the delta advances the scroll
    s32 charCount; //0x38, MsgChar entries in use
#ifdef VERSION_G8ME01
    MsgChar entries[0xA00]; //0x3C (US: twice as many, so every field below moves by 0x7800)
#else
    MsgChar entries[0x500]; //0x3C
#endif
    s32 page; //0x783C
    s32 pageCount; //0x7840, number of pageY entries
    s32 pageY[(0x79FC - 0x7844) / 4]; //0x7844
    s32 callback; //0x79FC, void (*)(s32 event, smartEntry*), see MSG_CB
    struct EventEntry* event; //0x7A00 (US 0xF200), event that opened the window; read by map msg callbacks
    char speakerName[0x20]; //0x7A04, speaker NPC name, passed to npcNameToPtr (SPM npcName)
    s32 unitId; //0x7A24
    s32 talkSfxGroup; //0x7A28, <se n> tag value (0xFFFA entry); not read in C
    u8 talkVolume; //0x7A2C, default 0x7F
    u8 pad_7A2D[0x7A30 - 0x7A2D]; //0x7A2D
    s32 parseTime; //0x7A30, msgAnalize state
    s16 parseX; //0x7A34
    s16 parseY; //0x7A36
    s16 parsePage; //0x7A38, msgAnalize: key-wait counter stored in 0xFFFE entries' x
    s16 parseScroll; //0x7A3A, msgAnalize: pending scroll stored in 0xFFFD entries' y
    s32 parseSpeed; //0x7A3C, text speed (default 25)
    s8 numSel; //0x7A40
    s8 defSel; //0x7A41
    s8 cancel; //0x7A42
    s8 sel; //0x7A43
    s16 selWidth; //0x7A44
    s16 selY; //0x7A46
    f32 cursorY; //0x7A48
    WindowEntry* window; //0x7A4C
    s32 talkTimer; //0x7A50, frames until the talk pose is set
    s32 talkPoseSet; //0x7A54
} MsgWinData;

typedef struct MessageEntry {
    u32 length; //0x0
    void* address; //0x4
    s32 count; //0x8
    smartEntry* lookup; //0xC
} MessageEntry;

typedef struct MessageWork {
    MessageEntry entries[2]; //0x0
    s32 poseId; //0x20
} MessageWork;

void msgInit(void);
void msgLoad(const char* filename, s32 index);


MessageWork* msgGetWork(void);

const char* msgSearch(const char* tag);
BOOL _ismbblead(u32 c);

char* msgGetCommand(char* msg, char* cmd, char* arg);
s32 msgIconStr2ID(const char* str);

extern const char R_no_messages_JP[];
extern const char R_no_messages_US[];

extern const char R_no_messages_DE[];
extern const char R_no_messages_DU[];
extern const char R_no_messages_ES[];
extern const char R_no_messages_FR[];
extern const char R_no_messages_IT[];
void msgWindow_Continue(s32 id);
void msgWindow_Repeat(s32 id);
