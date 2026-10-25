#include "eff/eff_starstone.h"
#include "icon_ids.h"
#include <dolphin/pad.h>
#pragma optimize_for_size on
#pragma dont_inline on
#include "battle/battle.h"
#include "battle/battle_audience.h"
#include "battle/battle_unit.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/icondrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "manager/filemgr.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <string.h>
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/sac/sac_common.h"
#include "party/party_motion.h"
#include "battle/sac/sac_bakugame.h"
#include "battle/battle_database_common.h"
#include "pmario_sound.h"

EffectEntry* effSandarsEntry(s32 type, f32 x, f32 y, f32 z, f32 w);
void btlGetScreenPoint(Vec* in, Vec* out);
void BattleAudienceSoundWhistleKind(s32 kind);
void BattleAudienceSoundCallKind(s32 kind);
void BattleAudienceSoundBooing(void);
void BattleAudienceSoundBooingKind(s32 kind);
void BattleAcDrawGauge(s32 x, s32 y, s32 icon, f32 ratio, s32 a, s32 b, s32 c, s32 d, s32 e);
void btl_camera_shake_h(s32 a, f32 b, f32 c, s32 d, s32 e);
void btlDispPoseAnime(BattleUnitPart* part);
s32 BtlUnit_snd_se_pos(BattleUnit* unit, const char* name, s32 vol, s32 pan, Vec* pos);
void psndSFX_pit(s32 id, s32 pitch);
s32 psndSFXChk(s32 id);
s32 BattleTransID(EventEntry* event, s32 type);

#define INTPL(mode, cur, steps, a, b) ((f32(*)(f32, f32, s32, s32, s32))intplGetValue)(a, b, mode, cur, steps)
#define GET_EXIST(i) ((u8(*)(s32))BattleAudience_GetExist)(i)
#define GET_WAITING(i) ((u8(*)(s32))BattleAudience_GetWaiting)(i)

typedef struct BakuBtn {
    s32 state;  // 0x0
    s32 timer;  // 0x4
    s32 button; // 0x8
    s32 target; // 0xC
    f32 hp;     // 0x10
    Vec pos;    // 0x14
    f32 scale;  // 0x20
} BakuBtn;

typedef struct BakuWork {
    u32 flags;          // 0x0
    s32 state;          // 0x4
    s32 timer;          // 0x8
    BakuBtn btn[3];     // 0xC
    s32 unk78;          // 0x78
    s32 s7c;            // 0x7C
    s32 s80;            // 0x80
    s32 s84;            // 0x84
    Vec pos;            // 0x88
    Vec from;           // 0x94
    Vec to;             // 0xA0
    f32 unkac;          // 0xAC
    f32 vy;             // 0xB0
    u8 padb4[0xC];      // 0xB4
    f32 c0;             // 0xC0
    f32 c4;             // 0xC4
    f32 c8;             // 0xC8
    u8 cc;              // 0xCC
    u8 cd;              // 0xCD
    u8 padce[2];        // 0xCE
    f32 gauge;          // 0xD0
    f32 vel;            // 0xD4
    f32 acc;            // 0xD8
    f32 dc;             // 0xDC
    f32 e0;             // 0xE0
    f32 e4;             // 0xE4
    f32 rate;           // 0xE8
    s32 poseId;         // 0xEC
    s32 f0;             // 0xF0
    s32 f4;             // 0xF4
    u8 weapon[0xC0];    // 0xF8
    s32 sfx1;           // 0x1B8
    s32 sfx2;           // 0x1BC
    s32 hstate;         // 0x1C0
    s32 htimer;         // 0x1C4
    s32 hsteps;         // 0x1C8
    s8 hflag;           // 0x1CC
    u8 pad1cd[3];       // 0x1CD
    Vec hpos;           // 0x1D0
    Vec hfrom;          // 0x1DC
    Vec hto;            // 0x1E8
    f32 unk1f4;         // 0x1F4
    f32 hvy;            // 0x1F8
    f32 unk1fc;         // 0x1FC
    s32 texIdx;         // 0x200
    s32 starState;      // 0x204
    s32 starTimer;      // 0x208
    Vec spos;           // 0x20C
    Vec sfrom;          // 0x218
    Vec sto;            // 0x224
    u8 pad230[0xC];     // 0x230
    Vec scale;          // 0x23C
    Vec rot;            // 0x248
    EffectEntry* eff;   // 0x254
    Vec mfrom;          // 0x258
    Vec mto;            // 0x264
    u8 pad270[4];       // 0x270
    f32 mvy;            // 0x274
    u8 pad278[0x298 - 0x278];
    f32 pvy;            // 0x298
    u8 pad29c[8];
    f32 audY;           // 0x2A4
    u8 pad2a8[8];
    f32 audVy;          // 0x2B0
    u8 pad2b4[4];
} BakuWork;

typedef struct StarEffWork {
    u8 pad0[8];
    Vec pos;  // 0x8
    f32 scl;  // 0x14
    Vec rot;  // 0x18
} StarEffWork;

typedef struct AudM {
    u32 flags;
    u8 pad4[0xE8 - 0x4];
    f32 rot;       // 0xE8
    u8 padec[0xF8 - 0xEC];
    u8 buf[0x34];  // 0xF8
} AudM;

typedef struct GaugeCol {
    u8 r, g, b, a;
    u8 pad;
    s8 dg, db;
} GaugeCol;

USER_FUNC(btlevtcmd_ACSuccessEffect);
USER_FUNC(btlevtcmd_AddRotate);
USER_FUNC(btlevtcmd_AnimeChangePoseType);
USER_FUNC(btlevtcmd_CheckDamage);
USER_FUNC(btlevtcmd_CheckPartsAttribute);
USER_FUNC(btlevtcmd_CheckStatus);
USER_FUNC(btlevtcmd_GetBodyId);
USER_FUNC(btlevtcmd_GetFaceDirection);
USER_FUNC(btlevtcmd_GetHitPos);
USER_FUNC(btlevtcmd_GetSelectEnemy);
USER_FUNC(btlevtcmd_GetSelectNextEnemy);
USER_FUNC(btlevtcmd_GetUnitId);
USER_FUNC(btlevtcmd_ResetFaceDirection);
USER_FUNC(btlevtcmd_SetRotate);
USER_FUNC(evt_audience_abnormal_natural_all);

