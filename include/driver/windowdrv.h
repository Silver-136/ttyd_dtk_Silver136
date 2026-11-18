// windowdrv.c: the window driver (WindowEntry, WindowType): window frames, speech bubbles and system windows.

#pragma once

#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "memory.h"
#include "sdk/texPalette.h"

#define WINDOW_MAX 7

typedef struct WindowEntry WindowEntry;

typedef enum WindowType {
    WINDOW_TYPE_NORMAL = 0,
    WINDOW_TYPE_SMALL = 1,
    WINDOW_TYPE_PLATE = 2,
    WINDOW_TYPE_SYSTEM = 3,
    WINDOW_TYPE_KANBAN = 4, // kanban = sign/billboard
    WINDOW_TYPE_SELECT = 5,
    WINDOW_TYPE_BOSS = 6,
    WINDOW_TYPE_DIARY = 7,
    WINDOW_TYPE_TEC = 8,
    WINDOW_TYPE_MAJO = 9, // majo = witch, shadow sirens
    WINDOW_TYPE_HOUSOU = 10, // housou = broadcast/announcement
    WINDOW_TYPE_CLEAR = 11
} WindowType;

// WindowEntry.flags
typedef enum WindowEntryFlags {
    WINDOW_FLAG_ACTIVE = (1 << 0),  // 0x1, slot in use
    WINDOW_FLAG_BUSY = (1 << 1),    // 0x2, windowCheckID; scripts wait on it
    WINDOW_FLAG_WPOS = (1 << 2),    // 0x4, message tag <wpos>
    WINDOW_FLAG_NOWINSE = (1 << 3), // 0x8, message tag <nowinse>: no window sound
} WindowEntryFlags;

// WindowEntry.tailMode, the speech-balloon tail (set by evt_msg's print functions; read by msgdrv and
// windowDispGX_Message). Any nonzero value makes msgdrv open the balloon from the speaker position. 0x2 is set
// with every tail mode but no code tests it alone.
typedef enum WindowTailFlags {
    WINDOW_TAIL_DRAW = (1 << 0),       // 0x1, draw the tail
    WINDOW_TAIL_RIGHT = (1 << 2),      // 0x4, tail base at 0.7 of the width (else 0.38): speaker to the right
    WINDOW_TAIL_TOP = (1 << 3),        // 0x8, tail on the top edge pointing up; small windows sit below the speaker
    WINDOW_TAIL_SCREEN_POS = (1 << 4), // 0x10, speakerPos is a screen position already (no getScreenPoint)
} WindowTailFlags;

struct WindowEntry {
    u16 state; // 0x0, message-window state
    u16 flags; // 0x2
    s16 priority; // 0x4, windowEntry argument; display z = 400 - priority
    s16 alpha; // 0x6, window alpha
    s32 type; // 0x8, WindowType
    f32 x; // 0xC, window xpos
    f32 y; // 0x10, window ypos
    f32 width; // 0x14, horizontal extent
    f32 height; // 0x18, vertical extent
    u8 pad_1C[0x20 - 0x1C]; // 0x1C, never accessed
    OSTime timestamp;               // 0x20, gp->renderTime at the start of the current fade/scroll step
    smartEntry* userdata; // 0x28
    s16 tailMode; // 0x2C, WindowTailFlags; 0 = no tail, balloon drawn in place
    Vec speakerPos; // 0x30, speech-bubble tail target (world position unless WINDOW_TAIL_SCREEN_POS)
    void (*main)(WindowEntry* entry); // 0x3C
    void (*delete)(WindowEntry* entry); // 0x40
    u8 pad_44[0x48 - 0x44]; // 0x44, never accessed
};

void windowInit(void);
void windowTexSetup(void);
void windowReInit(void);
s32 windowEntry(s16 priority);
BOOL windowDelete(WindowEntry* entry);
BOOL windowDeleteID(s32 id);
void windowMain(void);
void windowDispGX_Kanban(s32 type, u8 alpha, f32 x, f32 y, f32 width, f32 height);
void windowDispGX_System(s32 type, u8 alpha, f32 x, f32 y, f32 width, f32 height);
void windowDispGX_Message(s32 type, s32 tailFlags, u8 alpha, f32 x, f32 y, f32 width, f32 height, f32 tailX, f32 tailY);
void windowDispGX_Waku_col(f32 x, f32 y, f32 width, f32 height, f32 curve, u16 gxTexMapID, GXColor color);
void windowDispGX2_Waku_col(Mtx mtx, u16 gxTexMapID, GXColor color, f32 x, f32 y, f32 width, f32 height, f32 curve);
s32 windowCheckID(s32 id);
WindowEntry* windowGetPointer(s32 id);
GXTexObj* getWakuTexObj(s32 id);

extern GXTexObj* wakuTexObj;
extern s16 windowTexNumber;

extern BOOL tpl_loaded;
