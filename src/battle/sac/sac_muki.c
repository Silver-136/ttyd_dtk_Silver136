#include "eff/eff_starstone.h"
#include "battle/battle.h"
#include "eff/n64/eff_stamp_n64.h"
#include "battle/battle_audience.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/icondrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <string.h>
#include "icon_ids.h"
#include "version.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/sac/sac_common.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "pmario_sound.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"
#include "battle/battle_disp.h"
#include "eff/eff_hit.h"
#include "battle/battle_sub.h"
// US battle texture ids are 0x16 higher for these entries.
#define BTL_TEX(id) ((id) + VERSION_PICK(0, 0x16))




static USER_FUNC(star_stone_appear);
static USER_FUNC(wait_game_end);
static USER_FUNC(start_game);
static USER_FUNC(set_weapon);
USER_FUNC(end_muki);
USER_FUNC(main_muki);

typedef struct MukiObj { // 0x38
    s32 kind;          // 0x
    s32 timer;         // 0x4
    s32 type;          // 0x8
    Vec pos;           // 0xc
    Vec scale;         // 0x18
    Vec rot;           // 0x24
    EffectEntry* eff;  // 0x30
    u8 alpha;          // 0x34
    u8 pad35[3];
} MukiObj;

typedef struct Muki3D { // 0x24
    s32 kind;          // 0x
    s32 timer;         // 0x4
    Vec pos;           // 0x8
    Vec scale;         // 0x14
    MukiObj* link;     // 0x20
} Muki3D;

typedef struct MukiWork {
    u32 flags;             // 0x
    s32 state;             // 0x4
    s32 timer;             // 0x8
    s32 timeLeft;          // 0xc
    s32 unitId;            // 0x10
    f32 f14;               // 0x14
    f32 f18;               // 0x18
    f32 f1c;               // 0x1c
    f32 f20;               // 0x20
    s32 t24;               // 0x24
    s32 t28;               // 0x28
    f32 f2c;               // 0x2c
    f32 f30;               // 0x30
    s32 t34;               // 0x34
    s32 kind38;            // 0x38
    s32 t3c;               // 0x3c
    s32 idx;               // 0x40
    Vec cpos;              // 0x44
    Vec cfrom;             // 0x50
    Vec cto;               // 0x5c
    Vec cscale;            // 0x68
    GXColor col74;         // 0x74
    Muki3D o3d[9];         // 0x78
    MukiObj obj[9];        // 0x1bc
    s32 star;              // 0x3b4
    s32 starTimer;         // 0x3b8
    Vec spos;              // 0x3bc
    Vec from;              // 0x3c8
    Vec to;                // 0x3d4
    u8 pad3e0[0xc];
    Vec scale;             // 0x3ec
    Vec rot;               // 0x3f8
    EffectEntry* eff;      // 0x404
    u8 weapon[0xc0];       // 0x408
} MukiWork;

static void disp_3D_alpha(CameraId cam, void* param);
static void disp_3D(CameraId cam, void* param);
static void disp_2D(CameraId cam, void* param);
static MukiWork* get_ptr(void);
static void main_star(void);
static void main_base(void);
void main_icon(s32 idx);
void main_point(s32 idx);
void main_cursor(void);


static BattleWeapon weapon;
EvtWord nice_event_pow[8];
EvtWord marioAttackEvent_Muki[254];