BakuWork* GetBakuGamePtr(void);
static USER_FUNC(star_stone_appear);
static USER_FUNC(main_star);
USER_FUNC(init_bakugame);
USER_FUNC(end_bakugame);
USER_FUNC(bakuGameDecideWeapon);
USER_FUNC(bakuGameBombEntry);
USER_FUNC(bakuGameMain);
USER_FUNC(bakuGameHeihoReturn);
void bakuGameDecideButton(s32 first);
BOOL bakuGamePartyExist(BattleUnit* unit);
BOOL bakuGameAudienceCanThrowPos(s32 idx);
void bakuGameMarioSurpriseReset(BattleUnit* mario, BattleUnit* party);
void bakuGameEnemySurpriseReset(void);
void bakuGameAudienceSurpriseReset(void);
void bakuGameMarioSurprise(BattleUnit* mario, BattleUnit* party);
void bakuGameEnemySurprise(void);
void bakuGameAudienceSurprise(void);
void bakuGameDisp2D(CameraId cam, void* param);
void bakuGameDisp3D(CameraId cam, void* param);
void bakuGameDispStar(CameraId cam, void* param);

static BattleWeapon weapon = {
    "btl_wn_sac_bakugame", // name
    ICON_198, // icon
    {0}, // pad_06
    0, // itemId
    "msg_bakugame", // description
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
    0x2100060, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_3", // acHelpMsg
    0x20088, // special
    0x7FF, // counterResistFlags
    0, 0, 0, // targetWeightFlags..sleepTime
    100, // stopChance
    1, // stopTime
    0, 0, 0, 0, 0, 0, 0, 0, // dizzyChance..electricChance
    0, 0, 0, 0, 0, 0, 0, 0, // electricTime..sizeChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeTime..defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_BakuGame, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BakuWork* GetBakuGamePtr(void) {
    return (BakuWork*)_battleWorkPointer->actionCommands.work;
}

static USER_FUNC(star_stone_appear) {
    BakuWork* w = GetBakuGamePtr();
    w->starState = 1;
    return 2;
}

static USER_FUNC(main_star) {
    BakuWork* w = GetBakuGamePtr();
    BattleUnit* mario = BattleGetMarioPtr(_battleWorkPointer);

    switch (w->starState) {
    case 1:
        w->starState = 2;
        w->starTimer = 0;
        w->eff = effStarStoneEntry(1, 0.0f, -1000.0f, 0.0f, 1.0f);
        BtlUnit_GetPos(mario, &w->spos.x, &w->spos.y, &w->spos.z);
        w->spos.y += mario->mSizeMultiplier * (f32)mario->mHeight + 37.0f;
        w->sfrom = w->spos;
        w->sto = w->spos;
        w->sto.y += 50.0f;
        w->sto.z += -1.0f;
        w->scale = (Vec){0.0f, 0.0f, 0.0f};
        w->rot = (Vec){0.0f, 0.0f, 0.0f};
    case 2:
        w->starTimer++;
        if (w->starTimer <= 100) {
            w->spos.x = INTPL(0, w->starTimer, 100, w->sfrom.x, w->sto.x);
            w->spos.y = INTPL(0, w->starTimer, 100, w->sfrom.y, w->sto.y);
            w->spos.z = INTPL(0, w->starTimer, 100, w->sfrom.z, w->sto.z);
            w->scale.x = w->scale.y = w->scale.z = INTPL(0, w->starTimer, 100, 0.0f, 2.0f);
        } else {
            w->spos.x = w->sto.x;
            w->spos.y = w->sto.y;
            w->spos.z = w->sto.z;
            w->scale.x = w->scale.y = w->scale.z = 2.0f;
        }
        w->rot.y = INTPL(4, w->starTimer, 120, 0.0f, 2160.0f);
        if (w->starTimer >= 120) {
            w->starState = 3;
            w->starTimer = 0;
            w->sfrom = w->spos;
            w->sto.y = 300.0f;
        }
        break;
    case 3:
        w->starTimer++;
        w->spos.y = INTPL(1, w->starTimer, 60, w->sfrom.y, w->sto.y);
        if (w->starTimer >= 60) {
            w->starState = 4;
            w->starTimer = 0;
        }
        break;
    case 0:
    case 4:
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
    }
    dispEntry(4, 0.0f, 2, bakuGameDispStar, NULL);
    return 0;
}

USER_FUNC(init_bakugame) {
    memset(GetBakuGamePtr(), 0, 0x2B8);
    return 2;
}

USER_FUNC(end_bakugame) {
    BakuWork* w = GetBakuGamePtr();
    effDelete(((EffStarStoneWork*)w->eff->userdata)->dust);
    effDelete(w->eff);
    return 2;
}

s32 bakuGameDecideWeapon(EventEntry *event, BOOL isFirstCall)
{
  EvtWord *args = event->args;
  s32 v;
  BakuWork *w;
  u8 *q;
  BattleWeapon *wp;
  evtGetFloat(event, args[0]);
  v = evtGetValue(event, args[1]);
  w = GetBakuGamePtr();

  wp = (BattleWeapon *) w->weapon;
  q = &wp->sleepChance;
  memcpy(w->weapon, &weapon, 0xC0);

  q[2] = 100;
  if (v != (-1))
  {
    if (w->gauge >= 100.0f)
    {
      q[2] = 127;
      // FAKE: same-value second def of q (q == &wp->sleepChance already). It keeps q a named multi-def
      // pointer (addi r28) with no in-block copy. That keeps the q addi at height 1, so lis issues before
      // `mr w,r3` and takes r4, as in the target.
      q = &w->weapon[0x80];
    }
    else
    {
      q[2] = 100;
    }
    q[3] = v - 1;
  }
  else
  {
    q[2] = 0;
    q[3] = 0;
  }
  evtSetValue(event, args[2], (EvtWord) w->weapon);
  return 2;
}

USER_FUNC(bakuGameBombEntry) {
    BakuWork* w = GetBakuGamePtr();
    if (!animGroupBaseAsync("c_star_bom", 2, NULL)) {
        return 0;
    }
    w->poseId = animPoseEntry("c_star_bom", 2);
    animPoseSetAnim(w->poseId, "Z_1", 1);
    return 2;
}

