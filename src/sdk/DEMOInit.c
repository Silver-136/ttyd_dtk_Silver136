#include "sdk/DEMOInit.h"

void* DemoCurrentBuffer;
void* rmode;

void* L_DEMOGetCurrentBuffer(void) {
    return DemoCurrentBuffer;
}

void* DEMOGetRenderModeObj(void) {
    return rmode;
}


u8 __NoHangDoneRender(void) {
    return 0;
}


u8 DEMOInit(void* param_1) {
    return 0;
}


void __DEMODiagnoseHang(void) {
    extern void GXReadXfRasMetric(s32* xf, s32* ras, s32* xfiread, s32* xfwrite);
    extern void GXGetGPStatus(u8* overhi, u8* underlow, u8* readIdle, u8* cmdIdle, u8* brkpt);
    extern s32 sprintf(char* s, const char* fmt, ...);
    extern char* strcat(char* dst, const char* src);
    extern char* strcpy(char* dst, const char* src);
    extern char __GXErrorMessage[];
    extern const char str_GP_status_PCTdPCTdPC_802bfea0[];
    extern const char str_GP_hang_due_to_XF_st_802bfebc[];
    extern const char str_GP_hang_due_to_unter_802bfedc[];
    extern const char str_GP_hang_due_to_illeg_802bff04[];
    extern const char str_GP_appears_to_be_not_802bff2c[];
    extern const char str_GP_is_in_unknown_sta_802bff5c[];
    char msg[4096];
    char add[4112];
    s32 xfTop0;
    s32 rasTop0;
    s32 xfBot0;
    s32 rasBot0;
    s32 xfTop1;
    s32 rasTop1;
    s32 xfBot1;
    s32 rasBot1;
    u8 overhi;
    u8 cmdIdle;
    u8 readIdle[2];
    s32 sameXfTop;
    s32 sameRasTop;
    s32 diffRasBot;
    s32 diffXfBot;

    GXReadXfRasMetric(&xfTop0, &rasTop0, &xfBot0, &rasBot0);
    GXReadXfRasMetric(&xfTop1, &rasTop1, &xfBot1, &rasBot1);
    diffRasBot = rasBot1 != rasBot0;
    sameRasTop = rasTop1 == rasTop0;
    sameXfTop = xfTop1 == xfTop0;
    diffXfBot = xfBot1 != xfBot0;
    GXGetGPStatus(&overhi, &overhi, readIdle, &cmdIdle, &overhi);
    sprintf(msg, str_GP_status_PCTdPCTdPC_802bfea0, readIdle[0], cmdIdle, sameRasTop, sameXfTop, diffRasBot, diffXfBot);
    if (sameXfTop == 0 && diffRasBot) {
        sprintf(add, str_GP_hang_due_to_XF_st_802bfebc);
        strcat(msg, add);
    } else if (sameRasTop == 0 && sameXfTop != 0 && diffRasBot) {
        sprintf(add, str_GP_hang_due_to_unter_802bfedc);
        strcat(msg, add);
    } else if (cmdIdle == 0 && sameRasTop != 0 && sameXfTop != 0 && diffRasBot) {
        sprintf(add, str_GP_hang_due_to_illeg_802bff04);
        strcat(msg, add);
    } else if (readIdle[0] != 0 && cmdIdle != 0 && sameRasTop != 0 && sameXfTop != 0 && diffRasBot && diffXfBot) {
        sprintf(add, str_GP_appears_to_be_not_802bff2c);
        strcat(msg, add);
    } else {
        sprintf(add, str_GP_is_in_unknown_sta_802bff5c);
        strcat(msg, add);
    }
    strcpy(__GXErrorMessage, add);
}

void DEMOBeforeRender(void) {
}


u8 DEMOEnableGPHangWorkaround(int param_1) {
    return 0;
}


void DEMODoneRender(void) {
}


u8 __NoHangRetraceCallback(void) {
    return 0;
}