static BattleWeapon weapon = {
    "btl_wn_sac_mukimuki_body", // name
    ICON_SAC_MUKI, // icon
    {0}, // pad_06
    0, // itemId
    "msg_mukimuki_body", // description
    100, // baseAccuracy
    0, // baseFpCost
    3, // baseSpCost
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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_ENEMY, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_4", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_Muki, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

EVT_BEGIN(nice_event_pow)
    CALL(btlevtcmd_ACSuccessEffect, LW(0), -100, 175, 0)
    RETURN
EVT_END()


USER_FUNC(main_muki) {
    MukiWork* w = get_ptr();
    EvtWord* args = event->args;
    s32 i;

    if (isFirstCall) {
        memset(w, 0, 0x4C8);
        w->unitId = evtGetValue(event, args[0]);
        for (i = 0; i < 9; i++) {
            w->obj[i].eff = effStarStoneEntry(9, 0.0f, -1000.0f, 0.0f, 1.0f);
        }
    }
    main_base();
    main_cursor();
    for (i = 0; i < 9; i++) {
        main_point(i);
    }
    for (i = 0; i < 9; i++) {
        main_icon(i);
    }
    main_star();
    dispEntry(CAMERA_2D, 901.0f, 1, disp_2D, 0);
    dispEntry(CAMERA_3D, 0.0f, 1, disp_3D, 0);
    dispEntry(CAMERA_3D, 0.0f, 2, disp_3D_alpha, 0);
    return 0;
}

static void main_base(void) {
    MukiWork* w = get_ptr();
    BattleUnit* mario = BattleGetMarioPtr(_battleWorkPointer);
    BattleUnitPart* part = BtlUnit_GetPartsPtr(mario, BtlUnit_GetBodyPartsId(mario));
    s32 i;
    s32 n;
    f32 d;
    f32 step;

    if (w->t24 > 0) {
        w->t24--;
    }
    if (w->t28 > 0) {
        w->t28--;
    }
    switch (w->state) {
    case 1:
        w->state = 2;
        w->f14 = w->f18 = 0.0f;
        w->f1c = w->f20 = 0.0f;
        w->f2c = -400.0f;
        w->f30 = 90.0f;
        w->timeLeft = 899;
        for (i = 0; i < 9; i++) {
            w->o3d[i].kind = 1;
        }
    case 2:
        n = 0;
        for (i = 0; i < 9; i++) {
            if (w->o3d[i].kind != 4) {
                break;
            }
            n++;
        }
        if (n == 9) {
            w->state = 3;
            w->timer = 0;
        }
        break;
    case 3:
        w->timer++;
        w->f2c = intplGetValue(EASING_QUADRATIC_OUT, -400.0f, -120.0f, w->timer, 30);
        if (w->timer >= 30) {
            w->state = 4;
            w->timer = 0;
            w->kind38 = 1;
            for (i = 0; i < 9; i++) {
                w->o3d[i].kind = 5;
            }
        }
        break;
    case 4:
        if (w->flags & 1) {
            w->t34++;
            if (w->t34 == 30) {
                BtlUnit_SetAnim(part, "M_X_1");
            }
        }
        w->timeLeft--;
        if (w->timeLeft <= 0) {
            w->state = 5;
            w->timer = 0;
            w->kind38 = 7;
            BtlUnit_SetAnim(part, "M_S_1");
            for (i = 0; i < 9; i++) {
                w->o3d[i].kind = 6;
                w->obj[i].kind = 9;
            }
            n = (s32)(w->f14 + w->f1c);
            if (n >= 1) {
                if (n == 1) {
                    n = 2;
                } else if (n == 2) {
                    n = 3;
                } else if (n == 3) {
                    n = 4;
                } else if (n == 4) {
                    n = 5;
                } else if (n >= 5) {
                    n = 6;
                }
                evtEntry(nice_event_pow, 0, EVENT_START_IMMEDIATE)->lwData[0] = n;
            }
        }
        break;
    case 5:
        n = 0;
        for (i = 0; i < 9; i++) {
            if (w->o3d[i].kind != 8) {
                break;
            }
            n++;
        }
        if (n == 9) {
            w->state = 6;
            w->timer = 0;
        }
        break;
    case 6:
        w->timer++;
        if (w->timer >= 30) {
            w->state = 7;
            w->timer = 0;
        }
        break;
    case 0:
    case 7:
        break;
    }
    d = w->f14 - w->f18;
    if (d != 0.0f) {
        if (d > 0.0f) {
            step = (0.1f * d < 0.1f) ? 0.1f : 0.1f * d;
            w->f18 += step;
            if (w->f14 - w->f18 < 0.0f) {
                w->f18 = w->f14;
            }
        } else {
            step = (0.1f * d > -0.1f) ? 0.1f : -0.1f * d;
            w->f18 -= step;
            if (w->f14 - w->f18 > 0.0f) {
                w->f18 = w->f14;
            }
        }
    }
    d = w->f1c - w->f20;
    if (d != 0.0f) {
        if (d > 0.0f) {
            step = (0.1f * d < 0.1f) ? 0.1f : 0.1f * d;
            w->f20 += step;
            if (w->f1c - w->f20 < 0.0f) {
                w->f20 = w->f1c;
            }
        } else {
            step = (0.1f * d > -0.1f) ? 0.1f : -0.1f * d;
            w->f20 -= step;
            if (w->f1c - w->f20 > 0.0f) {
                w->f20 = w->f1c;
            }
        }
    }
}

#define AUD_CALL(num)                         \
    if ((num) >= 1 && (num) <= 49) {          \
        BattleAudienceSoundCallKind(1);       \
    }                                         \
    if ((num) >= 50 && (num) <= 99) {         \
        BattleAudienceSoundCallKind(1);       \
        BattleAudienceSoundHandBeat();        \
    }                                         \
    if ((num) >= 100 && (num) <= 149) {       \
        BattleAudienceSoundCallKind(2);       \
    }                                         \
    if ((num) >= 150) {                       \
        BattleAudienceSoundCallKind(2);       \
        BattleAudienceSoundHandBeat();        \
    }

void main_cursor(void) {
    MukiWork* w = get_ptr();
    BattleUnit* mario = BattleGetMarioPtr(_battleWorkPointer);
    BattleUnitPart* part = BtlUnit_GetPartsPtr(mario, BtlUnit_GetBodyPartsId(mario));
    s32 num = BattleAudience_GetAudienceNum();
    MukiObj* ob;
    f32 prev;
    f32 x, y, z;
    EffectEntry* eff;

    switch (w->kind38) {
    case 1:
        if (w->o3d[4].kind < 5) {
            break;
        }
        w->kind38 = 2;
        w->cpos = w->o3d[4].pos;
        w->cpos.z += 5.0f;
        w->cscale = (Vec){0.5f, 0.5f, 1.0f};
        w->idx = 4;
        w->col74 = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    case 2:
        w->col74 = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
        if (w->t34 >= 120) {
            BtlUnit_SetAnim(part, "M_I_Y");
            w->flags &= ~1;
            w->t34 = 0;
        }
        if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
            w->kind38 = 5;
            ob = w->o3d[w->idx].link;
            if (ob) {
                ob->kind = 5;
                effHitEntry(0, 5, ob->pos.x, ob->pos.y, ob->pos.z, 1.0f);
                switch (ob->type) {
                case 0:
                    prev = w->f14;
                    w->f14 += VERSION_PICK(0.25f, 0.2f);
                    if (w->f14 > 9.0f) {
                        w->f14 = 9.0f;
                    }
                    if ((s32)w->f14 > (s32)prev) {
                        w->t24 = 60;
                        psndSFXOn("SFX_BTL_SAC_BODY6");
                        AUD_CALL(num)
                    }
                    psndSFXOn("SFX_BTL_SAC_BODY4");
                    break;
                case 1:
                    prev = w->f1c;
                    w->f1c += VERSION_PICK(0.25f, 0.2f);
                    if (w->f1c > 9.0f) {
                        w->f1c = 9.0f;
                    }
                    if ((s32)w->f1c > (s32)prev) {
                        w->t28 = 60;
                        psndSFXOn("SFX_BTL_SAC_BODY6");
                        AUD_CALL(num)
                    }
                    psndSFXOn("SFX_BTL_SAC_BODY4");
                    break;
                case 2:
                    BtlUnit_SetAnim(part, "M_D_1");
                    BtlUnit_GetPos(mario, &x, &y, &z);
                    eff = effStampN64Entry(2, x, 20.0f + y, 10.0f + z);
                    ((EffStampN64Work*)eff->userdata)->r = 0;
                    ((EffStampN64Work*)eff->userdata)->g = 200;
                    ((EffStampN64Work*)eff->userdata)->b = 0;
                    ((EffStampN64Work*)eff->userdata)->r2 = 0;
                    ((EffStampN64Work*)eff->userdata)->g2 = 10;
                    ((EffStampN64Work*)eff->userdata)->b2 = 0;
                    w->t34 = 0;
                    w->flags |= 1;
                    psndSFXOn("SFX_BTL_SAC_BODY5");
                    if (num >= 1 && num <= 49) {
                        BattleAudienceSoundBooingKind(1);
                    }
                    if (num >= 50 && num <= 99) {
                        BattleAudienceSoundBooingKind(1);
                    }
                    if (num >= 100 && num <= 149) {
                        BattleAudienceSoundBooingKind(2);
                    }
                    if (num >= 150) {
                        BattleAudienceSoundBooingKind(3);
                    }
                    break;
                }
            }
        }
        if (keyGetDirTrg(0) & DEMO_STICK_RIGHT) {
            if (w->idx % 3 != 2) {
                w->kind38 = 3;
                w->idx++;
            }
        }
        if (keyGetDirTrg(0) & DEMO_STICK_LEFT) {
            if (w->idx % 3) {
                w->kind38 = 3;
                w->idx--;
            }
        }
        if (keyGetDirTrg(0) & DEMO_STICK_UP) {
            if (w->idx / 3) {
                w->kind38 = 3;
                w->idx -= 3;
            }
        }
        if (keyGetDirTrg(0) & DEMO_STICK_DOWN) {
            if (w->idx / 3 != 2) {
                w->kind38 = 3;
                w->idx += 3;
            }
        }
        break;
    case 3:
        w->kind38 = 4;
        w->t3c = 0;
        w->cfrom = w->cpos;
        w->cto = w->o3d[w->idx].pos;
        psndSFXOn("SFX_BTL_SAC_BODY3");
    case 4:
        w->t3c++;
        w->col74 = (GXColor){0x0, 0x0, 0xFF, 0xFF};
        if (!(w->flags & 1)) {
            w->cpos.x = intplGetValue(EASING_QUADRATIC_OUT, w->cfrom.x, w->cto.x, w->t3c, 5);
            w->cpos.y = intplGetValue(EASING_QUADRATIC_OUT, w->cfrom.y, w->cto.y, w->t3c, 5);
            if (w->t3c >= 5) {
                w->kind38 = 2;
            }
        } else {
            w->cpos.x = intplGetValue(EASING_QUADRATIC_OUT, w->cfrom.x, w->cto.x, w->t3c, 25);
            w->cpos.y = intplGetValue(EASING_QUADRATIC_OUT, w->cfrom.y, w->cto.y, w->t3c, 25);
            if (w->t3c >= 25) {
                w->kind38 = 2;
            }
        }
        break;
    case 5:
        w->kind38 = 6;
        w->t3c = 0;
    case 6:
        w->t3c++;
        w->col74 = (GXColor){0xFF, 0x0, 0x0, 0xFF};
        if (w->t3c >= 1) {
            w->kind38 = 2;
            w->t3c = 0;
        }
        break;
    case 0:
    case 7:
        break;
    }
}