USER_FUNC(bakuGameMain) {
    s32 unused[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    BakuWork* w = GetBakuGamePtr();
    EvtWord* args;
    BattleWork* bw = _battleWorkPointer;
    BattleUnit* mario = BattleGetUnitPtr(bw, BattleTransID(event, -3));
    BattleUnit* party = BattleGetUnitPtr(bw, BattleTransID(event, -4));
    f32 x = 0.0f, y = 0.0f, z = 0.0f;
    f32 hx = 0.0f, hy = 0.0f, hz = 0.0f;
    s32 btns[3] = {0x100, 0x200, 0x400};
    BakuBtn* b;
    f32 accel;
    f32 k;
    s32 flag;
    s32 i;
    s32 t;
    s32 n;
    BattleUnit* u;
    AudM* m;
    f32 v200 = -200.0f;
    f32 v100 = -100.0f;
    f32 frames = 40.0f;

    args = event->args;
    if (isFirstCall) {
        w->state = 0;
        bakuGameDecideButton(1);
        for (i = 0; i < 3; i++) {
            b = &w->btn[i];
            b->button = btns[i];
            b->hp = 100.0f;
            b->scale = 0.0f;
        }
        memcpy(w->weapon, &weapon, 0xC0);
        flag = 1;
        for (i = 0; i < 64; i++) {
            u = BattleGetUnitPtr(bw, i);
            if (u != NULL && u->mAlliance == 1) {
                if ((f32)u->mWidth >= 100.0f) {
                    flag = 1;
                    break;
                }
                if (!(u->mAttributeFlags & 2)) {
                    flag = 0;
                }
            }
        }
        if (!flag) {
            w->flags &= ~1;
        } else {
            w->flags |= 1;
        }
        flag = 0;
        for (i = 0; i < 200; i++) {
            if (bakuGameAudienceCanThrowPos(i) == 1 && GET_WAITING(i) == 1) {
                flag = 1;
            }
        }
        if (!flag) {
            w->flags |= 2;
        } else {
            w->flags &= ~2;
        }
    }

    switch (w->state) {
    case 0:
        for (i = 0; i < 3; i++) {
            if (w->btn[i].state != 5) {
                goto state_end;
            }
        }
        if (w->s7c != 8) {
            break;
        }
        if ((w->flags & 1) && w->hstate != 5) {
            break;
        }
        w->state = 5;
        break;
    case 5:
        w->state = 10;
        w->timer = 0;
        for (i = 0; i < 3; i++) {
            b = &w->btn[i];
            b->state = 10;
            b->timer = 0;
        }
        w->s7c = 10;
        w->f0 = irand(120) + 540;
    case 10:
        w->timer++;
        if (w->timer > 60 && irand(150) == 0) {
            for (i = 0; i < 3; i++) {
                w->btn[i].state = 10;
            }
            bakuGameDecideButton(0);
            w->timer = 0;
        }
        if (w->timer == 180) {
            w->timer = 180;
            for (i = 0; i < 3; i++) {
                w->btn[i].state = 10;
            }
            bakuGameDecideButton(0);
            w->timer = 0;
        }
        t = -1;
        if (w->cc == 1) {
            if (irand(2) == 0) {
                t = 0;
            } else {
                t = 2;
            }
        }
        if (w->cc == 2) {
            if (irand(2) == 0) {
                t = 0;
            } else {
                t = 1;
            }
        }
        if (t != -1) {
            for (i = 0; i < 3; i++) {
                b = &w->btn[i];
                if (b->target == t) {
                    b->hp -= VERSION_PICK(5.0f, 2.5f);
                }
            }
        }
        break;
    case 20:
        for (i = 0; i < 3; i++) {
            w->btn[i].state = 20;
        }
        w->s7c = 40;
        w->s80 = 60;
        w->state = 25;
        break;
    case 25:
        if (w->s7c == 50) {
            w->state = 30;
        }
        break;
    case 30:
        animPoseRelease(w->poseId);
        w->poseId = -1;
        evtSetValue(event, args[0], w->cc);
        evtSetFloat(event, args[1], w->gauge);
        t = -1;
        if (w->gauge > 0.0f && w->gauge < 33.0f) {
            t = 2;
        }
        if (w->gauge >= 33.0f && w->gauge < 66.0f) {
            t = 3;
        }
        if (w->gauge >= 66.0f) {
            t = 4;
        }
        n = 4;
        if (t < 4) {
            n = t;
        }
        evtSetValue(event, args[2], n);
        return 2;
    }
state_end:

    for (i = 0; i < 3; i++) {
        b = &w->btn[i];
        switch (b->state) {
        case 0:
            b->state = 5;
            break;
        case 10:
            b->state = 15;
            b->scale = 1.0f;
            break;
        case 12:
            b->scale = INTPL(4, b->timer, 30, 0.0f, 1.0f);
            b->timer++;
            if (b->timer > 30) {
                b->state = 15;
            }
            break;
        case 15:
            if ((keyGetButtonTrg(0) & b->button) && w->f4 == 0) {
                b->hp -= 100.0f;
            }
            break;
        case 20:
            b->pos = (Vec){0.0f, -1000.0f, 0.0f};
            break;
        }
    }

    switch (w->hstate) {
    case 0:
        if (!(w->flags & 1)) {
            break;
        }
        {
            Vec unusedv = {125.0f, 0.0f, -30.0f};
        }
        w->hstate = 2;
        w->hflag = -1;
        w->texIdx = 0x5A;
        w->hpos = (Vec){125.0f, 0.0f, 100.0f};
        w->hfrom = w->hpos;
        w->hpos.x = 300.0f;
        w->hfrom.x = 300.0f;
        w->hto = (Vec){125.0f, 0.0f, 100.0f};
        w->htimer = 0;
        w->hsteps = 30;
    case 2:
        w->hpos.x = INTPL(0, w->htimer, w->hsteps, w->hfrom.x, w->hto.x);
        {
            s32 r = w->htimer % 12;
            if (r >= 0 && r <= 3) {
                w->texIdx = 0x5A;
            }
            if (r >= 4 && r <= 7) {
                w->texIdx = 0x5B;
            }
            if (r >= 8 && r <= 11) {
                w->texIdx = 0x5C;
            }
        }
        w->htimer++;
        if (w->htimer > w->hsteps) {
            w->hstate = 5;
            w->hpos = w->hto;
        }
        break;
    case 5:
        w->texIdx = 0x52;
        break;
    case 10:
        w->texIdx = 0x54;
        break;
    case 15:
        break;
    }

    if (w->s7c >= 10 && w->s7c <= 20 && w->f0 <= 0) {
        w->state = 20;
        switch (w->cc) {
        case 0:
            bakuGameMarioSurpriseReset(mario, party);
            break;
        case 1:
            bakuGameEnemySurpriseReset();
            break;
        case 2:
            bakuGameAudienceSurpriseReset();
            break;
        }
    }

    if (w->s7c >= 10 && w->s7c <= 30) {
        if (keyGetButtonTrg(0) & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X)) {
            w->vel = 1.5f;
        }
        w->acc = -0.3f;
        w->gauge += w->vel;
        w->gauge += w->acc;
        if (w->gauge > 110.0f) {
            w->gauge = 110.0f;
        }
        if (w->gauge < 0.0f) {
            w->gauge = 0.0f;
            if (!psndSFXChk(w->sfx1)) {
                psndSFXOff(w->sfx1);
            }
        }
        if (w->gauge > 0.0f) {
            if (psndSFXChk(w->sfx1) == -1) {
                w->sfx1 = psndSFXOn("SFX_BTL_SAC_TIME1");
            }
        }
        if (!psndSFXChk(w->sfx1)) {
            psndSFX_pit(w->sfx1, 4096.0f * w->gauge / 100.0f);
        }
        w->vel -= 0.25f;
        if (w->vel < 0.0f) {
            w->vel = 0.0f;
        }
        w->rate = w->gauge >= 100.0f ? 1.0f : w->gauge / 100.0f;
        w->f0--;
        w->f4--;
        if (w->f4 < 0) {
            w->f4 = 0;
        }
        if (w->f0 == 480) {
            psndSFXOn("SFX_BTL_SAC_TIME4");
        }
        if (w->f0 >= 480 && w->f0 < 490) {
            w->c4 = w->c8 = INTPL(0, 489 - w->f0, 10, 2.0f, 3.0f);
        }
        if (w->f0 == 360) {
            psndSFXOn("SFX_BTL_SAC_TIME4");
        }
        if (w->f0 >= 360 && w->f0 < 370) {
            w->c4 = w->c8 = INTPL(0, 369 - w->f0, 10, 3.0f, 4.0f);
        }
        if (w->f0 == 240) {
            psndSFXOn("SFX_BTL_SAC_TIME4");
        }
        if (w->f0 >= 240 && w->f0 < 250) {
            w->c4 = w->c8 = INTPL(0, 249 - w->f0, 10, 4.0f, 5.0f);
        }
        if (w->f0 == 120) {
            psndSFXOn("SFX_BTL_SAC_TIME4");
        }
        if (w->f0 >= 120 && w->f0 < 130) {
            w->c4 = w->c8 = INTPL(0, 129 - w->f0, 10, 5.0f, 6.0f);
        }
        if (w->f0 == 599) {
            animPoseSetAnim(w->poseId, "S_1", 0);
        }
        if (w->f0 == 400) {
            animPoseSetAnim(w->poseId, "S_2", 0);
        }
        if (w->f0 == 200) {
            animPoseSetAnim(w->poseId, "S_3", 0);
        }
    }

    switch (w->s7c) {
    case 0:
        w->s7c = 2;
        w->e0 = 50.0f;
        w->s80 = 0;
        w->pos = (Vec){0.0f, -1000.0f, 0.0f};
        w->f0 = -1;
        w->c4 = w->c8 = 2.0f;
        w->c0 = 90.0f;
    case 2:
        w->dc = INTPL(4, w->s80, 30, -1000.0f, 50.0f);
        w->s80++;
        if (w->s80 > 30) {
            w->s7c = 4;
        }
        break;
    case 4:
        w->s7c = 5;
        w->pos = (Vec){-125.0f, 500.0f, -30.0f};
        w->from = w->pos;
        w->to = (Vec){-125.0f, 0.0f, -30.0f};
        w->vy = (2.0f * (w->to.y - w->from.y) - -0.5f * 40.0f * 40.0f) / (2.0f * 40.0f);
        w->s84 = w->s80 = 40;
    case 5:
        w->pos.y += w->vy;
        w->vy += -0.5f;
        ((0, w))->s80--;
        if (w->s80 < 0) {
            w->s7c = 8;
            w->pos.y = w->to.y;
            w->cc = 0;
            bakuGameMarioSurprise(mario, party);
            btl_camera_shake_h(0, 4.0f, 0.0f, 10, 4);
            psndSFXOn("SFX_BTL_SAC_TIME2");
            BtlUnit_snd_se_pos(mario, "SFX_VOICE_MARIO_SURPRISED1", 0x7F, 0, &mario->mPosition);
        }
        break;
    case 10:
        w->s7c = 11;
        w->s80 = 0;
        w->c0 = 70.0f;
    case 11:
        for (i = 0; i < 3; i++) {
            b = &w->btn[i];
            if (b->hp <= 0.0f) {
                w->s7c = 20;
                w->s80 = 0;
                w->cd = b->target;
#ifdef VERSION_G8ME01
                if (w->cc == w->cd) {
                    w->s7c = 30;
                }
#endif
                break;
            }
        }
        if (w->cc != 0) {
            break;
        }
        BtlUnit_GetPos(mario, &x, &y, &z);
        BtlUnit_GetHomePos(mario, &hx, &hy, &hz);
        mario->faceDirection = -1;
        if (y == hy) {
            if (irand(80) == 0) {
                w->mvy = -v200 / 20.0f;
                BtlUnit_SetPos(mario, x, 0.1f + y, z);
            }
        } else {
            y += w->mvy;
            w->mvy += -2.0f;
            if (y < hy) {
                y = hy;
            }
            BtlUnit_SetPos(mario, x, y, z);
        }
        if (bakuGamePartyExist(party) == 1) {
            BtlUnit_GetPos(party, &x, &y, &z);
            BtlUnit_GetHomePos(party, &hx, &hy, &hz);
            if (y == hy) {
                if (irand(80) == 0) {
                    w->pvy = -v200 / 20.0f;
                    BtlUnit_SetPos(party, x, 0.1f + y, z);
                }
            } else {
                y += w->pvy;
                w->pvy += -2.0f;
                if (y < hy) {
                    y = hy;
                }
                BtlUnit_SetPos(party, x, y, z);
            }
        }
        break;
    case 20:
#ifndef VERSION_G8ME01
        if (w->cc == w->cd) {
            w->s7c = 30;
            break;
        }
#endif
        switch (w->cc) {
        case 0:
            if (w->s80 == 0) {
                BtlUnit_GetPos(mario, &x, &y, &z);
                BtlUnit_GetHomePos(mario, &hx, &hy, &hz);
                BtlUnit_SetPos(mario, hx, hy, hz);
                w->mfrom = (Vec){x, hy, z};
                w->mto = (Vec){w->pos.x, hy, 7.0f + w->pos.z};
                mario->faceDirection = -1;
                if (bakuGamePartyExist(party) == 1) {
                    BtlUnit_GetPos(party, &x, &y, &z);
                    BtlUnit_GetHomePos(party, &hx, &hy, &hz);
                    BtlUnit_SetPos(party, hx, hy, hz);
                }
                BtlUnit_SetBodyAnim(mario, "M_W_1");
            }
            if (w->s80 >= 0 && w->s80 < 8) {
                x = INTPL(0, w->s80, 7, w->mfrom.x, w->mto.x);
                y = w->mfrom.y;
                z = INTPL(0, w->s80, 7, w->mfrom.z, w->mto.z);
                BtlUnit_SetPos(mario, x, y, z);
            }
            if (w->s80 == 8) {
                BtlUnit_SetBodyAnim(mario, "M_U_2");
                mario->faceDirection = 1;
                w->to = w->pos;
                w->from = w->to;
                w->to.y += 30.0f * mario->mSizeMultiplier;
            }
            if (w->s80 >= 8 && w->s80 < 13) {
                w->pos.y = INTPL(4, w->s80 - 8, 4, w->from.y, w->to.y);
            }
            if (w->s80 > 15) {
                w->s7c = 22;
            }
            break;
        case 1:
            if (!(w->flags & 1)) {
                if (w->s80 == 26) {
                    w->hvy = -v100 / 20.0f;
                    w->hpos.y = 0.0f;
                }
                if (w->s80 >= 26) {
                    w->hpos.y += w->hvy;
                    w->hvy += -1.0f;
                }
                if (w->s80 > 30) {
                    w->s7c = 22;
                }
                for (i = 0; i < 64; i++) {
                    u = BattleGetUnitPtr(bw, i);
                    if (u != NULL && !(u->mAttributeFlags & 0x20000) && !BtlUnit_CheckStatus(u, 27) &&
                        u->mAlliance == 1 && BtlUnit_CanActStatus(u)) {
                        if (w->s80 == 0) {
                            BtlUnit_SetBodyAnimType(u, 0x41);
                        }
                        if (w->s80 == 26) {
                            btlDispPoseAnime(BtlUnit_GetPartsPtr(u, BtlUnit_GetBodyPartsId(u)));
                            if (!(u->mAttributeFlags & 4) && !(u->mAttributeFlags & 2)) {
                                BtlUnit_GetHomePos(u, &hx, &hy, &hz);
                                BtlUnit_SetPos(u, hx, hy, hz);
                            }
                        }
                        if (w->s80 >= 26 && !(u->mAttributeFlags & 4) && !(u->mAttributeFlags & 2)) {
                            BtlUnit_GetHomePos(u, &hx, &hy, &hz);
                            BtlUnit_SetPos(u, hx, hy + w->hpos.y, hz);
                        }
                    }
                }
            } else {
                w->hstate = 15;
                if (w->s80 == 26) {
                    w->hvy = -v200 / 20.0f;
                    accel = -2.0f;
                }
                if (w->s80 >= 26) {
                    w->hpos.y += w->hvy;
                    w->hvy += accel;
                    w->pos.y = 30.0f + w->hpos.y;
                }
                if (w->s80 > 30) {
                    w->s7c = 22;
                }
            }
            break;
        case 2:
            if (w->cd == 1) {
                k = 2.0f;
            } else {
                k = 1.0f;
            }
            if (w->flags & 2) {
                w->s7c = 30;
                goto main_end;
            }
            if ((f32)w->s80 == 26.0f / k) {
                w->audY = 0.0f;
                w->audVy = -v200 / 20.0f;
            }
            if ((f32)w->s80 >= 26.0f / k) {
                w->audY += w->audVy;
                w->audVy += -2.0f;
            }
            if ((f32)w->s80 > 30.0f / k) {
                w->s7c = 22;
            }
            for (i = 0; i < 200; i++) {
                m = (AudM*)BattleAudienceGetPtr(i);
                if (bakuGameAudienceCanThrowPos(i) == 1 && GET_EXIST(i) && (m->flags & 0x10)) {
                    BattleAudience_GetHomePosition(i, &x, &y, &z);
                    BattleAudience_SetPosition(i, x, y + w->audY, z);
                    w->pos.y = 20.0f + (y + w->audY);
                }
            }
            break;
        }
        w->s80++;
        if (keyGetButtonTrg(0) & PAD_BUTTON_X) {
            w->cd = w->btn[2].target;
        }
        if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
            w->cd = w->btn[1].target;
        }
        if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
            w->cd = w->btn[0].target;
        }
        break;
    case 22:
        switch (w->cc) {
        case 0:
            w->pos = (Vec){-125.0f, 0.0f, -30.0f};
            w->from = w->pos;
            w->from.y += 30.0f;
            w->pos.y += 30.0f;
            bakuGameMarioSurpriseReset(mario, party);
            BtlUnit_SetBodyAnim(mario, "M_C_1");
            break;
        case 1:
            w->pos = (Vec){125.0f, 0.0f, -30.0f};
            w->from = w->pos;
            if (w->flags & 1) {
                w->pos = w->hpos;
                w->from = w->pos;
                w->from.y = w->pos.y = 30.0f + w->hpos.y;
            }
            bakuGameEnemySurpriseReset();
            break;
        case 2:
            w->pos = (Vec){0.0f, -50.0f, 150.0f};
            w->from = w->pos;
            w->from.y += 30.0f;
            w->pos.y += 20.0f;
            bakuGameAudienceSurpriseReset();
            break;
        }
        switch (w->cd) {
        case 0:
            w->to = (Vec){-125.0f, 0.0f, -30.0f};
            bakuGameMarioSurprise(mario, party);
            break;
        case 1:
            w->to = (Vec){125.0f, 0.0f, -30.0f};
            if (w->flags & 1) {
                w->to = (Vec){125.0f, 0.0f, 100.0f};
                w->to.y += 30.0f;
            }
            bakuGameEnemySurprise();
            break;
        case 2:
            if (w->flags & 2) {
                w->to = (Vec){0.0f, -50.0f, 150.0f};
            } else {
                w->to = (Vec){0.0f, -50.0f, 150.0f};
                w->to.y += 20.0f;
            }
            bakuGameAudienceSurprise();
            break;
        }
        w->s7c = 25;
        w->vy = (2.0f * (w->to.y - w->from.y) - -0.5f * 40.0f * 40.0f) / (2.0f * 40.0f);
        w->s80 = w->s84 = (s32)frames;
        if (w->cc == 0) {
            BtlUnit_GetPos(mario, &x, &y, &z);
            w->mfrom = (Vec){x, y, z};
            BtlUnit_GetHomePos(mario, &x, &y, &z);
            w->mto = (Vec){x, y, z};
        }
        w->sfx2 = psndSFXOn("SFX_BTL_SAC_TIME3");
        n = BattleAudience_GetAudienceNum();
        switch (w->cd) {
        case 0:
            BattleAudienceSoundBooing();
            break;
        case 1:
            if (n >= 1 && n <= 49) {
                BattleAudienceSoundCallKind(1);
            }
            if (n >= 50 && n <= 99) {
                BattleAudienceSoundCallKind(1);
                BattleAudienceSoundWhistleKind(1);
            }
            if (n >= 100 && n <= 149) {
                BattleAudienceSoundCallKind(2);
                BattleAudienceSoundWhistleKind(2);
            }
            if (n >= 150) {
                BattleAudienceSoundCallKind(2);
                BattleAudienceSoundWhistleKind(3);
            }
            break;
        case 2:
            if (n >= 1 && n <= 49) {
                BattleAudienceSoundBooingKind(5);
            }
            if (n >= 50 && n <= 99) {
                BattleAudienceSoundBooingKind(6);
            }
            if (n >= 100 && n <= 149) {
                BattleAudienceSoundBooingKind(7);
            }
            if (n >= 150) {
                BattleAudienceSoundBooingKind(8);
            }
            break;
        }
    case 25:
        w->pos.x = INTPL(0, w->s84 - w->s80, w->s84, w->from.x, w->to.x);
        w->pos.z = INTPL(0, w->s84 - w->s80, w->s84, w->from.z, w->to.z);
        w->pos.y += w->vy;
        w->vy += -0.5f;
        switch (w->cc) {
        case 0:
            if (w->s84 - w->s80 == 20) {
                BtlUnit_SetBodyAnim(mario, "M_W_1");
            }
            t = w->s84 - w->s80;
            if (t >= 20 && t <= 34) {
                x = INTPL(0, t - 20, 14, w->mfrom.x, w->mto.x);
                y = w->mfrom.y;
                z = INTPL(0, w->s84 - w->s80 - 20, 14, w->mfrom.z, w->mto.z);
                BtlUnit_SetPos(mario, x, y, z);
            }
            if (w->s84 - w->s80 == 34) {
                btlDispPoseAnime(BtlUnit_GetPartsPtr(mario, BtlUnit_GetBodyPartsId(mario)));
            }
            break;
        case 1:
            if (w->hpos.y != 0.0) {
                w->hpos.y += w->hvy;
                w->hvy += -1.0f;
            }
            if (w->hpos.y < 0.0f) {
                w->hpos.y = 0.0f;
            }
            if (!(w->flags & 1)) {
                for (i = 0; i < 64; i++) {
                    u = BattleGetUnitPtr(bw, i);
                    if (u != NULL && u->mAlliance == 1 && BtlUnit_CanActStatus(u) &&
                        !(u->mAttributeFlags & 0x20000) && !BtlUnit_CheckStatus(u, 27) &&
                        !(u->mAttributeFlags & 4) && !(u->mAttributeFlags & 2)) {
                        BtlUnit_GetHomePos(u, &hx, &hy, &hz);
                        BtlUnit_SetPos(u, hx, hy + w->hpos.y, hz);
                    }
                }
            } else {
                w->hstate = 15;
                if (w->hpos.y != 0.0f) {
                    w->hpos.y += w->hvy;
                    w->hvy += -2.0f;
                }
                if (w->hpos.y <= 0.0f) {
                    w->hpos.y = 0.0f;
                    w->hstate = 5;
                }
            }
            break;
        case 2:
            if (w->audY != 0.0f) {
                w->audY += w->audVy;
                w->audVy += -2.0f;
            }
            if (w->audY <= 0.0f) {
                w->audY = 0.0f;
            }
            for (i = 0; i < 200; i++) {
                m = (AudM*)BattleAudienceGetPtr(i);
                if (bakuGameAudienceCanThrowPos(i) == 1 && GET_EXIST(i) && (m->flags & 0x10)) {
                    BattleAudience_GetHomePosition(i, &x, &y, &z);
                    BattleAudience_SetPosition(i, x, y + w->audY, z);
                }
            }
            break;
        }
        ((0, w))->s80--;
        if (w->s80 < 0) {
            w->s7c = 30;
            w->pos = w->to;
            w->cc = w->cd;
            btl_camera_shake_h(0, w->c4, 0.0f, 10, 4);
            psndSFXOn("SFX_BTL_SAC_TIME2");
            if (!psndSFXChk(w->sfx2)) {
                psndSFXOff(w->sfx2);
            }
        }
        break;
    case 30:
        w->s7c = 10;
        for (i = 0; i < 3; i++) {
            w->btn[i].hp = 100.0f;
        }
        if (w->cc == 1 && (w->flags & 1)) {
            w->hstate = 10;
        }
        if (w->cd == 0) {
            BtlUnit_snd_se_pos(mario, "SFX_VOICE_MARIO_SURPRISED1", 0x7F, 0, &mario->mPosition);
        }
        break;
    case 40:
        effSandarsEntry(3, w->pos.x, w->pos.y, w->pos.z, 0.0f);
        btl_camera_shake_h(0, w->c4, 0.0f, 30, 4);
        psndSFXOn("SFX_BTL_SAC_TIME5");
        if (!psndSFXChk(w->sfx1)) {
            psndSFXOff(w->sfx1);
        }
        if (!psndSFXChk(w->sfx2)) {
            psndSFXOff(w->sfx2);
        }
        w->s7c = 50;
        for (i = 0; i < 200; i++) {
            m = (AudM*)BattleAudienceGetPtr(i);
            m->flags &= ~0x10;
            if (bakuGameAudienceCanThrowPos(i) == 1) {
                BattleAudience_GetHomePosition(i, &x, &y, &z);
                BattleAudience_SetPosition(i, x, y, z);
            }
        }
        break;
    case 50:
        break;
    }
