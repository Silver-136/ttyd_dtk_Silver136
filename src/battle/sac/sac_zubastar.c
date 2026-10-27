#include "eff/eff_starstone.h"
#include "battle/battle.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/icondrv.h"
#include "system.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "pmario_sound.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "driver/camdrv.h"
#include <string.h>
#include "icon_ids.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/sac/sac_common.h"
#include "evt/evt_fade.h"
#include "party/party_motion.h"
#include "battle/sac/sac_zubastar.h"
#include "battle/battle_database_common.h"
#include <dolphin/pad.h>
#include "libc_decls.h"
#include "battle/battle_audience.h"
#include "battle/battle_sub.h"

void BattleAcDrawGauge(s32 x, s32 y, s32 width, f32 ratio, s32 level, s32 a, s32 b, s32 c, s32 flags);

typedef struct ZSObj {
    /* 0x00 */ s32 a;
    /* 0x04 */ s32 n;
    /* 0x08 */ Vec* v;
    /* 0x0C */ Vec pos;
    /* 0x18 */ Vec center;
    /* 0x24 */ Vec start;
    /* 0x30 */ Vec end;
    /* 0x3C */ f32 rot;
    /* 0x40 */ f32 rotSpeed;
    /* 0x44 */ f32 vy;
    /* 0x48 */ s32 f48;
    /* 0x4C */ s32 f4c;
} ZSObj;

typedef struct ZSCursor {
    /* 0x00 */ s32 timer;
    /* 0x04 */ s32 f4;
    /* 0x08 */ Vec pos;
    /* 0x14 */ Vec trail[4];
} ZSCursor;

typedef struct ZSLine {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 f4;
    /* 0x08 */ s32 timer;
    /* 0x0C */ f32 x0;
    /* 0x10 */ f32 y0;
    /* 0x14 */ f32 x1;
    /* 0x18 */ f32 y1;
} ZSLine;

typedef struct ZubaStarWork {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ s32 phase;
    /* 0x008 */ s32 f8;
    /* 0x00C */ s32 acTimer;
    /* 0x010 */ s32 level;
    /* 0x014 */ s32 count;
    /* 0x018 */ ZSCursor cursor;
    /* 0x05C */ Vec lstart;
    /* 0x068 */ Vec lend;
    /* 0x074 */ s32 ltimer;
    /* 0x078 */ s16 lidx;
    /* 0x07C */ s32 gaugeState;
    /* 0x080 */ s32 gaugeTimer;
    /* 0x084 */ f32 gauge;
    /* 0x088 */ f32 gaugeV;
    /* 0x08C */ f32 gaugeAcc;
    /* 0x090 */ f32 f90;
    /* 0x094 */ Vec gaugePos; // gauge screen position
    /* 0x0A0 */ u8 alpha;
    /* 0x0A1 */ u8 fa1;
    /* 0x0A4 */ s32 sfx;
    /* 0x0A8 */ ZSObj* ptrs[200];
    /* 0x3C8 */ ZSLine lines[16];
    /* 0x588 */ smartEntry* capture;
    /* 0x58C */ s32 stone_state;
    /* 0x590 */ s32 timer;
    /* 0x594 */ Vec pos;
    /* 0x5A0 */ Vec start;
    /* 0x5AC */ Vec end;
    /* 0x5B8 */ u8 pad5b8[0xC];
    /* 0x5C4 */ Vec scale;
    /* 0x5D0 */ Vec rot;
    /* 0x5DC */ EffectEntry* eff;
} ZubaStarWork;

ZubaStarWork* GetZubaStarPtr(void);


