// Gundan zako (the X-Naut horde in the chapter 5 boss fight): the shared work of
// unit_boss_gundan_zako_group1.c (owner of `zakogun` and `work`), group2 and group3.
#pragma once

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

USER_FUNC(zakogun_escape3);
USER_FUNC(zakogun_ext_init2);
USER_FUNC(zakogun_get_kanbu_id_evtcmd);