main_end:
    if (w->poseId != -1) {
        animPoseMain(w->poseId);
    }
    dispEntry(8, 900.0f, 1, bakuGameDisp2D, NULL);
    dispEntry(4, 0.0f, 1, bakuGameDisp3D, NULL);
    return 0;
}

USER_FUNC(bakuGameHeihoReturn) {
    BakuWork* w = GetBakuGamePtr();

    if (!(w->flags & 1)) {
        return 2;
    }
    if (isFirstCall) {
        w->hstate = 0;
    }
    switch (w->hstate) {
    case 0:
        if (w->hpos.y != 0.0f) {
            w->hpos.y += w->hvy;
            w->hvy += -2.0f;
        }
        if (w->hpos.y <= 0.0f) {
            w->hpos.y = 0.0f;
            w->hstate = 5;
        }
        break;
    case 5:
        w->hstate = 10;
        w->hflag = 1;
        w->hfrom = w->hpos;
        {
            Vec v = {300.0f, 0.0f, 0.0f};
            v.z = w->hpos.z;
            w->hto = v;
        }
        w->htimer = 0;
        w->hsteps = 30;
    case 10:
        w->hpos.x = INTPL(0, w->htimer, w->hsteps, w->hfrom.x, w->hto.x);
        {
            s32 r = w->htimer % 12;
            if (r >= 0 && r <= 3) {
                w->texIdx = 0x5A;
            }
            if (r >= 4 && r <= 7) {
                w->texIdx = 0x5B;
            }
            if (r >= 8 && r <= 11) {
                w->texIdx = 0x5C;
            }
        }
        w->htimer++;
        if (w->htimer > w->hsteps) {
            return 2;
        }
        break;
    }
    dispEntry(4, 0.0f, 1, bakuGameDisp3D, NULL);
    return 0;
}