void main_point(s32 idx) {
    MukiWork* w = get_ptr();
    Muki3D* o = &w->o3d[idx];
    s32 i;

    switch (o->kind) {
    case 1: {
        o->kind = 2;
        o->timer = irand(30) + 30;
        {
        Vec p = {0.0f, 0.0f, 20.0f};
        p.x = (f32)((idx % 3) * 48 - 48);
        p.y = (f32)(125 - (idx / 3) * 48);
        o->pos = p;
        }
        o->scale = (Vec){0.0f, 0.0f, 1.0f};
        o->link = NULL;
    }
    case 2:
        o->timer--;
        if (o->timer <= 30) {
            o->scale.x = o->scale.y = intplGetValue(EASING_LINEAR, 0.0f, 0.5f, 30 - o->timer, 30);
        }
        if (o->timer <= 0) {
            o->kind = 3;
            o->timer = 0;
        }
        break;
    case 3:
        o->kind = 4;
        o->timer = 0;
        o->scale = (Vec){0.5f, 0.5f, 1.0f};
        break;
    case 5:
        if (o->link) {
            if (o->link->kind == 0) {
                o->link = NULL;
            }
        } else if (irand(65) == 0) {
            for (i = 0; i < 9; i++) {
                MukiObj* ob = &w->obj[i];
                if (ob->kind == 0) {
                    ob->kind = 1;
                    ob->pos = o->pos;
                    ob->pos.z = 0.0f;
                    o->link = ob;
                    break;
                }
            }
        }
        break;
    case 6:
        o->kind = 7;
        o->timer = irand(30) + 30;
        o->link = NULL;
    case 7:
        o->timer--;
        if (o->timer <= 30) {
            o->scale.x = o->scale.y = intplGetValue(EASING_LINEAR, 0.5f, 0.0f, 30 - o->timer, 30);
        }
        if (o->timer <= 0) {
            o->kind = 8;
            o->timer = 0;
        }
        break;
    case 0:
    case 4:
    case 8:
        break;
    }
}

