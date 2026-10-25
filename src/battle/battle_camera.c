#include "battle/battle_camera.h"
#include "battle/battle.h"
#include "battle/battle_audience.h"
#include "battle/battle_sub.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "manager/evtmgr_cmd.h"
#include "system.h"
#include <string.h>

double sqrt(double);

void btl_camera_nomove_x_onoff(s32 pri, s32 on);
void btl_camera_nomove_y_onoff(s32 pri, s32 on);
void btl_camera_nomove_z_onoff(s32 pri, s32 on);
f32 battleCameraGetPosMoveSpeed(void);
void btl_camera_set_moveto(s32 pri, f32 x, f32 y, f32 z, f32 atX, f32 atY, f32 atZ, s32 frames, s32 type);
void battleCameraMoveTo(s32 mode, s32 clamp, f32 x, f32 y, f32 z, f32 atX, f32 atY, f32 atZ, f32* dX, f32* dY, f32* dZ, f32* dAtX, f32* dAtY, f32* dAtZ);

// this file uses -O4,s
void battleCameraInit(void) {
  BattleWork *wp = _battleWorkPointer;
  BattleCameraWork *camera;

  camera = &wp->cameraWork;
  memset(camera, 0, sizeof(BattleCameraWork));
  camera->flags = 0;
  switch (wp->fbatInfo->partyId) {
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
  camera->priLimit = 0;
  camera->zoom = 0.0f;
  camera->moveSpeedLv = 1;
  camera->zoomSpeedLv = 1;
  camera->position = (Vec){0.0f, 0.0f, 0.0f};
  camera->modeFrame = 0;
}

void battleCameraMain(void) {
  CameraEntry* cam = camGetPtr(CAMERA_3D);
  BattleWork* wp = _battleWorkPointer;
  BattleAudience* aud = BattleAudienceBaseGetPtr();
  f32 zoomZ = 0.0f;
  BattleUnit* mario;
  char name[64];
  Vec pos;
  Vec hit;
  f32 dX, dY, dZ, dAtX, dAtY, dAtZ;
  f32 mx, my, mz;
  f32 px, py, pz;
  BattleUnit* unit2;
  BattleUnitPart* part;
  BattleUnit* party;
  BattleUnitPart* part2;
  BattleUnit* unit;
  f32 w, w2, zz, y, ofs, t, d, yy, atX, atY, atZ;
  s16 moveLv, zoomLv;
  s32 phase;
  s16 moveLv2, zoomLv2;
  s32 phase2;
  s16 moveLv3;

  part2 = 0; // FAKE: dead store, moves part2's variable record first so its case-8 web is a split temp (target r27)
  unit = BattleGetMarioPtr(wp); // FAKE: real zero-code first web of unit (the self-assign below emits nothing), so the case-8 unit web is a split temp (target r26)
  unit->mPosition.x = unit->mPosition.x;
  dX = 0.0f;
  dY = 0.0f;
  dZ = 0.0f;
  dAtX = 0.0f;
  dAtY = 0.0f;
  dAtZ = 0.0f;
  if (wp->cameraWork.mode != 3) {
    wp->cameraWork.moveToTimer = 0;
  }
  cam->cameraPos.x = wp->cameraWork.pos.x;
  cam->cameraPos.y = wp->cameraWork.pos.y;
  cam->cameraPos.z = wp->cameraWork.pos.z;
  cam->cameraAt.x = wp->cameraWork.at.x;
  cam->cameraAt.y = wp->cameraWork.at.y;
  cam->cameraAt.z = wp->cameraWork.at.z;
  wp->cameraWork.prevPos.x = cam->cameraPos.x;
  wp->cameraWork.prevPos.y = cam->cameraPos.y;
  wp->cameraWork.prevPos.z = cam->cameraPos.z;
  wp->cameraWork.prevAt.x = cam->cameraAt.x;
  wp->cameraWork.prevAt.y = cam->cameraAt.y;
  wp->cameraWork.prevAt.z = cam->cameraAt.z;
  switch (wp->cameraWork.mode) {
  case 0:
    battleCameraMoveTo(1, 1, 0.0f, 110.0f, 750.0f, 0.0f, 60.0f, 0.0f, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\305\202\323\202\247\202\351\202\306");
    break;
  case 1:
    zz = 0.5f * wp->cameraWork.zoom;
    battleCameraMoveTo(1, 1, 0.0f, 110.0f, 750.0f - zz, 0.0f, 60.0f, -zz, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\305\202\323\202\247\202\351\202\306\203\131\201\133\203\200");
    break;
  case 2:
    battleCameraMoveTo(1, 1, 0.0f, 160.0f, 750.0f, 0.0f, 110.0f, 0.0f, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\251\201\133\202\273\202\351\202\252\202\244\202\246\202\311");
  case 3:
    if (wp->cameraWork.moveToTimer > 0) {
      moveLv3 = wp->cameraWork.moveSpeedLv;
      wp->cameraWork.moveToTimer--;
      pos.x = intplGetValue(wp->cameraWork.moveToType, wp->cameraWork.moveToFrames - wp->cameraWork.moveToTimer, wp->cameraWork.moveToFrames, wp->cameraWork.moveToStartPos.x, wp->cameraWork.moveToEndPos.x);
      pos.y = intplGetValue(wp->cameraWork.moveToType, wp->cameraWork.moveToFrames - wp->cameraWork.moveToTimer, wp->cameraWork.moveToFrames, wp->cameraWork.moveToStartPos.y, wp->cameraWork.moveToEndPos.y);
      pos.z = intplGetValue(wp->cameraWork.moveToType, wp->cameraWork.moveToFrames - wp->cameraWork.moveToTimer, wp->cameraWork.moveToFrames, wp->cameraWork.moveToStartPos.z, wp->cameraWork.moveToEndPos.z);
      atX = intplGetValue(wp->cameraWork.moveToType, wp->cameraWork.moveToFrames - wp->cameraWork.moveToTimer, wp->cameraWork.moveToFrames, wp->cameraWork.moveToStartAt.x, wp->cameraWork.moveToEndAt.x);
      atY = intplGetValue(wp->cameraWork.moveToType, wp->cameraWork.moveToFrames - wp->cameraWork.moveToTimer, wp->cameraWork.moveToFrames, wp->cameraWork.moveToStartAt.y, wp->cameraWork.moveToEndAt.y);
      atZ = intplGetValue(wp->cameraWork.moveToType, wp->cameraWork.moveToFrames - wp->cameraWork.moveToTimer, wp->cameraWork.moveToFrames, wp->cameraWork.moveToStartAt.z, wp->cameraWork.moveToEndAt.z);
      wp->cameraWork.moveSpeedLv = 3;
      wp->cameraWork.zoomSpeedLv = 3;
      battleCameraMoveTo(0, 0, pos.x, pos.y, pos.z, atX, atY, atZ, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
      wp->cameraWork.moveSpeedLv = moveLv3;
    }
    strcpy(name, "\202\251\202\337\202\347\201\100\202\323\202\350\201\133");
    break;
  case 4:
    battleCameraMoveTo(1, 1, 0.0f, 110.0f, -750.0f, 0.0f, 60.0f, 0.0f, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\324\202\275\202\242\202\244\202\347");
    break;
  case 5:
    battleCameraMoveTo(1, 1, 0.0f, 400.0f, -617.0f, 0.0f, 60.0f, 0.0f, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\324\202\275\202\242\202\244\202\347\202\121");
    break;
  case 6:
    switch (pouchGetPtr()->rank) {
    case 0:
      zoomZ = 750.0f;
      break;
    case 1:
      zoomZ = 830.0f;
      break;
    case 2:
      zoomZ = 920.0f;
      break;
    case 3:
      zoomZ = 1000.0f;
      break;
    }
    battleCameraMoveTo(1, 1, 0.0f, 110.0f, zoomZ, 0.0f, 60.0f, 0.0f, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\251\202\361\202\253\202\341\202\255");
    break;
  case 7:
    unit2 = (0, BattleGetUnitPtr(wp, wp->cameraWork.unitId)); // FAKE: comma makes this unit2 web a compiler temp coloured early (target r28)
    if (unit2 == NULL) {
      btl_camera_set_mode(0, 0);
      break;
    }
    BattleGetUnitPartsPtr(wp->cameraWork.unitId, wp->cameraWork.partsId);
    BtlUnit_GetPos(unit2, &pos.x, &pos.y, &pos.z);
    ofs = (BtlUnit_GetHeight(unit2) - 50.0f) / 2.0f;
    pos.y += ofs;
    w = BtlUnit_GetWidth(unit2) - 24.0f;
    if (w < 0.0f) {
      w = 0.0f;
    }
    zz = wp->cameraWork.zoom;
    zz -= 1.5f * w;
    ofs = -zz / 9.0f;
    t = pos.y;
    y = t;
    if (t > 100.0f) {
      t = 100.0f;
    }
    if (y < 0.0f) {
      t = 0.0f;
    }
    battleCameraMoveTo(1, 1, pos.x, 110.0f + t + ofs, 750.0f - zz, pos.x, 60.0f + t + ofs, -zz, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\344\202\311\202\301\202\306\202\302\202\242\202\321");
    break;
  case 8: {f32 w, w2, d, ofs, t, zz, yy, atX, y;
    unit = BattleGetUnitPtr(wp, wp->cameraWork.unitId);
    unit2 = (0, BattleGetUnitPtr(wp, wp->cameraWork.unitId2)); // FAKE: comma makes this unit2 web a compiler temp coloured early (target r28)
    if (unit == NULL || unit2 == NULL) {
      btl_camera_set_mode(0, 0);
      break;
    }
    part2 = BattleGetUnitPartsPtr(wp->cameraWork.unitId, wp->cameraWork.partsId);
    part2->shadowScale = part2->shadowScale; // FAKE: zero-code use; makes this discarded result part2's first web
    part2 = BattleGetUnitPartsPtr(wp->cameraWork.unitId2, wp->cameraWork.partsId2);
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    BtlUnit_GetHitPos(unit2, part2, &hit.x, &hit.y, &hit.z);
    w = BtlUnit_GetWidth(unit) - 24.0f;
    w2 = BtlUnit_GetWidth(unit2) - 24.0f;
    if (w < 0.0f) {
      w = 0.0f;
    }
    if (w2 < 0.0f) {
      w2 = 0.0f;
    }
    d = wp->cameraWork.homingOffset2.x - wp->cameraWork.homingOffset.x;
    yy = hit.x - pos.x;
    if (d == 0.0f) {
      t = 0.5f;
    } else {
      t = yy / d;
    }
    if (t > 1.0f) {
      t = 1.0f;
    }
    if (t < 0.5f) {
      t = 0.5f;
    }
    yy = (wp->cameraWork.homingOffset2.y / 80.0f) * (pos.y - wp->cameraWork.homingOffset2.y);
    atX = (1.0f - t) * wp->cameraWork.homingOffset2.x;
    ofs = (1.0f - t) * -(1.5f * (w + w2) - 2.0f * wp->cameraWork.zoom);
    zz = -ofs / 10.0f;
    if (yy > 100.0f) {
      yy = 100.0f;
    }
    if (yy < 0.0f) {
      yy = 0.0f;
    }
    y = 20.0 * -(1.0 - t) + yy;
    battleCameraMoveTo(1, 1, atX, 110.0f + y + zz, 750.0f - ofs, atX, 60.0f + y + zz, -ofs, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\121\202\344\202\311\202\301\202\306\202\251\202\361\202\302\202\242\202\321");
    break;
  }
  case 9:
    unit = BattleGetUnitPtr(wp, wp->cameraWork.unitId);
    party = BattleGetUnitPtr(wp, wp->cameraWork.unitId2);
    if (unit == NULL || party == NULL) {
      btl_camera_set_mode(0, 0);
      break;
    }
    BattleGetUnitPartsPtr(wp->cameraWork.unitId, wp->cameraWork.partsId);
    part = BattleGetUnitPartsPtr(wp->cameraWork.unitId2, wp->cameraWork.partsId2);
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    BtlUnit_GetHitPos(party, part, &hit.x, &hit.y, &hit.z);
    w = BtlUnit_GetWidth(unit) - 24.0f;
    BtlUnit_GetWidth(party);
    if (w < 0.0f) {
      w = 0.0f;
    }
    zz = 2.0f * wp->cameraWork.zoom;
    zz -= 1.5f * w;
    yy = 0.5f * (pos.y + hit.y);
    ofs = -zz / 10.0f;
    if (yy > 50.0f) {
      atX = 50.0f;
    }
    if (yy < 0.0f) {
      atX = 0.0f;
    }
    atY = 0.5f * (pos.x + hit.x);
    battleCameraMoveTo(1, 1, atY, 110.0f + atX + ofs, 750.0f - zz, atY, 60.0f + atX + ofs, -zz, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\344\202\311\202\301\202\306\202\302\202\242\202\321");
    break;
  case 10:
    unit = BattleGetUnitPtr(wp, wp->cameraWork.unitId);
    if (unit == NULL) {
      btl_camera_set_mode(0, 0);
      break;
    }
    part = BattleGetUnitPartsPtr(wp->cameraWork.unitId, wp->cameraWork.partsId);
    if (part == NULL) {
      btl_camera_set_mode(0, 0);
      break;
    }
    BtlUnit_GetHitPos(unit, part, &pos.x, &pos.y, &pos.z);
    ofs = (BtlUnit_GetHeight(unit) - 50.0f) / 2.0f;
    pos.y += ofs;
    w = BtlUnit_GetWidth(unit) - 24.0f;
    if (w < 0.0f) {
      w = 0.0f;
    }
    zz = wp->cameraWork.zoom;
    zz -= 1.5f * w;
    ofs = -zz / 9.0f;
    y = pos.y;
    t = y;
    if (y > 100.0f) {
      y = 100.0f;
    }
    if (t < 0.0f) {
      y = 0.0f;
    }
    battleCameraMoveTo(1, 1, pos.x, 110.0f + y + ofs, 750.0f - zz, pos.x, 60.0f + y + ofs, -zz, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\317\201\133\202\302\202\302\202\242\202\321");
    break;
  case 11: {f32 w, w2, t, zz, yy, d, ofs, y, atX;
    unit = BattleGetUnitPtr(wp, wp->cameraWork.unitId);
    unit2 = (0, BattleGetUnitPtr(wp, wp->cameraWork.unitId2)); // FAKE: comma makes this unit2 web a compiler temp coloured early (target r28)
    if (unit == NULL || unit2 == NULL) {
      btl_camera_set_mode(0, 0);
      break;
    }
    party = (BattleUnit*)BattleGetUnitPartsPtr(wp->cameraWork.unitId, wp->cameraWork.partsId); // FAKE: case-11 part held in party (role swap)
    part = BattleGetUnitPartsPtr(wp->cameraWork.unitId2, wp->cameraWork.partsId2);
    if ((BattleUnitPart*)party == NULL || part == NULL) {
      btl_camera_set_mode(0, 0);
      break;
    }
    BtlUnit_GetPartsWorldPos((BattleUnitPart*)party, &pos.x, &pos.y, &pos.z);
    BtlUnit_GetHitPos(unit2, part, &hit.x, &hit.y, &hit.z);
    w = BtlUnit_GetWidth(unit) - 24.0f;
    w2 = BtlUnit_GetWidth(unit2) - 24.0f;
    if (w < 0.0f) {
      w = 0.0f;
    }
    if (w2 < 0.0f) {
      w2 = 0.0f;
    }
    d = wp->cameraWork.homingOffset2.x - wp->cameraWork.homingOffset.x;
    yy = hit.x - pos.x;
    if (d == 0.0f) {
      d = 1e-05f;
    }
    t = yy / d;
    if (t > 1.0f) {
      t = 1.0f;
    }
    if (t < 0.5f) {
      t = 0.5f;
    }
    yy = (wp->cameraWork.homingOffset2.y / 80.0f) * (pos.y - wp->cameraWork.homingOffset2.y);
    atX = (1.0f - t) * wp->cameraWork.homingOffset2.x;
    zz = (1.0f - t) * -(1.5f * (w + w2) - 2.0f * wp->cameraWork.zoom);
    ofs = -zz / 10.0f;
    if (yy > 100.0f) {
      yy = 100.0f;
    }
    if (yy < 0.0f) {
      yy = 0.0f;
    }
    y = 20.0 * -(1.0 - t) + yy;
    battleCameraMoveTo(1, 1, atX, 110.0f + y + ofs, 750.0f - zz, atX, 60.0f + y + ofs, -zz, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\121\202\317\201\133\202\302\202\251\202\361\202\302\202\242\202\321");
    break;
  }
  case 12:
    {
      BattleAudienceMember* m = &aud->members[wp->cameraWork.audienceId];
      battleCameraMoveTo(1, 0, m->position.x, 200.0f, 400.0f + m->position.z, m->position.x, 0.0f, m->position.z, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    }
    strcpy(name, "\202\251\202\337\202\347\201\100\202\251\202\361\202\253\202\341\202\255\202\302\202\242\202\321");
    break;
  case 13:
    unit = BattleGetUnitPtr(wp, wp->cameraWork.unitId);
    if (unit == NULL) {
      btl_camera_set_mode(0, 0);
      break;
    }
    if (BattleGetUnitPartsPtr(wp->cameraWork.unitId, wp->cameraWork.partsId) == NULL) {
      btl_camera_set_mode(0, 0);
      break;
    }
    wp->cameraWork.audienceId = 10;
    aud->mAudienceDeltaMultiplier = aud->mAudienceDeltaMultiplier; // FAKE: no code; keeps aud live into case 13 (target unit r27)
    d = wp->cameraWork.homingOffset2.x - wp->cameraWork.homingOffset.x;
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    ofs = wp->cameraWork.homingOffset2.x - pos.x;
    if (d == 0.0f) {
      t = 0.5f;
    } else {
      t = ofs / d;
    }
    if (t > 1.0f) {
      t = 1.0f;
    }
    if (t < 0.5f) {
      t = 0.5f;
    }
    ofs = pos.y - wp->cameraWork.homingOffset.y;
    d = 2.0f * wp->cameraWork.zoom;
    atX = (1.0f - t) * wp->cameraWork.homingOffset2.x;
    ofs -= 100.0f;
    if (ofs > 0.0f) {
      ofs = 0.0f;
    }
    if (ofs < -30.0f) {
      ofs = -30.0f;
    }
    zz = (1.0f - t) * d;
    y = -(1.0 - t) * (125.0f - ofs);
    battleCameraMoveTo(1, 1, atX, 110.0f + y, 750.0f - zz, atX, 60.0f + y, -zz, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\344\202\311\202\301\202\306\202\251\202\361\202\253\202\341\202\255\202\251\202\361\202\302\202\242\202\321");
    break;
  case 14:
    wp->cameraWork.modeFrame++;
    if (wp->cameraWork.modeFrame == 1) {
      switch (pouchGetPtr()->rank) {
      case 0:
        zoomZ = 750.0f;
        break;
      case 1:
        zoomZ = 830.0f;
        break;
      case 2:
        zoomZ = 920.0f;
        break;
      case 3:
        zoomZ = 1000.0f;
        break;
      }
      wp->cameraWork.moveSpeedLv = 3;
      wp->cameraWork.zoomSpeedLv = 3;
      battleCameraMoveTo(1, 1, 0.0f, 110.0f, zoomZ, 0.0f, 60.0f, 0.0f, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    }
    if (wp->cameraWork.modeFrame == 10) {
      wp->cameraWork.modeFrame = 0;
      btl_camera_set_mode(0, 3);
      btl_camera_set_moveto(0, 0.0f, 110.0f, 750.0f, 0.0f, 60.0f, 0.0f, 120, 4);
    }
    strcpy(name, "\202\251\202\337\202\347\201\100\202\323\202\302\202\244\202\311\202\315\202\242\202\301\202\275");
    break;
  case 15:
    wp->cameraWork.moveSpeedLv = 3;
    wp->cameraWork.zoomSpeedLv = 3;
    battleCameraMoveTo(1, 1, 0.0f, 110.0f, 500.0f, 0.0f, 60.0f, 0.0f, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\244\202\334\202\255\202\315\202\242\202\301\202\275");
    break;
  case 16:
    wp->cameraWork.moveSpeedLv = 3;
    wp->cameraWork.zoomSpeedLv = 3;
    battleCameraMoveTo(1, 1, -100.0f, 110.0f, 500.0f, -100.0f, 60.0f, 0.0f, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\276\202\337\202\311\202\315\202\242\202\301\202\275");
    break;
  case 17:
  case 18:
    mario = BattleGetMarioPtr(wp);
    part = (BattleUnitPart*)BattleGetPartyPtr(wp); // FAKE: party held in part (role swap)
    if ((BattleUnit*)part != NULL) {
      BtlUnit_GetHomePos(mario, &mx, &my, &mz);
      BtlUnit_GetHomePos((BattleUnit*)part, &px, &py, &pz);
      atX = 0.5f * (mx + px);
      atY = 0.5f * (my + py);
      atZ = 0.5f * (mz + pz);
      if (wp->cameraWork.mode == 18) {
        atY -= 20.0f;
      }
    } else {
      BtlUnit_GetHomePos(mario, &mx, &my, &mz);
      atX = mx;
      atY = my;
      atZ = mz;
    }
    wp->cameraWork.moveSpeedLv = 1;
    wp->cameraWork.zoomSpeedLv = 1;
    battleCameraMoveTo(0, 1, atX, 100.0f + atY, 350.0f + atZ, atX, 50.0f + atY, 0.0f, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\265\202\345\202\244\202\350");
    break;
  case 19:
    strcpy(name, "\202\251\202\337\202\347\201\100\202\315\202\242\202\332\202\255");
    break;
  case 20:
    wp->cameraWork.moveSpeedLv = 2;
    wp->cameraWork.zoomSpeedLv = 2;
    battleCameraMoveTo(1, 1, -100.0f, 100.0f, 400.0f, -100.0f, 50.0f, 0.0f, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
    strcpy(name, "\202\251\202\337\202\347\201\100\202\311\202\260\202\304\202\351");
    break;
  default:
    strcpy(name, "\202\251\202\337\202\347\201\100\202\250\202\251\202\265\202\242");
    break;
  }
  if (cam->cameraPos.z >= 1000.0f) {
    cam->cameraPos.z = 1000.0f;
  }
  if (wp->cameraWork.flags & 4) {
    cam->cameraPos.x = wp->cameraWork.prevPos.x;
    cam->cameraAt.x = wp->cameraWork.prevAt.x;
    dX = 0.0f;
    dAtX = 0.0f;
  }
  if (wp->cameraWork.flags & 8) {
    cam->cameraPos.y = wp->cameraWork.prevPos.y;
    cam->cameraAt.y = wp->cameraWork.prevAt.y;
    dY = 0.0f;
    dAtY = 0.0f;
  }
  if (wp->cameraWork.flags & 0x10) {
    cam->cameraPos.z = wp->cameraWork.prevPos.z;
    cam->cameraAt.z = wp->cameraWork.prevAt.z;
    dZ = 0.0f;
    dAtZ = 0.0f;
  }
  wp->cameraWork.pos.x = cam->cameraPos.x;
  wp->cameraWork.pos.y = cam->cameraPos.y;
  wp->cameraWork.pos.z = cam->cameraPos.z;
  wp->cameraWork.at.x = cam->cameraAt.x;
  wp->cameraWork.at.y = cam->cameraAt.y;
  wp->cameraWork.at.z = cam->cameraAt.z;
  wp->cameraWork.moveSpeed = (Vec){dX, dY, dZ};
  wp->cameraWork.moveSpeedAt = (Vec){dAtX, dAtY, dAtZ};
  if (!(wp->cameraWork.flags & 1) && !(wp->cameraWork.flags & 2)) {
    padRumbleOff(0);
  }
  if (wp->cameraWork.flags & 0x40) {
    padRumbleOff(0);
  }
  if (wp->cameraWork.flags & 1) {
    moveLv = wp->cameraWork.moveSpeedLv;
    zoomLv = wp->cameraWork.zoomSpeedLv;
    phase = (wp->cameraWork.shakeWFrames - wp->cameraWork.shakeWTimer) & 2;
    wp->cameraWork.moveSpeedLv = 3;
    wp->cameraWork.zoomSpeedLv = 3;
    wp->cameraWork.shakeWOfs = intplGetValue(wp->cameraWork.shakeWType, (wp->cameraWork.shakeWFrames - wp->cameraWork.shakeWTimer) / 2, wp->cameraWork.shakeWFrames / 2, wp->cameraWork.shakeWAmp[0], wp->cameraWork.shakeWAmp[1]);
    if (wp->cameraWork.shakeWTimer > 0) {
      if (phase == 0) {
        battleCameraMoveTo(0, 0, cam->cameraPos.x + wp->cameraWork.shakeWOfs, cam->cameraPos.y, cam->cameraPos.z, cam->cameraAt.x + wp->cameraWork.shakeWOfs, cam->cameraAt.y, cam->cameraAt.z, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
      } else {
        battleCameraMoveTo(0, 0, cam->cameraPos.x - wp->cameraWork.shakeWOfs, cam->cameraPos.y, cam->cameraPos.z, cam->cameraAt.x - wp->cameraWork.shakeWOfs, cam->cameraAt.y, cam->cameraAt.z, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
      }
    } else if (wp->cameraWork.shakeWTimer == 0) {
      wp->cameraWork.flags &= ~1;
    }
    wp->cameraWork.moveSpeedLv = moveLv;
    wp->cameraWork.zoomSpeedLv = zoomLv;
    wp->cameraWork.shakeWTimer--;
  }
  if (wp->cameraWork.flags & 2) {
    moveLv2 = wp->cameraWork.moveSpeedLv;
    zoomLv2 = wp->cameraWork.zoomSpeedLv;
    phase2 = (wp->cameraWork.shakeHFrames - wp->cameraWork.shakeHTimer) & 2;
    wp->cameraWork.moveSpeedLv = 3;
    wp->cameraWork.zoomSpeedLv = 3;
    wp->cameraWork.shakeHOfs = intplGetValue(wp->cameraWork.shakeHType, (wp->cameraWork.shakeHFrames - wp->cameraWork.shakeHTimer) / 2, wp->cameraWork.shakeHFrames / 2, wp->cameraWork.shakeHAmp[0], wp->cameraWork.shakeHAmp[1]);
    if (wp->cameraWork.shakeHTimer > 0) {
      if (phase2 == 0) {
        battleCameraMoveTo(0, 0, cam->cameraPos.x, cam->cameraPos.y + wp->cameraWork.shakeHOfs, cam->cameraPos.z, cam->cameraAt.x, cam->cameraAt.y + wp->cameraWork.shakeHOfs, cam->cameraAt.z, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
      } else {
        battleCameraMoveTo(0, 0, cam->cameraPos.x, cam->cameraPos.y - wp->cameraWork.shakeHOfs, cam->cameraPos.z, cam->cameraAt.x, cam->cameraAt.y - wp->cameraWork.shakeHOfs, cam->cameraAt.z, &dX, &dY, &dZ, &dAtX, &dAtY, &dAtZ);
      }
    } else if (wp->cameraWork.shakeHTimer == 0) {
      wp->cameraWork.flags &= ~2;
    }
    wp->cameraWork.moveSpeedLv = moveLv2;
    wp->cameraWork.zoomSpeedLv = zoomLv2;
    wp->cameraWork.shakeHTimer--;
  }
}

void battleCameraMoveTo(s32 mode, s32 clamp, f32 x, f32 y, f32 z, f32 atX, f32 atY, f32 atZ,
                        f32* dX, f32* dY, f32* dZ, f32* dAtX, f32* dAtY, f32* dAtZ) {
  CameraEntry* cam = camGetPtr(CAMERA_3D);
  BattleWork* wp = _battleWorkPointer;
  BattleUnit* mario = BattleGetMarioPtr(wp);
  f32 half;

  if (wp->cameraWork.moveSpeedLv < 0 || wp->cameraWork.moveSpeedLv >= 5) {
    wp->cameraWork.moveSpeedLv = 1;
  }
  if (wp->cameraWork.zoomSpeedLv < 0 || wp->cameraWork.zoomSpeedLv >= 6) {
    wp->cameraWork.zoomSpeedLv = 1;
  }
  if (mode == 1) {
    if (wp->flags & 0x100) {
      f32 mz = mario->mHomePosition.z;
      f32 ox = 0.0f;
      f32 oy = 0.25f * -mz;
      f32 oz = mz;
      x += ox;
      y += oy;
      z += oz;
      atX += ox;
      atY += oy;
      atZ += oz;
    }
    x += wp->cameraWork.position.x;
    y += wp->cameraWork.position.y;
    z += wp->cameraWork.position.z;
    atX += wp->cameraWork.position.x;
    atY += wp->cameraWork.position.y;
    atZ += wp->cameraWork.position.z;
  }
  half = (600.0f - z) / 2.0f;
  if (half < 0.0f) {
    half = 0.0f;
  }
  if (clamp == 1) {
    if (x > 100.0f + half) {
      x = 100.0f + half;
    }
    if (x < -100.0f - half) {
      x = -100.0f - half;
    }
    if (atX > 100.0f + half) {
      atX = 100.0f + half;
    }
    if (atX < -100.0f - half) {
      atX = -100.0f - half;
    }
  }
  switch (wp->cameraWork.moveSpeedLv) {
  case 0:
    *dX = (x - cam->cameraPos.x) / 1.5f;
    *dY = (y - cam->cameraPos.y) / 1.5f;
    *dAtX = (atX - cam->cameraAt.x) / 1.5f;
    *dAtY = (atY - cam->cameraAt.y) / 1.5f;
    cam->cameraPos.x += *dX;
    cam->cameraPos.y += *dY;
    cam->cameraAt.x += *dAtX;
    cam->cameraAt.y += *dAtY;
    cam->cameraUp.x = 0.0f;
    cam->cameraUp.y = 1.0f;
    cam->cameraUp.z = 0.0f;
    break;
  case 1:
    *dX = (x - cam->cameraPos.x) / 15.0f;
    *dY = (y - cam->cameraPos.y) / 15.0f;
    *dAtX = (atX - cam->cameraAt.x) / 15.0f;
    *dAtY = (atY - cam->cameraAt.y) / 15.0f;
    cam->cameraPos.x += *dX;
    cam->cameraPos.y += *dY;
    cam->cameraPos.z += *dZ;
    cam->cameraAt.x += *dAtX;
    cam->cameraAt.y += *dAtY;
    cam->cameraUp.x = 0.0f;
    cam->cameraUp.y = 1.0f;
    cam->cameraUp.z = 0.0f;
    break;
  case 2:
    *dX = (x - cam->cameraPos.x) / 5.0f;
    *dY = (y - cam->cameraPos.y) / 5.0f;
    *dAtX = (atX - cam->cameraAt.x) / 5.0f;
    *dAtY = (atY - cam->cameraAt.y) / 5.0f;
    cam->cameraPos.x += *dX;
    cam->cameraPos.y += *dY;
    cam->cameraAt.x += *dAtX;
    cam->cameraAt.y += *dAtY;
    cam->cameraUp.x = 0.0f;
    cam->cameraUp.y = 1.0f;
    cam->cameraUp.z = 0.0f;
    break;
  case 3:
    *dX = x - cam->cameraPos.x;
    *dY = y - cam->cameraPos.y;
    *dAtX = atX - cam->cameraAt.x;
    *dAtY = atY - cam->cameraAt.y;
    cam->cameraPos.x += *dX;
    cam->cameraPos.y += *dY;
    cam->cameraAt.x += *dAtX;
    cam->cameraAt.y += *dAtY;
    cam->cameraUp.x = 0.0f;
    cam->cameraUp.y = 1.0f;
    cam->cameraUp.z = 0.0f;
    break;
  case 4:
    *dX = (x - cam->cameraPos.x) / 200.0f;
    *dY = (y - cam->cameraPos.y) / 200.0f;
    *dAtX = (atX - cam->cameraAt.x) / 200.0f;
    *dAtY = (atY - cam->cameraAt.y) / 200.0f;
    cam->cameraPos.x += *dX;
    cam->cameraPos.y += *dY;
    cam->cameraAt.x += *dAtX;
    cam->cameraAt.y += *dAtY;
    cam->cameraUp.x = 0.0f;
    cam->cameraUp.y = 1.0f;
    cam->cameraUp.z = 0.0f;
    break;
  case 5:
    *dX = (x - cam->cameraPos.x) / 100.0f;
    *dY = (y - cam->cameraPos.y) / 100.0f;
    *dAtX = (atX - cam->cameraAt.x) / 100.0f;
    *dAtY = (atY - cam->cameraAt.y) / 100.0f;
    cam->cameraPos.x += *dX;
    cam->cameraPos.y += *dY;
    cam->cameraAt.x += *dAtX;
    cam->cameraAt.y += *dAtY;
    cam->cameraUp.x = 0.0f;
    cam->cameraUp.y = 1.0f;
    cam->cameraUp.z = 0.0f;
    break;
  }
  switch (wp->cameraWork.zoomSpeedLv) {
  case 0:
    *dZ = (z - cam->cameraPos.z) / 1.5f;
    *dAtZ = (atZ - cam->cameraAt.z) / 1.5f;
    cam->cameraPos.z += *dZ;
    cam->cameraAt.z += *dAtZ;
    break;
  case 1:
    *dZ = (z - cam->cameraPos.z) / 15.0f;
    *dAtZ = (atZ - cam->cameraAt.z) / 15.0f;
    cam->cameraPos.z += *dZ;
    cam->cameraAt.z += *dAtZ;
    break;
  case 2:
    *dZ = (z - cam->cameraPos.z) / 5.0f;
    *dAtZ = (atZ - cam->cameraAt.z) / 5.0f;
    cam->cameraPos.z += *dZ;
    cam->cameraAt.z += *dAtZ;
    break;
  case 3:
    *dZ = z - cam->cameraPos.z;
    *dAtZ = atZ - cam->cameraAt.z;
    cam->cameraPos.z += *dZ;
    cam->cameraAt.z += *dAtZ;
    break;
  case 4:
    *dZ = (z - cam->cameraPos.z) / 200.0f;
    *dAtZ = (atZ - cam->cameraAt.z) / 200.0f;
    cam->cameraPos.z += *dZ;
    cam->cameraAt.z += *dAtZ;
    break;
  case 5:
    *dZ = (z - cam->cameraPos.z) / 100.0f;
    *dAtZ = (atZ - cam->cameraAt.z) / 100.0f;
    cam->cameraPos.z += *dZ;
    cam->cameraAt.z += *dAtZ;
    break;
  }
}

f32 battleCameraGetPosMoveSpeed(void) {
  BattleWork* wp = _battleWorkPointer;
  f32 x = wp->cameraWork.moveSpeed.x;
  f32 y = wp->cameraWork.moveSpeed.y;
  f32 z = wp->cameraWork.moveSpeed.z;
  return sqrt(x * x + y * y + z * z);
}

void btl_camera_set_prilimit(s32 pri) {
  _battleWorkPointer->cameraWork.priLimit = pri;
}

USER_FUNC(evt_btl_camera_set_prilimit) {
  btl_camera_set_prilimit(evtGetValue(event, event->args[0]));
  return 2;
}

void btl_camera_set_mode(s32 pri, s32 mode) {
  BattleWork* wp = _battleWorkPointer;
  if (pri >= wp->cameraWork.priLimit) {
    wp->cameraWork.mode = mode;
    wp->cameraWork.zoom = 0.0f;
    wp->cameraWork.moveSpeedLv = 1;
    wp->cameraWork.zoomSpeedLv = 1;
    wp->cameraWork.modeFrame = 0;
    btl_camera_nomove_x_onoff(0, 0);
    btl_camera_nomove_y_onoff(0, 0);
    btl_camera_nomove_z_onoff(0, 0);
    wp->cameraWork.flags &= ~0x20;
  }
}

USER_FUNC(evt_btl_camera_set_mode) {
  EvtWord* args = event->args;
  s32 pri = evtGetValue(event, args[0]);
  s32 val = evtGetValue(event, args[1]);
  btl_camera_set_mode(pri, val);
  return 2;
}

USER_FUNC(evt_btl_camera_set_homing_unit) {
  BattleWork* wp;
  BattleUnit* u;
  BattleUnit* u2;
  EvtWord* args = event->args;
  s32 unit;
  s32 unit2;
  BattleUnitPart* part;
  Vec pos;
  Vec hit;

  evtGetValue(event, args[0]);
  unit = evtGetValue(event, args[1]);
  unit2 = evtGetValue(event, args[2]);
  wp = _battleWorkPointer;
  wp->cameraWork.unitId = BattleTransID(event, unit);
  u = BattleGetUnitPtr(wp, wp->cameraWork.unitId);
  wp->cameraWork.partsId = BtlUnit_GetBodyPartsId(u);
  BattleGetUnitPartsPtr(wp->cameraWork.unitId, wp->cameraWork.partsId);
  BtlUnit_GetHomePos(u, &pos.x, &pos.y, &pos.z);
  wp->cameraWork.homingOffset = (Vec){pos.x, pos.y, pos.z};
  if (unit2 != -1) {
    wp->cameraWork.unitId2 = BattleTransID(event, unit2);
    u2 = BattleGetUnitPtr(wp, wp->cameraWork.unitId2);
    wp->cameraWork.partsId2 = BtlUnit_GetBodyPartsId(u2);
    part = BattleGetUnitPartsPtr(wp->cameraWork.unitId2, wp->cameraWork.partsId2);
    BtlUnit_GetHitPos(u2, part, &hit.x, &hit.y, &hit.z);
    wp->cameraWork.homingOffset2 = (Vec){hit.x, hit.y, hit.z};
  }
  return 2;
}

USER_FUNC(evt_btl_camera_set_homing_unitparts) {
  EvtWord* args = event->args;
  BattleWork* wp;
  BattleUnit* u;
  s32 unit, parts, unit2, parts2;
  BattleUnitPart* part;
  Vec pos;
  Vec hit;

  /* pri shares a register with u in the original */
  u = (BattleUnit*)evtGetValue(event, args[0]); // FAKE: u holds the priority word first (one register in the target)
  unit = evtGetValue(event, args[1]);
  parts = evtGetValue(event, args[2]);
  unit2 = evtGetValue(event, args[3]);
  parts2 = evtGetValue(event, args[4]);
  if ((EvtWord)u < (wp = _battleWorkPointer)->cameraWork.priLimit) {
    return 2;
  }
  wp->cameraWork.unitId = BattleTransID(event, unit);
  wp->cameraWork.partsId = parts;
  u = BattleGetUnitPtr(wp, wp->cameraWork.unitId);
  BattleGetUnitPartsPtr(wp->cameraWork.unitId, wp->cameraWork.partsId);
  BtlUnit_GetHomePos(u, &pos.x, &pos.y, &pos.z);
  wp->cameraWork.homingOffset = (Vec){pos.x, pos.y, pos.z};
  if (unit2 != -1) {
    wp->cameraWork.unitId2 = BattleTransID(event, unit2);
    wp->cameraWork.partsId2 = parts2;
    u = BattleGetUnitPtr(wp, wp->cameraWork.unitId2);
    part = BattleGetUnitPartsPtr(wp->cameraWork.unitId2, wp->cameraWork.partsId2);
    BtlUnit_GetHitPos(u, part, &hit.x, &hit.y, &hit.z);
    wp->cameraWork.homingOffset2 = (Vec){hit.x, hit.y, hit.z};
  }
  return 2;
}

USER_FUNC(evt_btl_camera_set_homing_unit_audience) {
  s32 audId;
  BattleWork* wp;
  BattleAudience* aud;
  EvtWord* args = event->args;
  s32 pri, unit, parts;
  BattleUnit* u;
  BattleUnitPart* part;
  Vec pos;
  Vec hit;

  pri = evtGetValue(event, args[0]);
  unit = evtGetValue(event, args[1]);
  parts = evtGetValue(event, args[2]);
  audId = evtGetValue(event, args[3]);
  wp = _battleWorkPointer;
  aud = BattleAudienceBaseGetPtr();
  if (pri < wp->cameraWork.priLimit) {
    return 2;
  }
  wp->cameraWork.unitId = BattleTransID(event, unit);
  wp->cameraWork.partsId = parts;
  u = BattleGetUnitPtr(wp, wp->cameraWork.unitId);
  part = BattleGetUnitPartsPtr(wp->cameraWork.unitId, wp->cameraWork.partsId);
  BtlUnit_GetPos(u, &pos.x, &pos.y, &pos.z);
  BtlUnit_GetHitPos(u, part, &hit.x, &hit.y, &hit.z);
  wp->cameraWork.homingOffset = (Vec){pos.x, pos.y, pos.z};
  wp->cameraWork.audienceId = audId;
  {
    BattleAudienceMember* m = &aud->members[audId];
    wp->cameraWork.homingOffset2 = (Vec){m->position.x, m->position.y, m->position.z};
  }
  return 2;
}

void btl_camera_set_zoom(s32 pri, f32 zoom) {
  BattleWork* wp = _battleWorkPointer;
  if (pri >= wp->cameraWork.priLimit) {
    wp->cameraWork.zoom = zoom;
  }
}

void btl_camera_add_zoom(s32 pri, f32 zoom) {
  BattleWork* wp = _battleWorkPointer;
  if (pri >= wp->cameraWork.priLimit) {
    wp->cameraWork.zoom += zoom;
  }
}

USER_FUNC(evt_btl_camera_set_zoom) {
  EvtWord* args = event->args;
  s32 pri = evtGetValue(event, args[0]);
  btl_camera_set_zoom(pri, (s32)evtGetFloat(event, args[1]));
  return 2;
}

USER_FUNC(evt_btl_camera_add_zoom) {
  EvtWord* args = event->args;
  s32 pri = evtGetValue(event, args[0]);
  btl_camera_add_zoom(pri, (s32)evtGetFloat(event, args[1]));
  return 2;
}

void btl_camera_set_moveSpeedLv(s32 pri, s32 lv) {
  BattleWork* wp = _battleWorkPointer;
  if (pri >= wp->cameraWork.priLimit) {
    if (lv >= 0 && lv <= 5) {
      wp->cameraWork.moveSpeedLv = lv;
    } else {
      wp->cameraWork.moveSpeedLv = 1;
    }
    wp->cameraWork.zoomSpeedLv = wp->cameraWork.moveSpeedLv;
  }
}

void btl_camera_set_zoomSpeedLv(s32 pri, s32 lv) {
  BattleWork* wp = _battleWorkPointer;
  if (pri >= wp->cameraWork.priLimit) {
    if (lv >= 0 && lv <= 5) {
      wp->cameraWork.zoomSpeedLv = lv;
    } else {
      wp->cameraWork.zoomSpeedLv = 1;
    }
  }
}

USER_FUNC(evt_btl_camera_set_moveSpeedLv) {
  EvtWord* args = event->args;
  s32 pri = evtGetValue(event, args[0]);
  s32 val = evtGetValue(event, args[1]);
  btl_camera_set_moveSpeedLv(pri, val);
  return 2;
}

USER_FUNC(evt_btl_camera_set_zoomSpeedLv) {
  EvtWord* args = event->args;
  s32 pri = evtGetValue(event, args[0]);
  s32 val = evtGetValue(event, args[1]);
  btl_camera_set_zoomSpeedLv(pri, val);
  return 2;
}

void btl_camera_set_moveto(s32 pri, f32 x, f32 y, f32 z, f32 atX, f32 atY, f32 atZ, s32 frames,
                           s32 type) {
  BattleWork* wp = _battleWorkPointer;
  CameraEntry* cam = camGetPtr(CAMERA_3D);
  if (pri >= wp->cameraWork.priLimit) {
    wp->cameraWork.moveToStartPos.x = cam->cameraPos.x;
    wp->cameraWork.moveToStartPos.y = cam->cameraPos.y;
    wp->cameraWork.moveToStartPos.z = cam->cameraPos.z;
    wp->cameraWork.moveToEndPos.x = x;
    wp->cameraWork.moveToEndPos.y = y;
    wp->cameraWork.moveToEndPos.z = z;
    wp->cameraWork.moveToStartAt.x = cam->cameraAt.x;
    wp->cameraWork.moveToStartAt.y = cam->cameraAt.y;
    wp->cameraWork.moveToStartAt.z = cam->cameraAt.z;
    wp->cameraWork.moveToEndAt.x = atX;
    wp->cameraWork.moveToEndAt.y = atY;
    wp->cameraWork.moveToEndAt.z = atZ;
    wp->cameraWork.moveToTimer = frames;
    wp->cameraWork.moveToFrames = frames;
    wp->cameraWork.moveToType = type;
  }
}

USER_FUNC(evt_btl_camera_set_moveto) {
  EvtWord* args = event->args;
  s32 pri = evtGetValue(event, args[0]);
  f32 x = evtGetFloat(event, args[1]);
  f32 y = evtGetFloat(event, args[2]);
  f32 z = evtGetFloat(event, args[3]);
  f32 atX = evtGetFloat(event, args[4]);
  f32 atY = evtGetFloat(event, args[5]);
  f32 atZ = evtGetFloat(event, args[6]);
  s32 frames = evtGetValue(event, args[7]);
  btl_camera_set_moveto(pri, x, y, z, atX, atY, atZ, frames, evtGetValue(event, args[8]));
  return 2;
}

void btl_camera_shake_w(s32 pri, f32 a, f32 b, s32 frames, s32 type) {
  BattleWork* wp = _battleWorkPointer;
  if (pri >= wp->cameraWork.priLimit) {
    wp->cameraWork.flags |= 1;
    wp->cameraWork.shakeWAmp[0] = a;
    wp->cameraWork.shakeWAmp[1] = b;
    wp->cameraWork.shakeWType = type;
    wp->cameraWork.shakeWFrames = frames;
    wp->cameraWork.shakeWTimer = frames;
    if (!(wp->cameraWork.flags & 0x40)) {
      padRumbleOn(0);
    }
  }
}

USER_FUNC(evt_btl_camera_shake_w) {
  EvtWord* args = event->args;
  s32 pri = evtGetValue(event, args[0]);
  f32 a = evtGetValue(event, args[1]);
  f32 b = evtGetValue(event, args[2]);
  s32 frames = evtGetValue(event, args[3]);
  btl_camera_shake_w(pri, a, b, frames, evtGetValue(event, args[4]));
  return 2;
}

void btl_camera_shake_h(s32 pri, f32 a, f32 b, s32 frames, s32 type) {
  BattleWork* wp = _battleWorkPointer;
  if (pri >= wp->cameraWork.priLimit) {
    wp->cameraWork.flags |= 2;
    wp->cameraWork.shakeHAmp[0] = a;
    wp->cameraWork.shakeHAmp[1] = b;
    wp->cameraWork.shakeHType = type;
    wp->cameraWork.shakeHFrames = frames;
    wp->cameraWork.shakeHTimer = frames;
    if (!(wp->cameraWork.flags & 0x40)) {
      padRumbleOn(0);
    }
  }
}

USER_FUNC(evt_btl_camera_shake_h) {
  EvtWord* args = event->args;
  s32 pri = evtGetValue(event, args[0]);
  f32 a = evtGetValue(event, args[1]);
  f32 b = evtGetValue(event, args[2]);
  s32 frames = evtGetValue(event, args[3]);
  btl_camera_shake_h(pri, a, b, frames, evtGetValue(event, args[4]));
  return 2;
}

void btl_camera_noshake(s32 pri) {
  BattleWork* wp = _battleWorkPointer;
  if (pri >= wp->cameraWork.priLimit) {
    wp->cameraWork.flags &= ~1;
    wp->cameraWork.flags &= ~2;
  }
}

USER_FUNC(evt_btl_camera_noshake) {
  btl_camera_noshake(evtGetValue(event, event->args[0]));
  return 2;
}

void btl_camera_nomove_x_onoff(s32 pri, s32 on) {
  BattleWork* wp = _battleWorkPointer;
  if (pri >= wp->cameraWork.priLimit) {
    if (on == 1) {
      wp->cameraWork.flags |= 4;
    } else {
      wp->cameraWork.flags &= ~4;
    }
  }
}

USER_FUNC(evt_btl_camera_nomove_x_onoff) {
  EvtWord* args = event->args;
  s32 pri = evtGetValue(event, args[0]);
  s32 val = evtGetValue(event, args[1]);
  btl_camera_nomove_x_onoff(pri, val);
  return 2;
}

void btl_camera_nomove_y_onoff(s32 pri, s32 on) {
  BattleWork* wp = _battleWorkPointer;
  if (pri >= wp->cameraWork.priLimit) {
    if (on == 1) {
      wp->cameraWork.flags |= 8;
    } else {
      wp->cameraWork.flags &= ~8;
    }
  }
}

USER_FUNC(evt_btl_camera_nomove_y_onoff) {
  EvtWord* args = event->args;
  s32 pri = evtGetValue(event, args[0]);
  s32 val = evtGetValue(event, args[1]);
  btl_camera_nomove_y_onoff(pri, val);
  return 2;
}

void btl_camera_nomove_z_onoff(s32 pri, s32 on) {
  BattleWork* wp = _battleWorkPointer;
  if (pri >= wp->cameraWork.priLimit) {
    if (on == 1) {
      wp->cameraWork.flags |= 0x10;
    } else {
      wp->cameraWork.flags &= ~0x10;
    }
  }
}

USER_FUNC(evt_btl_camera_nomove_z_onoff) {
  EvtWord* args = event->args;
  s32 pri = evtGetValue(event, args[0]);
  s32 val = evtGetValue(event, args[1]);
  btl_camera_nomove_z_onoff(pri, val);
  return 2;
}

void btl_camera_set_posoffset(s32 pri, f32 x, f32 y, f32 z) {
  BattleWork* wp = _battleWorkPointer;
  if (pri >= wp->cameraWork.priLimit) {
    wp->cameraWork.flags |= 0x20;
    wp->cameraWork.position.x = x;
    wp->cameraWork.position.y = y;
    wp->cameraWork.position.z = z;
  }
}

USER_FUNC(evt_btl_camera_set_posoffset) {
  EvtWord* args = event->args;
  s32 pri = evtGetValue(event, args[0]);
  f32 x = evtGetFloat(event, args[1]);
  f32 y = evtGetFloat(event, args[2]);
  f32 z = evtGetFloat(event, args[3]);
  btl_camera_set_posoffset(pri, x, y, z);
  return 2;
}

void btl_camera_off_posoffset_manual(s32 pri) {
  BattleWork* wp = _battleWorkPointer;
  if (pri >= wp->cameraWork.priLimit) {
    wp->cameraWork.flags &= ~0x20;
    wp->cameraWork.position.x = 0.0f;
    wp->cameraWork.position.y = 0.0f;
    wp->cameraWork.position.z = 0.0f;
  }
}

USER_FUNC(evt_btl_camera_off_posoffset_manual) {
  btl_camera_off_posoffset_manual(evtGetValue(event, event->args[0]));
  return 2;
}

USER_FUNC(evt_btl_camera_wait_move_end) {
  if (battleCameraGetPosMoveSpeed() >= 2.0f) {
    return 0;
  }
  return 2;
}

