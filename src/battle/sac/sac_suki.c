#include "eff/n64/eff_crystal_n64.h"
#include "eff/eff_starstone.h"
#include "battle/battle.h"
#include "battle/battle_audience.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/icondrv.h"
#include "driver/mapdrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <string.h>
#include "icon_ids.h"
#include "battle/sac/sac_common.h"
#include "battle/sac/sac_suki.h"
#include "battle/battle_database_common.h"
#include "pmario_sound.h"
#include <dolphin/pad.h>
#include "eff/n64/eff_stardust_n64.h"

void btlGetScreenPoint(Vec* in, Vec* out);

typedef struct SukiBtn {
    s32 state;  // 0x0
    s32 unk4;   // 0x4
    s32 button; // 0x8
    Vec pos;    // 0xC
} SukiBtn;

typedef struct SukiWork {
    s32 state;          // 0x0
    s32 timer;          // 0x4
    s32 unitId;         // 0x8
    s32 round;          // 0xC
    s32 cur;            // 0x10
    s32 prev;           // 0x14
    s32 need;           // 0x18
    s32 time;           // 0x1C
    u8 flash;           // 0x20
    u8 pad21[3];
    f32 offsetY;        // 0x24
    Vec starPos[6];     // 0x28
    char names[6][16];  // 0x70
    Vec pos;            // 0xD0
    SukiBtn btn[7];     // 0xDC
    SukiBtn btn2[7];    // 0x184
    s32 starState;      // 0x22C
    s32 starTimer;      // 0x230
    Vec spos;           // 0x234
    Vec from;           // 0x240
    Vec to;             // 0x24C
    u8 pad258[0xC];
    Vec scale;          // 0x264
    Vec rot;            // 0x270
    EffectEntry* eff;   // 0x27C
    s32 effMode;        // 0x280
    u8 weapon[0xC0];    // 0x284
    u8 pad344[0x4C8 - 0x344];
} SukiWork;

typedef struct StarEffWork {
    u8 pad0[8];
    Vec pos;  // 0x8
    f32 scl;  // 0x14
    Vec rot;  // 0x18
    u8 pad24[0x38 - 0x24];
    s32 mode; // 0x38
} StarEffWork;

USER_FUNC(btlevtcmd_ACSuccessEffect);
USER_FUNC(btlevtcmd_ChoiceSamplingEnemy);
USER_FUNC(btlevtcmd_GetEnemyBelong);
USER_FUNC(btlevtcmd_GetSelectEnemy);
USER_FUNC(btlevtcmd_GetSelectNextEnemy);
USER_FUNC(btlevtcmd_OffPartsAttribute);
USER_FUNC(btlevtcmd_OnPartsAttribute);
USER_FUNC(btlevtcmd_SamplingEnemy);
USER_FUNC(btlevtcmd_SetAlpha);
USER_FUNC(btlevtcmd_SetRGB);
USER_FUNC(btlevtcmd_AnimeChangePose);
USER_FUNC(btlevtcmd_AnimeChangePoseType);
USER_FUNC(btlevtcmd_AttackDeclare);
USER_FUNC(btlevtcmd_CheckDamage);
USER_FUNC(btlevtcmd_CheckPartsAttribute);
USER_FUNC(btlevtcmd_CheckStatus);
USER_FUNC(btlevtcmd_CommandGetWeaponAddress);
USER_FUNC(btlevtcmd_CommandPayWeaponCost);
USER_FUNC(btlevtcmd_GetBodyId);
USER_FUNC(btlevtcmd_GetUnitId);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(btlevtcmd_WaitGuardMove);
USER_FUNC(btlevtcmd_WeaponAftereffect);
USER_FUNC(btlevtcmd_reset_move_color_lv_all);
USER_FUNC(btlevtcmd_snd_se);
USER_FUNC(evt_batstage_return_aud_dark_base);
USER_FUNC(evt_batstage_return_stg_dark_base);
USER_FUNC(evt_batstage_set_aud_dark);
USER_FUNC(evt_batstage_set_stg_dark);
USER_FUNC(evt_map_set_tevcallback);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_btl_camera_set_mode);
USER_FUNC(evt_btl_camera_set_moveSpeedLv);
USER_FUNC(evt_btl_camera_shake_h);
USER_FUNC(evt_eff);
USER_FUNC(evt_img_alloc_capture);
USER_FUNC(evt_img_clear_virtual_point);
USER_FUNC(evt_img_entry);
USER_FUNC(evt_img_onoff);
USER_FUNC(evt_img_release);
USER_FUNC(evt_img_set_paper);
USER_FUNC(evt_img_set_paper_anim);
USER_FUNC(evt_paper_delete);
USER_FUNC(evt_paper_entry);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(sac_ac_help_off);
USER_FUNC(sac_ac_help_on);
USER_FUNC(sac_cheer);
USER_FUNC(sac_cheer_end);
extern s32 tou_gamen_screen_tev[];

