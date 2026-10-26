#include "battle/battle.h"
#include "battle/battle_audience.h"
#include "battle/battle_disp.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/icondrv.h"
#include "driver/mapdrv.h"
#include "eff/eff_small_star.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mariost.h"
#include "memory.h"
#include "pmario_sound.h"
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
#include "battle/sac/sac_deka.h"
#include <dolphin/pad.h>
#include "battle/battle_status_effect.h"
#include "eff/eff_starstone.h"
#include "libc_decls.h"
#include "battle/battle_sub.h"
// US battle texture ids are 0x16 higher for these entries.
#define BTL_TEX(id) ((id) + VERSION_PICK(0, 0x16))


void psndSFX_pit(s32 id, s32 pitch);
void test_kururing_mapdisp(CameraId cam);


typedef struct DekaTiming {
    s32 state;  // 0x0
    s32 unk4;   // 0x4
    s32 type;   // 0x8
    f32 pos;    // 0xC
    Vec off;    // 0x10
    u8 pad1C[0x34 - 0x1C];
    u8 alpha;   // 0x34
    u8 hit;     // 0x35
    u8 pad36[2];
} DekaTiming;

typedef struct YukaMesh {
    Vec pos[2][129];   // 0x0
    Vec disp[2][129];  // 0xC18
    Vec vel[2][129];   // 0x1830
    Vec prev[2][129];  // 0x2448
    f32 u[2][129];     // 0x3060
    f32 v[2][129];     // 0x3468
} YukaMesh;

typedef struct DekaWork {
    /* 0x000 */ smartEntry* tex;
    /* 0x004 */ Vec min;
    /* 0x010 */ Vec max;
    /* 0x01C */ s32 w0;
    /* 0x020 */ s32 h0;
    /* 0x024 */ s32 texW;
    /* 0x028 */ s32 texH;
    /* 0x02C */ smartEntry* mesh;
    /* 0x030 */ s32 state;
    /* 0x034 */ s32 timer;
    /* 0x038 */ f32 gaugeLen;
    /* 0x03C */ f32 cursor;
    /* 0x040 */ s32 round;
    /* 0x044 */ s32 result;
    /* 0x048 */ s32 unitId;
    /* 0x04C */ f32 offsetY;
    /* 0x050 */ s32 gaugeState;
    /* 0x054 */ s32 gaugeTimer;
    /* 0x058 */ f32 gaugeW;
    /* 0x05C */ Vec gaugePos;
    /* 0x068 */ u8 alpha;
    /* 0x069 */ u8 pad69[3];
    /* 0x06C */ s32 sfx;
    /* 0x070 */ DekaTiming timing[8];
    /* 0x230 */ DekaTiming timing2[8];
    /* 0x3F0 */ s32 starState;
    /* 0x3F4 */ s32 starTimer;
    /* 0x3F8 */ s32 starCount;
    /* 0x3FC */ Vec starPos;
    /* 0x408 */ Vec from;
    /* 0x414 */ Vec to;
    /* 0x420 */ f32 unk420;
    /* 0x424 */ f32 velY;
    /* 0x428 */ f32 unk428;
    /* 0x42C */ Vec scale;
    /* 0x438 */ Vec rot;
    /* 0x444 */ f32 sclFrom;
    /* 0x448 */ f32 sclTo;
    /* 0x44C */ EffectEntry* eff;
    /* 0x450 */ s32 enemies[10];
    /* 0x478 */ Vec enemyVel[10];
    /* 0x4F0 */ f32 enemyRot[10];
    /* 0x518 */ u8 waveActive[6];
    /* 0x51E */ u8 pad51E[2];
    /* 0x520 */ f32 waveSpeed[6];
    /* 0x538 */ s32 waveIdx[6];
    /* 0x550 */ f32 wavePos[6];
    /* 0x568 */ f32 waveAmp[6];
} DekaWork;

static DekaWork* get_ptr(void);
USER_FUNC(get_result);
static USER_FUNC(star_stone_attack);
USER_FUNC(get_ride_pos);
static USER_FUNC(star_stone_appear);
static USER_FUNC(wait_game_end);
static USER_FUNC(start_game);
USER_FUNC(wait_star_stone_take_on);
void create_timing(void);
void yuka_disp(CameraId cam, void* param);
void yuka_capture(CameraId cam, void* param);
static void disp_3D_alpha(CameraId cam, void* param);
static void disp_3D(CameraId cam, void* param);
static void disp_2D(CameraId cam, void* param);
USER_FUNC(end_deka);
void main_enemy_sub(BattleUnit* unit, s32* n);
void main_enemy(void);
static void main_star(void);
void main_timing(s32 idx);
void main_gauge(void);
static void main_base(void);
USER_FUNC(yuka_end);
static void yuka_main(void);
static void yuka_init(void);
USER_FUNC(main_deka);


static EVT_BEGIN(nice_event)
    CALL(btlevtcmd_ACSuccessEffect, LW(0), 0, 100, 0)
    RETURN
EVT_END()

USER_FUNC(main_deka) {
    EvtWord* args;
    DekaWork* w = get_ptr();
    s32 i;

    args = event->args;
    if (isFirstCall) {
        memset(w, 0, 0x580);
        w->unitId = evtGetValue(event, args[0]);
        yuka_init();
    }
    if (w->tex->address == NULL) {
        return 2;
    }
    if (w->mesh->address == NULL) {
        return 2;
    }
    main_base();
    main_gauge();
    for (i = 0; i < 8; i++) {
        main_timing(i);
    }
    main_star();
    main_enemy();
    yuka_main();
    dispEntry(CAMERA_2D, 901.0f, 1, disp_2D, 0);
    dispEntry(CAMERA_3D, 0.0f, 1, disp_3D, 0);
    dispEntry(CAMERA_3D, 0.0f, 2, disp_3D_alpha, 0);
    dispEntry(CAMERA_3D, 0.0f, 0, yuka_disp, 0);
    return 0;
}