BattleWeapon weapon_zubastar = {
    "btl_wn_sac_zubastar", // name
    ICON_SAC_ZUBASTAR, // icon
    {0}, // pad_06
    0, // itemId
    "msg_zubastar", // description
    100, // baseAccuracy
    0, // baseFpCost
    6, // baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPower_ZubaStar, // hpDamageFn
    {0, 3, 6, 9, 12, 15, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_sac_8", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    marioAttackEvent_ZubaStar, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};
s32 zubastar_get_kouten(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4, f32* ox, f32* oy);

static USER_FUNC(star_stone_appear) {
    ZubaStarWork* w = GetZubaStarPtr();
    w->stone_state = 1;
    return 2;
}

s32 wait_star_stone_up(void) {
    return GetZubaStarPtr()->stone_state == 4 ? 2 : 0;
}

void zubaStarDispStar(CameraId cam, void* param);


static USER_FUNC(main_star) {
    ZubaStarWork* w = GetZubaStarPtr();
    BattleUnit* mario = BattleGetMarioPtr(_battleWorkPointer);
    switch (w->stone_state) {
    case 0:
        break;
    case 1:
        w->stone_state = 2;
        w->timer = 0;
        w->eff = effStarStoneEntry(6, 0.0f, -1000.0f, 0.0f, 1.0f);
        BtlUnit_GetPos(mario, &w->pos.x, &w->pos.y, &w->pos.z);
        w->pos.y += mario->mSizeMultiplier * mario->mHeight + 37.0f;
        w->start = w->pos;
        w->end = w->pos;
        w->end.y += 50.0f;
        w->end.z += -1.0f;
        w->scale = (Vec){0.0f, 0.0f, 0.0f};
        w->rot = (Vec){0.0f, 0.0f, 0.0f};
    case 2:
        w->timer++;
        if (w->timer <= 100) {
            w->pos.x = intplGetValue(EASING_LINEAR, w->start.x, w->end.x, w->timer, 100);
            w->pos.y = intplGetValue(EASING_LINEAR, w->start.y, w->end.y, w->timer, 100);
            w->pos.z = intplGetValue(EASING_LINEAR, w->start.z, w->end.z, w->timer, 100);
            w->scale.x = w->scale.y = w->scale.z = intplGetValue(EASING_LINEAR, 0.0f, 2.0f, w->timer, 100);
        } else {
            w->pos.x = w->end.x;
            w->pos.y = w->end.y;
            w->pos.z = w->end.z;
            w->scale.x = w->scale.y = w->scale.z = 2.0f;
        }
        w->rot.y = intplGetValue(EASING_QUADRATIC_OUT, 0.0f, 2160.0f, w->timer, 120);
        if (w->timer >= 120) {
            w->stone_state = 3;
            w->timer = 0;
            w->start = w->pos;
            w->end.y = 300.0f;
        }
        break;
    case 3:
        w->timer++;
        w->pos.y = intplGetValue(EASING_QUADRATIC_IN, w->start.y, w->end.y, w->timer, 60);
        if (w->timer >= 60) {
            w->stone_state = 4;
            w->timer = 0;
        }
        break;
    case 4:
        break;
    }
    if (w->eff) {
        EffStarStoneWork* d = (EffStarStoneWork*)w->eff->userdata;
        d->position.x = w->pos.x;
        d->position.y = w->pos.y;
        d->position.z = w->pos.z;
        d->rot.x = w->rot.x;
        d->rot.y = w->rot.y;
        d->rot.z = w->rot.z;
        d->scale = 1.5f * w->scale.x;
    }
    dispEntry(CAMERA_3D, 0.0f, 2, zubaStarDispStar, NULL);
    return 0;
}

void zubaStarDispStar(CameraId cam, void* param) {
}

ZubaStarWork* GetZubaStarPtr(void) {
    return (ZubaStarWork*)_battleWorkPointer->actionCommands.work;
}

s32 weaponGetPower_ZubaStar(void* a, s32* tbl) {
    return tbl[GetZubaStarPtr()->level + 8];
}


void zubastar_create_takaku(s32 n, Vec* pts) {
    ZubaStarWork* w = GetZubaStarPtr();
    ZSObj* t;
    s32 i;
    w->ptrs[w->count] = BattleAlloc(sizeof(ZSObj));
    t = w->ptrs[w->count];
    t->v = BattleAlloc(n * sizeof(Vec));
    t->a = 0;
    t->n = n;
    t->pos = (Vec){0.0f, 0.0f, 0.0f};
    t->f48 = 0;
    t->f4c = 1;
    for (i = 0; i < n; i++) {
        t->v[i] = pts[i];
    }
    w->count++;
}

void zubastar_bunkatu(Vec* p0, Vec* p1) {
    ZubaStarWork* w = GetZubaStarPtr();
    ZSObj* t;
    s32 i;
    Vec hit[2];
    s32 flags[2];
    s32 idx[2];
    f32 ox;
    f32 oy;
    s32 nhit;
    s32 j;
    s32 k;
    s32 count = w->count;
    for (i = 0; i < count; i++) {
        t = w->ptrs[i];
        nhit = 0;
        for (j = 0; j < 2; j++) {
            flags[j] = 0;
        }
        for (j = 0; j < t->n; j++) {
            if (nhit == 2) {
                break;
            }
            if (zubastar_get_kouten(p0->x, p0->y, p1->x, p1->y, t->v[j].x, t->v[j].y,
                                    t->v[(j + 1) % t->n].x, t->v[(j + 1) % t->n].y, &ox, &oy)) {
                for (k = 0; k < nhit; k++) {
                    if (flags[k] == 1) {
                        if ((s32)ox == (s32)hit[k].x && (s32)oy == (s32)hit[k].y) {
                            break;
                        }
                    }
                }
                if (k == nhit) {
                    hit[k].x = ox;
                    hit[k].y = oy;
                    hit[k].z = 0.0f;
                    flags[k] = 1;
                    idx[k] = j;
                    nhit++;
                }
            }
        }
        if (nhit == 2) {
            ZSObj* a = BattleAlloc(sizeof(ZSObj));
            ZSObj* b = BattleAlloc(sizeof(ZSObj));
            for (j = 0; j < t->n; j++) {
                if (j == idx[0]) {
                    a->n = (t->n - idx[1]) + 2 + j;
                    b->n = idx[1] - idx[0] + 2;
                }
            }
            a->v = BattleAlloc(a->n * sizeof(Vec));
            b->v = BattleAlloc(b->n * sizeof(Vec));
            k = 0;
            for (j = 0; j <= idx[0]; j++) {
                a->v[k++] = t->v[j];
            }
            a->v[k] = hit[0];
            k++;
            a->v[k] = hit[1];
            k++;
            for (j = idx[1] + 1; j < t->n; j++) {
                a->v[k++] = t->v[j];
            }
            for (j = 0; j < t->n; j++) {
                if (j == idx[0]) {
                    b->v[0] = hit[0];
                    for (k = 1; k <= b->n - 2; k++) {
                        b->v[k] = t->v[j + k];
                    }
                    b->v[b->n - 1] = hit[1];
                }
            }
            a->a = 0;
            a->pos = (Vec){0.0f, 0.0f, 0.0f};
            a->f48 = irand(0x80);
            a->f4c = irand(5) + 1;
            b->a = 0;
            b->pos = (Vec){0.0f, 0.0f, 0.0f};
            b->f48 = irand(0x80);
            b->f4c = irand(5) + 1;
            BattleFree(w->ptrs[i]->v);
            BattleFree(w->ptrs[i]);
            w->ptrs[i] = a;
            w->ptrs[w->count] = b;
            w->count++;
        }
    }
}

s32 zubastar_get_kouten(f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, f32 x4, f32 y4, f32* ox, f32* oy) {
    f32 dx1 = x2 - x1;
    f32 d, a, t, e1, e2, dx2, b;
    if (dx1 == 0.0f && (x4 - x3) == 0.0f) {
        return 0;
    }
    if (dx1 != 0.0f) {
        e1 = dx1;
    } else {
        e1 = 1e-5f;
    }
    dx2 = x4 - x3;
    if (dx2 != 0.0f) {
        e2 = dx2;
    } else {
        e2 = 1e-5f;
    }
    a = (y2 - y1) / e1;
    b = (y4 - y3) / e2;
    d = a - b;
    if (d == 0.0f) {
        return 0;
    }
    *ox = ((y3 - y1) + (a * x1 - b * x3)) / d;
    if (dx1 != 0.0f) {
        *oy = a * (*ox - x1) + y1;
    } else {
        *oy = b * (*ox - x3) + y3;
    }
    t = (*ox - x3) / e2;
    if (t >= 0.0f && t <= 1.0f) {
        return 1;
    }
    return 0;
}

static EVT_BEGIN(nice_event)
    CALL(btlevtcmd_ACSuccessEffect, LW(0), -120, 140, 0)
    RETURN
EVT_END()
void zubastar_disp2D(CameraId cam, void* param);
void psndSFX_pit(s32 id, s32 pitch);

USER_FUNC(zubastar_main) {
    ZubaStarWork* w = GetZubaStarPtr();
    Vec pts[4] = {{0.0f, 240.0f, 0.0f}, {304.0f, 240.0f, 0.0f}, {304.0f, -240.0f, 0.0f}, {0.0f, -240.0f, 0.0f}};
    s32 audience = BattleAudience_GetAudienceNum();
    s32 i;
    if (isFirstCall) {
        w->capture = smartAlloc(GXGetTexBufferSize(400, 480, 4, 0, 0), 1);
        pts[0] = (Vec){-96.0f, 240.0f, 0.0f};
        pts[1] = (Vec){304.0f, 240.0f, 0.0f};
        pts[2] = (Vec){304.0f, -240.0f, 0.0f};
        pts[3] = (Vec){-96.0f, -240.0f, 0.0f};
        zubastar_create_takaku(4, pts);
        w->cursor.pos = (Vec){0.0f, -1000.0f, 0.0f};
    }
    switch (w->phase) {
    case 0:
        w->phase = 5;
        w->cursor.timer = 5;
        break;
    case 5:
        w->phase = 6;
        w->f8 = 0;
        if (w->gaugeState == 0) {
            w->gaugeState = 5;
        }
        if (w->gaugeState == 15) {
            w->gaugeState = 16;
        }
    case 6:
        if (w->cursor.timer == 10 && w->gaugeState == 10) {
            w->phase = 10;
        }
        break;
    case 10:
        w->phase = 11;
        w->acTimer = 300;
    case 11:
        if (w->gauge >= 100.0f) {
            EventEntry* ev;
            w->phase = 20;
            w->level++;
            ev = evtEntry(nice_event, 0, EVENT_START_IMMEDIATE);
            {
                s32 v = w->level + 1;
                ev->lwData[0] = v < 6 ? v : 6;
            }
            psndSFXOff(w->sfx);
            break;
        }
        w->acTimer--;
        if (w->acTimer <= 0) {
            w->phase = 15;
            psndSFXOff(w->sfx);
            break;
        }
        if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
            w->gaugeV = 2.8f;
        }
        break;
    case 15:
        w->gaugeState = 20;
        psndSFXOff(w->sfx);
        w->phase = 30;
        break;
    case 20:
        w->gaugeState = 15;
        w->gaugeTimer = 0;
        psndSFXOff(w->sfx);
        if (audience >= 1 && audience <= 49) {
            BattleAudienceSoundCallKind(1);
        }
        if (audience >= 50 && audience <= 99) {
            BattleAudienceSoundCallKind(1);
            BattleAudienceSoundWhistleKind(1);
        }
        if (audience >= 100 && audience <= 149) {
            BattleAudienceSoundCallKind(2);
            BattleAudienceSoundWhistleKind(2);
        }
        if (audience >= 150) {
            BattleAudienceSoundCallKind(2);
            BattleAudienceSoundWhistleKind(3);
        }
        w->phase = 21;
        w->cursor.timer = 15;
    case 21:
        if (w->cursor.timer == 20) {
            w->phase = 25;
            w->f8 = 0;
        }
        break;
    case 25: {
        f32 top[18];
        f32 bot[18];
        f32 dtop[17];
        f32 dbot[17];
        Vec a;
        Vec b;
        s32 n;
        s32 j;
        s32 ti;
        s32 bi;
        f32 mt, mb, f;
        w->phase = 26;
        w->cursor.timer = 25;
        if (irand(2) == 0) {
            w->lstart = (Vec){0.0f, 240.0f, 0.0f};
            w->lend = (Vec){0.0f, -240.0f, 0.0f};
        } else {
            w->lstart = (Vec){0.0f, -240.0f, 0.0f};
            w->lend = (Vec){0.0f, 240.0f, 0.0f};
        }
        top[0] = bot[0] = -96.0f;
        i = 0;
        n = 1;
        // BUG: reads 17 entries of lines[16]; the extra entry overlaps capture, whose value is never 5
        for (; i < 17; i++) {
            ZSLine* l = &w->lines[i];
            if (l->state == 5) {
                if (240.0f == l->y0) {
                    top[n] = l->x0;
                    bot[n] = l->x1;
                } else {
                    top[n] = l->x1;
                    bot[n] = l->x0;
                }
                n++;
            }
        }
        top[n] = 344.0f;
        bot[n] = 264.0f;
        for (i = 0; i < n; i++) {
            for (j = i + 1; j < n + 1; j++) {
                if (top[i] > top[j]) {
                    f = top[i];
                    top[i] = top[j];
                    top[j] = f;
                }
                if (bot[i] > bot[j]) {
                    f = bot[i];
                    bot[i] = bot[j];
                    bot[j] = f;
                }
            }
        }
        for (i = 0; i < n; i++) {
            dtop[i] = top[i + 1] - top[i];
            dbot[i] = bot[i + 1] - bot[i];
        }
        mt = mb = 0.0f;
        ti = bi = i = 0;
        for (; i < n; i++) {
            if (dtop[i] > mt) {
                mt = dtop[i];
                ti = i;
            }
            if (dbot[i] > mb) {
                mb = dbot[i];
                bi = i;
            }
        }
        f = top[ti] + dtop[ti] * rand() / 32767.0f;
        if (240.0f == w->lstart.y) {
            w->lstart.x = f;
        } else {
            w->lend.x = f;
        }
        // BUG: bi is the widest gap along the bottom edge (dbot), but the point is taken from top[] and dtop[]
        f = top[bi] + dtop[bi] * rand() / 32767.0f;
        if (-240.0f == w->lstart.y) {
            w->lstart.x = f;
        } else {
            w->lend.x = f;
        }
        b = w->lend;
        a = w->lstart;
        zubastar_bunkatu(&a, &b);
    }
    case 26:
        if (w->cursor.timer == 30) {
            w->f8++;
            if (w->f8 >= 3) {
                w->phase = 33;
            } else {
                w->phase = 25;
            }
        }
        break;
    case 30:
        w->phase = 31;
        w->cursor.timer = 15;
    case 31:
        if (w->cursor.timer == 20) {
            w->phase = 35;
        }
        break;
    case 33:
        if (w->level >= 5) {
            w->phase = 35;
        } else {
            w->phase = 0;
        }
        break;
    case 35:
        w->phase = 36;
        w->cursor.timer = 25;
        w->lstart = (Vec){-96.0f, 240.0f, 0.0f};
        w->lend = (Vec){-96.0f, -240.0f, 0.0f};
        break;
    case 36:
        if (w->cursor.timer == 30) {
            w->phase = 38;
            w->f8 = 60;
            btl_camera_shake_h(0, 5.0f, 5.0f, 60, 0);
        }
        break;
    case 38:
        w->f8--;
        if (w->f8 <= 0) {
            w->phase = 40;
        }
        break;
    case 40:
        for (i = 0; i < 16; i++) {
            w->lines[i].state = 0;
        }
        return 2;
    }

    switch (w->cursor.timer) {
    case 0:
        break;
    case 5:
        w->cursor.timer = 6;
        w->cursor.f4 = 0;
    case 6:
        w->cursor.f4++;
        w->cursor.pos.x = 0.0f;
        w->cursor.pos.y = intplGetValue(EASING_QUADRATIC_OUT, 300.0f, 300.0f, w->cursor.f4, 15);
        w->cursor.pos.z = 0.0f;
        if (w->cursor.f4 >= 15) {
            w->cursor.timer = 10;
        }
        break;
    case 10:
        w->cursor.pos = (Vec){0.0f, 300.0f, 0.0f};
        break;
    case 15:
        w->cursor.timer = 16;
        w->cursor.f4 = 0;
    case 16:
        w->cursor.f4++;
        w->cursor.pos.x = 0.0f;
        w->cursor.pos.y = intplGetValue(EASING_QUADRATIC_OUT, 300.0f, 300.0f, w->cursor.f4, 15);
        w->cursor.pos.z = 0.0f;
        if (w->cursor.f4 >= 15) {
            w->cursor.timer = 20;
        }
        break;
    case 25:
        w->cursor.timer = 26;
        w->ltimer = 0;
        for (i = 0; i < 16; i++) {
            ZSLine* l = &w->lines[i];
            if (l->state == 0) {
                w->lidx = i;
                l->state = 5;
                l->f4 = 0;
                l->x0 = l->x1 = w->lstart.x;
                l->y0 = l->y1 = w->lstart.y;
                l->timer = 0;
                break;
            }
        }
        psndSFXOn("SFX_BTL_SAC_ZUBA_STAR2");
    case 26:
        w->ltimer++;
        w->cursor.pos.x = intplGetValue(EASING_LINEAR, w->lstart.x, w->lend.x, w->ltimer, 15);
        w->cursor.pos.y = intplGetValue(EASING_LINEAR, w->lstart.y, w->lend.y, w->ltimer, 15);
        {
            ZSLine* l = &w->lines[w->lidx];
            l->x1 = w->cursor.pos.x;
            l->y1 = w->cursor.pos.y;
        }
        if (w->ltimer >= 15) {
            w->cursor.timer = 27;
            w->cursor.f4 = 0;
            btl_camera_shake_h(0, 8.0f, 0.0f, 30, 0);
        }
        break;
    case 27:
        w->cursor.f4++;
        if (w->cursor.f4 >= 5) {
            w->cursor.timer = 28;
        }
        break;
    case 28:
        w->cursor.timer = 30;
    case 30:
        w->cursor.pos = (Vec){0.0f, -1000.0f, 0.0f};
        break;
    }
    w->cursor.trail[3] = w->cursor.trail[2];
    w->cursor.trail[2] = w->cursor.trail[1];
    w->cursor.trail[1] = w->cursor.trail[0];
    w->cursor.trail[0] = w->cursor.pos;

    switch (w->gaugeState) {
    case 0:
        break;
    case 5:
        w->gaugeState = 6;
        w->gaugeTimer = 0;
        w->gaugePos = (Vec){-1000.0f, 0.0f, 0.0f};
        w->f90 = 200.0f;
        w->gauge = 0.0f;
        w->gaugeV = 0.0f;
        w->gaugeAcc = -1.0f;
        w->alpha = 255;
        w->fa1 = 0;
        break;
    case 6:
        w->gaugeTimer++;
        w->gaugePos.x = intplGetValue(EASING_QUADRATIC_OUT, -1000.0f, 50.0f, w->gaugeTimer, 30);
        if (w->gaugeTimer >= 30) {
            w->gaugeState = 10;
        }
        break;
    case 10:
        w->gauge += w->gaugeV;
        w->gauge += w->gaugeAcc;
        if (w->gauge > 100.0f) {
            w->gauge = 100.0f;
        }
        if (w->gauge < 0.0f) {
            w->gauge = 0.0f;
        }
        w->gaugeV -= 0.25f;
        if (w->gaugeV < 0.0f) {
            w->gaugeV = 0.0f;
        }
        w->fa1++;
        if (w->fa1 >= 8) {
            w->fa1 = 0;
        }
        if (w->gauge <= 0.0f) {
            w->gauge = 0.0f;
            if (psndSFXChk(w->sfx) == 0) {
                psndSFXOff(w->sfx);
            }
        }
        if (w->gauge > 0.0f) {
            if (psndSFXChk(w->sfx) == -1) {
                w->sfx = psndSFXOn("SFX_BTL_SAC_ZUBA_STAR1");
            }
        }
        if (psndSFXChk(w->sfx) == 0) {
            psndSFX_pit(w->sfx, 683.0f * w->level + 4096.0f * w->gauge / 100.0f);
        }
        break;
    case 15:
        w->gauge = 100.0f;
        w->gaugeV = 0.0f;
        w->gaugeAcc = 0.0f;
        w->fa1 = 0;
        w->gaugeTimer++;
        w->gaugePos.x = intplGetValue(EASING_QUADRATIC_OUT, 50.0f, -1000.0f, w->gaugeTimer, 30);
        if (w->gaugeTimer >= 30) {
            w->gaugeTimer = 30;
        }
        break;
    case 16:
        w->gaugeState = 6;
        w->gaugeTimer = 0;
        w->gauge = 0.0f;
        w->gaugeV = 0.0f;
        w->gaugeAcc = -0.9f - 0.1f * w->level;
        break;
    case 20:
        w->gaugeState = 21;
        w->gaugeTimer = 0;
        break;
    case 21:
        w->gaugeTimer++;
        w->gaugePos.x = intplGetValue(EASING_QUADRATIC_OUT, 50.0f, -1000.0f, w->gaugeTimer, 30);
        if (w->gaugeTimer >= 30) {
            w->gaugeTimer = 30;
        }
        break;
    }
    for (i = 0; i < 16; i++) {
        ZSLine* l = &w->lines[i];
        switch (l->state) {
        case 0:
            break;
        case 5:
            l->timer++;
            if (l->timer > 60) {
                l->timer = 0;
            }
            break;
        }
    }
    dispEntry(CAMERA_2D, 900.0f, 1, zubastar_disp2D, NULL);
    return 0;
}

void zubastar_disp_bomb(CameraId cam, void* param);
void zubastar_capture(CameraId cam, void* param);

USER_FUNC(zubastar_bomb) {
    ZubaStarWork* w = GetZubaStarPtr();
    const char* sfx;
    s32 i;
    s32 k;
    if (isFirstCall) {
        dispEntry(CAMERA_2D, 0.0f, 0, zubastar_capture, NULL);
        w->phase = 0;
        return 0;
    }
    switch (w->phase) {
    case 0: {
        s32 c = 0;
        for (i = 0; i < w->count; i++) {
            if (w->ptrs[i]->a != 10) {
                break;
            }
            c++;
        }
        if (c == w->count) {
            return 2;
        }
    }
    }
    sfx = "SFX_BTL_SAC_ZUBA_STAR3";
    for (k = 0; k < w->count; k++) {
        ZSObj* t = w->ptrs[k];
        switch (t->a) {
        case 0: {
            s32 steps = irand(30) + 30;
            f32 sx, sy, sz;
            s32 j;
            t->f4c = steps;
            sx = 0.0f;
            sy = 0.0f;
            sz = 0.0f;
            t->start = (Vec){0.0f, 0.0f, 0.0f};
            t->end = (Vec){0.0f, -1000.0f, 200.0f};
            for (j = 0; j < t->n; j++) {
                sx += t->v[j].x;
                sy += t->v[j].y;
                sz += t->v[j].z;
            }
            t->center.x = sx / t->n;
            t->center.y = sy / t->n;
            t->center.z = sz / t->n;
            t->end.x += 600.0f * (t->center.x - 104.0f) / 200.0f;
            t->rot = 0.0f;
            t->rotSpeed = 70.0f * rand() / 32767.0f - 35.0f;
            t->vy = (2.0f * (t->end.y - t->start.y) - steps * (-1.25f * steps)) / (2.0f * steps);
            t->f48 = 0;
            t->a = 5;
            psndSFXOn(sfx);
            break;
        }
        case 5:
            t->pos.x = intplGetValue(EASING_LINEAR, t->start.x, t->end.x, t->f48, t->f4c);
            t->pos.z = intplGetValue(EASING_LINEAR, t->start.z, t->end.z, t->f48, t->f4c);
            t->pos.y += t->vy;
            t->rot += t->rotSpeed;
            t->vy += -1.25f;
            t->f48++;
            if (t->f48 > t->f4c) {
                t->a = 10;
            }
            break;
        case 10:
            break;
        }
    }
    dispEntry(CAMERA_2D, 0.0f, 1, zubastar_disp_bomb, NULL);
    return 0;
}

s32 zubastar_init(void) {
    memset(GetZubaStarPtr(), 0, sizeof(ZubaStarWork));
    return 2;
}

s32 zubastar_end(void) {
    ZubaStarWork* w = GetZubaStarPtr();
    s32 i;
    effDelete(((EffStarStoneWork*)w->eff->userdata)->dust);
    effDelete(w->eff);
    for (i = 0; i < 200; i++) {
        if (w->ptrs[i] != NULL) {
            if (w->ptrs[i]->v != NULL) {
                BattleFree(w->ptrs[i]->v);
                w->ptrs[i]->v = NULL;
            }
            BattleFree(w->ptrs[i]);
            w->ptrs[i] = NULL;
        }
    }
    smartFree(w->capture);
    return 2;
}

void zubastar_capture(CameraId cam, void* param) {
    ZubaStarWork* w = GetZubaStarPtr();
    GXSetTexCopySrc(0xd0, 0, 400, 480);
    GXSetTexCopyDst(400, 480, GX_TF_RGB565, GX_FALSE);
    GXCopyTex(w->capture->address, GX_FALSE);
    GXPixModeSync();
}

void zubastar_disp_bomb(CameraId cam, void* param) {
    ZubaStarWork* w = GetZubaStarPtr();
    CameraEntry* camera = camGetPtr(CAMERA_2D);
    Mtx tc;
    Mtx tn;
    Mtx rot;
    Mtx tp;
    Mtx m;
    GXTexObj tex;
    ZSObj* t;
    s32 i;
    s32 j;
    GXSetCullMode(GX_CULL_NONE);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_NEVER, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXInitTexObj(&tex, w->capture->address, 400, 480, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tex, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&tex, GX_TEXMAP0);
    for (i = 0; i < w->count; i++) {
        t = w->ptrs[i];
        PSMTXTrans(tc, t->center.x, t->center.y, t->center.z);
        PSMTXTrans(tn, -t->center.x, -t->center.y, -t->center.z);
        PSMTXTrans(tp, t->pos.x, t->pos.y, t->pos.z);
        PSMTXRotRad(rot, 'z', 0.017453292f * t->rot);
        PSMTXConcat(rot, tn, m);
        PSMTXConcat(tc, m, m);
        PSMTXConcat(tp, m, m);
        PSMTXConcat(camera->view, m, m);
        GXLoadPosMtxImm(m, GX_PNMTX0);
        GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, t->n);
        for (j = 0; j < t->n; j++) {
            f32 u = (t->v[j].x + 96.0f) / 400.0f;
            f32 v = (240.0f - t->v[j].y) / 480.0f;
            GXPosition3f32(t->v[j].x, t->v[j].y, 0.0f);
            GXTexCoord2f32(u, v);
        }
    }
}