USER_FUNC(aud_set_draw);
USER_FUNC(exist_map);
USER_FUNC(flash_update);
USER_FUNC(get_img_name);
USER_FUNC(eff_crystal);
USER_FUNC(get_screen_point);
USER_FUNC(wait_star_stone_attack_end);
USER_FUNC(star_stone_get_attack_point);
USER_FUNC(wait_star_stone_up_end);
USER_FUNC(end_suki);
USER_FUNC(main_suki);
void set_tev(MapTevCallbackInfo* info);
static USER_FUNC(set_weapon);
static USER_FUNC(star_stone_attack);
static USER_FUNC(star_stone_appear);
static USER_FUNC(wait_game_end);
static USER_FUNC(start_game);
static void disp_3D_alpha(CameraId cam, void* param);
static void disp_3D(CameraId cam, void* param);
static void disp_2D(CameraId cam, void* param);
static SukiWork* get_ptr(void);
static void main_star(void);
static void main_base(void);
void main_button(s32 idx);
void N__disp_2D(s32 side, s32 idx);

static BattleWeapon weapon = {
    "btl_wn_sac_sukkari_sukkiri", // name
    ICON_199, // icon
    {0}, // pad_06
    0, // itemId
    "msg_sukkari_sukkiri", // description
    100, // baseAccuracy
    0, // baseFpCost
    2, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x2101260, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_7", // acHelpMsg
    0x20008, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, // atkChangeTime..allergicTime
    100, // ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Suki, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static EVT_BEGIN(nice_event)
    CALL(btlevtcmd_ACSuccessEffect, LW(0), -120, 120, 0)
    RETURN
EVT_END()

#define INTPL(mode, cur, steps, a, b) ((f32(*)(f32, f32, s32, s32, s32))intplGetValue)(a, b, mode, cur, steps)

USER_FUNC(main_suki) {
    SukiWork* w = get_ptr();
    EvtWord* args = event->args;
    f32 posTbl[18] = {
        95.0f, 35.0f, -150.0f, 180.0f, 80.0f, -150.0f, 95.0f, 110.0f, -150.0f,
        10.0f, 80.0f, -150.0f, 30.0f, -10.0f, -150.0f, 160.0f, -10.0f, -150.0f,
    };
    s32 i;

    if (isFirstCall) {
        memset(w, 0, 0x4C8);
        w->unitId = evtGetValue(event, args[0]);
        memcpy(w->starPos, posTbl, sizeof(posTbl));
        strcpy(w->names[0], "sac_suki_img0");
        strcpy(w->names[1], "sac_suki_img1");
        strcpy(w->names[2], "sac_suki_img2");
        strcpy(w->names[3], "sac_suki_img3");
        strcpy(w->names[4], "sac_suki_img4");
        strcpy(w->names[5], "sac_suki_img5");
        w->pos = (Vec){0.0f, -1000.0f, 0.0f};
    }
    main_base();
    for (i = 0; i < 7; i++) {
        main_button(i);
    }
    main_star();
    dispEntry(CAMERA_2D, 901.0f, 1, disp_2D, 0);
    dispEntry(CAMERA_3D, 0.0f, 1, disp_3D, 0);
    dispEntry(CAMERA_3D, 0.0f, 2, disp_3D_alpha, 0);
    return 0;
}

static void main_base(void) {
    SukiWork* w = get_ptr();
    s32 num = BattleAudience_GetAudienceNum();
    s32 i;

    switch (w->state) {
    case 1:
        w->state = 2;
        w->timer = 0;
    case 2:
        if (w->round == 0) {
            Vec v = {0.0f, -45.0f, 0.0f};
            v.x = INTPL(4, w->timer, 15, -1000.0f, 0.0f);
            w->pos = v;
        }
        if (w->timer == 0) {
            if (w->starState != 4) {
                break;
            }
            memcpy(w->btn2, w->btn, sizeof(w->btn));
            w->prev = w->cur;
            if (w->round < 5) {
                w->cur = 0;
                w->need = w->round + 3;
                w->time = w->round * 10 + 180;
                for (i = 0; i < 7; i++) {
                    SukiBtn* bt = &w->btn[i];
                    if (i < w->need) {
                        bt->state = 1;
                    } else {
                        bt->state = 0;
                    }
                }
            } else {
                for (i = 0; i < 7; i++) {
                    w->btn[i].state = 0;
                }
            }
        }
        w->offsetY = INTPL(0, w->timer, 15, -80.0f, 0.0f);
        w->timer++;
        if (w->timer >= 15) {
            if (w->round >= 5) {
                w->state = 5;
                w->timer = 0;
                w->starState = 8;
            } else {
                w->state = 4;
            }
            w->pos = (Vec){0.0f, -45.0f, 0.0f};
        }
        break;
    case 3:
        if (w->timer == 0) {
            if (w->starState != 4) {
                break;
            }
            psndSFXOn("SFX_BTL_SAC_SUKKIRI2");
            memcpy(w->btn2, w->btn, sizeof(w->btn));
            w->prev = w->cur;
            for (i = 0; i < 7; i++) {
                w->btn[i].state = 0;
            }
        }
        w->offsetY = INTPL(0, w->timer, 15, -60.0f, 0.0f);
        w->timer++;
        if (w->timer >= 15) {
            w->state = 5;
            w->timer = 0;
            w->starState = 8;
        }
        break;
    case 4:
        w->time--;
        if (w->time <= 0) {
            w->state = 3;
            w->timer = 0;
            break;
        }
        if (keyGetButtonTrg(0) & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X | PAD_BUTTON_Y)) {
            u32 trg = keyGetButtonTrg(0);
            if (w->btn[w->cur].button & trg) {
                EventEntry* e;
                s32 n;
                w->cur++;
                psndSFXOn("SFX_BTL_SAC_SUKKIRI1");
                if (w->cur < w->need) {
                    break;
                }
                w->state = 2;
                w->timer = 0;
                w->round++;
                w->starState = 5;
                e = evtEntry(nice_event, 0, EVENT_START_IMMEDIATE);
                n = w->round + 1;
                e->lwData[0] = n < 6 ? n : 6;
                if (num >= 1 && num <= 49) {
                    BattleAudienceSoundCallKind(1);
                }
                if (num >= 50 && num <= 99) {
                    BattleAudienceSoundCallKind(1);
                    BattleAudienceSoundWhistleKind(1);
                }
                if (num >= 100 && num <= 149) {
                    BattleAudienceSoundCallKind(2);
                    BattleAudienceSoundWhistleKind(2);
                }
                if (num >= 150) {
                    BattleAudienceSoundCallKind(2);
                    BattleAudienceSoundWhistleKind(3);
                }
            } else {
                w->state = 3;
                w->timer = 0;
            }
        }
        break;
    case 5:
        w->timer++;
        if (w->timer <= 30) {
            Vec v = {0.0f, -45.0f, 0.0f};
            v.x = INTPL(4, w->timer, 30, 0.0f, -1000.0f);
            w->pos = v;
        }
        break;
    }
}