static void yuka_init(void) {
    DekaWork* w = get_ptr();
    MapObject* mo = mapGetMapObj("stg_b_yuka");
    YukaMesh* m;
    s32 i;
    s32 r;
    f32 x;
    f32 z;

    mapGrpFlagOn("stg_b_yuka", 0x400001);
    PSMTXMultVec(mo->modelWorldMtx, &mo->joints->bboxMin, &w->min);
    PSMTXMultVec(mo->modelWorldMtx, &mo->joints->bboxMax, &w->max);
    w->w0 = w->texW = w->max.x - w->min.x;
    w->h0 = w->texH = w->max.z - w->min.z;
    w->texW = w->texW + (w->texW & 1);
    w->texH = w->texH + (w->texH & 1);
    if (w->texW > 0x260) {
        w->texW = 0x260;
    }
    if (w->texH > 0x1E0) {
        w->texH = 0x1E0;
    }
    w->tex = smartAlloc(GXGetTexBufferSize(w->texW, w->texH, 4, 0, 0), 2);
    w->mesh = smartAlloc(0x3870, 2);
    m = w->mesh->address;
    for (r = 0; r < 2; r++) {
        for (i = 0; i < 129; i++) {
            x = w->w0 / 128.0f * i + w->min.x;
            z = (f32)w->h0 * (f32)r + w->min.z;
            {
                Vec a = {0.0f, 0.0f, 0.0f};
                a.x = x;
                a.z = z;
                m->disp[r][i] = a;
            }
            {
                Vec b = {0.0f, 0.0f, 0.0f};
                b.x = x;
                b.z = z;
                m->pos[r][i] = b;
            }
            m->vel[r][i] = (Vec){0.0f, 0.0f, 0.0f};
            m->u[r][i] = 0.0078125f * i;
            m->v[r][i] = r;
        }
    }
    dispEntry(CAMERA_SHADOW, 0.0f, 0, yuka_capture, 0);
}

static void yuka_main(void) {
    DekaWork* w = get_ptr();
    YukaMesh* m = w->mesh->address;
    s32 r;
    s32 i;
    s32 p;
    s32 j;

    for (i = 0; i < 6; i++) {
        if (w->waveActive[i] == 1) {
            w->wavePos[i] += w->waveSpeed[i];
            for (;;) {
                p = w->wavePos[i];
                if ((p > w->waveIdx[i] && w->waveSpeed[i] > 0.0f) || (p < w->waveIdx[i] && w->waveSpeed[i] < 0.0f)) {
                    for (r = 0; r < 2; r++) {
                        m->vel[r][w->waveIdx[i]].y += w->waveAmp[i];
                    }
                    if (w->waveSpeed[i] > 0.0f) {
                        w->waveIdx[i]++;
                    } else {
                        w->waveIdx[i]--;
                    }
                    if (w->waveIdx[i] > 0 && w->waveIdx[i] < 128) {
                        continue;
                    }
                    w->waveActive[i] = 0;
                }
                break;
            }
        }
    }
    memcpy_as4(m->prev, m->pos, sizeof(m->pos));
    for (r = 0; r < 2; r++) {
        for (j = 0; j < 129; j++) {
            Vec acc = {0.0f, 0.0f, 0.0f};
            Vec d;
            Vec n;
            Vec t;

            PSVECSubtract(&m->prev[r][j], &m->prev[r][j + 1], &d);
            PSVECNormalize(&d, &n);
            PSVECScale(&n, &n, 1.0f - PSVECMag(&d));
            PSVECAdd(&acc, &n, &acc);
            PSVECSubtract(&m->prev[r][j], &m->prev[r][j - 1], &d);
            PSVECNormalize(&d, &n);
            PSVECScale(&n, &n, 1.0f - PSVECMag(&d));
            PSVECAdd(&acc, &n, &acc);
            acc.x = 0.0f;
            acc.z = 0.0f;
            PSVECScale(&acc, &acc, 0.01f);
            PSVECAdd(&m->vel[r][j], &acc, &m->vel[r][j]);
            PSVECScale(&m->vel[r][j], &t, 0.1f);
            PSVECAdd(&m->pos[r][j], &t, &m->pos[r][j]);
            m->pos[r][j].y += m->vel[r][j].y;
            m->vel[r][j].y -= 2.0f;
            if (m->pos[r][j].y <= 0.0f) {
                m->pos[r][j].y = 0.0f;
                m->vel[r][j].y = 0.0f;
            }
        }
    }
    for (r = 0; r < 2; r++) {
        for (i = 1; i < 128; i++) {
            m->pos[r][i].y = 0.5f * (m->pos[r][i - 1].y + m->pos[r][i + 1].y);
        }
    }
    for (r = 0; r < 2; r++) {
        for (i = 0; i < 129; i++) {
            m->disp[r][i].x = m->pos[r][i].x;
            m->disp[r][i].y = m->pos[r][i].y;
            m->disp[r][i].z = m->pos[r][i].z;
        }
    }
}

USER_FUNC(yuka_end) {
    DekaWork* w = get_ptr();

    smartFree(w->tex);
    w->tex->address = NULL;
    smartFree(w->mesh);
    w->mesh->address = NULL;
    mapGrpFlagOff("stg_b_yuka", 0x400001);
    return 2;
}

