#pragma once

#include "manager/filemgr.h"
#include <dolphin/types.h>

typedef struct NameEntryWork {
    u16 flags; //0x0, bit0 = done/closing, bit1 = cancelled
    u8 pad_2[2]; //0x2
    s32 loadState; //0x4, tpl load step (0 idle, 1 load, 3 free)
    s32 state; //0x8, input state; 100 = confirm, 0x65 = cancelled
    s32 type; //0xC, nameEntInit type; 1 = partner (default name "name_party3")
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
    const char** charTable; //0x50, hiragana_tbl or katakana_tbl
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
