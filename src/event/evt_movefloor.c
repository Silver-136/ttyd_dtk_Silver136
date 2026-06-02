#include "event/evt_movefloor.h"
#include "event/evt_cmd.h"

extern f32 float_500_8042658c;

USER_FUNC(moving_floor_halt_se) {
    void* floor = (void*)evtGetValue(event, event->args[0]);
    *(s32*)((s32)floor + 0x78) = 1;
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(moving_floor_get_mode) {
    s32* args = event->args;
    void* floor = (void*)evtGetValue(event, args[0]);

    evtSetValue(event, args[1], *(s32*)((s32)floor + 0x20));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(moving_floor_get_kind) {
    s32* args = event->args;
    void* floor = (void*)evtGetValue(event, args[0]);

    evtSetValue(event, args[1], *(s32*)((s32)floor + 4));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(moving_floor_get_pause) {
    s32* args = event->args;
    void* floor = (void*)evtGetValue(event, args[0]);

    if (*(s32*)floor & 2) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(moving_floor_getf_xz_scale) {
    s32* args = event->args;
    void* floor = (void*)evtGetValue(event, args[0]);
    s32 value = *(f32*)((s32)floor + 0x48) + *(f32*)((s32)floor + 0x5C);
    f32 scale = evtGetFloat(event, value);

    evtSetFloat(event, args[1], (float_500_8042658c - scale) / float_500_8042658c);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_moving_floor_alloc(void* pEvt) {
    return 0;
}


u8 evt_moving_floor_init(s32 pEvt) {
    return 0;
}


s32 evt_moving_floor_main(int param_1) {
    return 0;
}


s32 moving_floor_getf_pos(int param_1) {
    return 0;
}


s32 moving_floor_set_se(void* param_1) {
    return 0;
}


s32 moving_floor_get_end(int param_1) {
    return 0;
}


s32 evt_moving_floor_delete(int param_1) {
    return 0;
}


s32 moving_floor_reverse(void* pEvt) {
    return 0;
}


s32 moving_floor_getf_now_bpoint(int param_1) {
    return 0;
}


s32 moving_floor_update_turn(int param_1) {
    return 0;
}


s32 moving_floor_get_stmsec(void* pEvt) {
    return 0;
}


s32 evt_moving_floor_free(int param_1) {
    return 0;
}


s32 moving_floor_stop_se(int param_1) {
    return 0;
}


s32 evt_moving_floor_main2(void* param_1) {
    return 0;
}
