#pragma once

#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "memory.h"
#include "sdk/texPalette.h"

typedef struct WindowEntry WindowEntry;

typedef enum WindowType {
    WINDOW_TYPE_NORMAL = 0,
    WINDOW_TYPE_SMALL = 1,
    WINDOW_TYPE_PLATE = 2,
    WINDOW_TYPE_SYSTEM = 3,
    WINDOW_TYPE_KANBAN = 4, //kanban = sign/billboard
    WINDOW_TYPE_SELECT = 5,
    WINDOW_TYPE_BOSS = 6,
    WINDOW_TYPE_DIARY = 7,
    WINDOW_TYPE_TEC = 8,
    WINDOW_TYPE_MAJO = 9, //majo = witch, shadow sirens
    WINDOW_TYPE_HOUSOU = 10, //housou = broadcast/announcement
    WINDOW_TYPE_CLEAR = 11
} WindowType;

#define WINDOW_FLAG_ACTIVE 1 // WindowEntry.flags: slot in use

struct WindowEntry {
    u8 state[2]; //0x0, u16 message-window state (WIN_STATE in msgdrv.c)
    u16 flags; //0x2
    s16 priority; //0x4, windowEntry argument; display z = 400 - priority
    s16 alpha; //0x6, window alpha
    s32 type; //0x8, WindowType
    f32 x; //0xC, window xpos
    f32 y; //0x10, window ypos
    f32 height; //0x14
    f32 width; //0x18
    u8 pad_1C[0x20 - 0x1C]; // 0x1C, never accessed
    OSTime timestamp;               // 0x20, gp->renderTime at the start of the current fade/scroll step
    smartEntry* userdata; //0x28
    s16 tailMode; //0x2C, speech-bubble tail: 0 none, 0x10 position already on screen, 8 window above the speaker
    u8 pad_2E[2]; //0x2E
    Vec speakerPos; //0x30, speech-bubble tail target (world position unless tailMode & 0x10)
    void (*main)(WindowEntry* entry); //0x3C
    void (*delete)(WindowEntry* entry); //0x40
    u8 pad_44[0x48 - 0x44]; //0x44, never accessed
};

void windowInit(void);
void windowTexSetup(void);
void windowReInit(void);
s32 windowEntry(u16 priority);
BOOL windowDelete(WindowEntry* entry);
BOOL windowDeleteID(s32 id);
void windowMain(void);
void windowDispGX_Kanban(s32 type, u8 alpha, f32 x, f32 y, f32 height, f32 width);
void windowDispGX_System(s32 type, u8 alpha, f32 x, f32 y, f32 height, f32 width);
void windowDispGX_Message(s32 type, s32 tailFlags, u8 alpha, f32 x, f32 y, f32 height, f32 width, f32 tailX, f32 tailY);
void windowDispGX_Waku_col(u16 gxTexMapID, GXColor color, f32 x, f32 y, f32 width, f32 height, f32 curve);
void windowDispGX2_Waku_col(Mtx mtx, u16 gxTexMapID, GXColor color, f32 x, f32 y, f32 width, f32 height, f32 curve);
s32 windowCheckID(s32 id);
WindowEntry* windowGetPointer(s32 id);
GXTexObj* getWakuTexObj(s32 id);

extern GXTexObj* wakuTexObj;

extern BOOL tpl_loaded;
extern TEXPalette* winTpl;
