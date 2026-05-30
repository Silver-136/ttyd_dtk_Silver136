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


u8 __DEMODiagnoseHang(void) {
    return 0;
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