static void main_base(void) {
    DekaWork* w = get_ptr();
    s32 idx[8];
    f32 pos[8];
    s32 i;
    s32 j;
    DekaTiming* t;
    EventEntry* ev;
    s32 v;
    s32 found;

    switch (w->state) {
    case 1:
        w->state = 2;
        w->round = 1;
        w->result = 0;
        w->gaugeLen = 180.0f;
        w->cursor = 0.0f;
        create_timing();
        w->gaugeState = 1;
    case 2:
        if (w->gaugeState != 3) {
            break;
        }
        w->state = 3;
        w->sfx = psndSFXOn("SFX_BTL_SAC_DOKKAN1_1");
    case 3:
        if (w->gaugeState == 3) {
            w->cursor += 0.1f * w->result + 0.8f;
            psndSFX_pit(w->sfx, 683.0f * w->result + 4096.0f * w->cursor / w->gaugeLen);
            if (w->cursor >= w->gaugeLen) {
                w->cursor = 0.0f;
                w->round++;
                w->result++;
                w->starState = 6;
                psndSFXOff(w->sfx);
                ev = evtEntry(nice_event, 0, EVENT_START_IMMEDIATE);
                v = w->result + 1;
                ev->lwData[0] = v < 6 ? v : 6;
                if (w->round > 5) {
                    w->state = 5;
                    w->gaugeState = 5;
                } else {
                    w->state = 4;
                    w->timer = 30;
                    w->offsetY = -40.0f;
                    create_timing();
                }
            }
        }
        for (i = 0; i < 8; i++) {
            idx[i] = i;
            pos[i] = w->timing[i].pos;
        }
        for (i = 0; i < 7; i++) {
            for (j = i + 1; j < 8; j++) {
                f32 pj;
                f32 pi;
                pi = pos[i];
                pj = pos[j];
                if (pi > pj) {
                    s32 ti = idx[i];
                    pos[i] = pj;
                    idx[i] = idx[j];
                    idx[j] = ti;
                    pos[j] = pi;
                }
            }
        }
        if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
            for (i = 0; i < 8; i++) {
                t = &w->timing[idx[i]];
                if (t->state == 2 && w->cursor >= t->pos - 7.0f && w->cursor <= 7.0f + t->pos) {
                    if (t->type == 1) {
                        t->state = 3;
                        t->hit = 1;
                        psndSFXOn("SFX_BTL_SAC_DOKKAN2");
                        BattleAudienceSoundHandBeat();
                    } else {
                        w->result = 0;
                        w->state = 5;
                        w->gaugeState = 4;
                        w->starState = 8;
                        t->hit = 2;
                        psndSFXOn("SFX_BTL_SAC_DOKKAN3");
                        BattleAudienceSoundBooing();
                        psndSFXOff(w->sfx);
                    }
                    break;
                }
            }
            if (i == 8) {
                found = 0;
                for (j = 0; j < 8; j++) {
                    DekaTiming* t2 = &w->timing[idx[j]];
                    t = t2;
                    if (t->state == 2 && t->type == 1) {
                        w->state = 5;
                        w->gaugeState = 4;
                        t->hit = 2;
                        psndSFXOn("SFX_BTL_SAC_DOKKAN3");
                        BattleAudienceSoundBooing();
                        psndSFXOff(w->sfx);
                        found = 1;
                        break;
                    }
                }
                if (found == 1) {
                    break;
                }
            }
        }
        for (i = 0; i < 8; i++) {
            t = &w->timing[idx[i]];
            if (t->state == 2 && t->type != 2 && w->cursor > 7.0f + t->pos) {
                w->state = 5;
                w->gaugeState = 4;
                t->hit = 2;
                psndSFXOn("SFX_BTL_SAC_DOKKAN3");
                BattleAudienceSoundBooing();
                psndSFXOff(w->sfx);
                break;
            }
        }
        break;
    case 4:
        w->timer--;
        w->offsetY = intplGetValue(EASING_LINEAR, -40.0f, 0.0f, 30 - w->timer, 30);
        if (w->timer <= 0) {
            w->state = 3;
            w->sfx = psndSFXOn("SFX_BTL_SAC_DOKKAN1_1");
        }
        break;
    case 5:
        break;
    }
}

void main_gauge(void) {
    DekaWork* w = get_ptr();

    switch (w->gaugeState) {
    case 0:
        break;
    case 1:
        w->gaugeState = 2;
        w->gaugeTimer = 0;
        w->gaugePos = (Vec){-1000.0f, 100.0f, 0.0f};
        w->gaugeW = 500.0f;
        w->alpha = 255;
    case 2:
        w->gaugeTimer++;
        w->gaugePos.x = intplGetValue(EASING_QUADRATIC_OUT, -1000.0f, 0.0f, w->gaugeTimer, 30);
        if (w->gaugeTimer >= 30) {
            w->gaugeState = 3;
            w->gaugeTimer = 0;
        }
        break;
    case 3:
        w->gaugeTimer = 0;
        break;
    case 4:
        w->gaugeTimer++;
        if (w->gaugeTimer % 12 >= 0 && w->gaugeTimer % 12 <= 5) {
            w->alpha = 0;
        } else {
            w->alpha = 255;
        }
        if (w->gaugeTimer >= 80) {
            w->gaugeState = 5;
            w->gaugeTimer = 0;
            w->alpha = 255;
        }
        break;
    case 5:
        w->gaugeTimer++;
        w->gaugePos.x = intplGetValue(EASING_QUADRATIC_OUT, 0.0f, -1000.0f, w->gaugeTimer, 30);
        if (w->gaugeTimer >= 30) {
            w->gaugeState = 3;
            w->gaugeTimer = 0;
        }
        break;
    case 6:
        break;
    }
}

void main_timing(s32 idx) {
    DekaWork* w = get_ptr();
    DekaTiming* t = &w->timing[idx];

    switch (t->state) {
    case 0:
        t->type = 0;
        break;
    case 2:
        t->alpha = 255;
        t->off = (Vec){0.0f, 0.0f, 0.0f};
        break;
    case 3:
        t->state = 4;
        t->alpha = 255;
        t->unk4 = 0;
        break;
    case 4:
        break;
    case 5:
        break;
    }
}