void main_icon(s32 idx) {
    MukiWork* w = get_ptr();
    MukiObj* o = &w->obj[idx];

    switch (o->kind) {
    case 0:
        o->pos = (Vec){0.0f, -1000.0f, 0.0f};
        o->alpha = 0xFF;
        break;
    case 1:
        o->kind = 2;
        o->timer = 0;
        o->type = irand(3);
        o->rot.y = 0.0f;
        o->scale = (Vec){0.3f, 0.3f, 0.3f};
    case 2:
        o->timer++;
        o->rot.y = intplGetValue(EASING_QUADRATIC_OUT, 180.0f, 1080.0f, o->timer, 30);
        o->alpha = intplGetValue(EASING_LINEAR, 0.0f, 255.0f, o->timer, 30);
        if (o->timer >= 30) {
            o->kind = 7;
        }
        break;
    case 3:
        o->kind = 4;
        o->timer = 0;
        o->rot.y = 0.0f;
    case 4:
        o->timer++;
        o->rot.y = intplGetValue(EASING_QUADRATIC_IN, 0.0f, 900.0f, o->timer, 30);
        o->alpha = intplGetValue(EASING_LINEAR, 255.0f, 0.0f, o->timer, 30);
        if (o->timer >= 30) {
            o->kind = 0;
        }
        break;
    case 5:
        o->kind = 6;
    case 6:
        o->kind = 0;
        break;
    case 7:
        o->kind = 8;
        o->timer = 0;
        o->rot.y = 0.0f;
        break;
    case 8:
        o->timer++;
        o->alpha = 0xFF;
        if (o->timer >= 90) {
            o->kind = 3;
        }
        break;
    case 9:
        if (o->alpha >= 8) {
            o->alpha -= 8;
        } else {
            o->alpha = 0;
        }
        break;
    }
    if (o->eff) {
        EffStarStoneWork* e = (EffStarStoneWork*)o->eff->userdata;
        e->position.x = o->pos.x;
        e->position.y = o->pos.y;
        e->position.z = o->pos.z;
        e->rot.x = o->rot.x;
        e->rot.y = o->rot.y;
        e->rot.z = o->rot.z;
        e->scale = 9.0f * o->scale.x;
        e->a = o->alpha;
    }
}

