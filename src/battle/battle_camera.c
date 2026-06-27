#include "battle/battle_camera.h"

#include "battle/battle.h"



void btl_camera_set_mode(s32 priority, s32 mode);
void* camGetPtr(s32 cameraId);
void btl_camera_shake_h(s32 priority, f32 a, f32 b, s32 time, s32 type);
void btl_camera_shake_w(s32 priority, f32 a, f32 b, s32 time, s32 type);
void btl_camera_set_moveto(s32 priority, f32 x, f32 y, f32 z, f32 tx, f32 ty, f32 tz, s32 frames, s32 type);

/* Reorder pilot prototypes: needed because this file originally relied on source order. */
void battleCameraInit(void);
f32 battleCameraGetPosMoveSpeed(void);
void btl_camera_set_prilimit(s32 limit);
void btl_camera_set_zoom(s32 priority, f32 zoom);
void btl_camera_add_zoom(s32 priority, f32 zoom);
void btl_camera_set_posoffset(s32 priority, f32 x, f32 y, f32 z);
void btl_camera_off_posoffset_manual(s32 priority);
void btl_camera_set_zoomSpeedLv(s32 priority, s32 level);
void btl_camera_set_moveSpeedLv(s32 priority, s32 level);
void btl_camera_nomove_x_onoff(s32 priority, s32 on);
void btl_camera_nomove_y_onoff(s32 priority, s32 on);
void btl_camera_nomove_z_onoff(s32 priority, s32 on);
void btl_camera_noshake(s32 priority);
s32 evt_btl_camera_off_posoffset_manual(void* evt);
s32 evt_btl_camera_noshake(void* evt);
s32 evt_btl_camera_nomove_x_onoff(void* evt);
s32 evt_btl_camera_nomove_y_onoff(void* evt);
s32 evt_btl_camera_nomove_z_onoff(void* evt);
s32 evt_btl_camera_set_moveSpeedLv(void* evt);
s32 evt_btl_camera_set_zoomSpeedLv(void* evt);
s32 evt_btl_camera_set_mode(void* evt);
s32 evt_btl_camera_set_zoom(void* evt);
void battleCameraMain(void);
u8 battleCameraMoveTo(void);
s32 evt_btl_camera_set_homing_unitparts(int param_1);
s32 evt_btl_camera_set_homing_unit_audience(int param_1);
s32 evt_btl_camera_set_homing_unit(int param_1);
u8 evt_btl_camera_shake_h(void* event);
u8 evt_btl_camera_shake_w(void* event);
u8 evt_btl_camera_set_moveto(void* event);
s32 evt_btl_camera_set_posoffset(void* event);
s32 evt_btl_camera_add_zoom(int param_1);
void btl_camera_shake_h(s32 priority, f32 a, f32 b, s32 frames, s32 type);
void btl_camera_shake_w(s32 priority, f32 a, f32 b, s32 frames, s32 type);

