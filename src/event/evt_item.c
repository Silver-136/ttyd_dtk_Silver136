#include "event/evt_item.h"

s32 evtGetValue(EventEntry* event, s32 value);
f32 evtGetFloat(EventEntry* event, s32 value);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void* itemNameToPtr(s32 name);
void itemDelete(s32 name);
void itemFlagOff(void* item, u16 flag);
void itemFlagOn(void* item, u16 flag);
void itemStatusOn(void* item, s32 status);

s32 evt_item_entry(void* pEvt) {
    extern s32 itemEntry(s32 name, s32 item, u16 mode, s32 script, s32 arg, f32 x, f32 y, f32 z);

    EventEntry* event = pEvt;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 item = evtGetValue(event, args[1]);
    s32 x = evtGetValue(event, args[2]);
    s32 y = evtGetValue(event, args[3]);
    s32 z = evtGetValue(event, args[4]);
    s32 mode = evtGetValue(event, args[5]);
    s32 script = args[6];
    s32 arg = evtGetValue(event, args[7]);

    itemEntry(name, item, mode, script, arg, (f32)x, (f32)y, (f32)z);
    return 2;
}

USER_FUNC(evt_item_delete) {
    itemDelete(evtGetValue(event, event->args[0]));
    return 2;
}
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_item_set_move_dir_speed(void* pEvt) {
    EventEntry* event = pEvt;
    s32* args = event->args;
    void* item = itemNameToPtr(evtGetValue(event, args[0]));
    s32 dir = evtGetValue(event, args[1]);
    f32 speed = evtGetFloat(event, args[2]);
    if (item == 0) {
        return 2;
    }
    *(s32*)((s32)item + 0x54) = dir;
    *(f32*)((s32)item + 0x50) = speed;
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

u32 evt_item_get_item(void* pEvt, int param_2) {
    extern void itemForceGet(void* item);
    extern void* itemStatus(void* item, s32 status);

    EventEntry* event = pEvt;
    void* item = itemNameToPtr(evtGetValue(event, event->args[0]));
    s32 value;

    if (param_2 != 0) {
        itemForceGet(item);
        return 0;
    }
    if (item == 0) {
        return 2;
    }
    if (!(*(u16*)item & 1)) {
        return 2;
    }
    if (itemStatus(item, 2) != 0) {
        return 2;
    }
    value = (s32)itemStatus(item, 0x10000);
    return 2 & ((-value | value) >> 31);
}

USER_FUNC(evt_item_get_item_end_wait) {
    extern void* itemStatus(void* item, s32 status);
    void* item = itemNameToPtr(evtGetValue(event, event->args[0]));
    s32 value;
    if (item == 0) {
        return 2;
    }
    if (!(*(u16*)item & 1)) {
        return 2;
    }
    if (itemStatus(item, 2) != 0) {
        return 2;
    }
    value = (s32)itemStatus(item, 0x10000);
    return 2 & ((-value | value) >> 31);
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_item_change_mode(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* itemNameToPtr(s32 name);
    extern void itemModeChange(void* item, u16 mode);
    void* event = pEvt;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 mode = evtGetValue(event, args[1]);
    void* item = itemNameToPtr(name);

    if (item == 0) {
        return 2;
    }
    itemModeChange(item, mode);
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
USER_FUNC(evt_item_set_position) {
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    void* item = itemNameToPtr(name);
    if (item == 0) {
        return 2;
    }
    *(f32*)((s32)item + 0x3C) = x;
    *(f32*)((s32)item + 0x40) = y;
    *(f32*)((s32)item + 0x44) = z;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(evt_item_get_position) {
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    s32 x = args[1];
    s32 y = args[2];
    s32 z = args[3];
    void* item = itemNameToPtr(name);
    if (item == 0) {
        return 2;
    }
    evtSetFloat(event, x, *(f32*)((s32)item + 0x3C));
    evtSetFloat(event, y, *(f32*)((s32)item + 0x40));
    evtSetFloat(event, z, *(f32*)((s32)item + 0x44));
    return 2;
}
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

u8 evt_item_move_3d_position(s32 pEvt, s32 param_2) {
    extern void* gp;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);

    s32* args = *(s32**)(pEvt + 0x18);
    s32 name = evtGetValue((EventEntry*)pEvt, args[0]);
    f32 x = (f32)evtGetValue((EventEntry*)pEvt, args[1]);
    f32 y = (f32)evtGetValue((EventEntry*)pEvt, args[2]);
    f32 z = (f32)evtGetValue((EventEntry*)pEvt, args[3]);
    s32 duration = evtGetValue((EventEntry*)pEvt, args[4]);
    s32 type = evtGetValue((EventEntry*)pEvt, args[5]);
    void* item = itemNameToPtr(name);
    s32 elapsed;

    if (param_2 != 0) {
        if (item == 0) {
            return 2;
        }
        *(s32*)(pEvt + 0x78) = *(s32*)((s32)gp + 0x3C);
        *(f32*)(pEvt + 0x7C) = *(f32*)((s32)item + 0x3C);
        *(f32*)(pEvt + 0x80) = *(f32*)((s32)item + 0x40);
        *(f32*)(pEvt + 0x84) = *(f32*)((s32)item + 0x44);
    }

    elapsed = (*(s32*)((s32)gp + 0x3C) - *(s32*)(pEvt + 0x78)) / (((*(u32*)0x800000F8) >> 2) / 1000);

    if (elapsed < duration) {
        *(f32*)((s32)item + 0x3C) = intplGetValue(type, elapsed, duration, *(f32*)(pEvt + 0x7C), x);
        *(f32*)((s32)item + 0x40) = intplGetValue(type, elapsed, duration, *(f32*)(pEvt + 0x80), y);
        *(f32*)((s32)item + 0x44) = intplGetValue(type, elapsed, duration, *(f32*)(pEvt + 0x84), z);
        return 0;
    }

    *(f32*)((s32)item + 0x3C) = x;
    *(f32*)((s32)item + 0x40) = y;
    *(f32*)((s32)item + 0x44) = z;
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off


s32 evt_item_get_important_item(s32 pEvt, s32 param_2) {
    extern void* marioGetPtr(void);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);

    extern f32 float_0p5_80421070;
    extern f32 float_0p1_80421074;
    extern f32 float_6p2832_80421078;
    extern f32 float_360_8042107c;
    extern f32 float_0p00761_80421080;
    extern f32 float_0p16605_80421084;
    extern f32 float_1_80421088;
    extern f32 float_3p1416_8042108c;
    extern f32 float_1p5708_80421090;
    extern f32 float_4p7124_80421094;
    extern f32 float_0p03705_80421098;
    extern f32 float_0p4967_8042109c;
    extern f32 float_0_804210a0;

    s32* args = *(s32**)(pEvt + 0x18);
    void* item = itemNameToPtr(evtGetValue((EventEntry*)pEvt, args[0]));
    void* mario = marioGetPtr();
    f32 marioY = *(f32*)((s32)mario + 0x90) + *(f32*)((s32)mario + 0x1BC);
    f32 marioX = *(f32*)((s32)mario + 0x8C);
    f32 marioZ = *(f32*)((s32)mario + 0x94);
    f32 angle;
    f32 t;
    f32 t2;
    f32 value;

    if (param_2 != 0) {
        t = distABf(marioX, marioZ, *(f32*)((s32)item + 0x3C), *(f32*)((s32)item + 0x44)) / float_0p5_80421070;
        *(f32*)(pEvt + 0x78) = t;
        *(f32*)(pEvt + 0x7C) = (float_0p1_80421074 * t) * float_0p5_80421070
            + (marioY - *(f32*)((s32)item + 0x40)) / t;
        *(f32*)(pEvt + 0x80) = float_0p5_80421070;
        *(s32*)(pEvt + 0x84) = 0;
    }

    angle = (float_6p2832_80421078 * angleABf(*(f32*)((s32)item + 0x3C),
                                                *(f32*)((s32)item + 0x44),
                                                marioX,
                                                marioZ)) / float_360_8042107c;

    if (angle <= float_3p1416_8042108c) {
        if (angle <= float_1p5708_80421090) {
            t = angle;
            t2 = t * t;
            value = ((float_0p00761_80421080 * t2 - float_0p16605_80421084) * t2 + float_1_80421088) * t;
        } else {
            t = float_1p5708_80421090 - (angle - float_1p5708_80421090);
            t2 = t * t;
            value = ((float_0p00761_80421080 * t2 - float_0p16605_80421084) * t2 + float_1_80421088) * t;
        }
    } else if (angle >= float_4p7124_80421094) {
        t = float_1p5708_80421090 - (angle - float_4p7124_80421094);
        t2 = t * t;
        value = -(((float_0p00761_80421080 * t2 - float_0p16605_80421084) * t2 + float_1_80421088) * t);
    } else {
        t = angle - float_3p1416_8042108c;
        t2 = t * t;
        value = -(((float_0p00761_80421080 * t2 - float_0p16605_80421084) * t2 + float_1_80421088) * t);
    }

    *(f32*)((s32)item + 0x3C) += *(f32*)(pEvt + 0x80) * value;

    if (angle <= float_3p1416_8042108c) {
        if (angle <= float_1p5708_80421090) {
            t = angle;
            t2 = t * t;
            value = (float_0p03705_80421098 * t2 - float_0p4967_8042109c) * t2 + float_1_80421088;
        } else {
            t = float_1p5708_80421090 - (angle - float_1p5708_80421090);
            t2 = t * t;
            value = -((float_0p03705_80421098 * t2 - float_0p4967_8042109c) * t2 + float_1_80421088);
        }
    } else if (angle >= float_4p7124_80421094) {
        t = float_1p5708_80421090 - (angle - float_4p7124_80421094);
        t2 = t * t;
        value = (float_0p03705_80421098 * t2 - float_0p4967_8042109c) * t2 + float_1_80421088;
    } else {
        t = angle - float_3p1416_8042108c;
        t2 = t * t;
        value = -((float_0p03705_80421098 * t2 - float_0p4967_8042109c) * t2 + float_1_80421088);
    }

    *(f32*)((s32)item + 0x44) -= *(f32*)(pEvt + 0x80) * value;
    *(f32*)((s32)item + 0x40) += *(f32*)(pEvt + 0x7C);

    *(f32*)(pEvt + 0x78) -= float_1_80421088;
    if (*(f32*)(pEvt + 0x78) >= float_0_804210a0) {
        *(f32*)(pEvt + 0x7C) -= float_0p1_80421074;
        return 0;
    }

    *(f32*)((s32)item + 0x3C) = marioX;
    *(f32*)((s32)item + 0x40) = marioY;
    *(f32*)((s32)item + 0x44) = marioZ;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

