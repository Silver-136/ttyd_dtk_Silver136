#include "window/win_mario.h"

char tmp2[0x100];
extern const char str_PCTd_80423948[3];

void animPoseRelease(s32 poseId);
s32 sprintf(char* str, const char* fmt, ...);
typedef struct WinMarioLinkEntry {
    s16 x;
    s16 y;
    char pad_04[0x10];
    char* msg;
    char pad_18[4];
} WinMarioLinkEntry;
extern WinMarioLinkEntry linkDt[];
extern char* hammer_help[4];
extern char* boots_help[4];
s32 pouchGetHammerLv(void);
s32 pouchGetJumpLv(void);
void winMsgEntry(void* pWin, s32 param_2, char* msg, s32 param_4);

void winMarioMain2(void) {
    ;
}

char* winZenkakuStr(s32 value) {
    sprintf(tmp2, str_PCTd_80423948, value);
    return tmp2;
}

void winMarioExit(void* wp) {
    animPoseRelease(*(s32*)((s32)wp + 0x188));
}


u8 winMarioDisp(void) {
    return 0;
}


u8 fukidashi(double x, double y, void* menu, int type) {
    return 0;
}


s32 winMarioMain(void* pWin) {
    return 0;
}


u8 winMarioInit(void* pWin) {
    return 0;
}


void winMarioInit2(void* pWin) {
    s32 index;
    f32 value;

    *(s32*)((s32)pWin + 0x160) = 0;
    index = *(s32*)((s32)pWin + 0x160);
    value = (f32)linkDt[index].x;
    *(f32*)((s32)pWin + 0x158) = value;
    *(f32*)((s32)pWin + 0x158) = value;
    index = *(s32*)((s32)pWin + 0x160);
    value = (f32)linkDt[index].y;
    *(f32*)((s32)pWin + 0x15C) = value;
    *(f32*)((s32)pWin + 0x15C) = value;

    switch (*(s32*)((s32)pWin + 0x160)) {
        case 2:
            winMsgEntry(pWin, 0, hammer_help[pouchGetHammerLv()], 0);
            break;
        case 3:
            winMsgEntry(pWin, 0, boots_help[pouchGetJumpLv()], 0);
            break;
        default:
            winMsgEntry(pWin, 0, linkDt[*(s32*)((s32)pWin + 0x160)].msg, 0);
            break;
    }
    *(s32*)((s32)pWin + 0x18C) = 0;
    *(s32*)((s32)pWin + 0x190) = 0;
    *(s32*)((s32)pWin + 0x194) = 0;
}

char* unk_801703e8(s32 value, s32 width) {
    extern const char str_PCTPCT0PCTdd_802f6688[];
    extern char unk_803e5f78[];
    char buffer[0x100];

    sprintf(buffer, str_PCTPCT0PCTdd_802f6688, width);
    sprintf(unk_803e5f78, buffer, value);
    return unk_803e5f78;
}