static void main_star(void) {
    MukiWork* w = get_ptr();
    BattleUnit* mario = BattleGetMarioPtr(_battleWorkPointer);

    switch (w->star) {
    case 1:
        w->star = 2;
        w->starTimer = 0;
        w->eff = effStarStoneEntry(2, 0.0f, -1000.0f, 0.0f, 1.0f);
        BtlUnit_GetPos(mario, &w->spos.x, &w->spos.y, &w->spos.z);
        w->spos.y += mario->mSizeMultiplier * (f32)mario->mHeight + 37.0f;
        w->from = w->spos;
        w->to = w->spos;
        w->to.y += 50.0f;
        w->to.z += -1.0f;
        w->scale = (Vec){0.0f, 0.0f, 0.0f};
        w->rot = (Vec){0.0f, 0.0f, 0.0f};
    case 2:
        w->starTimer++;
        if (w->starTimer <= 100) {
            w->spos.x = intplGetValue(EASING_LINEAR, w->from.x, w->to.x, w->starTimer, 100);
            w->spos.y = intplGetValue(EASING_LINEAR, w->from.y, w->to.y, w->starTimer, 100);
            w->spos.z = intplGetValue(EASING_LINEAR, w->from.z, w->to.z, w->starTimer, 100);
            w->scale.x = w->scale.y = w->scale.z = intplGetValue(EASING_LINEAR, 0.0f, 2.0f, w->starTimer, 100);
        } else {
            w->spos.x = w->to.x;
            w->spos.y = w->to.y;
            w->spos.z = w->to.z;
            w->scale.x = w->scale.y = w->scale.z = 2.0f;
        }
        w->rot.y = intplGetValue(EASING_QUADRATIC_OUT, 0.0f, 2160.0f, w->starTimer, 120);
        if (w->starTimer >= 120) {
            w->star = 3;
            w->starTimer = 0;
            w->from = w->spos;
            w->to.y = 300.0f;
        }
        break;
    case 3:
        w->starTimer++;
        w->spos.y = intplGetValue(EASING_QUADRATIC_IN, w->from.y, w->to.y, w->starTimer, 60);
        if (w->starTimer >= 60) {
            w->star = 5;
            w->starTimer = 0;
        }
        break;
    case 0:
    case 4:
    case 5:
        break;
    }
    if (w->eff) {
        EffStarStoneWork* e = (EffStarStoneWork*)w->eff->userdata;
        e->position.x = w->spos.x;
        e->position.y = w->spos.y;
        e->position.z = w->spos.z;
        e->rot.x = w->rot.x;
        e->rot.y = w->rot.y;
        e->rot.z = w->rot.z;
        e->scale = 1.5f * w->scale.x;
    }
}

USER_FUNC(end_muki) {
    MukiWork* w = get_ptr();
    s32 i;

    effDelete(((EffStarStoneWork*)w->eff->userdata)->dust);
    effDelete(w->eff);
    for (i = 0; i < 9; i++) {
        effDelete(w->obj[i].eff);
    }
    return 2;
}

