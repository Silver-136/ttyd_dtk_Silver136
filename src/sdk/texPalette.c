#include "sdk/texPalette.h"

void* TEXGet(void* tpl, s32 id) {
    return (void*)((s32)*(void**)((s32)tpl + 8) + id * 8);
}
