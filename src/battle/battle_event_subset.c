#include "battle/battle_event_subset.h"
extern void* _battleWorkPointer;

s32 _disable_restore_command_cursor(void) {
    void* battleWork;

    battleWork = _battleWorkPointer;
    *(s32*)((s32)battleWork + 0x1C70) = 0;
    *(u8*)((s32)battleWork + 0x1C74) = 0;

    return 2;
}


u8 _add_star_point_disp_offset(void) {
    return 0;
}


s32 _check_through_pos(int param_1) {
    return 0;
}


s32 _binta_effect(int param_1) {
    return 0;
}


s32 _check_at_dm_event_wait(int param_1) {
    return 0;
}