#pragma dont_inline on
void main_button(s32 idx) {
    SukiWork* w = get_ptr();
    SukiBtn* b = &w->btn[idx];
    s32 tbl[4] = {0x100, 0x200, 0x400, 0x800};

    switch (b->state) {
    case 0:
        break;
    case 2:
        break;
    case 1:
        b->state = 2;
        {
            Vec v = {0.0f, -40.0f, 0.0f};
            v.x = 60.0f * (f32)idx - 30.0f * (f32)(w->need - 1);
            b->pos = v;
        }
        b->button = tbl[irand(4)];
        break;
    }
}
#pragma dont_inline reset

static void main_star(void) {
    SukiWork* w = get_ptr();
    BattleUnit* mario = BattleGetMarioPtr(_battleWorkPointer);

    switch (w->starState) {
    case 1:
        w->starState = 2;
        w->starTimer = 0;
        w->eff = effStarStoneEntry(5, 0.0f, -1000.0f, 0.0f, 1.0f);
        BtlUnit_GetPos(mario, &w->spos.x, &w->spos.y, &w->spos.z);
        w->spos.y += mario->mSizeMultiplier * (f32)mario->mHeight + 37.0f;
        w->from = w->spos;
        w->to = w->spos;
        w->to.y += 50.0f;
        w->to.z += -1.0f;
        w->scale = (Vec){0.0f, 0.0f, 0.0f};
        w->rot = (Vec){0.0f, 0.0f, 0.0f};
        w->effMode = 4;
    case 2:
        w->starTimer++;
        if (w->starTimer <= 100) {
            w->spos.x = INTPL(0, w->starTimer, 100, w->from.x, w->to.x);
            w->spos.y = INTPL(0, w->starTimer, 100, w->from.y, w->to.y);
            w->spos.z = INTPL(0, w->starTimer, 100, w->from.z, w->to.z);
            w->scale.x = w->scale.y = w->scale.z = INTPL(0, w->starTimer, 100, 0.0f, 2.0f);
        } else {
            w->spos.x = w->to.x;
            w->spos.y = w->to.y;
            w->spos.z = w->to.z;
            w->scale.x = w->scale.y = w->scale.z = 2.0f;
        }
        w->rot.y = INTPL(4, w->starTimer, 120, 0.0f, 2160.0f);
        if (w->starTimer >= 120) {
            w->starState = 3;
            w->starTimer = 0;
            w->from = w->spos;
            w->to.y = 300.0f;
        }
        break;
    case 3:
        w->starTimer++;
        w->spos.y = INTPL(1, w->starTimer, 60, w->from.y, w->to.y);
        if (w->starTimer >= 60) {
            w->starState = 4;
            w->starTimer = 0;
            w->spos = (Vec){0.0f, 300.0f, 0.0f};
            w->scale = (Vec){2.0f, 2.0f, 2.0f};
            w->rot = (Vec){0.0f, 0.0f, 0.0f};
        }
        break;
    case 5:
        w->starState = 6;
        w->starTimer = 0;
        BtlUnit_GetPos(mario, &w->spos.x, &w->spos.y, &w->spos.z);
        w->spos.y += 100.0f;
        w->scale = (Vec){1.5f, 1.5f, 1.5f};
        w->rot = (Vec){0.0f, 0.0f, 0.0f};
        w->from.y = w->spos.y;
        w->to.y = 300.0f;
        effStardustN64Entry(0, w->spos.x, w->spos.y, w->spos.z, 32.0f);
        break;
    case 6:
        w->starTimer++;
        w->rot.y = INTPL(0, w->starTimer, 40, 0.0f, 1440.0f);
        if (w->starTimer >= 40) {
            w->starState = 7;
            w->starTimer = 0;
            effStardustN64Entry(4, w->spos.x, w->spos.y - 5.0f, w->spos.z, 40.0f);
            effStardustN64Entry(4, 30.0f + w->spos.x, w->spos.y, w->spos.z, 40.0f);
            effStardustN64Entry(4, w->spos.x - 30.0f, w->spos.y, w->spos.z, 40.0f);
            psndSFXOn("SFX_BTL_SAC_SUKKIRI3");
        }
        break;
    case 7:
        w->starTimer++;
        w->spos.y = INTPL(4, w->starTimer, 30, w->from.y, w->to.y);
        if (w->starTimer >= 30) {
            w->starState = 4;
            w->starTimer = 0;
            w->spos = (Vec){0.0f, 300.0f, 0.0f};
            w->scale = (Vec){2.0f, 2.0f, 2.0f};
            w->rot = (Vec){0.0f, 0.0f, 0.0f};
        }
        break;
    case 8:
        w->effMode = 4;
        break;
    case 9:
        w->starTimer++;
        w->spos.x = INTPL(0, w->starTimer, 20, w->from.x, w->to.x);
        w->spos.y = INTPL(1, w->starTimer, 20, w->from.y, w->to.y);
        w->spos.z = INTPL(0, w->starTimer, 20, w->from.z, w->to.z);
        w->effMode = 7;
        if (w->starTimer >= 20) {
            w->starState = 8;
            w->starTimer = 0;
        }
        break;
    case 0:
    case 10:
        break;
    }
    if (w->eff) {
        StarEffWork* e = (StarEffWork*)w->eff->userdata;
        e->pos.x = w->spos.x;
        e->pos.y = w->spos.y;
        e->pos.z = w->spos.z;
        e->rot.x = w->rot.x;
        e->rot.y = w->rot.y;
        e->rot.z = w->rot.z;
        e->scl = 1.5f * w->scale.x;
        e->mode = w->effMode;
    }
}