static inline f32 addf(f32 a, f32 b) { return a + b; }
static inline f32 mulf(f32 x, f32 y) { return x * y; }
static void disp_2D(CameraId cam, void* param) {
    MukiWork* w = get_ptr();
    Mtx m;
    Vec s;
    GXColor col[4];
    f32 d;
    f32 frac;
    f32 x;
    f32 y;
    u8 r, g, b, a;

    if (w->state >= 2 && w->state <= 7) {
        Vec p = {0.0f, 0.0f, 0.0f};
        GXColor c;
        s32 t;
        x = w->f2c;
        y = w->f30;
        p.x = x;
        p.y = y;
        p.z = 0.0f;
        s = p;
        btlGetScreenPoint(&s, &s);
        y = s.y;
        c = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
        PSMTXTrans(m, (x = s.x) - 32.0f - 8.0f, y, 0.0f);
        btlDispTexPlane2(m, BTL_TEX(0x43), c, 0);
        PSMTXScale(m, 16.0f, 1.0f, 1.0f);
        PSMTXTransApply(m, m, x, y, 0.0f);
        btlDispTexPlane2(m, BTL_TEX(0x44), c, 0);
        PSMTXScale(m, -1.0f, 1.0f, 1.0f);
        PSMTXTransApply(m, m, 8.0f + addf(x, 32.0f), y, 0.0f);
        btlDispTexPlane2(m, BTL_TEX(0x43), c, 0);
        x = x - 32.0f - 8.0f;
        frac = w->f18 - (f32)(s32)w->f18;
        btlDispGXInit2DRasta();
        GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, 0);
        d = w->f14 - w->f18;
        if (d != 0.0f) {
            r = 0xFF;
            g = 0xB4;
            b = 0xB4;
            a = 0xFF;
        } else {
            r = 0xF0;
            g = 0xA0;
            b = 0xA0;
            a = 0xFF;
        }
        btlDispGXQuads2DRasta(x, 8.0f + y, x + (frac *= 80.0f), y - 8.0f, r, g, b, a);
        t = w->t24 % 12;
        if (t >= 0 && t <= 5) {
            PSMTXScale(m, 0.5f, 0.5f, 0.5f);
            PSMTXTransApply(m, m, x - 30.0f, y, 0.0f);
            col[0] = col[1] = (GXColor){0xE9, 0x4A, 0x3F, 0xFF};
            col[2] = col[3] = (GXColor){0xFE, 0xE6, 0x3E, 0xFF};
            btlDispTexPlane3(m, BTL_TEX(0x45), col[0], col[1], col[2], col[3], 0);
            PSMTXScale(m, 0.7f, 0.7f, 0.7f);
            PSMTXTransApply(m, m, x - 35.0f, y - 14.0f, 0.0f);
            iconDispGx2(m, 0x10, ICON_PLUS);
            PSMTXScale(m, 0.7f, 0.7f, 0.7f);
            PSMTXTransApply(m, m, x - 25.0f, y - 14.0f, 0.0f);
            iconNumberDispGx(m, (s32)w->f18, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        }
    }
    if (w->state >= 2 && w->state <= 7) {
        Vec p = {0.0f, 0.0f, 0.0f};
        GXColor c;
        s32 t;
        x = w->f2c;
        y = w->f30 - 30.0f;
        p.x = x;
        p.y = y;
        p.z = 0.0f;
        s = p;
        btlGetScreenPoint(&s, &s);
        x = s.x;
        y = s.y;
        c = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
        PSMTXTrans(m, x - 32.0f - 8.0f, y, 0.0f);
        btlDispTexPlane2(m, BTL_TEX(0x43), c, 0);
        PSMTXScale(m, 16.0f, 1.0f, 1.0f);
        PSMTXTransApply(m, m, x, y, 0.0f);
        btlDispTexPlane2(m, BTL_TEX(0x44), c, 0);
        PSMTXScale(m, -1.0f, 1.0f, 1.0f);
        PSMTXTransApply(m, m, 8.0f + addf(x, 32.0f), y, 0.0f);
        btlDispTexPlane2(m, BTL_TEX(0x43), c, 0);
        x = x - 32.0f - 8.0f;
        frac = w->f20 - (f32)(s32)w->f20;
        btlDispGXInit2DRasta();
        GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, 0);
        if (d != 0.0f) {
            r = 0xB4;
            g = 0xB4;
            b = 0xFF;
            a = 0xFF;
        } else {
            r = 0xA0;
            g = 0xA0;
            b = 0xF0;
            a = 0xFF;
        }
        btlDispGXQuads2DRasta(x, 8.0f + y, x + (frac *= 80.0f), y - 8.0f, r, g, b, a);
        t = w->t28 % 12;
        if (t >= 0 && t <= 5) {
            PSMTXScale(m, 0.5f, 0.5f, 0.5f);
            PSMTXTransApply(m, m, x - 30.0f, y, 0.0f);
            col[0] = col[1] = (GXColor){0x3F, 0x3F, 0xE9, 0xFF};
            col[2] = col[3] = (GXColor){0x74, 0xC9, 0xFF, 0xFF};
            btlDispTexPlane3(m, BTL_TEX(0x45), col[0], col[1], col[2], col[3], 0);
            PSMTXScale(m, 0.7f, 0.7f, 0.7f);
            PSMTXTransApply(m, m, x - 35.0f, y - 14.0f, 0.0f);
            iconDispGx2(m, 0x10, ICON_PLUS);
            PSMTXScale(m, 0.7f, 0.7f, 0.7f);
            PSMTXTransApply(m, m, x - 25.0f, y - 14.0f, 0.0f);
            iconNumberDispGx(m, (s32)w->f20, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        }
    }
}

