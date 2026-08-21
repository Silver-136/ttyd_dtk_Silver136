#include "battle/battle_camera.h"

#include "battle/battle.h"



void* camGetPtr(s32 cameraId);
void BtlUnit_GetHomePos(void* unit, f32* x, f32* y, f32* z);
void padRumbleOn(s32 channel);
void btl_camera_shake_w(s32 priority, f32 a, f32 b, s32 time, s32 type);

/* Reorder pilot prototypes: needed because this file originally relied on source order. */
void btl_camera_set_prilimit(s32 limit);
void btl_camera_set_zoom(s32 priority, f32 zoom);
void btl_camera_add_zoom(s32 priority, f32 zoom);
void btl_camera_off_posoffset_manual(s32 priority);
void btl_camera_set_zoomSpeedLv(s32 priority, s32 level);
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
void battleCameraMoveTo(f32 posX, f32 posY, f32 posZ, f32 targetX, f32 targetY,
                        f32 targetZ, s32 useOffset, s32 clampX, f32* moveX,
                        f32* moveY, f32* moveZ, f32* targetMoveX, f32* targetMoveY, f32* targetMoveZ);
s32 evt_btl_camera_set_homing_unitparts(int param_1);
s32 evt_btl_camera_set_homing_unit_audience(int param_1);
s32 evt_btl_camera_set_homing_unit(int param_1);
u8 evt_btl_camera_shake_h(void* event);
u8 evt_btl_camera_shake_w(void* event);
u8 evt_btl_camera_set_moveto(void* event);
s32 evt_btl_camera_set_posoffset(void* event);
s32 evt_btl_camera_add_zoom(int param_1);

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

