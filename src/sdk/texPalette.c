#include "sdk/texPalette.h"

void* TEXGet(void* tpl, s32 id) {
    return (void*)((s32)*(void**)((s32)tpl + 8) + id * 8);
}


void* UnpackTexPalette(void* data) {
    return 0;
}


void TEXGetGXTexObjFromPalette(void* palette, void* texObj, s32 id) {
    ;
}