USER_FUNC(end_suki) {
    SukiWork* w = get_ptr();
    effDelete(((EffStarStoneWork*)w->eff->userdata)->dust);
    effDelete(w->eff);
    return 2;
}

static void disp_2D(CameraId cam, void* param) {
    Mtx m;
    SukiWork* w = get_ptr();
    s32 i;

    PSMTXScale(m, 26.0f, 1.0f, 1.0f);
    PSMTXTransApply(m, m, w->pos.x, 60.0f + w->pos.y, w->pos.z);
    iconDispGx2(m, 0x10, ICON_08E);
    {
        Vec a = {0.0f, 0.0f, 0.0f};
        a.x = 215.0f + w->pos.x;
        a.y = 60.0f + w->pos.y;
        a.z = w->pos.z;
        iconDispGx(&a, 0x10, ICON_090, 1.0f);
    }
    {
        Vec b = {0.0f, 0.0f, 0.0f};
        b.x = w->pos.x - 215.0f;
        b.y = 60.0f + w->pos.y;
        b.z = w->pos.z;
        iconDispGx(&b, 0x10, ICON_08F, 1.0f);
    }
    GXSetScissor(0, 0x91, gp->fbWidth, 0x50);
    for (i = 0; i < 7; i++) {
        N__disp_2D(0, i);
        N__disp_2D(1, i);
    }
    GXSetScissor(0, 0, gp->fbWidth, gp->efbHeight);
}