static void main_star(void) {
    DekaWork* w = get_ptr();
    BattleUnit* mario = BattleGetMarioPtr(_battleWorkPointer);
    s32 num = BattleAudience_GetAudienceNum();
    s32 alpha = 255;
    s32 j;
    s32 k;

    switch (w->starState) {
    case 1:
        w->starState = 2;
        w->starTimer = 0;
        w->eff = effStarStoneEntry(0, 0.0f, -1000.0f, 0.0f, 1.0f);
        BtlUnit_GetPos(mario, &w->starPos.x, &w->starPos.y, &w->starPos.z);
        w->starPos.y += mario->mSizeMultiplier * mario->mHeight + 37.0f;
        w->from = w->starPos;
        w->to = w->starPos;
        w->to.y += 50.0f;
        w->to.z += -1.0f;
        w->scale = (Vec){0.0f, 0.0f, 0.0f};
        w->rot = (Vec){0.0f, 0.0f, 0.0f};
    case 2:
        w->starTimer++;
        if (w->starTimer <= 100) {
            w->starPos.x = (intplGetValue(EASING_LINEAR, w->from.x, w->to.x, w->starTimer, 100));
            w->starPos.y = (intplGetValue(EASING_LINEAR, w->from.y, w->to.y, w->starTimer, 100));
            w->starPos.z = (intplGetValue(EASING_LINEAR, w->from.z, w->to.z, w->starTimer, 100));
            w->scale.x = w->scale.y = w->scale.z = (intplGetValue(EASING_LINEAR, 0.0f, 2.0f, w->starTimer, 100));
        } else {
            w->starPos.x = w->to.x;
            w->starPos.y = w->to.y;
            w->starPos.z = w->to.z;
            w->scale.x = w->scale.y = w->scale.z = 2.0f;
        }
        w->rot.y = (intplGetValue(EASING_QUADRATIC_OUT, 0.0f, 2160.0f, w->starTimer, 120));
        if (w->starTimer >= 120) {
            w->starState = 3;
            w->starTimer = 0;
            w->from = w->starPos;
            w->to.x = -100.0f;
            w->to.y = 30.0f;
            w->to.z = 50.0f;
        }
        break;
    case 3:
        w->starTimer++;
        w->starPos.x = (intplGetValue(EASING_QUADRATIC_OUT, w->from.x, w->to.x, w->starTimer, 60));
        w->starPos.y = (intplGetValue(EASING_QUADRATIC_OUT, w->from.y, w->to.y, w->starTimer, 60));
        w->starPos.z = (intplGetValue(EASING_QUADRATIC_OUT, w->from.z, w->to.z, w->starTimer, 60));
        if (w->starTimer >= 60) {
            w->starState = 4;
            w->starTimer = 0;
        }
        break;
    case 6:
        w->starState = 7;
        w->starTimer = 0;
        w->sclFrom = w->sclTo = w->scale.x;
        w->sclTo += 1.0f;
        psndSFXOn("SFX_BTL_SAC_DOKKAN4");
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
    case 7: {
        s32 tbl[11] = {1, 2, 0, 2, 3, 1, 3, 4, 2, 4, 5};
        w->starTimer++;
        w->scale.x = w->scale.y = w->scale.z = (intplGetValue(EASING_QUADRATIC_OUT, w->sclFrom, w->sclTo, tbl[w->starTimer / 4], 5));
        w->starPos.y = 12.5f * (w->scale.y - 2.0f) + 30.0f;
        if (w->starTimer >= 43) {
            w->starState = 5;
            w->starTimer = 0;
            w->scale.x = w->scale.y = w->scale.z = w->sclTo;
        }
        break;
    }
    case 8:
        w->starState = 9;
        w->starTimer = 0;
        w->sclFrom = w->sclTo = w->scale.x;
        w->sclTo = 2.0f;
    case 9: {
        s32 tbl[11] = {1, 2, 0, 2, 3, 1, 3, 4, 2, 4, 5};
        w->starTimer++;
        w->scale.x = w->scale.y = w->scale.z = (intplGetValue(EASING_QUADRATIC_OUT, w->sclFrom, w->sclTo, tbl[w->starTimer / 4], 5));
        w->starPos.y = 12.5f * (w->scale.y - 2.0f) + 30.0f;
        if (w->starTimer >= 43) {
            w->starState = 5;
            w->starTimer = 0;
            w->scale.x = w->scale.y = w->scale.z = w->sclTo;
        }
        break;
    }
    case 10:
        w->starState = 11;
        w->to.y = w->starPos.y;
        w->velY = 5.0f;
        w->starTimer = 0;
        w->starCount = 0;
    case 11:
        w->starPos.y += w->velY;
        w->velY += -0.5f;
        if (w->starPos.y < w->to.y) {
            f32 shake[6] = {1.0f, 1.3f, 2.0f, 2.6f, 3.3f, 3.8f};
            f32 amp[6] = {1.0f, 1.3f, 2.0f, 2.6f, 3.3f, 3.8f};
            w->starCount++;
            btl_camera_shake_h(0, w->starCount * shake[w->result], 0.0f, 20, 0);
            psndSFXOn("SFX_BTL_SAC_DOKKAN5");
            for (j = 0; j < 2; j++) {
                for (k = 0; k < 6; k++) {
                    if (w->waveActive[k] == 0) {
                        w->waveActive[k] = 1;
                        w->waveSpeed[k] = 2.0f;
                        if (j == 1) {
                            w->waveSpeed[k] *= -1.0f;
                        }
                        if (j == 0) {
                            w->waveIdx[k] = 42;
                        } else {
                            w->waveIdx[k] = 41;
                        }
                        w->wavePos[k] = 43.0f;
                        w->waveAmp[k] = w->starCount * amp[w->result] + 4.0f;
                        break;
                    }
                }
            }
            if (w->starCount >= 6) {
                w->starState = 12;
                w->starTimer = 30;
                w->to = w->starPos;
                w->from = w->to;
                w->to.y += 40.0f;
            } else {
                f32 h = 4.0f * w->starCount + 20.0f;
                w->starPos.y = w->to.y;
                w->velY = 0.5f * h * h / (2.0f * h);
            }
        }
        break;
    case 12:
        w->starTimer--;
        w->starPos.y = (intplGetValue(EASING_QUADRATIC_OUT, w->from.y, w->to.y, 30 - w->starTimer, 30));
        if (w->starTimer <= 0) {
            w->starState = 13;
            w->starTimer = 0;
        }
        break;
    case 13:
        w->starTimer++;
        if (w->starTimer > 30) {
            w->starTimer = 30;
        }
        alpha = (intplGetValue(EASING_LINEAR, 255.0f, 0.0f, w->starTimer, 30));
        break;
    }
    if (w->eff) {
        EffStarStoneWork* e = w->eff->userdata;
        e->position.x = w->starPos.x;
        e->position.y = w->starPos.y;
        e->position.z = w->starPos.z;
        e->rot.x = w->rot.x;
        e->rot.y = w->rot.y;
        e->rot.z = w->rot.z;
        e->scale = 1.5f * w->scale.x;
        e->a = alpha;
    }
}