static void disp_3D(CameraId cam, void* param) {
    MukiWork* w = get_ptr();
    s32 a[8] = {-8, 8, 8, 8, 8, -8, -8, -8};
    s32 b[8] = {-24, 24, 24, 24, 24, -24, -24, -24};
    s32 c[8] = {1, 1, -1, 1, -1, -1, 1, -1};
    Muki3D* o;
    s32 i, j;
    Mtx m;
    for (i = 0; i < 9; i++) {
        o = &w->o3d[i];
        if (o->kind >= 2 && o->kind < 8) {
            for (j = 0; j < 4; j++) {
                PSMTXScale(m, o->scale.x * (f32)c[j * 2], o->scale.y * (f32)c[j * 2 + 1], o->scale.z);
                PSMTXTransApply(m, m, o->pos.x, o->pos.y, o->pos.z);
                PSMTXTransApply(m, m, o->scale.x * (f32)a[j * 2], o->scale.y * (f32)a[j * 2 + 1], 0.0f);
                btlDispTexPlane2(m, BTL_TEX(0x42), (GXColor){0xDE, 0xDE, 0xDE, 0xFF}, 0);
            }
        }
    }
    if (w->kind38 >= 2 && w->kind38 < 7) {
        for (j = 0; j < 4; j++) {
            PSMTXScale(m, w->cscale.x * (f32)c[j * 2], w->cscale.y * (f32)c[j * 2 + 1], w->cscale.z);
            PSMTXTransApply(m, m, w->cpos.x, w->cpos.y, w->cpos.z);
            PSMTXTransApply(m, m, w->cscale.x * (f32)b[j * 2], w->cscale.y * (f32)b[j * 2 + 1], 0.0f);
            btlDispTexPlane2(m, BTL_TEX(0x41), w->col74, 0);
        }
    }
}

static void disp_3D_alpha(CameraId cam, void* param) {
    MukiWork* w = get_ptr();
    s32 i;
    MukiObj* o;
    Mtx m;
    GXColor col[4];
    for (i = 0; i < 9; i++) {
        o = &w->obj[i];
        if (o->kind >= 1 && o->kind <= 9) {
            f32 a;
            f32 c;
            switch (o->type) {
            case 0:
                PSMTXRotRad(m, 'y', 0.017453292f * o->rot.y);
                PSMTXScaleApply(m, m, 2.0f * o->scale.x, 2.0f * o->scale.y, o->scale.z);
                PSMTXTransApply(m, m, o->pos.x, o->pos.y, o->pos.z);
                c = 2.0f * o->rot.y;
                a = mulf(c, 3.141592f) / 360.0f;
                c = (f32)cos(a);
                PSMTXTransApply(m, m, 15.0f * (f32)sin(a), 0.0f, 15.0f * c);
                {
                col[0] = col[1] = (GXColor){0xE9, 0x4A, 0x3F, o->alpha};
                col[2] = col[3] = (GXColor){0xFE, 0xE6, 0x3E, o->alpha};
                btlDispTexPlane3(m, BTL_TEX(0x45), col[0], col[1], col[2], col[3], 0);
                }
                break;
            case 1: {

                PSMTXRotRad(m, 'y', 0.017453292f * o->rot.y);
                PSMTXScaleApply(m, m, 2.0f * o->scale.x, 2.0f * o->scale.y, o->scale.z);
                PSMTXTransApply(m, m, o->pos.x, o->pos.y, o->pos.z);
                c = 2.0f * o->rot.y;
                a = mulf(c, 3.141592f) / 360.0f;
                c = (f32)cos(a);
                PSMTXTransApply(m, m, 15.0f * (f32)sin(a), 0.0f, 15.0f * c);
                {
                col[0] = col[1] = (GXColor){0x3F, 0x3F, 0xE9, o->alpha};
                col[2] = col[3] = (GXColor){0x74, 0xC9, 0xFF, o->alpha};
                btlDispTexPlane3(m, BTL_TEX(0x45), col[0], col[1], col[2], col[3], 0);
                }
                }
break;
            case 2: {

                PSMTXRotRad(m, 'y', 0.017453292f * o->rot.y);
                PSMTXScaleApply(m, m, o->scale.x, o->scale.y, o->scale.z);
                PSMTXTransApply(m, m, o->pos.x, o->pos.y, o->pos.z);
                c = 2.0f * o->rot.y;
                a = mulf(c, 3.141592f) / 360.0f;
                c = (f32)cos(a);
                PSMTXTransApply(m, m, 15.0f * (f32)sin(a), 0.0f, 15.0f * c);
                {
                col[0] = (GXColor){0xFF, 0xFF, 0xFF, o->alpha};
                btlDispTexPlane2(m, BTL_TEX(0x31), col[0], 0);
                }
                }
break;
            }
        }
    }
}

