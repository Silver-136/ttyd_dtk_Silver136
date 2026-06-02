#include "event/evt_item.h"

s32 evtGetValue(EventEntry* event, s32 value);
f32 evtGetFloat(EventEntry* event, s32 value);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void* itemNameToPtr(s32 name);
void itemDelete(s32 name);
void itemFlagOff(void* item, u16 flag);
void itemFlagOn(void* item, u16 flag);
void itemStatusOn(void* item, s32 status);

USER_FUNC(evt_item_delete) {
    itemDelete(evtGetValue(event, event->args[0]));
    return 2;
}

USER_FUNC(evt_item_status_on) {
    s32* args = event->args;
    void* item = itemNameToPtr(evtGetValue(event, args[0]));
    s32 status = args[1];
    if (item == 0) {
        return 2;
    }
    itemStatusOn(item, status);
    return 2;
}

USER_FUNC(evt_item_flag_off) {
    s32* args = event->args;
    void* item = itemNameToPtr(evtGetValue(event, args[0]));
    u16 flag = args[1];
    if (item == 0) {
        return 2;
    }
    itemFlagOff(item, flag);
    return 2;
}

USER_FUNC(evt_item_flag_on) {
    s32* args = event->args;
    void* item = itemNameToPtr(evtGetValue(event, args[0]));
    u16 flag = args[1];
    if (item == 0) {
        return 2;
    }
    itemFlagOn(item, flag);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_item_set_bound_limit) {
    s32* args = event->args;
    void* item = itemNameToPtr(evtGetValue(event, args[0]));
    s32 value = evtGetValue(event, args[1]);
    if (item == 0) {
        return 2;
    }
    *(s32*)((s32)item + 0x88) = value;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_item_set_bound_next_dir) {
    s32* args = event->args;
    void* item = itemNameToPtr(evtGetValue(event, args[0]));
    s32 value = evtGetValue(event, args[1]);
    if (item == 0) {
        return 2;
    }
    *(s32*)((s32)item + 0x90) = value;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_item_set_bound_next_speed) {
    s32* args = event->args;
    void* item = itemNameToPtr(evtGetValue(event, args[0]));
    f32 value = evtGetFloat(event, args[1]);
    if (item == 0) {
        return 2;
    }
    *(f32*)((s32)item + 0x94) = value;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_item_set_bound_rate) {
    s32* args = event->args;
    void* item = itemNameToPtr(evtGetValue(event, args[0]));
    f32 value = evtGetFloat(event, args[1]);
    if (item == 0) {
        return 2;
    }
    *(f32*)((s32)item + 0x80) = value;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_item_set_gravity) {
    s32* args = event->args;
    void* item = itemNameToPtr(evtGetValue(event, args[0]));
    f32 value = evtGetFloat(event, args[1]);
    if (item == 0) {
        return 2;
    }
    *(f32*)((s32)item + 0x84) = value;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_item_set_jump_power) {
    s32* args = event->args;
    void* item = itemNameToPtr(evtGetValue(event, args[0]));
    f32 value = evtGetFloat(event, args[1]);
    if (item == 0) {
        return 2;
    }
    *(f32*)((s32)item + 0x58) = value;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_item_delete_check) {
    s32* args = event->args;
    if (itemNameToPtr(evtGetValue(event, args[0])) != 0) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], 1);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_item_set_alpha) {
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    u8 alpha = evtGetValue(event, args[1]);
    void* item = itemNameToPtr(name);
    if (item == 0) {
        return 2;
    }
    *(u8*)((s32)item + 0x4C) = alpha;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_item_set_scale) {
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    f32 scale = evtGetFloat(event, args[1]);
    void* item = itemNameToPtr(name);
    if (item == 0) {
        return 2;
    }
    *(f32*)((s32)item + 0x48) = scale;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 evt_item_get_important_item(s32 pEvt, s32 param_2) {
    return 0;
}


u8 evt_item_move_3d_position(void) {
    return 0;
}


s32 evt_item_entry(void* pEvt) {
    return 0;
}


s32 evt_item_set_position(void* pEvt) {
    return 0;
}


u32 evt_item_get_item(void* pEvt, int param_2) {
    return 0;
}


u32 evt_item_get_item_end_wait(void* pEvt) {
    return 0;
}


s32 evt_item_get_position(void* pEvt) {
    return 0;
}


s32 evt_item_set_move_dir_speed(void* pEvt) {
    return 0;
}


s32 evt_item_change_mode(int param_1) {
    return 0;
}