void N__disp_2D(s32 side, s32 idx) {
    SukiWork* w = get_ptr();
    SukiBtn* b;
    s32 cur;
    f32 yoff;
    u16 icon;
    s32 t;

    if (side == 0) {
        cur = w->prev;
        yoff = 80.0f;
        b = &w->btn2[idx];
    } else {
        cur = w->cur;
        yoff = 0.0f;
        b = &w->btn[idx];
    }
    if (side == 0 && b->state == 0) {
        return;
    }
    if (side == 1 && b->state != 2) {
        return;
    }
    switch (b->button) {
    case 0x100:
        t = ICON_064;
        if (idx < cur) {
            t = ICON_065;
        }
        icon = t;
        break;
    case 0x200:
        t = ICON_066;
        if (idx < cur) {
            t = ICON_067;
        }
        icon = t;
        break;
    case 0x400:
        t = ICON_068;
        if (idx < cur) {
            t = ICON_069;
        }
        icon = t;
        break;
    case 0x800:
        t = ICON_06A;
        if (idx < cur) {
            t = ICON_06B;
        }
        icon = t;
        break;
    case 0x40:
        t = ICON_07E;
        if (idx < cur) {
            t = ICON_07F;
        }
        icon = t;
        break;
    case 0x20:
        t = ICON_080;
        if (idx < cur) {
            t = ICON_081;
        }
        icon = t;
        break;
    case 0x10:
        t = ICON_082;
        if (idx < cur) {
            t = ICON_083;
        }
        icon = t;
        break;
    }
    if (icon) {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = b->pos.x;
        pos.y = 60.0f + (w->offsetY + (b->pos.y + yoff));
        pos.z = b->pos.z;
        iconDispGx(&pos, 0x10, icon, 1.0f);
    }
}

static void disp_3D(CameraId cam, void* param) {
}

static void disp_3D_alpha(CameraId cam, void* param) {
}

#pragma dont_inline on
static SukiWork* get_ptr(void) {
    return (SukiWork*)_battleWorkPointer->actionCommands.work;
}
#pragma dont_inline reset