void main_enemy(void) {
    BattleWork* bw = _battleWorkPointer;
    DekaWork* w = get_ptr();
    s32 n = 0;
    s32 i;
    BattleUnit* unit;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(bw, i);
        if (unit && unit->mAlliance == ALLIANCE_ENEMY) {
            main_enemy_sub(unit, &n);
        }
    }
    for (i = n; i < 10; i++) {
        w->enemies[i] = -1;
    }
}

void main_enemy_sub(BattleUnit* unit, s32* n) {
    DekaWork* w = get_ptr();
    YukaMesh* m = w->mesh->address;
    f32 x;
    f32 xc;
    f32 step;
    f32 x0;
    f32 y;
    s32 i;
    BattleUnitPart* part;
    s32 i2;

    unit->mRotationOffset = unit->offsetCenter;
    w->enemyVel[*n].y -= 1.25f;
    x = unit->mPosition.x;
    if (x >= 250.0f) {
        xc = 250.0f;
    } else if (x <= 0.0f) {
        xc = 0.0f;
    } else {
        xc = x;
    }
    for (i = 64; i < 129; i++) {
        step = w->w0 / 128.0f;
        x0 = step * i + w->min.x;
        if (xc >= x0 && xc < x0 + step) {
            y = (xc - x0) / step * (m->disp[0][i + 1].y - m->disp[0][i].y) + m->disp[0][i].y;
            if (unit->mPosition.y < y) {
                if ((unit->mAttributeFlags & kUnitOnCeiling) && y < 120.0f) {
                    break;
                }
                if (m->vel[0][i].y > 0.0f) {
                    if (w->enemyVel[*n].y < 0.0f) {
                        w->enemyVel[*n].y = 0.0f;
                    }
                    unit->mPosition.y = y;
                    unit->mRotation.z = 0.0f;
                    w->enemyRot[*n] = 0.0f;
                    w->enemyVel[*n].y += 0.5f * m->vel[0][i].y;
                    i2 = BtlUnit_GetBodyPartsId(unit);
                    part = BtlUnit_GetPartsPtr(unit, i2);
                    if (part->attributes & kImmuneToDamageAndStatus) {
                        continue;
                    }
                    if (part->attributes & kClone) {
                        continue;
                    }
                    if (!(unit->mAttributeFlags & kInactive) && !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
                        BtlUnit_SetAnimType(part, POSE_TYPE_DAMAGE);
                    }
                    effSmallStarEntry(0, 2, xc, unit->mPosition.y, unit->mPosition.z, 0.0f, -1.0f, 0.0f);
                    BtlUnit_snd_se(unit, unit->mKindParams->damageSfxName, EVT_NULLPTR, 0);
                    if (unit->mAttributeFlags & kUnitNoSpin) {
                        continue;
                    }
                    if (unit->mAttributeFlags & kInactive) {
                        continue;
                    }
                    if (BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
                        continue;
                    }
                    w->enemyRot[*n] = 20.0f;
                    if (irand(2) == 0) {
                        w->enemyRot[*n] *= -1.0f;
                    }
                }
            }
        }
    }
    unit->mPosition.x = x;
    unit->mPosition.x += w->enemyVel[*n].x;
    unit->mPosition.y += w->enemyVel[*n].y;
    unit->mPosition.z += w->enemyVel[*n].z;
    unit->mRotation.z += w->enemyRot[*n];
    if (unit->mPosition.y < unit->mHomePosition.y) {
        unit->mPosition.y = unit->mHomePosition.y;
        w->enemyVel[*n].y = 0.0f;
        if (!(unit->mAttributeFlags & kInactive) && !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
            unit->mRotation.z = 0.0f;
            w->enemyRot[*n] = 0.0f;
        }
    }
    w->enemies[*n] = BtlUnit_GetUnitId(unit);
    (*n)++;
}

USER_FUNC(end_deka) {
    DekaWork* w = get_ptr();

    effDelete(((EffStarStoneWork*)w->eff->userdata)->dust);
    effDelete(w->eff);
    return 2;
}