void battleCameraMain(void) {
    typedef void (*MoveToFn)(f32, f32, f32, f32, f32, f32, s32, s32,
                             f32*, f32*, f32*, f32*, f32*, f32*);
    extern f32 intplGetValue(f32, f32, s32, s32, s32);
    extern BattleWorkUnit* BattleGetUnitPtr(BattleWork*, s32);
    extern BattleWorkUnitPart* BattleGetUnitPartsPtr(s32, s32);
    extern void BtlUnit_GetPos(BattleWorkUnit*, f32*, f32*, f32*);
    extern void BtlUnit_GetHitPos(BattleWorkUnit*, BattleWorkUnitPart*, f32*, f32*, f32*);
    extern s32 BtlUnit_GetHeight(BattleWorkUnit*);
    extern s32 BtlUnit_GetWidth(BattleWorkUnit*);
    BattleWork* battleWork = _battleWorkPointer;
    BattleWorkCamera* camera = &battleWork->camera;
    u8* view = camGetPtr(4);
    MoveToFn moveTo = (MoveToFn)battleCameraMoveTo;
    f32 dx = 0.0f;
    f32 dy = 0.0f;
    f32 dz = 0.0f;
    f32 dtx = 0.0f;
    f32 dty = 0.0f;
    f32 dtz = 0.0f;
    f32 px;
    f32 py;
    f32 pz;
    f32 tx;
    f32 ty;
    f32 tz;
    s16 oldMove;
    s16 oldZoom;
    s32 current;
    BattleWorkUnit* unit;
    BattleWorkUnit* unit2;
    BattleWorkUnitPart* part;
    BattleWorkUnitPart* part2;
    f32 unitPos[3];
    f32 unitPos2[3];
    f32 height;
    f32 width;
    f32 spread;
    f32 focusY;
    f32 width2;
    f32 midpoint;

    *(f32*)(view + 0x0C) = *(f32*)((u8*)camera + 0x2C);
    *(f32*)(view + 0x10) = *(f32*)((u8*)camera + 0x30);
    *(f32*)(view + 0x14) = *(f32*)((u8*)camera + 0x34);
    *(f32*)(view + 0x18) = *(f32*)((u8*)camera + 0x44);
    *(f32*)(view + 0x1C) = *(f32*)((u8*)camera + 0x48);
    *(f32*)(view + 0x20) = *(f32*)((u8*)camera + 0x4C);
    *(f32*)((u8*)camera + 0x38) = *(f32*)(view + 0x0C);
    *(f32*)((u8*)camera + 0x3C) = *(f32*)(view + 0x10);
    *(f32*)((u8*)camera + 0x40) = *(f32*)(view + 0x14);
    *(f32*)((u8*)camera + 0x50) = *(f32*)(view + 0x18);
    *(f32*)((u8*)camera + 0x54) = *(f32*)(view + 0x1C);
    *(f32*)((u8*)camera + 0x58) = *(f32*)(view + 0x20);

    if (camera->mode != 3) {
        *(s32*)((u8*)camera + 0x84) = 0;
    }

    switch (camera->mode) {
        case 0:
            moveTo(0.0f, 110.0f, 750.0f, 0.0f, 60.0f, 0.0f, 1, 1,
                   &dx, &dy, &dz, &dtx, &dty, &dtz);
            break;
        case 1:
            pz = 750.0f - camera->zoom * 0.5f;
            tz = -camera->zoom * 0.5f;
            moveTo(0.0f, 110.0f, pz, 0.0f, 60.0f, tz, 1, 1,
                   &dx, &dy, &dz, &dtx, &dty, &dtz);
            break;
        case 2:
            moveTo(0.0f, 160.0f, 750.0f, 0.0f, 110.0f, 0.0f, 1, 1,
                   &dx, &dy, &dz, &dtx, &dty, &dtz);
            break;
        case 3:
            if (*(s32*)((u8*)camera + 0x84) > 0) {
                (*(s32*)((u8*)camera + 0x84))--;
                current = *(s32*)((u8*)camera + 0x88) - *(s32*)((u8*)camera + 0x84);
                px = intplGetValue(*(f32*)((u8*)camera + 0x8C), *(f32*)((u8*)camera + 0x98),
                                   *(s32*)((u8*)camera + 0x80), current,
                                   *(s32*)((u8*)camera + 0x88));
                py = intplGetValue(*(f32*)((u8*)camera + 0x90), *(f32*)((u8*)camera + 0x9C),
                                   *(s32*)((u8*)camera + 0x80), current,
                                   *(s32*)((u8*)camera + 0x88));
                pz = intplGetValue(*(f32*)((u8*)camera + 0x94), *(f32*)((u8*)camera + 0xA0),
                                   *(s32*)((u8*)camera + 0x80), current,
                                   *(s32*)((u8*)camera + 0x88));
                tx = intplGetValue(*(f32*)((u8*)camera + 0xA4), *(f32*)((u8*)camera + 0xB0),
                                   *(s32*)((u8*)camera + 0x80), current,
                                   *(s32*)((u8*)camera + 0x88));
                ty = intplGetValue(*(f32*)((u8*)camera + 0xA8), *(f32*)((u8*)camera + 0xB4),
                                   *(s32*)((u8*)camera + 0x80), current,
                                   *(s32*)((u8*)camera + 0x88));
                tz = intplGetValue(*(f32*)((u8*)camera + 0xAC), *(f32*)((u8*)camera + 0xB8),
                                   *(s32*)((u8*)camera + 0x80), current,
                                   *(s32*)((u8*)camera + 0x88));
                oldMove = camera->moveSpeedLevel;
                oldZoom = camera->zoomSpeedLevel;
                camera->moveSpeedLevel = 3;
                camera->zoomSpeedLevel = 3;
                moveTo(px, py, pz, tx, ty, tz, 0, 0, &dx, &dy, &dz, &dtx, &dty, &dtz);
                camera->moveSpeedLevel = oldMove;
                camera->zoomSpeedLevel = oldZoom;
            }
            break;
        case 4:
            moveTo(0.0f, 110.0f, -750.0f, 0.0f, 60.0f, 0.0f, 1, 1,
                   &dx, &dy, &dz, &dtx, &dty, &dtz);
            break;
        case 5:
            moveTo(0.0f, 400.0f, -617.0f, 0.0f, 60.0f, 0.0f, 1, 1,
                   &dx, &dy, &dz, &dtx, &dty, &dtz);
            break;
        case 7:
            unit = BattleGetUnitPtr(battleWork, *(s32*)((u8*)camera + 0x10));
            if (unit == 0) {
                btl_camera_set_mode(0, 0);
                break;
            }
            BattleGetUnitPartsPtr(*(s32*)((u8*)camera + 0x10),
                                  *(s32*)((u8*)camera + 0x18));
            BtlUnit_GetPos(unit, &unitPos[0], &unitPos[1], &unitPos[2]);
            height = (f32)BtlUnit_GetHeight(unit);
            width = (f32)BtlUnit_GetWidth(unit) - 24.0f;
            if (width < 0.0f) width = 0.0f;
            unitPos[1] += (height - 50.0f) * 0.5f;
            spread = -(1.5f * width - camera->zoom);
            focusY = -spread / 9.0f;
            if (unitPos[1] > 100.0f) unitPos[1] = 100.0f;
            if (unitPos[1] < 0.0f) unitPos[1] = 0.0f;
            moveTo(unitPos[0], 110.0f + unitPos[1] + focusY, 750.0f - spread,
                   unitPos[0], 60.0f + unitPos[1] + focusY, 0.0f, 1, 1,
                   &dx, &dy, &dz, &dtx, &dty, &dtz);
            break;
        case 8:
        case 9:
            unit = BattleGetUnitPtr(battleWork, *(s32*)((u8*)camera + 0x10));
            unit2 = BattleGetUnitPtr(battleWork, *(s32*)((u8*)camera + 0x14));
            if (unit == 0 || unit2 == 0) {
                btl_camera_set_mode(0, 0);
                break;
            }
            BattleGetUnitPartsPtr(*(s32*)((u8*)camera + 0x10),
                                  *(s32*)((u8*)camera + 0x18));
            part2 = BattleGetUnitPartsPtr(*(s32*)((u8*)camera + 0x14),
                                          *(s32*)((u8*)camera + 0x1C));
            BtlUnit_GetPos(unit, &unitPos[0], &unitPos[1], &unitPos[2]);
            BtlUnit_GetHitPos(unit2, part2, &unitPos2[0], &unitPos2[1], &unitPos2[2]);
            width = (f32)BtlUnit_GetWidth(unit) - 24.0f;
            width2 = (f32)BtlUnit_GetWidth(unit2) - 24.0f;
            if (width < 0.0f) width = 0.0f;
            if (width2 < 0.0f) width2 = 0.0f;
            midpoint = (unitPos[0] + unitPos2[0]) * 0.5f;
            spread = -(1.5f * (width + width2) - 2.0f * camera->zoom);
            focusY = -spread / 10.0f;
            height = (unitPos[1] + unitPos2[1]) * 0.5f;
            if (height > 100.0f) height = 100.0f;
            if (height < 0.0f) height = 0.0f;
            moveTo(midpoint, 110.0f + height + focusY, 750.0f - spread,
                   midpoint, 60.0f + height + focusY, 0.0f, 1, 1,
                   &dx, &dy, &dz, &dtx, &dty, &dtz);
            break;
        case 10:
            unit = BattleGetUnitPtr(battleWork, *(s32*)((u8*)camera + 0x10));
            if (unit == 0) {
                btl_camera_set_mode(0, 0);
                break;
            }
            part = BattleGetUnitPartsPtr(*(s32*)((u8*)camera + 0x10),
                                         *(s32*)((u8*)camera + 0x18));
            if (part == 0) {
                btl_camera_set_mode(0, 0);
                break;
            }
            BtlUnit_GetHitPos(unit, part, &unitPos[0], &unitPos[1], &unitPos[2]);
            height = (f32)BtlUnit_GetHeight(unit);
            width = (f32)BtlUnit_GetWidth(unit) - 24.0f;
            if (width < 0.0f) width = 0.0f;
            unitPos[1] += (height - 50.0f) * 0.5f;
            spread = -(1.5f * width - camera->zoom);
            focusY = -spread / 9.0f;
            if (unitPos[1] > 100.0f) unitPos[1] = 100.0f;
            if (unitPos[1] < 0.0f) unitPos[1] = 0.0f;
            moveTo(unitPos[0], 110.0f + unitPos[1] + focusY, 750.0f - spread,
                   unitPos[0], 60.0f + unitPos[1] + focusY, 0.0f, 1, 1,
                   &dx, &dy, &dz, &dtx, &dty, &dtz);
            break;
        default:
            moveTo(0.0f, 110.0f, 750.0f, 0.0f, 60.0f, 0.0f, 1, 1,
                   &dx, &dy, &dz, &dtx, &dty, &dtz);
            break;
    }
    *(f32*)((u8*)camera + 0x68) = dx;
    *(f32*)((u8*)camera + 0x6C) = dy;
    *(f32*)((u8*)camera + 0x70) = dz;
}

