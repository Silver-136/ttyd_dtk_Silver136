#include "window/win_party.h"

typedef struct WinPartyDtEntry {
    char pad_00[0xC];
    char* msg;
    char pad_10[0x14];
} WinPartyDtEntry;

extern f32 float_360_80423550;
extern f32 float_neg200_804235fc;
extern f32 float_20_804234fc;
extern WinPartyDtEntry winPartyDt[];
void winMsgEntry(void* pWin, s32 param_2, char* msg, s32 param_4);

void winPartyMain2(void* work) {
    extern f32 float_0p25_804235f8;
    f32 target = *(f32*)((s32)work + 0x1E8);
    f32 current = *(f32*)((s32)work + 0x1E4);
    f32 rate = float_0p25_804235f8;

    *(f32*)((s32)work + 0x1E4) = (target - current) * rate + current;
}


u8 winPartyDisp(s32 param_1, void* pWin, s32 param_3) {
    return 0;
}


s32 winPartyMain(void* pWin) {
    return 0;
}


u8 winPartyInit(void* pWin) {
    return 0;
}


void winPartyInit2(void* pWin) {
    f32 angle;
    f32 scale;
    s32 denom = *(s32*)((s32)pWin + 0x1E0);
    s32 numer = *(s32*)((s32)pWin + 0x1DC);
    f32 neg = float_neg200_804235fc;
    f32 twenty = float_20_804234fc;

    scale = float_360_80423550 / (f32)denom;
    angle = scale * (f32)numer;
    *(f32*)((s32)pWin + 0x1E8) = angle;
    *(f32*)((s32)pWin + 0x1E4) = angle;
    *(s32*)((s32)pWin + 0x200) = 0;
    *(f32*)((s32)pWin + 0x158) = neg;
    *(f32*)((s32)pWin + 0x15C) = twenty;
    winMsgEntry(pWin, 0, winPartyDt[*(s32*)((s32)pWin + 0x1D8)].msg, 0);
}


u8 winPartyExit(void* pWin) {
    return 0;
}
