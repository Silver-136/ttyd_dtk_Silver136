// Gundan zako (the X-Naut horde in the chapter 5 boss fight): the shared work of
// unit_boss_gundan_zako_group1.c (owner of `zakogun` and `work`), group2 and group3.
#pragma once

#include "version.h"
#include "evt/evt_cmd.h"
#include <dolphin/mtx.h>

// One X-Naut of the horde
typedef struct ZakoGun {
    s8 field_0x0;
    s8 state;
    s32 timer;
    s32 timerMax;
    Vec pos;
    Vec home;
    s32 poseBase;
    s32 poseNum;
    s32 poseDiv;
    s32 poseCount;
    Vec start;
    Vec target;
    Vec vel;
    Vec ofs;
    Vec rot;
} ZakoGun;

// The horde's shared work (`work`, owned by group1). Field names with an offset give the JP offset; US and
// later insert targetPos, so everything after it sits 12 bytes further.
typedef struct ZakoGunWork {
    s32 flag;
    s32 targetId;
#if VERSION >= VERSION_US
    Vec targetPos; // last known target position (zakogun_ext_main2), used when the target unit is gone
#endif
    s32 state;
    s32 timer;
    Vec pos;
    u8 field_0x1c[0x34 - 0x1c];
    f32 radius;
    f32 angle;
    f32 speed;
    u8 field_0x40[0x48 - 0x40];
    Vec start;
    Vec end;
    s32 timer2; // frames left of the ceiling move from start to end (down_ceil, up_ceil)
    s32 steps;
    s32 count;
    s32 sfx;
    ZakoGun* zako;
} ZakoGunWork;

USER_FUNC(zakogun_escape3);
USER_FUNC(zakogun_ext_init2);
USER_FUNC(zakogun_get_kanbu_id_evtcmd);