void bakuGameDecideButton(s32 first) {
    BakuWork* w = GetBakuGamePtr();
    Vec pos[3] = {
        {-125.0f, 50.0f, 0.0f},
        {125.0f, 50.0f, 0.0f},
        {0.0f, 10.0f, 150.0f},
    };
    s32 tbl[6][3] = {
        {0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0},
    };
    s32 r = irand(6);
    s32 i;

    for (i = 0; i < 3; i++) {
        BakuBtn* b = &w->btn[i];
        b->target = tbl[r][i];
        b->pos = pos[tbl[r][i]];
    }
    if (first == 0) {
        w->f4 = 60;
    }
}

BOOL bakuGamePartyExist(BattleUnit* unit) {
    if (unit == NULL) {
        return FALSE;
    }
    if (BtlUnit_CheckStatus(unit, 27)) {
        return FALSE;
    }
    return BtlUnit_CanActStatus(unit) != 0;
}

BOOL bakuGameAudienceCanThrowPos(s32 idx) {
    s32 i;
    for (i = 0; i < 3; i++) {
        if (idx >= i * 20 + 8 && idx <= i * 20 + 11) {
            return TRUE;
        }
    }
    return FALSE;
}

void bakuGameMarioSurpriseReset(BattleUnit* mario, BattleUnit* party) {
    btlDispPoseAnime(BtlUnit_GetPartsPtr(mario, BtlUnit_GetBodyPartsId(mario)));
    if (bakuGamePartyExist(party)) {
        btlDispPoseAnime(BtlUnit_GetPartsPtr(party, BtlUnit_GetBodyPartsId(party)));
    }
}