void battleCameraMoveTo(f32 posX, f32 posY, f32 posZ, f32 targetX, f32 targetY,
                        f32 targetZ, s32 useOffset, s32 clampX, f32* moveX,
                        f32* moveY, f32* moveZ, f32* targetMoveX, f32* targetMoveY, f32* targetMoveZ) {
    typedef struct LocalCamera {
        u8 pad0[0xC];
        Vec position;
        Vec target;
        Vec up;
    } LocalCamera;
    extern BattleWorkUnit* BattleGetMarioPtr(BattleWork* battleWork);
    extern f32 float_0_8042273c;
    extern f32 float_0p25_80422740;
    extern f32 float_600_80422744;
    extern f32 float_0p5_80422748;
    extern f32 float_100_8042274c;
    extern f32 float_neg100_80422750;
    extern f32 float_1p5_80422754;
    extern f32 float_1_80422758;
    extern f32 float_15_8042275c;
    extern f32 float_5_80422760;
    extern f32 float_200_80422764;
    BattleWork* battleWork;
    BattleWorkCamera* cameraWork;
    BattleWorkUnit* mario;
    LocalCamera* camera;
    f32 limitOffset;
    f32 upperLimit;
    f32 lowerLimit;
    f32 homeZ;
    s16 level;

    camera = camGetPtr(4);
    battleWork = _battleWorkPointer;
    cameraWork = &battleWork->camera;
    mario = BattleGetMarioPtr(battleWork);

    if (cameraWork->moveSpeedLevel < 0 || cameraWork->moveSpeedLevel >= 5) {
        cameraWork->moveSpeedLevel = 1;
    }
    if (cameraWork->zoomSpeedLevel < 0 || cameraWork->zoomSpeedLevel >= 6) {
        cameraWork->zoomSpeedLevel = 1;
    }

    if (useOffset == 1) {
        if (battleWork->flags & 0x100) {
            homeZ = mario->homePosition.z;
            posX += float_0_8042273c;
            posY += float_0p25_80422740 * -homeZ;
            posZ += homeZ;
            targetX += float_0_8042273c;
            targetY += float_0p25_80422740 * -homeZ;
            targetZ += homeZ;
        }
        posX += cameraWork->offset.x;
        posY += cameraWork->offset.y;
        posZ += cameraWork->offset.z;
        targetX += cameraWork->offset.x;
        targetY += cameraWork->offset.y;
        targetZ += cameraWork->offset.z;
    }

    limitOffset = (float_600_80422744 - posZ) * float_0p5_80422748;
    if (limitOffset < float_0_8042273c) {
        limitOffset = float_0_8042273c;
    }
    if (clampX == 1) {
        upperLimit = float_100_8042274c + limitOffset;
        lowerLimit = float_neg100_80422750 - limitOffset;
        if (posX > upperLimit) {
            posX = upperLimit;
        }
        if (posX < lowerLimit) {
            posX = lowerLimit;
        }
        if (targetX > upperLimit) {
            targetX = upperLimit;
        }
        if (targetX < lowerLimit) {
            targetX = lowerLimit;
        }
    }

    level = cameraWork->moveSpeedLevel;
    switch (level) {
    case 0:
        *moveX = (posX - camera->position.x) / float_1p5_80422754;
        *moveY = (posY - camera->position.y) / float_1p5_80422754;
        *targetMoveX = (targetX - camera->target.x) / float_1p5_80422754;
        *targetMoveY = (targetY - camera->target.y) / float_1p5_80422754;
        camera->position.x += *moveX;
        camera->position.y += *moveY;
        camera->target.x += *targetMoveX;
        camera->target.y += *targetMoveY;
        camera->up.x = float_0_8042273c;
        camera->up.y = float_1_80422758;
        camera->up.z = float_0_8042273c;
        break;
    case 1:
        *moveX = (posX - camera->position.x) / float_15_8042275c;
        *moveY = (posY - camera->position.y) / float_15_8042275c;
        *targetMoveX = (targetX - camera->target.x) / float_15_8042275c;
        *targetMoveY = (targetY - camera->target.y) / float_15_8042275c;
        camera->position.x += *moveX;
        camera->position.y += *moveY;
        camera->position.z += *moveZ;
        camera->target.x += *targetMoveX;
        camera->target.y += *targetMoveY;
        camera->up.x = float_0_8042273c;
        camera->up.y = float_1_80422758;
        camera->up.z = float_0_8042273c;
        break;
    case 2:
        *moveX = (posX - camera->position.x) / float_5_80422760;
        *moveY = (posY - camera->position.y) / float_5_80422760;
        *targetMoveX = (targetX - camera->target.x) / float_5_80422760;
        *targetMoveY = (targetY - camera->target.y) / float_5_80422760;
        camera->position.x += *moveX;
        camera->position.y += *moveY;
        camera->target.x += *targetMoveX;
        camera->target.y += *targetMoveY;
        camera->up.x = float_0_8042273c;
        camera->up.y = float_1_80422758;
        camera->up.z = float_0_8042273c;
        break;
    case 3:
        *moveX = posX - camera->position.x;
        *moveY = posY - camera->position.y;
        *targetMoveX = targetX - camera->target.x;
        *targetMoveY = targetY - camera->target.y;
        camera->position.x += *moveX;
        camera->position.y += *moveY;
        camera->target.x += *targetMoveX;
        camera->target.y += *targetMoveY;
        camera->up.x = float_0_8042273c;
        camera->up.y = float_1_80422758;
        camera->up.z = float_0_8042273c;
        break;
    case 4:
        *moveX = (posX - camera->position.x) / float_200_80422764;
        *moveY = (posY - camera->position.y) / float_200_80422764;
        *targetMoveX = (targetX - camera->target.x) / float_200_80422764;
        *targetMoveY = (targetY - camera->target.y) / float_200_80422764;
        camera->position.x += *moveX;
        camera->position.y += *moveY;
        camera->target.x += *targetMoveX;
        camera->target.y += *targetMoveY;
        camera->up.x = float_0_8042273c;
        camera->up.y = float_1_80422758;
        camera->up.z = float_0_8042273c;
        break;
    case 5:
        *moveX = (posX - camera->position.x) / float_100_8042274c;
        *moveY = (posY - camera->position.y) / float_100_8042274c;
        *targetMoveX = (targetX - camera->target.x) / float_100_8042274c;
        *targetMoveY = (targetY - camera->target.y) / float_100_8042274c;
        camera->position.x += *moveX;
        camera->position.y += *moveY;
        camera->target.x += *targetMoveX;
        camera->target.y += *targetMoveY;
        camera->up.x = float_0_8042273c;
        camera->up.y = float_1_80422758;
        camera->up.z = float_0_8042273c;
        break;
    }

    level = cameraWork->zoomSpeedLevel;
    switch (level) {
    case 0:
        *moveZ = (posZ - camera->position.z) / float_1p5_80422754;
        *targetMoveZ = (targetZ - camera->target.z) / float_1p5_80422754;
        camera->position.z += *moveZ;
        camera->target.z += *targetMoveZ;
        break;
    case 1:
        *moveZ = (posZ - camera->position.z) / float_15_8042275c;
        *targetMoveZ = (targetZ - camera->target.z) / float_15_8042275c;
        camera->position.z += *moveZ;
        camera->target.z += *targetMoveZ;
        break;
    case 2:
        *moveZ = (posZ - camera->position.z) / float_5_80422760;
        *targetMoveZ = (targetZ - camera->target.z) / float_5_80422760;
        camera->position.z += *moveZ;
        camera->target.z += *targetMoveZ;
        break;
    case 3:
        *moveZ = posZ - camera->position.z;
        *targetMoveZ = targetZ - camera->target.z;
        camera->position.z += *moveZ;
        camera->target.z += *targetMoveZ;
        break;
    case 4:
        *moveZ = (posZ - camera->position.z) / float_200_80422764;
        *targetMoveZ = (targetZ - camera->target.z) / float_200_80422764;
        camera->position.z += *moveZ;
        camera->target.z += *targetMoveZ;
        break;
    case 5:
        *moveZ = (posZ - camera->position.z) / float_100_8042274c;
        *targetMoveZ = (targetZ - camera->target.z) / float_100_8042274c;
        camera->position.z += *moveZ;
        camera->target.z += *targetMoveZ;
        break;
    }
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

s32 evt_btl_camera_set_homing_unit(int event) {
    typedef struct Vec3f_HomingUnit {
        f32 x;
        f32 y;
        f32 z;
    } Vec3f_HomingUnit;
    extern s32 evtGetValue(void*, s32);
    extern s32 BattleTransID(void*, s32);
    extern void* BattleGetUnitPtr(void*, s32);
    extern s32 BtlUnit_GetBodyPartsId(void*);
    extern void* BattleGetUnitPartsPtr(s32, s32);
    extern void BtlUnit_GetHitPos(void*, void*, f32*, f32*, f32*);
    extern const Vec3f_HomingUnit vec3_802f0264;
    extern const Vec3f_HomingUnit vec3_802f0270;
    s32* args = *(s32**)(event + 0x18);
    u8* work;
    s32 unit1;
    s32 unit2;
    s32 partId;
    f32 pos1[3];
    f32 pos2[3];
    Vec3f_HomingUnit target1;
    Vec3f_HomingUnit target2;
    void* unit;
    void* part;

    evtGetValue((void*)event, args[0]);
    unit1 = evtGetValue((void*)event, args[1]);
    unit2 = evtGetValue((void*)event, args[2]);
    work = (u8*)_battleWorkPointer;
    unit1 = BattleTransID((void*)event, unit1);
    *(s32*)(work + 0x2764) = unit1;
    unit = BattleGetUnitPtr(work, *(s32*)(work + 0x2764));
    partId = BtlUnit_GetBodyPartsId(unit);
    *(s32*)(work + 0x276C) = partId;
    BattleGetUnitPartsPtr(*(s32*)(work + 0x2764), *(s32*)(work + 0x276C));
    BtlUnit_GetHomePos(unit, &pos1[0], &pos1[1], &pos1[2]);
    target1 = vec3_802f0264;
    target1.x = pos1[0];
    target1.y = pos1[1];
    target1.z = pos1[2];
    *(f32*)(work + 0x2810) = target1.x;
    *(f32*)(work + 0x2814) = target1.y;
    *(f32*)(work + 0x2818) = target1.z;
    if (unit2 != -1) {
        unit2 = BattleTransID((void*)event, unit2);
        *(s32*)(work + 0x2768) = unit2;
        unit = BattleGetUnitPtr(work, *(s32*)(work + 0x2768));
        partId = BtlUnit_GetBodyPartsId(unit);
        *(s32*)(work + 0x2770) = partId;
        part = BattleGetUnitPartsPtr(*(s32*)(work + 0x2768), *(s32*)(work + 0x2770));
        BtlUnit_GetHitPos(unit, part, &pos2[0], &pos2[1], &pos2[2]);
        target2 = vec3_802f0270;
        target2.x = pos2[0];
        target2.y = pos2[1];
        target2.z = pos2[2];
        *(f32*)(work + 0x281C) = target2.x;
        *(f32*)(work + 0x2820) = target2.y;
        *(f32*)(work + 0x2824) = target2.z;
    }
    return 2;
}

s32 evt_btl_camera_set_homing_unitparts(int event) {
    extern s32 evtGetValue(void*, s32);
    extern s32 BattleTransID(void*, s32);
    extern void* BattleGetUnitPtr(void*, s32);
    extern void* BattleGetUnitPartsPtr(s32, s32);
    extern void BtlUnit_GetHitPos(void*, void*, f32*, f32*, f32*);
    s32* args = *(s32**)(event + 0x18);
    u8* work = (u8*)_battleWorkPointer;
    s32 priority = evtGetValue((void*)event, args[0]);
    s32 unit1 = evtGetValue((void*)event, args[1]);
    s32 part1 = evtGetValue((void*)event, args[2]);
    s32 unit2 = evtGetValue((void*)event, args[3]);
    s32 part2 = evtGetValue((void*)event, args[4]);
    f32 pos[3];
    void* unit;
    void* part;

    if (priority >= *(s32*)(work + 0x275C)) {
        unit1 = BattleTransID((void*)event, unit1);
        *(s32*)(work + 0x2764) = unit1;
        *(s32*)(work + 0x276C) = part1;
        unit = BattleGetUnitPtr(work, unit1);
        BattleGetUnitPartsPtr(unit1, part1);
        BtlUnit_GetHomePos(unit, &pos[0], &pos[1], &pos[2]);
        *(f32*)(work + 0x2810) = pos[0];
        *(f32*)(work + 0x2814) = pos[1];
        *(f32*)(work + 0x2818) = pos[2];
        if (unit2 != -1) {
            unit2 = BattleTransID((void*)event, unit2);
            *(s32*)(work + 0x2768) = unit2;
            *(s32*)(work + 0x2770) = part2;
            unit = BattleGetUnitPtr(work, unit2);
            part = BattleGetUnitPartsPtr(unit2, part2);
            BtlUnit_GetHitPos(unit, part, &pos[0], &pos[1], &pos[2]);
            *(f32*)(work + 0x281C) = pos[0];
            *(f32*)(work + 0x2820) = pos[1];
            *(f32*)(work + 0x2824) = pos[2];
        }
    }
    return 2;
}

s32 evt_btl_camera_set_homing_unit_audience(int event) {
    extern s32 evtGetValue(void*, s32);
    extern s32 BattleTransID(void*, s32);
    extern void* BattleGetUnitPtr(void*, s32);
    extern void* BattleGetUnitPartsPtr(s32, s32);
    extern void BtlUnit_GetPos(void*, f32*, f32*, f32*);
    extern void BtlUnit_GetHitPos(void*, void*, f32*, f32*, f32*);
    extern void* BattleAudienceBaseGetPtr(void);
    s32* args = *(s32**)(event + 0x18);
    u8* work = (u8*)_battleWorkPointer;
    s32 priority = evtGetValue((void*)event, args[0]);
    s32 unitId = evtGetValue((void*)event, args[1]);
    s32 partId = evtGetValue((void*)event, args[2]);
    s32 audienceId = evtGetValue((void*)event, args[3]);
    f32 pos[3];
    f32 hit[3];
    void* unit;
    void* part;
    u8* audience;

    audience = BattleAudienceBaseGetPtr();
    if (priority >= *(s32*)(work + 0x275C)) {
        unitId = BattleTransID((void*)event, unitId);
        *(s32*)(work + 0x2764) = unitId;
        *(s32*)(work + 0x276C) = partId;
        unit = BattleGetUnitPtr(work, unitId);
        part = BattleGetUnitPartsPtr(unitId, partId);
        BtlUnit_GetPos(unit, &pos[0], &pos[1], &pos[2]);
        BtlUnit_GetHitPos(unit, part, &hit[0], &hit[1], &hit[2]);
        *(f32*)(work + 0x2810) = pos[0];
        *(f32*)(work + 0x2814) = pos[1];
        *(f32*)(work + 0x2818) = pos[2];
        *(s32*)(work + 0x2774) = audienceId;
        *(f32*)(work + 0x281C) = *(f32*)(audience + 0x140 + audienceId * 0x11C);
        *(f32*)(work + 0x2820) = *(f32*)(audience + 0x144 + audienceId * 0x11C);
        *(f32*)(work + 0x2824) = *(f32*)(audience + 0x148 + audienceId * 0x11C);
    }
    return 2;
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

s32 evt_btl_camera_add_zoom(int param_1) {
    s32* args;
    s32 priority;
    s32 zoom;
    void* event;

    event = (void*)param_1;
    args = *(s32**)((s32)event + 0x18);
    priority = evtGetValue(event, args[0]);
    zoom = evtGetFloat(event, args[1]);
    btl_camera_add_zoom(priority, zoom);

    return 2;
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

void btl_camera_shake_w(s32 priority, f32 a, f32 b, s32 frames, s32 type) {
    void* work;

    work = _battleWorkPointer;
    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    *(u32*)((s32)work + 0x2754) |= 1;
    *(f32*)((s32)work + 0x2840) = a;
    *(f32*)((s32)work + 0x2844) = b;
    *(s32*)((s32)work + 0x2828) = type;
    *(s32*)((s32)work + 0x2830) = frames;
    *(s32*)((s32)work + 0x2838) = frames;

    if ((*(u32*)((s32)work + 0x2754) & 0x40) == 0) {
        padRumbleOn(0);
    }
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

void btl_camera_shake_h(s32 priority, f32 a, f32 b, s32 frames, s32 type) {
    void* work;

    work = _battleWorkPointer;
    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    *(u32*)((s32)work + 0x2754) |= 2;
    *(f32*)((s32)work + 0x2848) = a;
    *(f32*)((s32)work + 0x284C) = b;
    *(s32*)((s32)work + 0x282C) = type;
    *(s32*)((s32)work + 0x2834) = frames;
    *(s32*)((s32)work + 0x283C) = frames;

    if ((*(u32*)((s32)work + 0x2754) & 0x40) == 0) {
        padRumbleOn(0);
    }
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

void btl_camera_noshake(s32 priority) {
    void* work;

    work = _battleWorkPointer;
    if (priority < *(s32*)((s32)work + 0x275C)) {
        return;
    }

    *(u32*)((s32)work + 0x2754) &= ~0x1;
    *(u32*)((s32)work + 0x2754) &= ~0x2;
}

s32 evt_btl_camera_noshake(void* evt) {
    s32* args;
    s32 priority;

    args = *(s32**)((s32)evt + 0x18);
    priority = evtGetValue(evt, args[0]);
    btl_camera_noshake(priority);

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

s32 evt_btl_camera_off_posoffset_manual(void* evt) {
    s32* args;
    s32 priority;

    args = *(s32**)((s32)evt + 0x18);
    priority = evtGetValue(evt, args[0]);
    btl_camera_off_posoffset_manual(priority);

    return 2;
}

USER_FUNC(evt_btl_camera_wait_move_end) {
    extern f32 float_2_80422738;
    if (battleCameraGetPosMoveSpeed() >= float_2_80422738) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}
