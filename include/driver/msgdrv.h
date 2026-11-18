#pragma once

#include "version.h"
#include <dolphin/os.h>
#include "driver/windowdrv.h"
#include "memory.h"

typedef void (*MsgWindowCallback)(s32 event, smartEntry* storage);

// MsgChar.value of a text character (msgAnalize, read by msgDisp)
typedef enum MsgCharFlags {
    MSG_CHAR_DYNAMIC = (1 << 0),        // 0x1, <dynamic>: drawn rotated by rot
    MSG_CHAR_SHAKE = (1 << 1),          // 0x2, <shake>
    MSG_CHAR_WAVE = (1 << 2),           // 0x4, <wave>
    MSG_CHAR_TALK = (1 << 3),           // 0x8, plays the voice and moves the speaker's mouth
    MSG_CHAR_VOICE = (1 << 4),          // 0x10, plays the voice only (set for the '・' character)
    MSG_CHAR_SEL_ROW_MASK = 0xFF00,     // <select> windows: the row of the character (MSG_CHAR_SEL_ROW)
    MSG_CHAR_SCALE_X = (1 << 16),       // 0x10000, <scaleX> (US and later): stretched on x only
} MsgCharFlags;
#define MSG_CHAR_SEL_ROW(n) (0x100 << (n))

// WindowEntry.state of a message window (msgWindow_Main, msgWindow_Clear_Main).
enum {
    MSG_WIN_STATE_PRINT = 1,
    MSG_WIN_STATE_WAIT_TAG = 2,    // stopped at <o>
    MSG_WIN_STATE_KEY_WAIT = 3,
    MSG_WIN_STATE_SELECT_DONE = 4,
    MSG_WIN_STATE_OPEN = 5,
    MSG_WIN_STATE_FADE_IN = 6,
    MSG_WIN_STATE_CLOSE = 7,
    MSG_WIN_STATE_FADE_OUT = 8,
    MSG_WIN_STATE_REVIEW_UP = 9,   // Z review: earlier pages
    MSG_WIN_STATE_REVIEW_DOWN = 10,
    MSG_WIN_STATE_REVIEW_KEY_WAIT = 11,
};

typedef struct MsgChar {
    union {
        u32 value; // 0x0
        GXColor color; // 0x0, colour-change entries
        struct NpcEntry* npc; // 0x0, paper entries (NULL = the speaker)
    };
    u16 glyph; // 0x4
    s16 x; // 0x6 (page-break entries: page number)
    s16 y; // 0x8 (page-scroll entries: scroll amount)
    s16 iconY; // 0xA, icon entries: extra y offset
    u32 time; // 0xC
    f32 scale; // 0x10
    f32 rot; // 0x14, animation entries: rotation; dynamic-scale entries: current scale
} MsgChar;

// MsgWinData.flags (msgdrv, evt_msg). 0x1 (set at creation, never read) has only LLCOOLT's name MSG_WIN_FLAG_OPEN
// at the end of this file.
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
    const char* tag; // 0x0, message tag ("DIRECT" for direct text; evt_msg); msgWindow_ForceClose matches it
    u32 flags; // 0x4, MsgWinFlags
    u16 curPage; // 0x8, page being shown (page-break entries carry their page number)
    u16 pageState; // 0xA, 1: waiting for the key, 2: advance to the next page
    u16 pageAdvanced; // 0xC, set to 1 when the next page starts; never read
    s32 nextEntry; // 0x10, index into entries
    OSTime openTime; // 0x18, set when the window opens; drives the open anim (SPM openTime)
    s32 playTime; // 0x20, playback clock (ms) compared with entries[].time
    s32 scrollTarget; // 0x24, scroll position scrollPos eases toward (scroll entries lower it; msgPageAdd logs it)
    s32 scrollPos; // 0x28, eases toward scrollTarget
    OSTime lastUpdateTime; // 0x30, reset every msgMain; the delta advances the scroll
    s32 charCount; // 0x38, MsgChar entries in use
#if VERSION >= VERSION_US
    MsgChar entries[0xA00]; // 0x3C (US and later: twice as many, so every field below moves by 0x7800)
#else
    MsgChar entries[0x500]; // 0x3C