static USER_FUNC(start_game) {
    SukiWork* w = get_ptr();
    w->state = 1;
    return 2;
}

static USER_FUNC(wait_game_end) {
    SukiWork* w = get_ptr();
    EvtWord* args = event->args;
    if (w->state == 5) {
        evtSetValue(event, args[0], w->round + 1);
        return 2;
    }
    return 0;
}

static USER_FUNC(star_stone_appear) {
    SukiWork* w = get_ptr();
    w->starState = 1;
    return 2;
}

USER_FUNC(wait_star_stone_up_end) {
    return get_ptr()->starState == 4 ? 2 : 0;
}

USER_FUNC(star_stone_get_attack_point) {
    SukiWork* w = get_ptr();
    EvtWord* args = event->args;
    s32 i = evtGetValue(event, args[0]);
    evtSetFloat(event, args[1], w->starPos[i].x);
    evtSetFloat(event, args[2], w->starPos[i].y);
    evtSetFloat(event, args[3], w->starPos[i].z);
    return 2;
}

static USER_FUNC(star_stone_attack) {
    SukiWork* w = get_ptr();
    s32 i = evtGetValue(event, event->args[0]);
    u8* cam;
    w->starState = 9;
    w->starTimer = 0;
    w->from = w->starPos[i];
    w->spos = w->from;
    cam = (u8*)camGetPtr(CAMERA_3D);
    w->to = *(Vec*)(cam + 0xC);
    w->to.y += 20.0f;
    w->to.z += 10.0f;
    return 2;
}

USER_FUNC(wait_star_stone_attack_end) {
    return get_ptr()->starState == 8 ? 2 : 0;
}

typedef struct OhkoRateTbl {
    u8 rate[6];
} OhkoRateTbl;

const OhkoRateTbl N_ohko_rate_tbl = {{50, 60, 70, 80, 90, 100}};

static USER_FUNC(set_weapon) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    SukiWork* w = get_ptr();
    OhkoRateTbl tbl = N_ohko_rate_tbl;
    memcpy(w->weapon, &weapon, 0xC0);
    w->weapon[0x9E] = tbl.rate[idx - 1];
    evtSetValue(event, args[1], (EvtWord)w->weapon);
    return 2;
}

USER_FUNC(aud_set_draw) {
    EvtWord* args = event->args;
    BattleAudience* a = BattleAudienceBaseGetPtr();
    if (evtGetValue(event, args[0]) == 0) {
        a->flags |= 0x20000;
    } else {
        a->flags &= ~0x20000;
    }
    return 2;
}

USER_FUNC(flash_update) {
    u8 v = evtGetValue(event, event->args[0]);
    get_ptr()->flash = v;
    return 2;
}

USER_FUNC(exist_map) {
    EvtWord* args = event->args;
    if (mapGetMapObj((char*)evtGetValue(event, args[0]))) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return 2;
}

USER_FUNC(get_screen_point) {
    EvtWord* args = event->args;
    Vec v;
    v.x = evtGetFloat(event, args[0]);
    v.y = evtGetFloat(event, args[1]);
    v.z = evtGetFloat(event, args[2]);
    btlGetScreenPoint(&v, &v);
    evtSetFloat(event, args[3], v.x);
    evtSetFloat(event, args[4], v.y);
    return 2;
}

USER_FUNC(eff_crystal) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    EffectEntry* e = effCrystalN64Entry(6, 60, x, y, 30.0f, 1.0f);
    ((EffCrystalN64Work*)e->userdata)->camera = 7;
    return 2;
}

USER_FUNC(get_img_name) {
    EvtWord* args = event->args;
    s8 idx = evtGetValue(event, args[0]);
    SukiWork* w = get_ptr();
    evtSetValue(event, args[1], (EvtWord)w->names[idx]);
    return 2;
}

static inline GXColor mk_suki(u8 v) { GXColor c = {0, 0, 0, 255}; c.r = v; c.g = v; c.b = v; return c; }
void set_tev(MapTevCallbackInfo* info) {
    u8 v = get_ptr()->flash;
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 2);
    GXSetTevAlphaIn(0, 7, 7, 7, 1);
    GXSetTevColor(1, mk_suki(v));
    info->tevStage = 1;
    info->texMap = 0;
    info->texCoord = 0;
}


