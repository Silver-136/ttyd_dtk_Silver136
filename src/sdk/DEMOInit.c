#include "sdk/DEMOInit.h"

void* DemoCurrentBuffer;
void* rmode;

void* L_DEMOGetCurrentBuffer(void) {
    return DemoCurrentBuffer;
}

void* DEMOGetRenderModeObj(void) {
    return rmode;
}