void bakuGameEnemySurpriseReset(void) {
    s32 i;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* u;

    for (i = 0; i < 64; i++) {
        u = BattleGetUnitPtr(wp, i);
        if (u != NULL && u->mAlliance == 1 && !(u->mAttributeFlags & 0x20000) && !BtlUnit_CheckStatus(u, 27)) {
            btlDispPoseAnime(BtlUnit_GetPartsPtr(u, BtlUnit_GetBodyPartsId(u)));
        }
    }
}

void bakuGameAudienceSurpriseReset(void) {
    s32 i;
    AudM* m;

    for (i = 0; i < 200; i++) {
        if (GET_EXIST(i)) {
            m = (AudM*)BattleAudienceGetPtr(i);
            m->rot = 90.0f;
            if (!bakuGameAudienceCanThrowPos(i)) {
                m->flags &= ~0x10;
            }
        }
    }
}

void bakuGameMarioSurprise(BattleUnit* mario, BattleUnit* party) {
    BtlUnit_SetBodyAnim(mario, "M_N_7");
    if (bakuGamePartyExist(party)) {
        switch (party->initialKind) {
        case 0xE0:
            BtlUnit_SetBodyAnim(party, "PKR_D_1");
            break;
        case 0xE1:
            BtlUnit_SetBodyAnim(party, "PNK_T_2");
            break;
        case 0xE3:
            BtlUnit_SetBodyAnim(party, "PWD_D_1");
            break;
        case 0xE2:
            BtlUnit_SetBodyAnim(party, "PYS_D_1");
            break;
        case 0xE4:
            BtlUnit_SetBodyAnim(party, "PTR_H_1");
            break;
        case 0xE5:
            BtlUnit_SetBodyAnim(party, "PBM_D_1");
            break;
        case 0xE6:
            BtlUnit_SetBodyAnim(party, "PCH_A2_3");
            break;
        }
    }
}