static void disp_2D(CameraId cam, void* param) {
    Mtx m;
    Mtx sc;
    Mtx tr;
    GXColor col;
    DekaWork* w = get_ptr();
    f32 right;
    f32 left;
    f32 ratio;
    f32 x;
    DekaTiming* t;
    s32 i;

    left = w->gaugePos.x - 0.5f * w->gaugeW - 8.0f;
    right = 8.0f + (w->gaugePos.x + 0.5f * w->gaugeW);
    if (w->gaugeState >= 2 && w->gaugeState <= 5) {
        col = (GXColor){0xFF, 0xFF, 0xFF, w->alpha};
        PSMTXTrans(m, w->gaugePos.x - 0.5f * w->gaugeW - 8.0f, w->gaugePos.y, w->gaugePos.z);
        btlDispTexPlane2(m, BTL_TEX(0x43), col, 0);
        PSMTXScale(sc, w->gaugeW / 4.0f, 1.0f, 1.0f);
        PSMTXTrans(tr, w->gaugePos.x, w->gaugePos.y, w->gaugePos.z);
        PSMTXConcat(tr, sc, m);
        btlDispTexPlane2(m, BTL_TEX(0x44), col, 0);
        PSMTXScale(sc, -1.0f, 1.0f, 1.0f);
        PSMTXTrans(tr, 8.0f + (0.5f * w->gaugeW + w->gaugePos.x), w->gaugePos.y, w->gaugePos.z);
        PSMTXConcat(tr, sc, m);
        btlDispTexPlane2(m, BTL_TEX(0x43), col, 0);
    }
    if (w->gaugeState >= 2 && w->gaugeState <= 5) {
        ratio = w->cursor / w->gaugeLen;
        if (ratio >= 0.0f && ratio <= 1.0f) {
            btlDispGXInit2DRasta();
            GXLoadPosMtxImm(camGetPtr(CAMERA_2D)->view, 0);
            btlDispGXQuads2DRasta(left, 8.0f + w->gaugePos.y, ratio * (right - left) + left, w->gaugePos.y - 8.0f, 0xFF, 0, 0, w->alpha);
        }
    }
    GXSetScissor(0, 0x78, gp->fbWidth, gp->efbHeight);
    if (w->gaugeState >= 2 && w->gaugeState <= 5) {
        right = right - left;
        for (i = 0; i < 8; i++) {
            t = &w->timing2[i];
            col = (GXColor){0xFF, 0xFF, 0xFF, w->alpha};
            ratio = t->pos / w->gaugeLen;
            x = t->off.x + (ratio * right + left);
            col.a *= t->alpha / 255.0f;
            if (t->type == 1) {
                PSMTXScale(sc, 0.75f, 0.75f, 0.75f);
                PSMTXTrans(tr, x, 40.0f + (w->offsetY + (w->gaugePos.y - 18.0f + t->off.y)), w->gaugePos.z + t->off.z);
                PSMTXConcat(tr, sc, m);
                iconDispGxCol(m, 0x10, ICON_AC_ON, col);
            } else if (t->type == 2) {
                PSMTXScale(sc, 1.0f, 1.0f, 1.0f);
                PSMTXTrans(tr, x, 40.0f + (w->offsetY + (w->gaugePos.y + t->off.y)), w->gaugePos.z + t->off.z);
                PSMTXConcat(tr, sc, m);
                btlDispTexPlane2(m, BTL_TEX(0x2C), col, 0);
            }
        }
        for (i = 0; i < 8; i++) {
            t = &w->timing[i];
            ratio = t->pos / w->gaugeLen;
            x = t->off.x + (ratio * right + left);
            if (t->state >= 2 && t->state <= 5) {
                col = (GXColor){0xFF, 0xFF, 0xFF, w->alpha};
                col.a *= t->alpha / 255.0f;
                if (t->type == 1) {
                    PSMTXScale(sc, 0.75f, 0.75f, 0.75f);
                    PSMTXTrans(tr, x, w->offsetY + (w->gaugePos.y - 18.0f + t->off.y), w->gaugePos.z + t->off.z);
                    PSMTXConcat(tr, sc, m);
                    {
                        s32 id;
                        if (w->cursor >= t->pos - 7.0f) {
                            id = ICON_AC_ON;
                        } else {
                            id = ICON_AC_OFF;
                        }
                        iconDispGxCol(m, 0x10, id, col);
                    }
                } else {
                    PSMTXScale(sc, 1.0f, 1.0f, 1.0f);
                    PSMTXTrans(tr, x, w->offsetY + (w->gaugePos.y + t->off.y), w->gaugePos.z + t->off.z);
                    PSMTXConcat(tr, sc, m);
                    btlDispTexPlane2(m, BTL_TEX(0x2C), col, 0);
                }
            }
            PSMTXScale(sc, 1.0f, 1.0f, 1.0f);
            PSMTXTrans(tr, x, w->gaugePos.y + t->off.y - 40.0f, w->gaugePos.z + t->off.z);
            PSMTXConcat(tr, sc, m);
            switch (t->hit) {
            case 0:
                break;
            case 1:
                btlDispTexPlane2(m, BTL_TEX(0x3E), col, 0);
                break;
            case 2:
                btlDispTexPlane2(m, BTL_TEX(0x3F), col, 0);
                break;
            }
        }
    }
    GXSetScissor(0, 0, gp->fbWidth, gp->efbHeight);
}

static void disp_3D(CameraId cam, void* param) {
}

static void disp_3D_alpha(CameraId cam, void* param) {
}

void yuka_capture(CameraId cam, void* param) {
    CameraEntry save;
    Vec c;
    DekaWork* w = get_ptr();
    CameraEntry* camera = camGetPtr(cam);

    memcpy(&save, camGetPtr(cam), sizeof(CameraEntry));
    c.x = (w->min.x + w->max.x) / 2.0f;
    c.y = (w->min.y + w->max.y) / 2.0f;
    c.z = (w->min.z + w->max.z) / 2.0f;
    camera->cameraUp = (Vec){0.0f, 0.0f, -1.0f};
    camera->cameraPos = c;
    camera->cameraAt = camera->cameraPos;
    {
        f32 t = tan(0.21816616f);
        f32 h = w->h0;
        camera->cameraPos.y += (f32)w->h0 / 2 / t;
    }
    C_MTXLookAt(camera->view, &camera->cameraPos, &camera->cameraUp, &camera->cameraAt);
    C_MTXPerspective(camera->projection, 25.0f, (f32)w->w0 / (f32)w->h0, 1.0f, 32767.0f);
    camera->projType = 0;
    GXSetProjection(camera->projection, camera->projType);
    GXSetViewport(0.0f, 0.0f, w->texW, w->texH, 0.0f, 1.0f);
    GXSetScissor(0, 0, w->texW, w->texH);
    test_kururing_mapdisp(cam);
    sysWaitDrawSync();
    GXSetTexCopySrc(0, 0, w->texW, w->texH);
    GXSetTexCopyDst(w->texW, w->texH, 4, 0);
    GXSetZMode(1, GX_LEQUAL, 1);
    GXCopyTex(w->tex->address, 1);
    GXPixModeSync();
    memcpy(camGetPtr(cam), &save, sizeof(CameraEntry));
}