static MukiWork* get_ptr(void) {
    return (MukiWork*)_battleWorkPointer->actionCommands.work;
}


static USER_FUNC(set_weapon) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 power = evtGetValue(event, args[1]);
    MukiWork* w = get_ptr();
    memcpy(w->weapon, &weapon, 0xc0);
    switch (type) {
    case 0:
        w->weapon[0x96] = 100;
        w->weapon[0x97] = 3;
        w->weapon[0x98] = power;
        break;
    case 1:
        w->weapon[0x99] = 100;
        w->weapon[0x9a] = 3;
        w->weapon[0x9b] = power;
        break;
    }
    evtSetValue(event, args[2], (EvtWord)w->weapon);
    return 2;
}

static USER_FUNC(start_game) {
    MukiWork* w = get_ptr();
    w->state = 1;
    return 2;
}

static USER_FUNC(wait_game_end) {
    MukiWork* w = get_ptr();
    EvtWord* args = event->args;
    if (w->state == 7) {
        evtSetValue(event, args[0], (s32)w->f14);
        evtSetValue(event, args[1], (s32)w->f1c);
        return 2;
    }
    return 0;
}

static USER_FUNC(star_stone_appear) {
    MukiWork* w = get_ptr();
    w->star = 1;
    return 2;
}

EVT_BEGIN(marioAttackEvent_Muki)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_PARTNER, LW(14))
    CALL(btlevtcmd_GetBodyId, LW(14), LW(15))
    CALL(btlevtcmd_CheckStatus, LW(14), 27, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), -1)
    END_IF
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(btlevtcmd_reset_move_color_lv_all)
    BEGIN_SIBLING_EVENT
        CALL(main_muki, LW(14))
    END_SIBLING_EVENT
    CALL_CHILD(sac_start_camera_evt)
    WAIT_MS(500)
    CALL(sac_enemy_slide_go)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_SAC_START1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL_CHILD(sac_mario_item_on_evt)
    WAIT_MS(500)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_SAC_START2"), EVT_NULLPTR, 0, LW(11))
    CALL(star_stone_appear)
    BEGIN_SIBLING_EVENT
        CALL_CHILD(sac_start_stardust)
    END_SIBLING_EVENT
    WAIT_FRAMES(100)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_SAC_START3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_snd_sfxoff, LW(11))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(40)
    CALL_CHILD(sac_mario_item_on_end_evt)
    CALL_CHILD(sac_mario_back_home)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_I_Y"))
    WAIT_MS(1000)
    BEGIN_SIBLING_EVENT
        CALL(sac_handbeat)
    END_SIBLING_EVENT
    CALL(sac_ac_help_on)
    CALL(start_game)
    CALL(wait_game_end, LW(0), LW(1))
    CALL(sac_ac_help_off)
    CALL(sac_handbeat_end)
    WAIT_MS(500)
    SET_VALUE(LW(2), LW(0))
    ADD(LW(2), LW(1))
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
        CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
        CALL(evt_btl_camera_set_zoom, 0, 200)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoomSpeedLv, 0, 2)
        WAIT_MS(1000)
        CALL(sac_wao)
        BEGIN_INLINE_EVENT
            WAIT_MS(1000)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            CALL(sac_enemy_slide_return)
            CALL(end_muki)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            WAIT_MS(400)
            IF_INT_NOT_EQUAL(LW(1), 0)
                CALL(set_weapon, 1, LW(1), LW(9))
                IF_INT_NOT_EQUAL(LW(14), -1)
                    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(14), LW(15), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
                END_IF
                CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_SELF, 1, LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
            END_IF
        END_INLINE_EVENT
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(set_weapon, 0, LW(0), LW(9))
            IF_INT_NOT_EQUAL(LW(14), -1)
                CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(14), LW(15), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
            END_IF
            CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_SELF, 1, LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
            RETURN
        END_IF
    END_IF
    CALL(sac_enemy_slide_return)
    WAIT_MS(1000)
    CALL(end_muki)
    WAIT_MS(1000)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()