EVT_BEGIN(marioAttackEvent_Suki)
    CALL(btlevtcmd_GetUnitId, -4, LW(14))
    CALL(btlevtcmd_GetBodyId, LW(14), LW(15))
    CALL(btlevtcmd_CheckStatus, LW(14), 27, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), -1)
    END_IF
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon), LW(3), LW(4))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(btlevtcmd_reset_move_color_lv_all)
    BEGIN_SIBLING_EVENT
        CALL(main_suki, LW(14))
    END_SIBLING_EVENT
    CALL_CHILD(sac_start_camera_evt)
    WAIT_MS(500)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_SAC_START1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL_CHILD(sac_mario_item_on_evt)
    WAIT_MS(500)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_SAC_START2"), EVT_NULLPTR, 0, LW(11))
    CALL(star_stone_appear)
    BEGIN_SIBLING_EVENT
        CALL_CHILD(sac_start_stardust)
    END_SIBLING_EVENT
    WAIT_FRAMES(100)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_SAC_START3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_snd_sfxoff, LW(11))
    WAIT_FRAMES(40)
    CALL_CHILD(sac_mario_item_on_end_evt)
    CALL_CHILD(sac_mario_back_home)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_I_Y"))
    CALL(wait_star_stone_up_end)
    WAIT_MS(100)
    BEGIN_SIBLING_EVENT
        CALL(sac_cheer, 0)
    END_SIBLING_EVENT
    CALL(sac_ac_help_on)
    CALL(start_game)
    CALL(wait_game_end, LW(0))
    CALL(sac_ac_help_off)
    CALL(sac_cheer_end)
    CALL(set_weapon, LW(0), LW(9))
    WAIT_MS(500)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    WAIT_MS(1000)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(0))
    CALL(evt_paper_entry, STRING("OFF_d_yabu_small"))
    LOOP(LW(11))
        CALL(star_stone_get_attack_point, LW(10), LW(1), LW(2), LW(3))
        CALL(get_img_name, LW(10), LW(13))
        CALL(evt_img_entry, LW(13))
        CALL(evt_img_set_paper, LW(13), STRING("OFF_d_yabu_small"))
        CALL(get_screen_point, LW(1), LW(2), LW(3), LW(4), LW(5))
        ADD(LW(4), 256)
        MULTIPLY(LW(5), -1)
        ADD(LW(5), 204)
        CALL(evt_img_alloc_capture, LW(13), 0, 0, 0, LW(4), LW(5), 96, 72)
        CALL(evt_img_clear_virtual_point, LW(13))
        CALL(evt_img_onoff, LW(13), 1)
        CALL(evt_img_set_paper_anim, LW(13), STRING("A_1"))
        BEGIN_SIBLING_EVENT
            CALL(star_stone_attack, LW(10))
            CALL(evt_btl_camera_shake_h, 0, 8, 0, 10, 0)
        END_SIBLING_EVENT
        CALL(eff_crystal, LW(1), LW(2))
        CALL(evt_eff, STRING(""), STRING("indirect"), 0, LW(1), LW(2), 30, 60, 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_SAC_SUKKIRI4"), 0)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_SAC_SUKKIRI4_2"), 0)
        IF_INT_EQUAL(LW(10), 0)
            CALL(btlevtcmd_GetEnemyBelong, -2, LW(1))
            CALL(btlevtcmd_SamplingEnemy, -2, LW(1), PTR(&weapon))
            CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon), LW(3), LW(4))
            CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
            LABEL(5)
            IF_INT_EQUAL(LW(3), -1)
                GOTO(8)
            END_IF
            CALL(btlevtcmd_CheckPartsAttribute, LW(3), LW(4), 536870912, LW(1))
            IF_INT_EQUAL(LW(1), 0)
                CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 39)
            END_IF
            CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
            GOTO(5)
        END_IF
        LABEL(8)
        WAIT_FRAMES(30)
        CALL(wait_star_stone_attack_end)
        ADD(LW(10), 1)
    END_LOOP
    CALL(evt_paper_delete, STRING("OFF_d_yabu_small"))
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), LW(0))
    LOOP(LW(11))
        CALL(get_img_name, LW(10), LW(13))
        CALL(evt_img_release, LW(13))
        ADD(LW(10), 1)
    END_LOOP
    CALL_CHILD(flash_evt)
    SET_VALUE(LW(10), 3)
    LOOP(4)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_SAC_SUKKIRI5"), 0)
        LOOP(4)
            ADD(LW(10), 63)
            CALL(flash_update, LW(10))
            CALL_CHILD(flash_main_evt)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(4)
            SUBTRACT(LW(10), 63)
            CALL(flash_update, LW(10))
            CALL_CHILD(flash_main_evt)
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    CALL_CHILD(flash_cancel_evt)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon), LW(3), LW(4))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(10)
    IF_INT_EQUAL(LW(3), -1)
        GOTO(20)
    END_IF
    CALL(btlevtcmd_CheckPartsAttribute, LW(3), LW(4), 16384, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    END_IF
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    GOTO(10)
    LABEL(20)
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(end_suki)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(flash_evt)
    CALL(evt_map_set_tevcallback, 1, PTR(set_tev))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("bti_S"), 16777216)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("bti_S"), 256)
    CALL(exist_map, STRING("A1"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("A1"), 251658240)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("A1"), 16777216)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("A1"), 256)
    END_IF
    CALL(exist_map, STRING("A2"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("A2"), 16777216)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("A2"), 256)
    END_IF
    CALL(exist_map, STRING("B"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("B"), 16777216)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("B"), 256)
    END_IF
    CALL(exist_map, STRING("C"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("C"), 16777216)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("C"), 256)
    END_IF
    CALL(btlevtcmd_SetAlpha, -2, 1, 0)
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, 33554432)
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_SetAlpha, LW(14), LW(15), 0)
        CALL(btlevtcmd_OnPartsAttribute, LW(14), LW(15), 33554432)
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon), LW(3), LW(4))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(5)
    IF_INT_EQUAL(LW(3), -1)
        GOTO(8)
    END_IF
    CALL(btlevtcmd_SetRGB, LW(3), LW(4), 0, 0, 0)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    GOTO(5)
    LABEL(8)
    CALL(aud_set_draw, 0)
    CALL(evt_batstage_set_stg_dark, 0, 0, 0)
    CALL(evt_batstage_set_aud_dark, 0, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(flash_main_evt)
    SET_VALUE(LW(11), 255)
    SUBTRACT(LW(11), LW(10))
    CALL(flash_update, LW(10))
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon), LW(3), LW(4))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(5)
    IF_INT_EQUAL(LW(3), -1)
        GOTO(8)
    END_IF
    CALL(btlevtcmd_SetRGB, LW(3), LW(4), LW(11), LW(11), LW(11))
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    GOTO(5)
    LABEL(8)
    RETURN