EVT_BEGIN(marioAttackEvent_Deka)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_PARTNER, LW(14))
    CALL(btlevtcmd_GetBodyId, LW(14), LW(15))
    CALL(btlevtcmd_CheckActStatus, LW(14), LW(0))
    IF_INT_EQUAL(LW(0), 0)
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
        CALL(main_deka, LW(14))
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
    CALL(btlevtcmd_SacBgsetMove, 0)
    CALL_CHILD(sac_mario_back_home)
    CALL(wait_star_stone_take_on)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_JUMP)
        CALL(get_ride_pos, 0, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 30, 0, FLOAT(0.39941406f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("M_I_Y"))
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        IF_INT_NOT_EQUAL(LW(14), -1)
            CALL(btlevtcmd_GetUnitKind, UNIT_ID_PARTNER, LW(0))
            IF_INT_EQUAL(LW(0), 228)
                LOOP(0)
                    CALL(btlevtcmd_SetUnitWork, LW(14), 3, 1)
                    CALL(btlevtcmd_GetPos, LW(14), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_SetUnitWork, LW(14), 0, LW(0))
                    CALL(btlevtcmd_SetUnitWork, LW(14), 1, LW(1))
                    CALL(btlevtcmd_SetUnitWork, LW(14), 2, LW(2))
                    WAIT_FRAMES(1)
                END_LOOP
            END_IF
        END_IF
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        IF_INT_NOT_EQUAL(LW(14), -1)
            CALL(btlevtcmd_AnimeChangePoseType, LW(14), LW(15), POSE_TYPE_JUMP)
            CALL(get_ride_pos, 1, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_JumpSetting, LW(14), 30, 0, FLOAT(0.39941406f))
            CALL(btlevtcmd_JumpPosition, LW(14), LW(0), LW(1), LW(2), 30, -1)
            CALL(btlevtcmd_AnimeChangePoseType, LW(14), LW(15), POSE_TYPE_NEUTRAL)
        END_IF
    END_SIBLING_EVENT
    WAIT_FRAMES(30)
    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(get_ride_pos, 0, LW(0), LW(1), LW(2))
            ADD(LW(2), FLOAT(5.0f))
            CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            IF_INT_NOT_EQUAL(LW(14), -1)
                CALL(get_ride_pos, 1, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_SetPos, LW(14), LW(0), LW(1), LW(2))
            END_IF
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(sac_cheer, 0)
    END_SIBLING_EVENT
    CALL(sac_ac_help_on)
    CALL(start_game)
    CALL(wait_game_end)
    CALL(sac_ac_help_off)
    CALL(sac_cheer_end)
    WAIT_MS(1000)
    CALL(star_stone_attack)
    LABEL(90)
    DELETE_EVENT(LW(13))
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL_CHILD(dmg_evt)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_JUMP)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 30, 0, FLOAT(0.39941406f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -1)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        IF_INT_NOT_EQUAL(LW(14), -1)
            CALL(btlevtcmd_AnimeChangePoseType, LW(14), LW(15), POSE_TYPE_JUMP)
            CALL(btlevtcmd_GetHomePos, LW(14), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_JumpSetting, LW(14), 30, 0, FLOAT(0.39941406f))
            CALL(btlevtcmd_JumpPosition, LW(14), LW(0), LW(1), LW(2), 30, -1)
            CALL(btlevtcmd_StartWaitEvent, LW(14))
        END_IF
    END_SIBLING_EVENT
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_SacBgsetMove, 1)
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_GetUnitKind, UNIT_ID_PARTNER, LW(0))
        IF_INT_EQUAL(LW(0), 228)
            CALL(btlevtcmd_SetUnitWork, LW(14), 3, 0)
        END_IF
    END_IF
    CALL(yuka_end)
    CALL(end_deka)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(dmg_evt)
    CALL(get_result, LW(10))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(10)
    IF_INT_EQUAL(LW(3), -1)
        RETURN
    END_IF
    IF_INT_LESS_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_CheckAttribute, LW(3), kUnitOnCeiling, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            GOTO(30)
        END_IF
        CALL(btlevtcmd_CheckAttribute, LW(3), kUnitAirborne, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_CheckPartsAttribute, LW(3), LW(4), kPartNotHammerTarget, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                GOTO(30)
            END_IF
        END_IF
        GOTO(20)
    END_IF
    IF_INT_LESS_OR_EQUAL(LW(10), 3)
        CALL(btlevtcmd_CheckAttribute, LW(3), kUnitOnCeiling, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            GOTO(30)
        END_IF
        GOTO(20)
    END_IF
    GOTO(20)
    LABEL(20)
    CALL(btlevtcmd_CheckPartsAttribute, LW(3), LW(4), kClone, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_MARIO, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    LABEL(30)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    GOTO(10)
    RETURN
EVT_END()

// Position writes go through a local FIFO pointer (keeps lis 0xcc01 ahead of the loop counters);
// components are read z, y, x like GXPosition3f32's argument evaluation.
#define YUKA_POSITION(fifo, v)        \
    {                                 \
        f32 _x, _y, _z;               \
        _z = (v)->z;                  \
        _y = (v)->y;                  \
        _x = (v)->x;                  \
        (fifo)->f32 = _x;             \
        (fifo)->f32 = _y;             \
        (fifo)->f32 = _z;             \
    }

void yuka_disp(CameraId cam, void* param) {
    DekaWork* w;
    CameraEntry* camera;
    YukaMesh* m;
    GXTexObj obj;
    s32 i;
    volatile PPCWGPipe* fifo;

    w = get_ptr();
    camera = camGetPtr(cam);
    if (w->tex->address != NULL) {
        m = w->mesh->address;
        if (m != NULL) {
            GXSetCullMode(GX_CULL_NONE);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
            GXSetNumChans(0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
            GXInitTexObj(&obj, w->tex->address, w->texW, w->texH, 4, GX_CLAMP, GX_CLAMP, 0);
            GXLoadTexObj(&obj, GX_TEXMAP0);
            GXLoadPosMtxImm(camera->view, 0);
            GXSetCurrentMtx(0);
            fifo = &GXWGFifo;
            i = 0;
            do {
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                YUKA_POSITION(fifo, &m->disp[0][i]);
                GXTexCoord2f32(m->u[0][i], m->v[0][i]);
                YUKA_POSITION(fifo, &m->disp[1][i]);
                GXTexCoord2f32(m->u[1][i], m->v[1][i]);
                YUKA_POSITION(fifo, &m->disp[1][i + 1]);
                GXTexCoord2f32(m->u[1][i + 1], m->v[1][i + 1]);
                YUKA_POSITION(fifo, &m->disp[0][i + 1]);
                GXTexCoord2f32(m->u[0][i + 1], m->v[0][i + 1]);
                i++;
            } while (i < 128);
        }
    }
}

static DekaWork* get_ptr(void) {
    return (DekaWork*)_battleWorkPointer->actionCommands.work;
}

void create_timing(void) {
    DekaWork* w = get_ptr();
    DekaTiming* ta;
    DekaTiming* t;
    s32 i;
    s32 j;
    s32 n;
    s32 cnt;
    f32 f;
    s32 idx[8];
    f32 pos[8];

    memcpy(w->timing2, w->timing, sizeof(w->timing));
top:
    n = w->round + 3;
    for (i = 0; i < 8; i++) {
        t = &w->timing[i];
        t->hit = 0;
        if (i < n) {
            t->type = 1;
            f = (w->gaugeLen - 40.0f) * rand() / 32767.0f;
            if (f <= 30.0f) {
                f = 30.0f;
            }
            if (f >= w->gaugeLen - 20.0f) {
                f = w->gaugeLen - 20.0f;
            }
            t->pos = f;
            t->state = 2;
        } else {
            t->state = 0;
        }
    }
    cnt = 0;
restart:
    for (i = 0; i < n; i++) {
        ta = &w->timing[i];
        for (j = 0; j < n; j++) {
            if (i != j) {
                t = &w->timing[j];
                if (t->state == 2 && __fabs(t->pos - ta->pos) <= 15.75) {
                    f = w->gaugeLen * rand() / 32767.0f;
                    if (f <= 30.0f) {
                        f = 30.0f;
                    }
                    if (f >= w->gaugeLen - 20.0f) {
                        f = w->gaugeLen - 20.0f;
                    }
                    cnt++;
                    ta->pos = f;
                    if (cnt != 100) {
                        goto restart;
                    }
                    ta->state = 0;
                    ta->hit = 0;
                    goto top;
                }
            }
        }
        cnt = 0;
    }
    for (i = 0; i < n; i++) {
        idx[i] = i;
        pos[i] = w->timing[i].pos;
    }
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            f32 pj;
            f32 pi;
            pi = pos[i];
            pj = pos[j];
            if (pi > pj) {
                s32 ti = idx[i];
                pos[i] = pj;
                idx[i] = idx[j];
                idx[j] = ti;
                pos[j] = pi;
            }
        }
    }
    w->timing[idx[n - 1]].pos = w->gaugeLen - 7.0f - 2.0f;
    if (w->timing[idx[0]].pos > 70.0f) {
        w->timing[idx[0]].pos = 70.0f;
    }
}

s32 weaponGetPower_Deka(void) {
    return get_ptr()->result + 1;
}

USER_FUNC(wait_star_stone_take_on) {
    return get_ptr()->starState == 4 ? 2 : 0;
}

static USER_FUNC(start_game) {
    DekaWork* w = get_ptr();
    w->state = 1;
    return 2;
}

static USER_FUNC(wait_game_end) {
    return get_ptr()->state == 5 ? 2 : 0;
}

static USER_FUNC(star_stone_appear) {
    DekaWork* w = get_ptr();
    w->starState = 1;
    return 2;
}

USER_FUNC(get_ride_pos) {
    DekaWork* w = get_ptr();
    EvtWord* args = event->args;
    f32 k = 64.0f;
    if (evtGetValue(event, *args++) == 0) {
        evtSetFloat(event, *args++, 0.5f * (0.25f * (w->scale.x * k)) + w->starPos.x);
    } else {
        evtSetFloat(event, *args++, w->starPos.x - 0.5f * (0.25f * (w->scale.x * k)));
    }
    evtSetFloat(event, *args++, 0.25f * (0.25f * (w->scale.y * k)) + w->starPos.y);
    evtSetFloat(event, *args++, 5.0f + w->starPos.z + 4.0f * w->scale.z);
    return 2;
}

static USER_FUNC(star_stone_attack) {
    DekaWork* w = get_ptr();
    if (isFirstCall) {
        w->starState = 10;
    }
    if (w->starState == 13) {
        return 2;
    } else {
        return 0;
    }
}

USER_FUNC(get_result) {
    DekaWork* w = get_ptr();
    evtSetValue(event, event->args[0], w->result);
    return 2;
}