static inline void setG(u8 a) {
    GXColor col = {0, 0, 0, 0xff};
    col.r = a;
    col.g = a;
    col.b = a;
    GXSetChanMatColor(GX_COLOR0A0, col);
}

static inline void dispA(Mtx m, s32 i) {
    GXColor ca = {0xff, 0xff, 0xff, 0};
    ca.a = 0xff - (i + 1) * 50;
    iconDispGxCol(m, ICON_FLAG_2D, ICON_SAC_GENKI1, ca);
}

void zubastar_disp2D(CameraId cam, void* param) {
    ZubaStarWork* w = GetZubaStarPtr();
    ZSCursor* cur = &w->cursor;
    ZSLine* l;
    s32 i;
    if (w->gaugeState >= 5) {
        s32 icon = (w->fa1 <= 3U) ? ICON_BUTTON_A : ICON_BUTTON_A_PRESSED;
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = w->gaugePos.x - 200.0f;
        pos.y = 60.0f + w->gaugePos.y;
        // FAKE: calls iconDispGx through a cast that passes the icon id as s32; the plain call changes the object
        ((void (*)(Vec*, s16, s32, f32))iconDispGx)(&pos, ICON_FLAG_2D, icon, 1.0f);
        BattleAcDrawGauge(w->gaugePos.x, w->gaugePos.y, 0xb1, 0.01f * w->gauge, 4, 0x1a, 0x34, 0x4e, 0);
    }
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetCurrentMtx(GX_PNMTX0);
    GXLoadPosMtxImm(camGetPtr(cam)->view, GX_PNMTX0);
    GXSetLineWidth(0x18, GX_TO_ZERO);
    for (i = 0; i < 16; i++) {
        s32 a;
        l = &w->lines[i];
        if (l->timer <= 30) {
            a = intplGetValue(EASING_LINEAR, 128.0f, 255.0f, l->timer, 30);
        } else {
            a = intplGetValue(EASING_LINEAR, 255.0f, 128.0f, l->timer - 30, 30);
        }
        if (l->state == 5) {
            setG(a);
            GXBegin(GX_LINES, GX_VTXFMT0, 2);
            GXPosition3f32(l->x0, l->y0, 0.0f);
            GXPosition3f32(l->x1, l->y1, 0.0f);
        }
    }
    iconDispGx(&cur->pos, ICON_FLAG_2D, ICON_SAC_GENKI1, 1.0f);
    if (cur->timer >= 25 && cur->timer < 30) {
        for (i = 0; i < 4; i++) {
            Mtx m;
            PSMTXTrans(m, cur->trail[i].x, cur->trail[i].y - 20.0f, 0.0f);
            dispA(m, i);
        }
    }
}

EVT_BEGIN(marioAttackEvent_ZubaStar)
    CALL(zubastar_init)
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
    CALL(wait_star_stone_up)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_I_Y"))
    WAIT_MS(500)
    BEGIN_SIBLING_EVENT
        CALL(sac_handbeat)
    END_SIBLING_EVENT
    CALL(sac_ac_help_on)
    CALL(zubastar_main)
    CALL(sac_ac_help_off)
    CALL(sac_handbeat_end)
    BEGIN_SIBLING_EVENT
        CALL(evt_fade_entry, 17, 1, 255, 255, 255)
        WAIT_MS(100)
        CALL(evt_fade_entry, 16, 250, 255, 255, 255)
    END_SIBLING_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_btl_camera_shake_h, 0, 10, 0, 60, 0)
        LOOP(6)
            CALL(evt_btl_camera_shake_w, 0, 10, 0, 10, 0)
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(95)
    IF_INT_EQUAL(LW(3), -1)
        GOTO(97)
    END_IF
    CALL(btlevtcmd_CheckPartsAttribute, LW(3), LW(4), kClone, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_zubastar), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    GOTO(95)
    LABEL(97)
    CALL(zubastar_bomb)
    CALL(zubastar_end)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()