void bakuGameEnemySurprise(void) {
    s32 i;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* u;

    for (i = 0; i < 64; i++) {
        u = BattleGetUnitPtr(wp, i);
        if (u != NULL && u->mAlliance == 1 && !(u->mAttributeFlags & 0x20000) && !BtlUnit_CheckStatus(u, 27)) {
            BtlUnit_SetBodyAnimType(u, 0x27);
        }
    }
}

void bakuGameAudienceSurprise(void) {
    s32 i;
    AudM* m;

    for (i = 0; i < 200; i++) {
        m = (AudM*)BattleAudienceGetPtr(i);
        if (GET_WAITING(i)) {
            m->flags |= 0x10;
            memset(m->buf, 0, 0x34);
            if (!bakuGameAudienceCanThrowPos(i)) {
                BattleAudience_SetAnim(i, 7, 0);
                if (irand(2) == 0) {
                    m->rot = 90.0f;
                } else {
                    m->rot = 270.0f;
                }
            } else {
                BattleAudience_SetAnim(i, 3, 0);
            }
        }
    }
}

void bakuGameDisp2D(CameraId cam, void* param) {
    BakuWork* w = GetBakuGamePtr();
    GaugeCol* col = (GaugeCol*)&_battleWorkPointer->actionCommands.disp.color;
    BakuBtn* b;
    s32 i;
    u16 icon;
    s32 full;
    Vec scr;

    for (i = 0; i < 3; i++) {
        b = &w->btn[i];
        switch (b->button) {
        case 0x100:
            icon = ICON_064;
            break;
        case 0x200:
            icon = ICON_066;
            break;
        case 0x400:
            icon = ICON_068;
            break;
        default:
            icon = 0;
            break;
        }
        if (icon) {
            btlGetScreenPoint(&b->pos, &scr);
            iconDispGx(&scr, 0x10, icon, b->scale);
        }
    }
    full = 0;
    if (w->gauge >= 100.0f) {
        col->r = 0;
        full = 1;
        col->a = 0xFF;
        col->g += col->dg;
        col->b += col->db;
        if (col->g <= 0x80 && col->dg <= 0) {
            col->dg = col->db = 0x10;
        }
        if (col->g >= 0xF5 && col->dg >= 0) {
            col->dg = col->db = -0x10;
        }
    }
    BattleAcDrawGauge(w->dc, w->e0, 0xB1, w->rate, 3, 0x21, 0x42, 0x64, full);
}

