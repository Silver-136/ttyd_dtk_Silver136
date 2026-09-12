#include "battle/battle_camera.h"

#include "battle/battle.h"



void* camGetPtr(s32 cameraId);
void BtlUnit_GetHomePos(void* unit, f32* x, f32* y, f32* z);
void padRumbleOn(s32 channel);
void btl_camera_shake_w(s32 priority, f32 a, f32 b, s32 time, s32 type);

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

typedef struct Vec3f_HomingUnit {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_HomingUnit;

const Vec vec3_802f024c = {0.0f, 0.0f, 0.0f};
const Vec vec3_802f0258 = {0.0f, 0.0f, 0.0f};
const Vec3f_HomingUnit vec3_802f0264 = {0.0f, 0.0f, 0.0f};
const Vec3f_HomingUnit vec3_802f0270 = {0.0f, 0.0f, 0.0f};
const Vec vec3_802f027c = {0.0f, 0.0f, 0.0f};
const Vec vec3_802f0288 = {0.0f, 0.0f, 0.0f};
const Vec vec3_802f0294 = {0.0f, 0.0f, 0.0f};
const Vec vec3_802f02a0 = {0.0f, 0.0f, 0.0f};

void battleCameraMain(void) {
    extern f32 intplGetValue(f32, f32, s32, s32, s32);
    extern BattleWorkUnit* BattleGetUnitPtr(BattleWork*, s32);
    extern BattleWorkUnit* BattleGetMarioPtr(BattleWork*);
    extern BattleWorkUnit* BattleGetPartyPtr(BattleWork*);
    extern BattleWorkUnitPart* BattleGetUnitPartsPtr(s32, s32);
    extern void BtlUnit_GetPos(BattleWorkUnit*, f32*, f32*, f32*);
    extern void BtlUnit_GetHitPos(BattleWorkUnit*, BattleWorkUnitPart*, f32*, f32*, f32*);
    extern void BtlUnit_GetPartsWorldPos(BattleWorkUnitPart*, f32*, f32*, f32*);
    extern s32 BtlUnit_GetHeight(BattleWorkUnit*);
    extern s32 BtlUnit_GetWidth(BattleWorkUnit*);
    extern void* BattleAudienceBaseGetPtr(void);
    extern void* pouchGetPtr(void);
    extern void padRumbleOff(s32);
    extern char* strcpy(char*, const char*);

    extern f32 float_0_8042273c;
    extern f32 float_0p5_80422748;
    extern f32 float_neg100_80422750;
    extern f32 float_1_80422758;
    extern f32 float_200_80422764;
    extern f32 float_110_80422768;
    extern f32 float_750_8042276c;
    extern f32 float_60_80422770;
    extern f32 float_160_80422774;
    extern f32 float_neg750_80422778;
    extern f32 float_400_8042277c;
    extern f32 float_neg617_80422780;
    extern f32 float_830_80422784;
    extern f32 float_920_80422788;
    extern f32 float_1000_8042278c;
    extern f32 float_80_8042279c;
    extern f32 float_1Eneg05_804227a4;
    extern f32 float_neg30_804227a8;
    extern f32 float_125_804227ac;
    extern f32 float_500_804227b0;
    extern f32 float_20_804227b4;
    extern f32 float_350_804227b8;

    BattleWork* battleWork;
    BattleWorkCamera* camera;
    u8* view;
    u8* audience;
    u8* pouch;
    BattleWorkUnit* unit;
    BattleWorkUnit* unit2;
    BattleWorkUnit* mario;
    BattleWorkUnit* party;
    BattleWorkUnitPart* part;
    BattleWorkUnitPart* part2;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dtx;
    f32 dty;
    f32 dtz;
    f32 pz;
    f32 tz;
    f32 unitPos[3];
    f32 unitPos2[3];
    f32 home1[3];
    f32 home2[3];
    f32 height;
    f32 width;
    f32 width2;
    f32 spread;
    f32 focusY;
    f32 midpoint;
    f32 ratio;
    f32 targetXRef;
    f32 targetYRef;
    f32 delta;
    f32 vertical;
    f32 zoomShift;
    f32 shake;
    f32 audienceX;
    f32 audienceZ;
    char cameraName[64];
    s16 oldMove;
    s16 oldZoom;
    s16 rank;
    s32 audienceIndex;
    s32 total;
    s32 remaining;
    s32 elapsed;
    u32 flags;

    view = camGetPtr(4);
    battleWork = _battleWorkPointer;
    camera = &battleWork->camera;
    audience = BattleAudienceBaseGetPtr();
    BattleGetMarioPtr(battleWork);

    dx = float_0_8042273c;
    dy = float_0_8042273c;
    dz = float_0_8042273c;
    dtx = float_0_8042273c;
    dty = float_0_8042273c;
    dtz = float_0_8042273c;

    if (camera->mode != 3) {
        *(s32*)((u8*)camera + 0x84) = 0;
    }

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

    do {
        if (camera->mode == 0) {
            battleCameraMoveTo(0.0f, 110.0f, 750.0f, 0.0f, 60.0f, 0.0f, 1, 1,
                               &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xc5\x82\xd3\x82\xa7\x82\xe9\x82\xc6");
            break;
        }
        else if (camera->mode == 1) {
            pz = 750.0f - camera->zoom * 0.5f;
            tz = -camera->zoom * 0.5f;
            battleCameraMoveTo(0.0f, 110.0f, pz, 0.0f, 60.0f, tz, 1, 1,
                               &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xc5\x82\xd3\x82\xa7\x82\xe9\x82\xc6\x83\x59\x81\x5b\x83\x80");
            break;
        }
        else if (camera->mode == 2 || camera->mode == 3) {
            if (camera->mode == 2) {
                battleCameraMoveTo(0.0f, 160.0f, 750.0f, 0.0f, 110.0f, 0.0f, 1, 1,
                                   &dx, &dy, &dz, &dtx, &dty, &dtz);
                strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xa9\x81\x5b\x82\xbb\x82\xe9\x82\xaa\x82\xa4\x82\xa6\x82\xc9");
            }
            if (*(s32*)((u8*)camera + 0x84) > 0) {
                Vec interpPos;
                f32 interpTargetX;
                f32 interpTargetY;
                f32 interpTargetZ;

                oldMove = camera->moveSpeedLevel;
                (*(s32*)((u8*)camera + 0x84))--;
                interpPos.x = intplGetValue(*(f32*)((u8*)camera + 0x8C), *(f32*)((u8*)camera + 0x98),
                                            *(s32*)((u8*)camera + 0x80),
                                            *(s32*)((u8*)camera + 0x88) - *(s32*)((u8*)camera + 0x84),
                                            *(s32*)((u8*)camera + 0x88));
                interpPos.y = intplGetValue(*(f32*)((u8*)camera + 0x90), *(f32*)((u8*)camera + 0x9C),
                                            *(s32*)((u8*)camera + 0x80),
                                            *(s32*)((u8*)camera + 0x88) - *(s32*)((u8*)camera + 0x84),
                                            *(s32*)((u8*)camera + 0x88));
                interpPos.z = intplGetValue(*(f32*)((u8*)camera + 0x94), *(f32*)((u8*)camera + 0xA0),
                                            *(s32*)((u8*)camera + 0x80),
                                            *(s32*)((u8*)camera + 0x88) - *(s32*)((u8*)camera + 0x84),
                                            *(s32*)((u8*)camera + 0x88));
                interpTargetX = intplGetValue(*(f32*)((u8*)camera + 0xA4), *(f32*)((u8*)camera + 0xB0),
                                              *(s32*)((u8*)camera + 0x80),
                                              *(s32*)((u8*)camera + 0x88) - *(s32*)((u8*)camera + 0x84),
                                              *(s32*)((u8*)camera + 0x88));
                interpTargetY = intplGetValue(*(f32*)((u8*)camera + 0xA8), *(f32*)((u8*)camera + 0xB4),
                                              *(s32*)((u8*)camera + 0x80),
                                              *(s32*)((u8*)camera + 0x88) - *(s32*)((u8*)camera + 0x84),
                                              *(s32*)((u8*)camera + 0x88));
                interpTargetZ = intplGetValue(*(f32*)((u8*)camera + 0xAC), *(f32*)((u8*)camera + 0xB8),
                                              *(s32*)((u8*)camera + 0x80),
                                              *(s32*)((u8*)camera + 0x88) - *(s32*)((u8*)camera + 0x84),
                                              *(s32*)((u8*)camera + 0x88));
                camera->moveSpeedLevel = 3;
                camera->zoomSpeedLevel = 3;
                battleCameraMoveTo(interpPos.x, interpPos.y, interpPos.z,
                                   interpTargetX, interpTargetY, interpTargetZ, 0, 0,
                                   &dx, &dy, &dz, &dtx, &dty, &dtz);
                camera->moveSpeedLevel = oldMove;
            }
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xd3\x82\xe8\x81\x5b");
            break;
        }
        else if (camera->mode == 4) {
            battleCameraMoveTo(0.0f, 110.0f, -750.0f, 0.0f, 60.0f, 0.0f, 1, 1,
                               &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xd4\x82\xbd\x82\xa2\x82\xa4\x82\xe7");
            break;
        }
        else if (camera->mode == 5) {
            battleCameraMoveTo(0.0f, 400.0f, -617.0f, 0.0f, 60.0f, 0.0f, 1, 1,
                               &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xd4\x82\xbd\x82\xa2\x82\xa4\x82\xe7\x82\x51");
            break;
        }
        else if (camera->mode == 6) {
            pouch = (u8*)pouchGetPtr();
            rank = *(s16*)(pouch + 0x88);
            pz = float_0_8042273c;
            if (rank == 0) {
                pz = float_750_8042276c;
            } else if (rank == 1) {
                pz = float_830_80422784;
            } else if (rank == 2) {
                pz = float_920_80422788;
            } else if (rank == 3) {
                pz = float_1000_8042278c;
            }
            battleCameraMoveTo(float_0_8042273c, float_110_80422768, pz,
                               float_0_8042273c, float_60_80422770, float_0_8042273c,
                               1, 1, &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xa9\x82\xf1\x82\xab\x82\xe1\x82\xad");
            break;
        }
        else if (camera->mode == 7) {
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
            unitPos[1] += (height - 50.0f) * float_0p5_80422748;
            if (width < float_0_8042273c) {
                width = float_0_8042273c;
            }
            spread = -(1.5f * width - camera->zoom);
            focusY = -spread / 9.0f;
            height = unitPos[1];
            if (height > 100.0f) {
                height = 100.0f;
            }
            if (unitPos[1] < float_0_8042273c) {
                height = float_0_8042273c;
            }
            battleCameraMoveTo(unitPos[0], float_110_80422768 + height + focusY,
                               float_750_8042276c - spread, unitPos[0],
                               float_60_80422770 + height + focusY, -spread, 1, 1,
                               &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xe4\x82\xc9\x82\xc1\x82\xc6\x82\xc2\x82\xa2\x82\xd1");
            break;
        }
        else if (camera->mode == 8) {
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
            if (width < float_0_8042273c) {
                width = float_0_8042273c;
            }
            if (width2 < float_0_8042273c) {
                width2 = float_0_8042273c;
            }
            targetXRef = *(f32*)((u8*)camera + 0xC8);
            delta = targetXRef - *(f32*)((u8*)camera + 0xBC);
            ratio = float_0p5_80422748;
            if (delta != float_0_8042273c) {
                ratio = (unitPos2[0] - unitPos[0]) / delta;
            }
            if (ratio > float_1_80422758) {
                ratio = float_1_80422758;
            }
            if (ratio < float_0p5_80422748) {
                ratio = float_0p5_80422748;
            }
            targetYRef = *(f32*)((u8*)camera + 0xCC);
            spread = (float_1_80422758 - ratio) *
                     -(1.5f * (width + width2) -
                       2.0f * camera->zoom);
            midpoint = (float_1_80422758 - ratio) * targetXRef;
            vertical = (targetYRef / float_80_8042279c) * (unitPos[1] - targetYRef);
            focusY = -spread / 10.0f;
            if (vertical > 100.0f) {
                vertical = 100.0f;
            }
            if (vertical < float_0_8042273c) {
                vertical = float_0_8042273c;
            }
            height = (f32)(20.0 * -(1.0 - ratio) + vertical);
            battleCameraMoveTo(midpoint, float_110_80422768 + height + focusY,
                               float_750_8042276c - spread, midpoint,
                               float_60_80422770 + height + focusY, -spread, 1, 1,
                               &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\x51\x82\xe4\x82\xc9\x82\xc1\x82\xc6\x82\xa9\x82\xf1\x82\xc2\x82\xa2\x82\xd1");
            break;
        }
        else if (camera->mode == 9) {
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
            BtlUnit_GetWidth(unit2);
            if (width < float_0_8042273c) {
                width = float_0_8042273c;
            }
            spread = -(1.5f * width - 2.0f * camera->zoom);
            height = float_0p5_80422748 * (unitPos[1] + unitPos2[1]);
            focusY = -spread / 10.0f;
            if (height > 50.0f) {
                height = 50.0f;
            }
            if (height < float_0_8042273c) {
                height = float_0_8042273c;
            }
            midpoint = float_0p5_80422748 * (unitPos[0] + unitPos2[0]);
            battleCameraMoveTo(midpoint, float_110_80422768 + height + focusY,
                               float_750_8042276c - spread, midpoint,
                               float_60_80422770 + height + focusY, -spread, 1, 1,
                               &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xe4\x82\xc9\x82\xc1\x82\xc6\x82\xc2\x82\xa2\x82\xd1");
            break;
        }
        else if (camera->mode == 10) {
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
            unitPos[1] += (height - 50.0f) * float_0p5_80422748;
            width = (f32)BtlUnit_GetWidth(unit) - 24.0f;
            if (width < float_0_8042273c) {
                width = float_0_8042273c;
            }
            spread = -(1.5f * width - camera->zoom);
            focusY = -spread / 9.0f;
            height = unitPos[1];
            if (height > 100.0f) {
                height = 100.0f;
            }
            if (unitPos[1] < float_0_8042273c) {
                height = float_0_8042273c;
            }
            battleCameraMoveTo(unitPos[0], float_110_80422768 + height + focusY,
                               float_750_8042276c - spread, unitPos[0],
                               float_60_80422770 + height + focusY, -spread, 1, 1,
                               &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xcf\x81\x5b\x82\xc2\x82\xc2\x82\xa2\x82\xd1");
            break;
        }
        else if (camera->mode == 11) {
            unit = BattleGetUnitPtr(battleWork, *(s32*)((u8*)camera + 0x10));
            unit2 = BattleGetUnitPtr(battleWork, *(s32*)((u8*)camera + 0x14));
            if (unit == 0 || unit2 == 0) {
                btl_camera_set_mode(0, 0);
                break;
            }
            part = BattleGetUnitPartsPtr(*(s32*)((u8*)camera + 0x10),
                                         *(s32*)((u8*)camera + 0x18));
            part2 = BattleGetUnitPartsPtr(*(s32*)((u8*)camera + 0x14),
                                          *(s32*)((u8*)camera + 0x1C));
            if (part == 0 || part2 == 0) {
                btl_camera_set_mode(0, 0);
                break;
            }
            BtlUnit_GetPartsWorldPos(part, &unitPos[0], &unitPos[1], &unitPos[2]);
            BtlUnit_GetHitPos(unit2, part2, &unitPos2[0], &unitPos2[1], &unitPos2[2]);
            width = (f32)BtlUnit_GetWidth(unit) - 24.0f;
            width2 = (f32)BtlUnit_GetWidth(unit2) - 24.0f;
            if (width < float_0_8042273c) {
                width = float_0_8042273c;
            }
            if (width2 < float_0_8042273c) {
                width2 = float_0_8042273c;
            }
            targetXRef = *(f32*)((u8*)camera + 0xC8);
            delta = targetXRef - *(f32*)((u8*)camera + 0xBC);
            if (delta == float_0_8042273c) {
                delta = float_1Eneg05_804227a4;
            }
            ratio = (unitPos2[0] - unitPos[0]) / delta;
            if (ratio > float_1_80422758) {
                ratio = float_1_80422758;
            }
            if (ratio < float_0p5_80422748) {
                ratio = float_0p5_80422748;
            }
            targetYRef = *(f32*)((u8*)camera + 0xCC);
            spread = (float_1_80422758 - ratio) *
                     -(1.5f * (width + width2) -
                       2.0f * camera->zoom);
            midpoint = (float_1_80422758 - ratio) * targetXRef;
            vertical = (targetYRef / float_80_8042279c) * (unitPos[1] - targetYRef);
            focusY = -spread / 10.0f;
            if (vertical > 100.0f) {
                vertical = 100.0f;
            }
            if (vertical < float_0_8042273c) {
                vertical = float_0_8042273c;
            }
            height = (f32)(20.0 * -(1.0 - ratio) + vertical);
            battleCameraMoveTo(midpoint, float_110_80422768 + height + focusY,
                               float_750_8042276c - spread, midpoint,
                               float_60_80422770 + height + focusY, -spread, 1, 1,
                               &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\x51\x82\xcf\x81\x5b\x82\xc2\x82\xa9\x82\xf1\x82\xc2\x82\xa2\x82\xd1");
            break;
        }
        else if (camera->mode == 12) {
            audienceIndex = *(s32*)((u8*)camera + 0x20);
            audienceX = *(f32*)(audience + audienceIndex * 0x134 + 0x204);
            audienceZ = *(f32*)(audience + audienceIndex * 0x134 + 0x20C);
            battleCameraMoveTo(audienceX, float_200_80422764,
                               float_400_8042277c + audienceZ,
                               audienceX, float_0_8042273c, audienceZ, 1, 0,
                               &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xa9\x82\xf1\x82\xab\x82\xe1\x82\xad\x82\xc2\x82\xa2\x82\xd1");
            break;
        }
        else if (camera->mode == 13) {
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
            *(s32*)((u8*)camera + 0x20) = 10;
            delta = *(f32*)((u8*)camera + 0xC8) - *(f32*)((u8*)camera + 0xBC);
            BtlUnit_GetPos(unit, &unitPos[0], &unitPos[1], &unitPos[2]);
            targetXRef = *(f32*)((u8*)camera + 0xC8);
            ratio = float_0p5_80422748;
            if (delta != float_0_8042273c) {
                ratio = (targetXRef - unitPos[0]) / delta;
            }
            if (ratio > float_1_80422758) {
                ratio = float_1_80422758;
            }
            if (ratio < float_0p5_80422748) {
                ratio = float_0p5_80422748;
            }
            vertical = (unitPos[1] - *(f32*)((u8*)camera + 0xC0)) - 100.0f;
            midpoint = (float_1_80422758 - ratio) * targetXRef;
            if (vertical > float_0_8042273c) {
                vertical = float_0_8042273c;
            }
            if (vertical < float_neg30_804227a8) {
                vertical = float_neg30_804227a8;
            }
            zoomShift = (float_1_80422758 - ratio) * 2.0f * camera->zoom;
            height = (f32)(-(1.0 - ratio) * (float_125_804227ac - vertical));
            battleCameraMoveTo(midpoint, float_110_80422768 + height,
                               float_750_8042276c - zoomShift, midpoint,
                               float_60_80422770 + height, -zoomShift, 1, 1,
                               &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xe4\x82\xc9\x82\xc1\x82\xc6\x82\xa9\x82\xf1\x82\xab\x82\xe1\x82\xad\x82\xa9\x82\xf1\x82\xc2\x82\xa2\x82\xd1");
            break;
        }
        else if (camera->mode == 14) {
            camera->counter++;
            if (camera->counter == 1) {
                pouch = (u8*)pouchGetPtr();
                rank = *(s16*)(pouch + 0x88);
                pz = float_0_8042273c;
                if (rank == 0) {
                    pz = float_750_8042276c;
                } else if (rank == 1) {
                    pz = float_830_80422784;
                } else if (rank == 2) {
                    pz = float_920_80422788;
                } else if (rank == 3) {
                    pz = float_1000_8042278c;
                }
                camera->moveSpeedLevel = 3;
                camera->zoomSpeedLevel = 3;
                battleCameraMoveTo(float_0_8042273c, float_110_80422768, pz,
                                   float_0_8042273c, float_60_80422770, float_0_8042273c,
                                   1, 1, &dx, &dy, &dz, &dtx, &dty, &dtz);
            }
            if (camera->counter == 10) {
                camera->counter = 0;
                btl_camera_set_mode(0, 3);
                btl_camera_set_moveto(0, float_0_8042273c, float_110_80422768,
                                      float_750_8042276c, float_0_8042273c,
                                      float_60_80422770, float_0_8042273c, 0x78, 4);
            }
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xd3\x82\xc2\x82\xa4\x82\xc9\x82\xcd\x82\xa2\x82\xc1\x82\xbd");
            break;
        }
        else if (camera->mode == 15) {
            camera->moveSpeedLevel = 3;
            camera->zoomSpeedLevel = 3;
            battleCameraMoveTo(float_0_8042273c, float_110_80422768, float_500_804227b0,
                               float_0_8042273c, float_60_80422770, float_0_8042273c,
                               1, 1, &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xa4\x82\xdc\x82\xad\x82\xcd\x82\xa2\x82\xc1\x82\xbd");
            break;
        }
        else if (camera->mode == 16) {
            camera->moveSpeedLevel = 3;
            camera->zoomSpeedLevel = 3;
            battleCameraMoveTo(float_neg100_80422750, float_110_80422768, float_500_804227b0,
                               float_neg100_80422750, float_60_80422770, float_0_8042273c,
                               1, 1, &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xbe\x82\xdf\x82\xc9\x82\xcd\x82\xa2\x82\xc1\x82\xbd");
            break;
        }
        else if (camera->mode == 17 || camera->mode == 18) {
            f32 centerX;
            f32 centerY;
            f32 centerZ;

            mario = BattleGetMarioPtr(battleWork);
            party = BattleGetPartyPtr(battleWork);
            if (party != 0) {
                BtlUnit_GetHomePos(mario, &home1[0], &home1[1], &home1[2]);
                BtlUnit_GetHomePos(party, &home2[0], &home2[1], &home2[2]);
                centerX = float_0p5_80422748 * (home1[0] + home2[0]);
                centerY = float_0p5_80422748 * (home1[1] + home2[1]);
                centerZ = float_0p5_80422748 * (home1[2] + home2[2]);
                if (camera->mode == 18) {
                    centerY -= float_20_804227b4;
                }
            } else {
                BtlUnit_GetHomePos(mario, &home1[0], &home1[1], &home1[2]);
                centerX = home1[0];
                centerY = home1[1];
                centerZ = home1[2];
            }
            camera->moveSpeedLevel = 1;
            camera->zoomSpeedLevel = 1;
            battleCameraMoveTo(centerX, 100.0f + centerY,
                               float_350_804227b8 + centerZ, centerX,
                               50.0f + centerY, float_0_8042273c,
                               0, 1, &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xb5\x82\xe5\x82\xa4\x82\xe8");
            break;
        }
        else if (camera->mode == 19) {
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xcd\x82\xa2\x82\xda\x82\xad");
            break;
        }
        else if (camera->mode == 20) {
            camera->moveSpeedLevel = 2;
            camera->zoomSpeedLevel = 2;
            battleCameraMoveTo(float_neg100_80422750, 100.0f, float_400_8042277c,
                               float_neg100_80422750, 50.0f, float_0_8042273c,
                               1, 1, &dx, &dy, &dz, &dtx, &dty, &dtz);
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xc9\x82\xb0\x82\xc4\x82\xe9");
            break;
        }
        else {
            strcpy(cameraName, "\x82\xa9\x82\xdf\x82\xe7\x81\x40\x82\xa8\x82\xa9\x82\xb5\x82\xa2");
            break;
        }
    } while (0);

    if (*(f32*)(view + 0x14) >= float_1000_8042278c) {
        *(f32*)(view + 0x14) = float_1000_8042278c;
    }

    if ((camera->flags & 4) != 0) {
        *(f32*)(view + 0x0C) = *(f32*)((u8*)camera + 0x38);
        *(f32*)(view + 0x18) = *(f32*)((u8*)camera + 0x50);
        dx = float_0_8042273c;
        dtx = float_0_8042273c;
    }
    if ((camera->flags & 8) != 0) {
        *(f32*)(view + 0x10) = *(f32*)((u8*)camera + 0x3C);
        *(f32*)(view + 0x1C) = *(f32*)((u8*)camera + 0x54);
        dy = float_0_8042273c;
        dty = float_0_8042273c;
    }
    if ((camera->flags & 0x10) != 0) {
        *(f32*)(view + 0x14) = *(f32*)((u8*)camera + 0x40);
        *(f32*)(view + 0x20) = *(f32*)((u8*)camera + 0x58);
        dz = float_0_8042273c;
        dtz = float_0_8042273c;
    }

    *(f32*)((u8*)camera + 0x2C) = *(f32*)(view + 0x0C);
    *(f32*)((u8*)camera + 0x30) = *(f32*)(view + 0x10);
    *(f32*)((u8*)camera + 0x34) = *(f32*)(view + 0x14);
    *(f32*)((u8*)camera + 0x44) = *(f32*)(view + 0x18);
    *(f32*)((u8*)camera + 0x48) = *(f32*)(view + 0x1C);
    *(f32*)((u8*)camera + 0x4C) = *(f32*)(view + 0x20);

    *(f32*)((u8*)camera + 0x68) = dx;
    *(f32*)((u8*)camera + 0x6C) = dy;
    *(f32*)((u8*)camera + 0x70) = dz;
    *(f32*)((u8*)camera + 0x74) = dtx;
    *(f32*)((u8*)camera + 0x78) = dty;
    *(f32*)((u8*)camera + 0x7C) = dtz;

    flags = camera->flags;
    if ((flags & 1) == 0 && (flags & 2) == 0) {
        padRumbleOff(0);
    }
    if ((camera->flags & 0x40) != 0) {
        padRumbleOff(0);
    }

    if ((camera->flags & 1) != 0) {
        oldMove = camera->moveSpeedLevel;
        oldZoom = camera->zoomSpeedLevel;
        total = *(s32*)((u8*)camera + 0xDC);
        remaining = *(s32*)((u8*)camera + 0xE4);
        camera->moveSpeedLevel = 3;
        camera->zoomSpeedLevel = 3;
        elapsed = total - remaining;
        shake = intplGetValue(*(f32*)((u8*)camera + 0xEC),
                              *(f32*)((u8*)camera + 0xF0),
                              *(s32*)((u8*)camera + 0xD4),
                              elapsed / 2, total / 2);
        *(f32*)((u8*)camera + 0xFC) = shake;
        if (remaining == 0) {
            camera->flags &= ~1;
        } else if ((elapsed & 2) == 0) {
            battleCameraMoveTo(*(f32*)(view + 0x0C) + shake,
                               *(f32*)(view + 0x10), *(f32*)(view + 0x14),
                               *(f32*)(view + 0x18) + shake,
                               *(f32*)(view + 0x1C), *(f32*)(view + 0x20),
                               0, 0, &dx, &dy, &dz, &dtx, &dty, &dtz);
        } else {
            battleCameraMoveTo(*(f32*)(view + 0x0C) - shake,
                               *(f32*)(view + 0x10), *(f32*)(view + 0x14),
                               *(f32*)(view + 0x18) - shake,
                               *(f32*)(view + 0x1C), *(f32*)(view + 0x20),
                               0, 0, &dx, &dy, &dz, &dtx, &dty, &dtz);
        }
        camera->moveSpeedLevel = oldMove;
        camera->zoomSpeedLevel = oldZoom;
        (*(s32*)((u8*)camera + 0xE4))--;
    }

    if ((camera->flags & 2) != 0) {
        oldMove = camera->moveSpeedLevel;
        oldZoom = camera->zoomSpeedLevel;
        total = *(s32*)((u8*)camera + 0xE0);
        remaining = *(s32*)((u8*)camera + 0xE8);
        camera->moveSpeedLevel = 3;
        camera->zoomSpeedLevel = 3;
        elapsed = total - remaining;
        shake = intplGetValue(*(f32*)((u8*)camera + 0xF4),
                              *(f32*)((u8*)camera + 0xF8),
                              *(s32*)((u8*)camera + 0xD8),
                              elapsed / 2, total / 2);
        *(f32*)((u8*)camera + 0x100) = shake;
        if (remaining == 0) {
            camera->flags &= ~2;
        } else if ((elapsed & 2) == 0) {
            battleCameraMoveTo(*(f32*)(view + 0x0C),
                               *(f32*)(view + 0x10) + shake, *(f32*)(view + 0x14),
                               *(f32*)(view + 0x18),
                               *(f32*)(view + 0x1C) + shake, *(f32*)(view + 0x20),
                               0, 0, &dx, &dy, &dz, &dtx, &dty, &dtz);
        } else {
            battleCameraMoveTo(*(f32*)(view + 0x0C),
                               *(f32*)(view + 0x10) - shake, *(f32*)(view + 0x14),
                               *(f32*)(view + 0x18),
                               *(f32*)(view + 0x1C) - shake, *(f32*)(view + 0x20),
                               0, 0, &dx, &dy, &dz, &dtx, &dty, &dtz);
        }
        camera->moveSpeedLevel = oldMove;
        camera->zoomSpeedLevel = oldZoom;
        (*(s32*)((u8*)camera + 0xE8))--;
    }
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
    extern s32 evtGetValue(void*, s32);
    extern s32 BattleTransID(void*, s32);
    extern void* BattleGetUnitPtr(void*, s32);
    extern s32 BtlUnit_GetBodyPartsId(void*);
    extern void* BattleGetUnitPartsPtr(s32, s32);
    extern void BtlUnit_GetHitPos(void*, void*, f32*, f32*, f32*);
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
    *(Vec3f_HomingUnit*)(work + 0x2810) = target1;
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
        *(Vec3f_HomingUnit*)(work + 0x281C) = target2;
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
    s32 priority = evtGetValue((void*)event, args[0]);
    s32 unit1 = evtGetValue((void*)event, args[1]);
    s32 part1 = evtGetValue((void*)event, args[2]);
    s32 unit2 = evtGetValue((void*)event, args[3]);
    s32 part2 = evtGetValue((void*)event, args[4]);
    u8* work = (u8*)_battleWorkPointer;
    Vec pos1;
    Vec pos2;
    Vec target1;
    Vec target2;
    void* unit;
    void* part;

    if (priority < *(s32*)(work + 0x275C)) {
        return 2;
    }

    unit1 = BattleTransID((void*)event, unit1);
    *(s32*)(work + 0x2764) = unit1;
    *(s32*)(work + 0x276C) = part1;
    unit = BattleGetUnitPtr(work, *(s32*)(work + 0x2764));
    BattleGetUnitPartsPtr(*(s32*)(work + 0x2764), *(s32*)(work + 0x276C));
    BtlUnit_GetHomePos(unit, &pos1.x, &pos1.y, &pos1.z);

    target1 = vec3_802f027c;
    target1.x = pos1.x;
    target1.y = pos1.y;
    target1.z = pos1.z;
    *(Vec*)(work + 0x2810) = target1;

    if (unit2 != -1) {
        unit2 = BattleTransID((void*)event, unit2);
        *(s32*)(work + 0x2768) = unit2;
        *(s32*)(work + 0x2770) = part2;
        unit = BattleGetUnitPtr(work, *(s32*)(work + 0x2768));
        part = BattleGetUnitPartsPtr(*(s32*)(work + 0x2768), *(s32*)(work + 0x2770));
        BtlUnit_GetHitPos(unit, part, &pos2.x, &pos2.y, &pos2.z);
        target2 = vec3_802f0288;
        target2.x = pos2.x;
        target2.y = pos2.y;
        target2.z = pos2.z;
        *(Vec*)(work + 0x281C) = target2;
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
    s32 priority = evtGetValue((void*)event, args[0]);
    s32 unitId = evtGetValue((void*)event, args[1]);
    s32 partId = evtGetValue((void*)event, args[2]);
    s32 audienceId = evtGetValue((void*)event, args[3]);
    u8* work = (u8*)_battleWorkPointer;
    u8* audience = BattleAudienceBaseGetPtr();
    Vec pos;
    Vec hit;
    Vec cameraPos;
    Vec audiencePos;
    void* unit;
    void* part;

    if (priority < *(s32*)(work + 0x275C)) {
        return 2;
    }

    unitId = BattleTransID((void*)event, unitId);
    *(s32*)(work + 0x2764) = unitId;
    *(s32*)(work + 0x276C) = partId;
    unit = BattleGetUnitPtr(work, unitId);
    part = BattleGetUnitPartsPtr(unitId, partId);
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    BtlUnit_GetHitPos(unit, part, &hit.x, &hit.y, &hit.z);

    cameraPos = vec3_802f0294;
    cameraPos.x = pos.x;
    cameraPos.y = pos.y;
    cameraPos.z = pos.z;
    *(Vec*)(work + 0x2810) = cameraPos;
    *(s32*)(work + 0x2774) = audienceId;

    audiencePos = vec3_802f02a0;
    audiencePos.x = *(f32*)(audience + 0x204 + audienceId * 0x134);
    audiencePos.y = *(f32*)(audience + 0x208 + audienceId * 0x134);
    audiencePos.z = *(f32*)(audience + 0x20C + audienceId * 0x134);
    *(Vec*)(work + 0x281C) = audiencePos;
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