#endif
    s32 page; // 0x783C (US 0xF03C), pageY index shown while paging back with Z
    s32 pageCount; // 0x7840 (US 0xF040), number of pageY entries
    s32 pageY[(0x79FC - 0x7844) / 4]; // 0x7844 (US 0xF044), scroll position of each logged page (msgPageAdd, up to 110)
    MsgWindowCallback callback; // 0x79FC (US 0xF1FC), message event callback
    struct EventEntry* event; // 0x7A00 (US 0xF200), event that opened the window; read by map msg callbacks
    char speakerName[0x20]; // 0x7A04 (US 0xF204), speaker NPC name, passed to npcNameToPtr (SPM npcName)
    s32 unitId; // 0x7A24 (US 0xF224), battle unit that speaks (evt_msg; MSG_WIN_FLAG_BATTLE_UNIT windows)
    s32 talkSfxGroup; // 0x7A28 (US 0xF228), <se n> tag value (0xFFFA entry); not read in C
    u8 talkVolume; // 0x7A2C (US 0xF22C), default 0x7F
    s32 parseTime; // 0x7A30 (US 0xF230), msgAnalize state
    s16 parseX; // 0x7A34 (US 0xF234), msgAnalize: x of the next character
    s16 parseY; // 0x7A36 (US 0xF236)
    s16 parsePage; // 0x7A38 (US 0xF238), msgAnalize: key-wait counter stored in 0xFFFE entries' x
    s16 parseScroll; // 0x7A3A (US 0xF23A), msgAnalize: pending scroll stored in 0xFFFD entries' y
    s32 parseSpeed; // 0x7A3C (US 0xF23C), text speed (default 25)
    s8 numSel; // 0x7A40 (US 0xF240), choices of the select window
    s8 defSel; // 0x7A41 (US 0xF241), choice selected when it opens (first <select> number)
    s8 cancel; // 0x7A42 (US 0xF242), B: -1 ignored, -2 closes with sel = -1, else closes with sel = cancel
    s8 sel; // 0x7A43 (US 0xF243), selected choice (wraps)
    s16 selWidth; // 0x7A44 (US 0xF244), select window width (<select> value; 0: widest row + 80)
    s16 selY; // 0x7A46 (US 0xF246), select window y (<select> value)
    f32 cursorY; // 0x7A48 (US 0xF248), y of the choice cursor; eases toward the selected row
    WindowEntry* window; // 0x7A4C (US 0xF24C), the window this data belongs to
    s32 talkTimer; // 0x7A50 (US 0xF250), frames the talk pose lasts (15 from msgTalkPoseOn); at 0 the stay pose returns
    s32 talkPoseSet; // 0x7A54 (US 0xF254), 1 while the talk pose is on (msgTalkPoseMain)
} MsgWinData;

typedef struct MessageEntry {
    u32 length; // 0x0
#if VERSION <= VERSION_DEMO_JP
    smartEntry* address;
#else
    void* address; // 0x4
#endif
    s32 count; // 0x8
    smartEntry* lookup; // 0xC
} MessageEntry;

typedef struct MessageWork {
    MessageEntry entries[2]; // 0x0
    s32 poseId; // 0x20
} MessageWork;

void msgInit(void);
void msgLoad(const char* filename, s32 index);

MessageWork* msgGetWork(void);

const char* msgSearch(const char* tag);
BOOL _ismbblead(u32 c);

char* msgGetCommand(char* msg, char* cmd, char* arg);
s32 msgIconStr2ID(const char* str);

s32 msgWindow_Entry(const char* msg, s32 callback, s16 priority);
void msgWindow_Continue(s32 id);
void msgWindow_Repeat(s32 id);
void msgWindow_ForceClose(const char* name);
void msgPageAdd(smartEntry* smart, s32 y);

// Names from LLCOOLT's tree.
enum {
    MSG_GLYPH_PAPER_END = 65522,
    MSG_GLYPH_PAPER = 65523,
    MSG_GLYPH_ANIM = 65524,
    MSG_GLYPH_DKEY = 65525,
    MSG_GLYPH_NOISE = 65526,
    MSG_GLYPH_COLOR = 65527,
    MSG_GLYPH_ICON = 65528,
    MSG_GLYPH_VOL = 65529,
    MSG_GLYPH_SE = 65530,
    MSG_GLYPH_NEWLINE = 65531,
    MSG_GLYPH_ONCE_STOP = 65532,
    MSG_GLYPH_SCROLL = 65533,
    MSG_GLYPH_KEY = 65534,
    MSG_GLYPH_END = 65535,
};
#define MSG_WIN_FLAG_OPEN 0x1