#pragma dont_inline reset
void bakuGameDisp3D(CameraId cam, void* param) {
    BakuWork* w = GetBakuGamePtr();
    CameraEntry* camera = camGetPtr(4);
    Mtx scl;
    Mtx trans;
    Mtx mtx;
    GXTexObj tex;
    s32 o;

    PSMTXScale(scl, -1.0f, 1.0f, 1.0f);
    PSMTXTrans(trans, w->pos.x, w->pos.y, w->pos.z);
    PSMTXConcat(trans, scl, mtx);
    if (w->poseId != -1) {
        animPoseDrawMtx(w->poseId, mtx, 2, 180.0f, w->c4);
    }
    if (w->flags & 1) {
        GXSetCullMode(GX_CULL_NONE);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_REG, 0, GX_DF_CLAMP, GX_AF_NONE);
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){0, 0, 0, 0xFF});
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, 0x7D);
        GXSetNumTevStages(1);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, 15, 12, 8, 10);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        TEXGetGXTexObjFromPalette(*(TEXPalette**)BattleAudienceBaseGetPtr()->normalTex->data, &tex, w->texIdx);
        GXLoadTexObj(&tex, GX_TEXMAP0);
        PSMTXTrans(trans, w->hpos.x, w->hpos.y, w->hpos.z);
        PSMTXConcat(camera->view, trans, mtx);
        GXLoadPosMtxImm(mtx, 0);
        {
        f32 uv[16] = {
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        };
        o = (w->hflag == 1) ? 4 : 0;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-20.0f, 40.0f, 0.0f);
        GXTexCoord2f32(uv[o * 2], uv[o * 2 + 1]);
        GXPosition3f32(20.0f, 40.0f, 0.0f);
        GXTexCoord2f32(uv[(o + 1) * 2], uv[(o + 1) * 2 + 1]);
        GXPosition3f32(20.0f, 0.0f, 0.0f);
        GXTexCoord2f32(uv[(o + 2) * 2], uv[(o + 2) * 2 + 1]);
        GXPosition3f32(-20.0f, 0.0f, 0.0f);
        GXTexCoord2f32(uv[(o + 3) * 2], uv[(o + 3) * 2 + 1]);
        }
    }
}

void bakuGameDispStar(CameraId cam, void* param) {
}

EVT_BEGIN(marioAttackEvent_BakuGame)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(btlevtcmd_reset_move_color_lv_all)
    CALL(init_bakugame)
    BEGIN_SIBLING_EVENT
        CALL(main_star)
    END_SIBLING_EVENT
    CALL_CHILD(sac_start_camera_evt)
    WAIT_MS(500)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_SAC_START1"), -250000000, 0, -250000000)
    CALL_CHILD(sac_mario_item_on_evt)
    WAIT_MS(500)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_SAC_START2"), -250000000, 0, LW(11))
    CALL(star_stone_appear)
    BEGIN_SIBLING_EVENT
        CALL_CHILD(sac_start_stardust)
    END_SIBLING_EVENT
    WAIT_FRAMES(100)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_SAC_START3"), -250000000, 0, -250000000)
    CALL(evt_snd_sfxoff, LW(11))
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_FRAMES(40)
    CALL_CHILD(sac_mario_item_on_end_evt)
    CALL_CHILD(sac_mario_back_home)
    CALL(bakuGameBombEntry)
    CALL(sac_ac_help_on)
    CALL(bakuGameMain, LW(0), LW(1), LW(2))
    CALL(sac_ac_help_off)
    CALL(end_bakugame)
    BEGIN_INLINE_EVENT
        CALL(bakuGameHeihoReturn)
    END_INLINE_EVENT
    CALL(bakuGameDecideWeapon, LW(1), LW(2), LW(9))
    WAIT_FRAMES(5)
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL_CHILD(hitCheck_MarioTeam)
        CASE_END
        CASE_EQUAL(1)
        CALL_CHILD(hitCheck_EnemyTeam)
        CASE_END
        CASE_EQUAL(2)
        CALL_CHILD(hitCheck_AudienceTeam)
        CASE_END
    END_SWITCH
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(hitCheck_MarioTeam)
    CALL(btlevtcmd_GetUnitId, -4, LW(3))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(50)
    END_IF
    CALL(btlevtcmd_CheckStatus, LW(3), 27, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(50)
    END_IF
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
    CALL(btlevtcmd_ACSuccessEffect, LW(2), LW(10), LW(11), LW(12))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    LABEL(50)
    CALL(btlevtcmd_GetUnitId, -3, LW(3))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckStatus, LW(3), 27, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_GetFaceDirection, LW(3), LW(0))
    IF_INT_EQUAL(LW(0), -1)
        LOOP(5)
            CALL(btlevtcmd_AddRotate, LW(3), 0, 18, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 39)
        LOOP(5)
            CALL(btlevtcmd_AddRotate, LW(3), 0, 18, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_ResetFaceDirection, LW(3))
        CALL(btlevtcmd_SetRotate, LW(3), 0, 0, 0)
    END_IF
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
    IF_INT_NOT_EQUAL(LW(2), -1)
        CALL(btlevtcmd_ACSuccessEffect, LW(2), LW(10), LW(11), LW(12))
    END_IF
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(hitCheck_EnemyTeam)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(10)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
    CALL(btlevtcmd_CheckPartsAttribute, LW(3), LW(4), 16384, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        IF_INT_NOT_EQUAL(LW(2), -1)
            CALL(btlevtcmd_ACSuccessEffect, LW(2), LW(10), LW(11), LW(12))
        END_IF
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    END_IF
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        GOTO(10)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(hitCheck_AudienceTeam)
    CALL(evt_audience_abnormal_natural_all, 100)
    IF_INT_NOT_EQUAL(LW(2), -1)
        CALL(btlevtcmd_ACSuccessEffect, LW(2), 0, 100, 0)
    END_IF
    RETURN
EVT_END()
