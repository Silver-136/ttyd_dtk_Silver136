// nameent.c: the name entry screen (NameEntryWork, nameEntOn/Off/Main) used to name a save file or Yoshi.

#pragma once

#include "manager/filemgr.h"
#include <dolphin/types.h>

// nameEntOn type: what is being named, which selects the prompt (msg_tbl) and the rules
typedef enum NameEntryType {
    NAMEENT_FILE = 0,       // the save file name (seq_load)
    NAMEENT_YOSHI = 1,      // Yoshi's name (tou_10); blank gives his default name
    NAMEENT_DOOPLISS = 2,   // guessing Doopliss's name before the Letter P is back (gra_00): n/N/p blanked
    NAMEENT_DOOPLISS_P = 3, // the same guess afterwards
    NAMEENT_PASSWORD = 4,   // the X-Naut Fortress password (aji_01)
} NameEntryType;

typedef struct NameEntryWork {
    u16 flags; //0x0, bit0 = nameEntWait called (permits input), bit1 = cancelled
    u8 pad_2[2]; //0x2
    s32 loadState; //0x4, tpl load step (0 idle, 1 load, 3 free)
    s32 state; //0x8, input state; 100 = confirm, 101 = finished (confirmed or cancelled)
    s32 type; //0xC, NameEntryType
    s32 cursorX; //0x10, character grid column
    s32 cursorY; //0x14, character grid row
    s32 nameLen; //0x18, characters entered (u16 SJIS units)
    f32 x1; //0x1C
    f32 y1; //0x20
    f32 x2; //0x24
    f32 y2; //0x28
    f32 caretX; //0x2C, eased toward caretTargetX
    f32 caretY; //0x30
    f32 caretTargetX; //0x34
    f32 caretTargetY; //0x38
    char name[16]; //0x3C
    u8 pad_4C[0x50 - 0x4C]; //0x4C
    const char** charTable; //0x50, hiragana_tbl or katakana_tbl; US and later also use latin_tbl
    f32 maskAlpha; //0x54, 0..1 fade of the confirm mask
    u8 pad_58[0x5C - 0x58]; //0x58
    s32 confirmSel; //0x5C, yes/no toggle on the confirm prompt
    FileEntry* tplFile; //0x60, name.tpl
} NameEntryWork;

void nameEntInit(void);
void nameEntReInit(void);
void nameEntMain(void);

extern const char* latin_tbl[];
const char* nameEntGetName(void);
BOOL nameEntIsCancel(void);
void nameEntOff(void);
void nameEntOn(s32 type);
BOOL nameEntPrepare(void);
BOOL nameEntWait(void);
void nameMaskGX(f32 t);

// Names from LLCOOLT's tree.
enum {
    NAMEENT_FLAG_INPUT_ON = 1,
    NAMEENT_FLAG_CANCELLED = 2,
};
