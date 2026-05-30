#include "window/win_mario.h"

char tmp2[0x100];
extern const char str_PCTd_80423948[3];

void animPoseRelease(s32 poseId);
s32 sprintf(char* str, const char* fmt, ...);

void winMarioMain2(void) {
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


u8 winMarioInit2(void* pWin) {
    return 0;
}
