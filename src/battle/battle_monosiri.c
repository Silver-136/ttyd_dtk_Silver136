#include "battle/battle_monosiri.h"

void* battleGetUnitMonosiriPtr(s32 id) {
    extern u8 monosiriMessageTable[];

    return &monosiriMessageTable[id * 0x18];
}
