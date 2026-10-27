#include "eff/eff_starstone.h"
#include "icon_ids.h"
#include <dolphin/pad.h>
#include "battle/battle_status_effect.h"
#include "eff/eff_sandars.h"
#include "battle/battle_sub.h"
#include "evt/evt_audience.h"
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
#include "battle/battle_disp.h"

void BattleAcDrawGauge(s32 x, s32 y, s32 width, f32 ratio, s32 level, s32 a, s32 b, s32 c, s32 flags);
void psndSFX_pit(s32 id, s32 pitch);


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
    s32 phase;          // 0x7C, step of the move (bakuGameMain switch)
    s32 frame;          // 0x80, frame counter of the current phase
    s32 frameTotal;     // 0x84, length of the current timed move
    Vec pos;            // 0x88
    Vec from;           // 0x94
    Vec to;             // 0xA0
    f32 unkac;          // 0xAC
    f32 vy;             // 0xB0
    u8 padb4[0xC];      // 0xB4
    f32 c0;             // 0xC0
    f32 bombScale;      // 0xC4, bomb draw scale and camera shake amount (2 -> 6 as the fuse burns)
    f32 c8;             // 0xC8
    u8 holder;          // 0xCC, who holds the bomb: 0 Mario, 1 an enemy, 2 the audience
    u8 throwTarget;     // 0xCD, target of the button whose gauge ran out
    u8 padce[2];        // 0xCE
    f32 gauge;          // 0xD0
    f32 vel;            // 0xD4
    f32 acc;            // 0xD8
    f32 gaugeX;         // 0xDC, gauge position (BattleAcDrawGauge)
    f32 gaugeY;         // 0xE0
    f32 e4;             // 0xE4
    f32 rate;           // 0xE8
    s32 poseId;         // 0xEC
    s32 fuseTimer;      // 0xF0, frames until the bomb goes off (540-659 at the start)
    s32 pressWait;      // 0xF4, frames before a button press counts again
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
    SacStarStone star;  // 0x204
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

typedef struct GaugeCol {
    u8 r, g, b, a;
    u8 pad;
    s8 dg, db;
} GaugeCol;


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
    ICON_SAC_BAKUGAME, // icon
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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_3", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_FREEZE_CRUSH | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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
    w->star.state = 1;
    return EVT_RETURN_DONE;
}

