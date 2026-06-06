#include "event/evt_cam.h"
#include "event/evt_cmd.h"

void* marioGetPtr(void);
void* camGetPtr(s32 id);
void camShiftReset(void* cam);
void camCtrlOn(s32 id);
void camCtrlOff(s32 id);
void psndSetFlag(s32 flag);
void psndClearFlag(s32 flag);

USER_FUNC(evt_cam_shift_reset) {
    marioGetPtr();
    camShiftReset(camGetPtr(4));
    return 2;
}

USER_FUNC(evt_cam3d_road_shift_onoff) {
    s32 onoff = evtGetValue(event, event->args[0]);
    void* cam = camGetPtr(4);

    if (onoff != 0) {
        *(u16*)cam &= ~0x20;
    } else {
        *(u16*)cam |= 0x20;
    }
    return 2;
}

USER_FUNC(evt_cam_letter_box_disable) {
    s32 disable = evtGetValue(event, event->args[0]);
    void* cam = camGetPtr(8);

    if (disable != 0) {
        *(u16*)cam |= 0x200;
        psndSetFlag(0x40);
    } else {
        *(u16*)cam &= ~0x200;
        psndClearFlag(0x40);
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_cam_get_at) {
    s32* args = event->args;
    void* cam = camGetPtr(evtGetValue(event, args[0]));

    evtSetFloat(event, args[1], *(f32*)((s32)cam + 0x18));
    evtSetFloat(event, args[2], *(f32*)((s32)cam + 0x1C));
    evtSetFloat(event, args[3], *(f32*)((s32)cam + 0x20));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_cam_get_pos) {
    s32* args = event->args;
    void* cam = camGetPtr(evtGetValue(event, args[0]));

    evtSetFloat(event, args[1], *(f32*)((s32)cam + 0xC));
    evtSetFloat(event, args[2], *(f32*)((s32)cam + 0x10));
    evtSetFloat(event, args[3], *(f32*)((s32)cam + 0x14));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_cam_ctrl_onoff) {
    s32* args = event->args;
    s32 camId = evtGetValue(event, args[0]);

    if (evtGetValue(event, args[1]) != 0) {
        camCtrlOn(camId);
    } else {
        camCtrlOff(camId);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 evt_cam_shake(s32 pEvt, s32 param_2) {
    return 0;
}


u8 evt_cam3d_evt_set_rel_dir(s32 pEvt) {
    return 0;
}


u8 evt_cam3d_evt_set_npc_rel(s32 pEvt) {
    return 0;
}


u8 evt_cam3d_evt_set(s32 pEvt) {
    return 0;
}


u8 evt_cam3d_evt_set_rel(s32 pEvt) {
    return 0;
}


s32 evt_cam3d_evt_zoom_in(void* pEvt) {
    return 0;
}


s32 evt_cam3d_evt_set_at(void* pEvt) {
    return 0;
}


s32 evt_cam3d_evt_set_xyz(void* pEvt) {
    return 0;
}


u8 evt_cam_road_reset2(s32 pEvt) {
    return 0;
}


u8 evt_cam3d_event_from_road(s32 pEvt) {
    return 0;
}


u8 evt_cam3d_get_shift(s32 pEvt) {
    return 0;
}


s32 evt_cam3d_evt_xyz_off(void* pEvt) {
    return 0;
}


s32 evt_cam3d_evt_off(void* pEvt) {
    return 0;
}


USER_FUNC(evt_cam_letter_box_onoff) {
    s32* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 hide = evtGetValue(event, args[1]);
    void* cam = camGetPtr(8);
    if (on != 0) {
        *(u16*)cam |= 0x100;
        psndSetFlag(0x20);
        *(u16*)((s32)cam + 0x1E0) = 0xFF;
        *(u16*)((s32)cam + 0x1E2) = 0;
        *(u16*)((s32)cam + 0x1E4) = 0;
        *(s32*)((s32)cam + 0x1E8) = 8;
    } else {
        *(u16*)cam &= ~0x100;
        psndClearFlag(0x20);
    }
    if (hide != 0) {
        *(u16*)cam |= 0x400;
    }
    return 2;
}

USER_FUNC(evt_cam3d_evt_set_now) {
    extern void* gp;
    void* cam = camGetPtr(4);
    *(u32*)((s32)cam + 0x40) = *(u32*)((s32)cam + 0xC);
    *(u32*)((s32)cam + 0x44) = *(u32*)((s32)cam + 0x10);
    *(u32*)((s32)cam + 0x48) = *(u32*)((s32)cam + 0x14);
    *(u32*)((s32)cam + 0x58) = *(u32*)((s32)cam + 0x40);
    *(u32*)((s32)cam + 0x5C) = *(u32*)((s32)cam + 0x44);
    *(u32*)((s32)cam + 0x60) = *(u32*)((s32)cam + 0x48);
    *(u32*)((s32)cam + 0x4C) = *(u32*)((s32)cam + 0x18);
    *(u32*)((s32)cam + 0x50) = *(u32*)((s32)cam + 0x1C);
    *(u32*)((s32)cam + 0x54) = *(u32*)((s32)cam + 0x20);
    *(u32*)((s32)cam + 0x64) = *(u32*)((s32)cam + 0x4C);
    *(u32*)((s32)cam + 0x68) = *(u32*)((s32)cam + 0x50);
    *(u32*)((s32)cam + 0x6C) = *(u32*)((s32)cam + 0x54);
    *(u32*)((s32)cam + 0x70) = *(u32*)((s32)gp + 0x38);
    *(u32*)((s32)cam + 0x74) = *(u32*)((s32)gp + 0x3C);
    *(s32*)((s32)cam + 0x7C) = 0;
    *(s32*)((s32)cam + 0x78) = 0;
    *(u16*)((s32)cam + 0x4) = 3;
    *(u8*)((s32)cam + 0x80) = 0xB;
    return 2;
}

USER_FUNC(evt_cam_road_reset) {
    extern f32 float_0p01_80421040;
    extern void camRoadReset(void);
    void* mario = marioGetPtr();
    void* cam = camGetPtr(4);
    *(f32*)((s32)cam + 0x94) = *(f32*)((s32)mario + 0x8C);
    *(f32*)((s32)cam + 0x98) = *(f32*)((s32)mario + 0x90) + float_0p01_80421040;
    *(f32*)((s32)cam + 0x9C) = *(f32*)((s32)mario + 0x94);
    *(f32*)((s32)cam + 0xAC) = *(f32*)((s32)cam + 0x94);
    *(f32*)((s32)cam + 0xB0) = *(f32*)((s32)cam + 0x98);
    *(f32*)((s32)cam + 0xB4) = *(f32*)((s32)cam + 0x9C);
    *(u32*)((s32)mario + 0xEC) = *(u32*)((s32)mario + 0x8C);
    *(u32*)((s32)mario + 0xF0) = *(u32*)((s32)mario + 0x90);
    *(u32*)((s32)mario + 0xF4) = *(u32*)((s32)mario + 0x94);
    *(u32*)((s32)mario + 0xF8) = *(u32*)((s32)mario + 0x8C);
    *(u32*)((s32)mario + 0xFC) = *(u32*)((s32)mario + 0x90);
    *(u32*)((s32)mario + 0x100) = *(u32*)((s32)mario + 0x94);
    camRoadReset();
    camShiftReset(cam);
    return 2;
}

s32 evt_cam_type_change(void* pEvt) {
    return 0;
}


s32 evt_cam_letter_box_camid(void* pEvt) {
    return 0;
}
