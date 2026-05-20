#include "battle/battle_event_subset.h"
extern void* _battleWorkPointer;

s32 _disable_restore_command_cursor(void) {
    void* battleWork;

    battleWork = _battleWorkPointer;
    *(s32*)((s32)battleWork + 0x1C70) = 0;
    *(u8*)((s32)battleWork + 0x1C74) = 0;

    return 2;
}