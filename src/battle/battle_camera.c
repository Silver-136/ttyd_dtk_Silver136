#include "battle/battle_camera.h"

#include "battle/battle.h"

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

f32 battleCameraGetPosMoveSpeed(void) {
    f32 x, y, z;

    x = _battleWorkPointer->camera.moveSpeed.x;
    y = _battleWorkPointer->camera.moveSpeed.y;
    z = _battleWorkPointer->camera.moveSpeed.z;
    return (f32)sqrt((x * x) + (y * y) + (z * z));
}

void btl_camera_set_prilimit(s32 limit) {
    _battleWorkPointer->camera.priorityLimit = limit;
}

USER_FUNC(evt_btl_camera_set_prilimit) {
    s32 limit;

    limit = evtGetValue(event, *event->args);
    btl_camera_set_prilimit(limit);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_btl_camera_wait_move_end) {
    extern f32 float_2_80422738;
    if (battleCameraGetPosMoveSpeed() >= float_2_80422738) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

void btl_camera_set_zoom(s32 priority, f32 zoom) {
    void* work;

    work = _battleWorkPointer;
    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    *(f32*)((s32)work + 0x2778) = zoom;
}

void btl_camera_add_zoom(s32 priority, f32 zoom) {
    void* work;

    work = _battleWorkPointer;
    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    *(f32*)((s32)work + 0x2778) += zoom;
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

void btl_camera_noshake(s32 priority) {
    void* work;

    work = _battleWorkPointer;
    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    *(u32*)((s32)work + 0x2754) &= ~0x1;
    *(u32*)((s32)work + 0x2754) &= ~0x2;
}

void btl_camera_set_mode(s32 priority, s32 mode);

s32 evt_btl_camera_off_posoffset_manual(void* evt) {
    s32* args;
    s32 priority;

    args = *(s32**)((s32)evt + 0x18);
    priority = evtGetValue(evt, args[0]);
    btl_camera_off_posoffset_manual(priority);

    return 2;
}

s32 evt_btl_camera_noshake(void* evt) {
    s32* args;
    s32 priority;

    args = *(s32**)((s32)evt + 0x18);
    priority = evtGetValue(evt, args[0]);
    btl_camera_noshake(priority);

    return 2;
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