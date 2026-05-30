#include "window/win_item.h"

void winItemExit(void* work) {
    extern void winMgrDelete(void* window);

    winMgrDelete(*(void**)((s32)work + 0x1210));
    winMgrDelete(*(void**)((s32)work + 0x1214));
}


u8 itemUseDisp(void* pWinMgr) {
    return 0;
}


u8 itemUseDisp2(void* pWinMgr) {
    return 0;
}


s32 winItemMain(void* pWin) {
    return 0;
}


u8 item_disp(s64 param_1, s64 param_2, void* pWin) {
    return 0;
}


u8 winItemDisp(s32 param_1, void* pWin, s32 param_3) {
    return 0;
}


u8 winItemInit(void* pWin) {
    return 0;
}


u8 winItemInit2(void* pWin) {
    return 0;
}


u8 winMakeSkipList(void* pWin) {
    return 0;
}


u8 winItemMain2(void* pWin) {
    return 0;
}
