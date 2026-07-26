#pragma once

#include <dolphin/os.h>

typedef struct GlobalWork {
    s32 flags;                   //0x0
    u8 unk4[0x14 - 0x4];         //0x4
    BOOL inBattle;               //0x14
    u8 unk18[0x1314 - 0x18];     //0x18
    OSTick deltaGame;            //0x1314
    OSTick deltaRender;          //0x1318
    u8 unk131C[0x13D8 - 0x131C]; //0x131C
} GlobalWork;
STATIC_ASSERT_SIZEOF(GlobalWork, 0x13D8);

extern GlobalWork* gp;

void marioStInit(void);
void marioStMain(void);
void marioStDisp(void);
void marioStSystemLevel(s32 level);