static USER_FUNC(main_star) {
    BakuWork* w = GetBakuGamePtr();
    BattleUnit* mario = BattleGetMarioPtr(_battleWorkPointer);

    switch (w->star.state) {
    case 1:
        w->star.state = 2;
        w->star.timer = 0;
        w->star.eff = effStarStoneEntry(1, 0.0f, -1000.0f, 0.0f, 1.0f);
        BtlUnit_GetPos(mario, &w->star.pos.x, &w->star.pos.y, &w->star.pos.z);
        w->star.pos.y += mario->mSizeMultiplier * (f32)mario->mHeight + 37.0f;
        w->star.from = w->star.pos;
        w->star.to = w->star.pos;
        w->star.to.y += 50.0f;
        w->star.to.z += -1.0f;
        w->star.scale = (Vec){0.0f, 0.0f, 0.0f};
        w->star.rot = (Vec){0.0f, 0.0f, 0.0f};
    case 2:
        w->star.timer++;
        if (w->star.timer <= 100) {
            w->star.pos.x = intplGetValue(EASING_LINEAR, w->star.from.x, w->star.to.x, w->star.timer, 100);
            w->star.pos.y = intplGetValue(EASING_LINEAR, w->star.from.y, w->star.to.y, w->star.timer, 100);
            w->star.pos.z = intplGetValue(EASING_LINEAR, w->star.from.z, w->star.to.z, w->star.timer, 100);
            w->star.scale.x = w->star.scale.y = w->star.scale.z = intplGetValue(EASING_LINEAR, 0.0f, 2.0f, w->star.timer, 100);
        } else {
            w->star.pos.x = w->star.to.x;
            w->star.pos.y = w->star.to.y;
            w->star.pos.z = w->star.to.z;
            w->star.scale.x = w->star.scale.y = w->star.scale.z = 2.0f;
        }
        w->star.rot.y = intplGetValue(EASING_QUADRATIC_OUT, 0.0f, 2160.0f, w->star.timer, 120);
        if (w->star.timer >= 120) {
            w->star.state = 3;
            w->star.timer = 0;
            w->star.from = w->star.pos;
            w->star.to.y = 300.0f;
        }
        break;
    case 3:
        w->star.timer++;
        w->star.pos.y = intplGetValue(EASING_QUADRATIC_IN, w->star.from.y, w->star.to.y, w->star.timer, 60);
        if (w->star.timer >= 60) {
            w->star.state = 4;
            w->star.timer = 0;
        }
        break;
    case 0:
    case 4:
        break;
    }
    if (w->star.eff) {
        EffStarStoneWork* e = w->star.eff->userdata;
        e->position.x = w->star.pos.x;
        e->position.y = w->star.pos.y;
        e->position.z = w->star.pos.z;
        e->rot.x = w->star.rot.x;
        e->rot.y = w->star.rot.y;
        e->rot.z = w->star.rot.z;
        e->scale = 1.5f * w->star.scale.x;
    }
    dispEntry(CAMERA_3D, 0.0f, 2, bakuGameDispStar, NULL);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(init_bakugame) {
    memset(GetBakuGamePtr(), 0, sizeof(BakuWork));
    return EVT_RETURN_DONE;
}

USER_FUNC(end_bakugame) {
    BakuWork* w = GetBakuGamePtr();
    effDelete(((EffStarStoneWork*)w->star.eff->userdata)->dust);
    effDelete(w->star.eff);
    return EVT_RETURN_DONE;
}

USER_FUNC(bakuGameDecideWeapon) {
    EvtWord* args = event->args;
    s32 v;
    BakuWork* w;
    u8* q;
    BattleWeapon* wp;
    evtGetFloat(event, args[0]);
    v = evtGetValue(event, args[1]);
    w = GetBakuGamePtr();

    wp = (BattleWeapon*)w->weapon;
    q = &wp->sleepChance;
    memcpy(w->weapon, &weapon, sizeof(BattleWeapon));

    q[2] = 100;
    if (v != -1) {
        if (w->gauge >= 100.0f) {
            q[2] = 127;
            // FAKE: same-value second def of q (q == &wp->sleepChance already). It keeps q a named multi-def
            // pointer (addi r28) with no in-block copy. That keeps the q addi at height 1, so lis issues before
            // `mr w,r3` and takes r4, as in the target.
            q = &w->weapon[0x80];
        } else {
            q[2] = 100;
        }
        q[3] = v - 1;
    } else {
        q[2] = 0;
        q[3] = 0;
    }
    evtSetValue(event, args[2], (EvtWord)w->weapon);
    return EVT_RETURN_DONE;
}

USER_FUNC(bakuGameBombEntry) {
    BakuWork* w = GetBakuGamePtr();
    if (!animGroupBaseAsync("c_star_bom", 2, NULL)) {
        return EVT_RETURN_BLOCK;
    }
    w->poseId = animPoseEntry("c_star_bom", 2);
    animPoseSetAnim(w->poseId, "Z_1", 1);
    return EVT_RETURN_DONE;
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
    s32 btns[3] = {PAD_BUTTON_A, PAD_BUTTON_B, PAD_BUTTON_X};
    BakuBtn* b;
    f32 accel;
    f32 k;
    s32 flag;
    s32 i;
    s32 t;
    s32 n;
    BattleUnit* u;
    BattleAudienceMember* m;
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
        memcpy(w->weapon, &weapon, sizeof(BattleWeapon));
        flag = 1;
        for (i = 0; i < 64; i++) {
            u = BattleGetUnitPtr(bw, i);
            if (u != NULL && u->mAlliance == ALLIANCE_ENEMY) {
                if ((f32)u->mWidth >= 100.0f) {
                    flag = 1;
                    break;
                }
                if (!(u->mAttributeFlags & kCeiling)) {
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
            if (bakuGameAudienceCanThrowPos(i) == 1 && BattleAudience_GetWaiting(i) == 1) {
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
        if (w->phase != 8) {
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
        w->phase = 10;
        w->fuseTimer = irand(120) + 540;
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
        if (w->holder == 1) {
            if (irand(2) == 0) {
                t = 0;
            } else {
                t = 2;
            }
        }
        if (w->holder == 2) {
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
        w->phase = 40;
        w->frame = 60;
        w->state = 25;
        break;
    case 25:
        if (w->phase == 50) {
            w->state = 30;
        }
        break;
    case 30:
        animPoseRelease(w->poseId);
        w->poseId = -1;
        evtSetValue(event, args[0], w->holder);
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
        return EVT_RETURN_DONE;
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
            b->scale = intplGetValue(EASING_QUADRATIC_OUT, 0.0f, 1.0f, b->timer, 30);
            b->timer++;
            if (b->timer > 30) {
                b->state = 15;
            }
            break;
        case 15:
            if ((keyGetButtonTrg(0) & b->button) && w->pressWait == 0) {
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
        w->hpos.x = intplGetValue(EASING_LINEAR, w->hfrom.x, w->hto.x, w->htimer, w->hsteps);
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

    if (w->phase >= 10 && w->phase <= 20 && w->fuseTimer <= 0) {
        w->state = 20;
        switch (w->holder) {
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

    if (w->phase >= 10 && w->phase <= 30) {
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
        w->fuseTimer--;
        w->pressWait--;
        if (w->pressWait < 0) {
            w->pressWait = 0;
        }
        if (w->fuseTimer == 480) {
            psndSFXOn("SFX_BTL_SAC_TIME4");
        }
        if (w->fuseTimer >= 480 && w->fuseTimer < 490) {
            w->bombScale = w->c8 = intplGetValue(EASING_LINEAR, 2.0f, 3.0f, 489 - w->fuseTimer, 10);
        }
        if (w->fuseTimer == 360) {
            psndSFXOn("SFX_BTL_SAC_TIME4");
        }
        if (w->fuseTimer >= 360 && w->fuseTimer < 370) {
            w->bombScale = w->c8 = intplGetValue(EASING_LINEAR, 3.0f, 4.0f, 369 - w->fuseTimer, 10);
        }
        if (w->fuseTimer == 240) {
            psndSFXOn("SFX_BTL_SAC_TIME4");
        }
        if (w->fuseTimer >= 240 && w->fuseTimer < 250) {
            w->bombScale = w->c8 = intplGetValue(EASING_LINEAR, 4.0f, 5.0f, 249 - w->fuseTimer, 10);
        }
        if (w->fuseTimer == 120) {
            psndSFXOn("SFX_BTL_SAC_TIME4");
        }
        if (w->fuseTimer >= 120 && w->fuseTimer < 130) {
            w->bombScale = w->c8 = intplGetValue(EASING_LINEAR, 5.0f, 6.0f, 129 - w->fuseTimer, 10);
        }
        if (w->fuseTimer == 599) {
            animPoseSetAnim(w->poseId, "S_1", 0);
        }
        if (w->fuseTimer == 400) {
            animPoseSetAnim(w->poseId, "S_2", 0);
        }
        if (w->fuseTimer == 200) {
            animPoseSetAnim(w->poseId, "S_3", 0);
        }
    }

    switch (w->phase) {
    case 0:
        w->phase = 2;
        w->gaugeY = 50.0f;
        w->frame = 0;
        w->pos = (Vec){0.0f, -1000.0f, 0.0f};
        w->fuseTimer = -1;
        w->bombScale = w->c8 = 2.0f;
        w->c0 = 90.0f;
    case 2:
        w->gaugeX = intplGetValue(EASING_QUADRATIC_OUT, -1000.0f, 50.0f, w->frame, 30);
        w->frame++;
        if (w->frame > 30) {
            w->phase = 4;
        }
        break;
    case 4:
        w->phase = 5;
        w->pos = (Vec){-125.0f, 500.0f, -30.0f};
        w->from = w->pos;
        w->to = (Vec){-125.0f, 0.0f, -30.0f};
        w->vy = (2.0f * (w->to.y - w->from.y) - -0.5f * 40.0f * 40.0f) / (2.0f * 40.0f);
        w->frameTotal = w->frame = 40;
    case 5:
        w->pos.y += w->vy;
        w->vy += -0.5f;
        w->frame--;
        if (w->frame < 0) {
            w->phase = 8;
            w->pos.y = w->to.y;
            w->holder = 0;
            bakuGameMarioSurprise(mario, party);
            btl_camera_shake_h(0, 4.0f, 0.0f, 10, 4);
            psndSFXOn("SFX_BTL_SAC_TIME2");
            BtlUnit_snd_se_pos(mario, "SFX_VOICE_MARIO_SURPRISED1", 0x7F, 0, &mario->mPosition);
        }
        break;
    case 10:
        w->phase = 11;
        w->frame = 0;
        w->c0 = 70.0f;
    case 11:
        for (i = 0; i < 3; i++) {
            b = &w->btn[i];
            if (b->hp <= 0.0f) {
                w->phase = 20;
                w->frame = 0;
                w->throwTarget = b->target;
#ifdef VERSION_G8ME01
                if (w->holder == w->throwTarget) {
                    w->phase = 30;
                }
#endif
                break;
            }
        }
        if (w->holder != 0) {
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
        if (w->holder == w->throwTarget) {
            w->phase = 30;
            break;
        }
#endif
        switch (w->holder) {
        case 0:
            if (w->frame == 0) {
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
            if (w->frame >= 0 && w->frame < 8) {
                x = intplGetValue(EASING_LINEAR, w->mfrom.x, w->mto.x, w->frame, 7);
                y = w->mfrom.y;
                z = intplGetValue(EASING_LINEAR, w->mfrom.z, w->mto.z, w->frame, 7);
                BtlUnit_SetPos(mario, x, y, z);
            }
            if (w->frame == 8) {
                BtlUnit_SetBodyAnim(mario, "M_U_2");
                mario->faceDirection = 1;
                w->to = w->pos;
                w->from = w->to;
                w->to.y += 30.0f * mario->mSizeMultiplier;
            }
            if (w->frame >= 8 && w->frame < 13) {
                w->pos.y = intplGetValue(EASING_QUADRATIC_OUT, w->from.y, w->to.y, w->frame - 8, 4);
            }
            if (w->frame > 15) {
                w->phase = 22;
            }
            break;
        case 1:
            if (!(w->flags & 1)) {
                if (w->frame == 26) {
                    w->hvy = -v100 / 20.0f;
                    w->hpos.y = 0.0f;
                }
                if (w->frame >= 26) {
                    w->hpos.y += w->hvy;
                    w->hvy += -1.0f;
                }
                if (w->frame > 30) {
                    w->phase = 22;
                }
                for (i = 0; i < 64; i++) {
                    u = BattleGetUnitPtr(bw, i);
                    if (u != NULL && !(u->mAttributeFlags & kInactive) && !BtlUnit_CheckStatus(u, STATUS_INSTANT_KILL) &&
                        u->mAlliance == ALLIANCE_ENEMY && BtlUnit_CanActStatus(u)) {
                        if (w->frame == 0) {
                            BtlUnit_SetBodyAnimType(u, POSE_TYPE_TALK);
                        }
                        if (w->frame == 26) {
                            btlDispPoseAnime(BtlUnit_GetPartsPtr(u, BtlUnit_GetBodyPartsId(u)));
                            if (!(u->mAttributeFlags & kFloating) && !(u->mAttributeFlags & kCeiling)) {
                                BtlUnit_GetHomePos(u, &hx, &hy, &hz);
                                BtlUnit_SetPos(u, hx, hy, hz);
                            }
                        }
                        if (w->frame >= 26 && !(u->mAttributeFlags & kFloating) && !(u->mAttributeFlags & kCeiling)) {
                            BtlUnit_GetHomePos(u, &hx, &hy, &hz);
                            BtlUnit_SetPos(u, hx, hy + w->hpos.y, hz);
                        }
                    }
                }
            } else {
                w->hstate = 15;
                if (w->frame == 26) {
                    w->hvy = -v200 / 20.0f;
                    accel = -2.0f;
                }
                // BUG: accel is set only on frame 26; on the later frames (later calls of this function) it is read
                // uninitialized.
                if (w->frame >= 26) {
                    w->hpos.y += w->hvy;
                    w->hvy += accel;
                    w->pos.y = 30.0f + w->hpos.y;
                }
                if (w->frame > 30) {
                    w->phase = 22;
                }
            }
            break;
        case 2:
            if (w->throwTarget == 1) {
                k = 2.0f;
            } else {
                k = 1.0f;
            }
            if (w->flags & 2) {
                w->phase = 30;
                goto main_end;
            }
            if ((f32)w->frame == 26.0f / k) {
                w->audY = 0.0f;
                w->audVy = -v200 / 20.0f;
            }
            if ((f32)w->frame >= 26.0f / k) {
                w->audY += w->audVy;
                w->audVy += -2.0f;
            }
            if ((f32)w->frame > 30.0f / k) {
                w->phase = 22;
            }
            for (i = 0; i < 200; i++) {
                m = BattleAudienceGetPtr(i);
                if (bakuGameAudienceCanThrowPos(i) == 1 && (u8)BattleAudience_GetExist(i) && (m->flags & AUDIENCE_MEMBER_FLAG_NO_SYS_CTRL)) {
                    BattleAudience_GetHomePosition(i, &x, &y, &z);
                    BattleAudience_SetPosition(i, x, y + w->audY, z);
                    w->pos.y = 20.0f + (y + w->audY);
                }
            }
            break;
        }
        w->frame++;
        if (keyGetButtonTrg(0) & PAD_BUTTON_X) {
            w->throwTarget = w->btn[2].target;
        }
        if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
            w->throwTarget = w->btn[1].target;
        }
        if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
            w->throwTarget = w->btn[0].target;
        }
        break;
    case 22:
        switch (w->holder) {
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
        switch (w->throwTarget) {
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
        w->phase = 25;
        w->vy = (2.0f * (w->to.y - w->from.y) - -0.5f * 40.0f * 40.0f) / (2.0f * 40.0f);
        w->frame = w->frameTotal = (s32)frames;
        if (w->holder == 0) {
            BtlUnit_GetPos(mario, &x, &y, &z);
            w->mfrom = (Vec){x, y, z};
            BtlUnit_GetHomePos(mario, &x, &y, &z);
            w->mto = (Vec){x, y, z};
        }
        w->sfx2 = psndSFXOn("SFX_BTL_SAC_TIME3");
        n = BattleAudience_GetAudienceNum();
        switch (w->throwTarget) {
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
        w->pos.x = intplGetValue(EASING_LINEAR, w->from.x, w->to.x, w->frameTotal - w->frame, w->frameTotal);
        w->pos.z = intplGetValue(EASING_LINEAR, w->from.z, w->to.z, w->frameTotal - w->frame, w->frameTotal);
        w->pos.y += w->vy;
        w->vy += -0.5f;
        switch (w->holder) {
        case 0:
            if (w->frameTotal - w->frame == 20) {
                BtlUnit_SetBodyAnim(mario, "M_W_1");
            }
            t = w->frameTotal - w->frame;
            if (t >= 20 && t <= 34) {
                x = intplGetValue(EASING_LINEAR, w->mfrom.x, w->mto.x, t - 20, 14);
                y = w->mfrom.y;
                z = intplGetValue(EASING_LINEAR, w->mfrom.z, w->mto.z, w->frameTotal - w->frame - 20, 14);
                BtlUnit_SetPos(mario, x, y, z);
            }
            if (w->frameTotal - w->frame == 34) {
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
                    if (u != NULL && u->mAlliance == ALLIANCE_ENEMY && BtlUnit_CanActStatus(u) &&
                        !(u->mAttributeFlags & kInactive) && !BtlUnit_CheckStatus(u, STATUS_INSTANT_KILL) &&
                        !(u->mAttributeFlags & kFloating) && !(u->mAttributeFlags & kCeiling)) {
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
                m = BattleAudienceGetPtr(i);
                if (bakuGameAudienceCanThrowPos(i) == 1 && (u8)BattleAudience_GetExist(i) && (m->flags & AUDIENCE_MEMBER_FLAG_NO_SYS_CTRL)) {
                    BattleAudience_GetHomePosition(i, &x, &y, &z);
                    BattleAudience_SetPosition(i, x, y + w->audY, z);
                }
            }
            break;
        }
        w->frame--;
        if (w->frame < 0) {
            w->phase = 30;
            w->pos = w->to;
            w->holder = w->throwTarget;
            btl_camera_shake_h(0, w->bombScale, 0.0f, 10, 4);
            psndSFXOn("SFX_BTL_SAC_TIME2");
            if (!psndSFXChk(w->sfx2)) {
                psndSFXOff(w->sfx2);
            }
        }
        break;
    case 30:
        w->phase = 10;
        for (i = 0; i < 3; i++) {
            w->btn[i].hp = 100.0f;
        }
        if (w->holder == 1 && (w->flags & 1)) {
            w->hstate = 10;
        }
        if (w->throwTarget == 0) {
            BtlUnit_snd_se_pos(mario, "SFX_VOICE_MARIO_SURPRISED1", 0x7F, 0, &mario->mPosition);
        }
        break;
    case 40:
        effSandarsEntry(3, w->pos.x, w->pos.y, w->pos.z, 0.0f);
        btl_camera_shake_h(0, w->bombScale, 0.0f, 30, 4);
        psndSFXOn("SFX_BTL_SAC_TIME5");
        if (!psndSFXChk(w->sfx1)) {
            psndSFXOff(w->sfx1);
        }
        if (!psndSFXChk(w->sfx2)) {
            psndSFXOff(w->sfx2);
        }
        w->phase = 50;
        for (i = 0; i < 200; i++) {
            m = BattleAudienceGetPtr(i);
            m->flags &= ~AUDIENCE_MEMBER_FLAG_NO_SYS_CTRL;
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
    dispEntry(CAMERA_2D, 900.0f, 1, bakuGameDisp2D, NULL);
    dispEntry(CAMERA_3D, 0.0f, 1, bakuGameDisp3D, NULL);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(bakuGameHeihoReturn) {
    BakuWork* w = GetBakuGamePtr();

    if (!(w->flags & 1)) {
        return EVT_RETURN_DONE;
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
        w->hpos.x = intplGetValue(EASING_LINEAR, w->hfrom.x, w->hto.x, w->htimer, w->hsteps);
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
            return EVT_RETURN_DONE;
        }
        break;
    }
    dispEntry(CAMERA_3D, 0.0f, 1, bakuGameDisp3D, NULL);
    return EVT_RETURN_BLOCK;
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
        w->pressWait = 60;
    }
}

BOOL bakuGamePartyExist(BattleUnit* unit) {
    if (unit == NULL) {
        return FALSE;
    }
    if (BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
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
        if (u != NULL && u->mAlliance == ALLIANCE_ENEMY && !(u->mAttributeFlags & kInactive) && !BtlUnit_CheckStatus(u, STATUS_INSTANT_KILL)) {
            btlDispPoseAnime(BtlUnit_GetPartsPtr(u, BtlUnit_GetBodyPartsId(u)));
        }
    }
}

void bakuGameAudienceSurpriseReset(void) {
    s32 i;
    BattleAudienceMember* m;

    for (i = 0; i < 200; i++) {
        if ((u8)BattleAudience_GetExist(i)) {
            m = BattleAudienceGetPtr(i);
            m->angle = 90.0f;
            if (!bakuGameAudienceCanThrowPos(i)) {
                m->flags &= ~AUDIENCE_MEMBER_FLAG_NO_SYS_CTRL;
            }
        }
    }
}

void bakuGameMarioSurprise(BattleUnit* mario, BattleUnit* party) {
    BtlUnit_SetBodyAnim(mario, "M_N_7");
    if (bakuGamePartyExist(party)) {
        switch (party->initialKind) {
        case UNIT_GOOMBELLA:
            BtlUnit_SetBodyAnim(party, "PKR_D_1");
            break;
        case UNIT_KOOPS:
            BtlUnit_SetBodyAnim(party, "PNK_T_2");
            break;
        case UNIT_FLURRIE:
            BtlUnit_SetBodyAnim(party, "PWD_D_1");
            break;
        case UNIT_YOSHI:
            BtlUnit_SetBodyAnim(party, "PYS_D_1");
            break;
        case UNIT_VIVIAN:
            BtlUnit_SetBodyAnim(party, "PTR_H_1");
            break;
        case UNIT_BOBBERY:
            BtlUnit_SetBodyAnim(party, "PBM_D_1");
            break;
        case UNIT_MS_MOWZ:
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
        if (u != NULL && u->mAlliance == ALLIANCE_ENEMY && !(u->mAttributeFlags & kInactive) && !BtlUnit_CheckStatus(u, STATUS_INSTANT_KILL)) {
            BtlUnit_SetBodyAnimType(u, POSE_TYPE_DAMAGE);
        }
    }
}

void bakuGameAudienceSurprise(void) {
    s32 i;
    BattleAudienceMember* m;

    for (i = 0; i < 200; i++) {
        m = BattleAudienceGetPtr(i);
        if (BattleAudience_GetWaiting(i)) {
            m->flags |= AUDIENCE_MEMBER_FLAG_NO_SYS_CTRL;
            memset(&m->interpFrameCounter, 0, 0x34); // interpFrameCounter..interpType
            if (!bakuGameAudienceCanThrowPos(i)) {
                BattleAudience_SetAnim(i, 7, 0);
                if (irand(2) == 0) {
                    m->angle = 90.0f;
                } else {
                    m->angle = 270.0f;
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
        case PAD_BUTTON_A:
            icon = ICON_BUTTON_A;
            break;
        case PAD_BUTTON_B:
            icon = ICON_BUTTON_B;
            break;
        case PAD_BUTTON_X:
            icon = ICON_BUTTON_X;
            break;
        default:
            icon = 0;
            break;
        }
        if (icon) {
            btlGetScreenPoint(&b->pos, &scr);
            iconDispGx(&scr, ICON_FLAG_2D, icon, b->scale);
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
    BattleAcDrawGauge(w->gaugeX, w->gaugeY, 0xB1, w->rate, 3, 33, 66, 100, full);
}

void bakuGameDisp3D(CameraId cam, void* param) {
    BakuWork* w = GetBakuGamePtr();
    CameraEntry* camera = camGetPtr(CAMERA_3D);
    Mtx scl;
    Mtx trans;
    Mtx mtx;
    GXTexObj tex;
    s32 o;

    PSMTXScale(scl, -1.0f, 1.0f, 1.0f);
    PSMTXTrans(trans, w->pos.x, w->pos.y, w->pos.z);
    PSMTXConcat(trans, scl, mtx);
    if (w->poseId != -1) {
        animPoseDrawMtx(w->poseId, mtx, 2, 180.0f, w->bombScale);
    }
    if (w->flags & 1) {
        GXSetCullMode(GX_CULL_NONE);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){0, 0, 0, 0xFF});
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetNumTevStages(1);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_TEXC, GX_CC_RASC);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        TEXGetGXTexObjFromPalette(*(TEXPalette**)BattleAudienceBaseGetPtr()->normalTex->data, &tex, w->texIdx);
        GXLoadTexObj(&tex, GX_TEXMAP0);
        PSMTXTrans(trans, w->hpos.x, w->hpos.y, w->hpos.z);
        PSMTXConcat(camera->view, trans, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
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
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(btlevtcmd_reset_move_color_lv_all)
    CALL(init_bakugame)
    BEGIN_SIBLING_EVENT
        CALL(main_star)
    END_SIBLING_EVENT
    CALL_CHILD(sac_start_camera_evt)
    WAIT_MS(500)
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
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(hitCheck_MarioTeam)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_PARTNER, LW(3))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(50)
    END_IF
    CALL(btlevtcmd_CheckStatus, LW(3), STATUS_INSTANT_KILL, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(50)
    END_IF
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
    CALL(btlevtcmd_ACSuccessEffect, LW(2), LW(10), LW(11), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(50)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_MARIO, LW(3))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckStatus, LW(3), STATUS_INSTANT_KILL, LW(0))
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
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_DAMAGE)
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
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(hitCheck_EnemyTeam)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(10)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
    CALL(btlevtcmd_CheckPartsAttribute, LW(3), LW(4), kClone, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        IF_INT_NOT_EQUAL(LW(2), -1)
            CALL(btlevtcmd_ACSuccessEffect, LW(2), LW(10), LW(11), LW(12))
        END_IF
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