USER_FUNC(evt_btl_camera_wait_move_end) {
    extern f32 float_2_80422738;
    if (battleCameraGetPosMoveSpeed() >= float_2_80422738) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

s32 evt_btl_camera_off_posoffset_manual(void* evt) {
    s32* args;
    s32 priority;

    args = *(s32**)((s32)evt + 0x18);
    priority = evtGetValue(evt, args[0]);
    btl_camera_off_posoffset_manual(priority);

    return 2;
}

void btl_camera_off_posoffset_manual(s32 priority) {
    extern f32 float_0_8042273c;
    void* work = _battleWorkPointer;
    f32 zero;

    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    zero = float_0_8042273c;
    *(u32*)((s32)work + 0x2754) &= ~0x20;
    *(f32*)((s32)work + 0x27B0) = zero;
    *(f32*)((s32)work + 0x27B4) = zero;
    *(f32*)((s32)work + 0x27B8) = zero;
}

s32 evt_btl_camera_set_posoffset(void* event) {
    s32* args;
    s32 priority;
    f32 x;
    f32 y;
    f32 z;

    args = *(s32**)((s32)event + 0x18);
    priority = evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    btl_camera_set_posoffset(priority, x, y, z);

    return 2;
}

void btl_camera_set_posoffset(s32 priority, f32 x, f32 y, f32 z) {
    void* work;

    work = _battleWorkPointer;
    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    *(u32*)((s32)work + 0x2754) |= 0x20;
    *(f32*)((s32)work + 0x27B0) = x;
    *(f32*)((s32)work + 0x27B4) = y;
    *(f32*)((s32)work + 0x27B8) = z;
}

s32 evt_btl_camera_nomove_z_onoff(void* evt) {
    s32* args;
    s32 priority;
    s32 on;

    args = *(s32**)((s32)evt + 0x18);
    priority = evtGetValue(evt, args[0]);
    on = evtGetValue(evt, args[1]);
    btl_camera_nomove_z_onoff(priority, on);

    return 2;
}

void btl_camera_nomove_z_onoff(s32 priority, s32 on) {
    void* work;

    work = _battleWorkPointer;
    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    if (on == 1) {
        *(u32*)((s32)work + 0x2754) |= 0x10;
    } else {
        *(u32*)((s32)work + 0x2754) &= ~0x10;
    }
}

s32 evt_btl_camera_nomove_y_onoff(void* evt) {
    s32* args;
    s32 priority;
    s32 on;

    args = *(s32**)((s32)evt + 0x18);
    priority = evtGetValue(evt, args[0]);
    on = evtGetValue(evt, args[1]);
    btl_camera_nomove_y_onoff(priority, on);

    return 2;
}

void btl_camera_nomove_y_onoff(s32 priority, s32 on) {
    void* work;

    work = _battleWorkPointer;
    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    if (on == 1) {
        *(u32*)((s32)work + 0x2754) |= 0x8;
    } else {
        *(u32*)((s32)work + 0x2754) &= ~0x8;
    }
}

s32 evt_btl_camera_nomove_x_onoff(void* evt) {
    s32* args;
    s32 priority;
    s32 on;

    args = *(s32**)((s32)evt + 0x18);
    priority = evtGetValue(evt, args[0]);
    on = evtGetValue(evt, args[1]);
    btl_camera_nomove_x_onoff(priority, on);

    return 2;
}

void btl_camera_nomove_x_onoff(s32 priority, s32 on) {
    void* work;

    work = _battleWorkPointer;
    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    if (on == 1) {
        *(u32*)((s32)work + 0x2754) |= 0x4;
    } else {
        *(u32*)((s32)work + 0x2754) &= ~0x4;
    }
}

s32 evt_btl_camera_noshake(void* evt) {
    s32* args;
    s32 priority;

    args = *(s32**)((s32)evt + 0x18);
    priority = evtGetValue(evt, args[0]);
    btl_camera_noshake(priority);

    return 2;
}

void btl_camera_noshake(s32 priority) {
    void* work;

    work = _battleWorkPointer;
    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    *(u32*)((s32)work + 0x2754) &= ~0x1;
    *(u32*)((s32)work + 0x2754) &= ~0x2;
}

u8 evt_btl_camera_shake_h(void* event) {
    s32* args;
    s32 priority;
    f32 a;
    f32 b;
    s32 frames;
    s32 type;

    args = *(s32**)((s32)event + 0x18);
    priority = evtGetValue(event, args[0]);
    a = evtGetValue(event, args[1]);
    b = evtGetValue(event, args[2]);
    frames = evtGetValue(event, args[3]);
    type = evtGetValue(event, args[4]);
    btl_camera_shake_h(priority, a, b, frames, type);

    return 2;
}

void btl_camera_shake_h(s32 priority, f32 a, f32 b, s32 frames, s32 type) {
    void* work = _battleWorkPointer;

    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    *(u32*)((s32)work + 0x2754) |= 1;
    *(f32*)((s32)work + 0x2780) = a;
    *(f32*)((s32)work + 0x2784) = b;
    *(s32*)((s32)work + 0x2788) = frames;
    *(s32*)((s32)work + 0x278C) = frames;
    *(s32*)((s32)work + 0x2790) = type;
}

u8 evt_btl_camera_shake_w(void* event) {
    s32* args;
    s32 priority;
    f32 a;
    f32 b;
    s32 frames;
    s32 type;

    args = *(s32**)((s32)event + 0x18);
    priority = evtGetValue(event, args[0]);
    a = evtGetValue(event, args[1]);
    b = evtGetValue(event, args[2]);
    frames = evtGetValue(event, args[3]);
    type = evtGetValue(event, args[4]);
    btl_camera_shake_w(priority, a, b, frames, type);

    return 2;
}

void btl_camera_shake_w(s32 priority, f32 a, f32 b, s32 frames, s32 type) {
    void* work = _battleWorkPointer;

    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    *(u32*)((s32)work + 0x2754) |= 2;
    *(f32*)((s32)work + 0x2794) = a;
    *(f32*)((s32)work + 0x2798) = b;
    *(s32*)((s32)work + 0x279C) = frames;
    *(s32*)((s32)work + 0x27A0) = frames;
    *(s32*)((s32)work + 0x27A4) = type;
}

u8 evt_btl_camera_set_moveto(void* event) {
    s32* args;
    s32 priority;
    f32 x;
    f32 y;
    f32 z;
    f32 tx;
    f32 ty;
    f32 tz;
    s32 frames;
    s32 type;

    args = *(s32**)((s32)event + 0x18);
    priority = evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    tx = evtGetFloat(event, args[4]);
    ty = evtGetFloat(event, args[5]);
    tz = evtGetFloat(event, args[6]);
    frames = evtGetValue(event, args[7]);
    type = evtGetValue(event, args[8]);
    btl_camera_set_moveto(priority, x, y, z, tx, ty, tz, frames, type);

    return 2;
}

void btl_camera_set_moveto(s32 priority, f32 x, f32 y, f32 z, f32 tx, f32 ty, f32 tz, s32 frames, s32 type) {
    void* work;
    void* cam;

    work = _battleWorkPointer;
    cam = camGetPtr(4);

    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    *(f32*)((s32)work + 0x27E0) = *(f32*)((s32)cam + 0xC);
    *(f32*)((s32)work + 0x27E4) = *(f32*)((s32)cam + 0x10);
    *(f32*)((s32)work + 0x27E8) = *(f32*)((s32)cam + 0x14);
    *(f32*)((s32)work + 0x27EC) = x;
    *(f32*)((s32)work + 0x27F0) = y;
    *(f32*)((s32)work + 0x27F4) = z;
    *(f32*)((s32)work + 0x27F8) = *(f32*)((s32)cam + 0x18);
    *(f32*)((s32)work + 0x27FC) = *(f32*)((s32)cam + 0x1C);
    *(f32*)((s32)work + 0x2800) = *(f32*)((s32)cam + 0x20);
    *(f32*)((s32)work + 0x2804) = tx;
    *(f32*)((s32)work + 0x2808) = ty;
    *(f32*)((s32)work + 0x280C) = tz;
    *(s32*)((s32)work + 0x27D8) = frames;
    *(s32*)((s32)work + 0x27DC) = frames;
    *(s32*)((s32)work + 0x27D4) = type;
}

s32 evt_btl_camera_set_zoomSpeedLv(void* evt) {
    s32* args;
    s32 priority;
    s32 level;

    args = *(s32**)((s32)evt + 0x18);
    priority = evtGetValue(evt, args[0]);
    level = evtGetValue(evt, args[1]);
    btl_camera_set_zoomSpeedLv(priority, level);

    return 2;
}

s32 evt_btl_camera_set_moveSpeedLv(void* evt) {
    s32* args;
    s32 priority;
    s32 level;

    args = *(s32**)((s32)evt + 0x18);
    priority = evtGetValue(evt, args[0]);
    level = evtGetValue(evt, args[1]);
    btl_camera_set_moveSpeedLv(priority, level);

    return 2;
}

void btl_camera_set_zoomSpeedLv(s32 priority, s32 level) {
    void* work = _battleWorkPointer;

    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    if (level < 0) {
        goto invalid;
    }
    if (level > 5) {
        goto invalid;
    }
    *(s16*)((s32)work + 0x277E) = level;
    return;
invalid:
    *(s16*)((s32)work + 0x277E) = 1;
}

void btl_camera_set_moveSpeedLv(s32 priority, s32 level) {
    void* work = _battleWorkPointer;

    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    if (level < 0) {
        goto invalid;
    }
    if (level > 5) {
        goto invalid;
    }
    *(s16*)((s32)work + 0x277C) = level;
    goto done;
invalid:
    *(s16*)((s32)work + 0x277C) = 1;
done:
    *(s16*)((s32)work + 0x277E) = *(s16*)((s32)work + 0x277C);
}

s32 evt_btl_camera_add_zoom(int param_1) {
    return 0;
}

s32 evt_btl_camera_set_zoom(void* evt) {
    s32* args;
    s32 priority;
    s32 zoom;

    args = *(s32**)((s32)evt + 0x18);

    priority = evtGetValue(evt, args[0]);
    zoom = evtGetFloat(evt, args[1]);

    btl_camera_set_zoom(priority, zoom);

    return 2;
}

void btl_camera_add_zoom(s32 priority, f32 zoom) {
    void* work;

    work = _battleWorkPointer;
    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    *(f32*)((s32)work + 0x2778) += zoom;
}

void btl_camera_set_zoom(s32 priority, f32 zoom) {
    void* work;

    work = _battleWorkPointer;
    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    *(f32*)((s32)work + 0x2778) = zoom;
}

s32 evt_btl_camera_set_homing_unit_audience(int param_1) {
    return 0;
}

s32 evt_btl_camera_set_homing_unitparts(int param_1) {
    return 0;
}

s32 evt_btl_camera_set_homing_unit(int param_1) {
    return 0;
}

s32 evt_btl_camera_set_mode(void* evt) {
    s32* args;
    s32 priority;
    s32 mode;

    args = *(s32**)((s32)evt + 0x18);
    priority = evtGetValue(evt, args[0]);
    mode = evtGetValue(evt, args[1]);
    btl_camera_set_mode(priority, mode);

    return 2;
}

void btl_camera_set_mode(s32 priority, s32 mode) {
    void* work;

    work = _battleWorkPointer;
    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    *(s32*)((s32)work + 0x2760) = mode;
    *(f32*)((s32)work + 0x2778) = 0.0f;
    *(s16*)((s32)work + 0x277C) = 1;
    *(s16*)((s32)work + 0x277E) = 1;
    *(s32*)((s32)work + 0x2758) = 0;
    btl_camera_nomove_x_onoff(0, 0);
    btl_camera_nomove_y_onoff(0, 0);
    btl_camera_nomove_z_onoff(0, 0);
    *(u32*)((s32)work + 0x2754) &= ~0x41;
}

USER_FUNC(evt_btl_camera_set_prilimit) {
    s32 limit;

    limit = evtGetValue(event, *event->args);
    btl_camera_set_prilimit(limit);
    return EVT_RETURN_DONE;
}

void btl_camera_set_prilimit(s32 limit) {
    _battleWorkPointer->camera.priorityLimit = limit;
}

f32 battleCameraGetPosMoveSpeed(void) {
    f32 x, y, z;

    x = _battleWorkPointer->camera.moveSpeed.x;
    y = _battleWorkPointer->camera.moveSpeed.y;
    z = _battleWorkPointer->camera.moveSpeed.z;
    return (f32)sqrt((x * x) + (y * y) + (z * z));
}

u8 battleCameraMoveTo(void) {
    return 0;
}

void battleCameraMain(void) {
    ;
}

void battleCameraInit(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleWorkCamera* camera = &wp->camera;

    memset(camera, 0, sizeof(BattleWorkCamera));
    camera->flags = 0;
    switch (wp->info->firstAttack) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        camera->mode = 15;
        break;
    case 9:
        camera->mode = 16;
        break;
    case 0:
        camera->mode = 14;
        break;
    }
    camera->priorityLimit = 0;
    camera->zoom = 0.0f;
    camera->moveSpeedLevel = 1;
    camera->zoomSpeedLevel = 1;
    camera->offset = (Vec) {0.0f, 0.0f, 0.0f};
    camera->counter = 0;
}