EVT_END()

EVT_BEGIN(flash_cancel_evt)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("bti_S"), 251658240)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("bti_S"), 256)
    CALL(exist_map, STRING("A1"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("A1"), 251658240)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("A1"), 256)
    END_IF
    CALL(exist_map, STRING("A2"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("A2"), 251658240)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("A2"), 256)
    END_IF
    CALL(exist_map, STRING("B"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("B"), 251658240)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("B"), 256)
    END_IF
    CALL(exist_map, STRING("C"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("C"), 251658240)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("C"), 256)
    END_IF
    CALL(exist_map, STRING("gamen"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_map_set_tevcallback, 8, PTR(tou_gamen_screen_tev))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("gamen"), 134217728)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("gamen"), 256)
    END_IF
    CALL(btlevtcmd_SetAlpha, -2, 1, 255)
    CALL(btlevtcmd_OffPartsAttribute, -2, 1, 33554432)
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_SetAlpha, LW(14), LW(15), 255)
        CALL(btlevtcmd_OffPartsAttribute, LW(14), LW(15), 33554432)
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon), LW(3), LW(4))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(5)
    IF_INT_EQUAL(LW(3), -1)
        GOTO(8)
    END_IF
    CALL(btlevtcmd_SetRGB, LW(3), LW(4), 255, 255, 255)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    GOTO(5)
    LABEL(8)
    CALL(aud_set_draw, 1)
    CALL(evt_batstage_return_stg_dark_base, 0, 0)
    CALL(evt_batstage_return_aud_dark_base, 0, 0)
    RETURN
EVT_END()
