#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "manager/evtmgr_cmd.h"
#include "driver/camdrv.h"
#include "mario/mario.h"
#include "memory.h"
#include "string.h"
#include "driver/dispdrv.h"
#include "driver/hitdrv.h"
#include "pmario_sound.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "driver/icondrv.h"
#include "mario/mariost.h"
#include "icon_ids.h"
#include "evt/evt_map.h"
#include "evt/evt_movefloor.h"
#include "yuu.h"

typedef struct RollWork { u32 flags; s32 phase; s32 coins; s32 result; s32 index; EventEntry* jumpEvent; Vec pos; s32 monteCount; EventEntry* viewEvent; } RollWork;
typedef struct MonteSub { s32 unk0; Vec pos; f32 scale; u8 unk14[0x38 - 0x14]; } MonteSub;
typedef struct Monte { u8 hidden; u8 unk1; s16 num; u8 state; u8 unk5[3]; MonteSub sub; MiniGameWork* wp; } Monte;
typedef struct MonteTable { f32 x0; f32 x1; f32 z0; f32 z1; s32 num; s32 type; } MonteTable;
static void* yuunpc_wp;
static Monte* monte_p; /* monte_p */
static Monte* icon_monte_p;
s32 yuucounter_get_data(void);
void yuucounter_flag_on(s32 flag);
void partyKeyOff(void);
void marioResetRollSpd(void);
void clrRollEvtFlag(void);
void monteCountDisp(void);
void marioChgRollMotion(void);
void marioRollFixOn(void);
extern MonteTable montetable[];
static void monte_disp(CameraId camera, void* param);
static BOOL get_monte_check(Vec* a, Vec* b);
void yuu_02_icon_monte_disp(CameraId camera, void* param);
void camShiftReset(void);

USER_FUNC(eff_call_minigame);
USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_cam_get_at);
USER_FUNC(evt_cam_get_pos);
USER_FUNC(evt_cam_letter_box_disable);
USER_FUNC(evt_cam_letter_box_onoff);
USER_FUNC(evt_eff);
USER_FUNC(evt_fade_in);
USER_FUNC(evt_fade_out);
USER_FUNC(evt_key_get_buttontrg);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mario_cam_y_off);
USER_FUNC(evt_mario_cam_y_on);
USER_FUNC(evt_mario_cont_onoff);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_get_name_hitobj_ride);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_jump_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_kill_party);
USER_FUNC(evt_mario_mov_pos2);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mario_set_prev_party_pos);
USER_FUNC(evt_mario_wait_move_end);
USER_FUNC(evt_moving_floor_delete);
USER_FUNC(evt_msg_select);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_yuucounter_alloc);
USER_FUNC(evt_yuucounter_free);
USER_FUNC(evt_yuuminigame_alloc);
USER_FUNC(evt_yuuranking_alloc);
USER_FUNC(evt_yuuranking_free);
USER_FUNC(evt_yuutitle_alloc);
USER_FUNC(evt_yuutitle_free);
static USER_FUNC(fall_rtn);
static USER_FUNC(re_fall_rtn);
USER_FUNC(yuu_02_icon_monte_entry);
USER_FUNC(yuu_02_cam_shift_reset);
USER_FUNC(yuu_atai_init);
USER_FUNC(yuu_atai_pop);
USER_FUNC(yuu_atai_push);
USER_FUNC(yuu_roll_reset);
USER_FUNC(yuugijou_add_montemedal);
USER_FUNC(yuugijou_get_montemedal);
USER_FUNC(yuugijou_montemedal_disp_onoff);
USER_FUNC(yuugijou_montemedal_wait);
USER_FUNC(yuuminigame3_opening_chk);
USER_FUNC(yuuminigame3_opening_on_off);
USER_FUNC(yuuminigame3_roll_fix_off);
USER_FUNC(yuuminigame3_roll_fix_on);
USER_FUNC(yuuminigame3_set_phase);
USER_FUNC(yuuminigame_get_dataadr);
USER_FUNC(yuuminigame_set_dataadr);
USER_FUNC(yuuranking_get_monte);
USER_FUNC(yuuranking_wait_button);
BOOL yuuminigame3_gamerule(EventEntry* event, BOOL isFirstCall, MiniGameWork* wp, RollWork* data);
void yuuminigame_disp_3roll(CameraId camera, void* param);
void yuuminigame_main_3roll(EventEntry* event, BOOL isFirstCall);
void yuuminigame_init_3roll(EventEntry* event, BOOL isFirstCall);
static void monte_disp(CameraId camera, void* param);
static BOOL get_monte_check(Vec* a, Vec* b);
void yuu_02_icon_monte_disp(CameraId camera, void* param);
USER_FUNC(yuu_02_icon_monte_onoff);
void yuu_02_icon_monte_off(s32 idx);
void yuu_02_icon_monte_on(s32 idx);

Vec fukkipoint[15];
EvtWord yuu_02_icon_monte_entry_evt[4];
static MonteTable montetable[11];
static EvtWord minigame_view_monte[4];
EvtWord yuu_02_jumpret[164];
EvtWord yuu_02_kaijo_init[12];
EvtWord yuu_02_kaijo_init2[12];
EvtWord minigame03_main[33];
EvtWord minigame03_start_cam[40];
EvtWord minigame03_start_cam_cancel[17];
EvtWord minigame03_start[193];
EvtWord minigame03_end[217];
EvtWord s_en1_turn[30];
#ifdef VERSION_G8ME01
EvtWord yuu_02_init_evt[262];
#else
EvtWord yuu_02_init_evt[29];
#endif

Vec fukkipoint[] = {
    {300.0f, 0.0f, 300.0f},
    {750.0f, 0.0f, 300.0f},
    {1150.0f, 0.0f, 300.0f},
    {1500.0f, 0.0f, 300.0f},
    {1550.0f, 0.0f, -300.0f},
    {925.0f, 200.0f, -300.0f},
    {250.0f, 200.0f, -300.0f},
    {-250.0f, 200.0f, -300.0f},
    {-950.0f, 200.0f, -300.0f},
    {-1450.0f, 0.0f, -300.0f},
    {-1550.0f, 0.0f, 300.0f},
    {-950.0f, 0.0f, 300.0f},
    {-250.0f, 0.0f, 300.0f},
    {250.0f, 0.0f, 300.0f},
    {250.0f, 0.0f, 300.0f},
};

EVT_BEGIN(yuu_02_icon_monte_entry_evt)
    CALL(yuu_02_icon_monte_entry)
    RETURN
EVT_END()

static MonteTable montetable[] = {
    {1500.0f, 990.0f, -200.0f, -300.0f, 1, 1},
    {1500.0f, 990.0f, -300.0f, -400.0f, 1, 2},
    {300.0f, 900.0f, -250.0f, -275.0f, 1, 1},
    {300.0f, 900.0f, -325.0f, -350.0f, 1, 2},
    {150.0f, -150.0f, -260.0f, -350.0f, 1, 2},
    {-1070.0f, -1330.0f, -225.0f, -375.0f, 1, 2},
    {-950.0f, -950.0f, 300.0f, 300.0f, 1, 1},
    {-1250.0f, -1250.0f, 300.0f, 300.0f, 1, 1},
    {-1350.0f, -1350.0f, 300.0f, 300.0f, 1, 1},
    {-1450.0f, -1450.0f, 300.0f, 300.0f, 1, 2},
    {0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
};

static EVT_BEGIN(minigame_view_monte)
    CALL(fall_rtn)
    RETURN
EVT_END()

USER_FUNC(yuu_02_cam_shift_reset) {
    camShiftReset();
    return EVT_RETURN_DONE;
}

void yuu_02_icon_monte_on(s32 idx) {
    icon_monte_p[idx].hidden = 0;
}

void yuu_02_icon_monte_off(s32 idx) {
    icon_monte_p[idx].hidden = 1;
}

USER_FUNC(yuu_02_icon_monte_onoff) {
    EvtWord* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 idx = evtGetValue(event, args[1]);

    if (icon_monte_p != NULL) {
        if (on) {
            icon_monte_p[idx].hidden = 0;
        } else {
            icon_monte_p[idx].hidden = 1;
        }
    }
    return EVT_RETURN_DONE;
}

void yuu_02_icon_monte_disp(CameraId camera, void* param) {
    Monte* monte = param;
    MarioWork* mario = marioGetPtr();
    s32 i;
    f32 dir;
    Vec pos;

    if (monte != NULL) {
        for (i = 0; i < monte->num; i++) {
            if (mario->position.z >= 0.0f) {
                dir = 1.0f;
            } else {
                dir = -1.0f;
            }
            if (monte[i].hidden == 0) {
                pos = monte[i].sub.pos;
                if ((gp->retraceCount & 0x1F) < 9) {
                    pos.x -= 5.0f * dir;
                }
                iconDispGx(&pos, 0, ICON_163, -2.0f * dir);
            }
        }
    }
}

USER_FUNC(yuu_02_icon_monte_entry) {
    BOOL exists = FALSE;
    MiniGameWork* wp = (MiniGameWork*)event->lwData[0];
    MarioWork* mario = marioGetPtr();
    s32 total;
    s32 i;
    MonteSub* sub;

    if (isFirstCall) {
        if (icon_monte_p != NULL) {
            exists = TRUE;
        }
        total = 0;
        for (i = 0; i < 1; i++) {
            total++;
        }
        icon_monte_p = _mapAlloc(total * sizeof(Monte));
        memset(icon_monte_p, 0, total * sizeof(Monte));
        icon_monte_p->num = total;
        icon_monte_p->wp = wp;
        for (i = 0; i < icon_monte_p->num; i++) {
            sub = &icon_monte_p[i].sub;
            sub->pos = (Vec){0.0f, 0.0f, 0.0f};
            sub->scale = 1.0f;
            icon_monte_p[i].hidden = 1;
        }
    }
    for (i = 0; i < icon_monte_p->num; i++) {
    }
    if (!exists) {
        dispEntry(CAMERA_2D, 0.0f, 0, yuu_02_icon_monte_disp, icon_monte_p);
    }
    return EVT_RETURN_BLOCK;
}

static BOOL get_monte_check(Vec* a, Vec* b) {
    if (PSVECDistance(a, b) < 20.0f && __fabs(a->y - b->y) < 20.0) {
        return TRUE;
    }
    return FALSE;
}

static void monte_disp(CameraId camera, void* param) {
    Monte* monte = param;
    s32 i;

    if (monte != NULL) {
        for (i = 0; i < monte->num; i++) {
            if (monte[i].hidden == 0) {
                iconDispGx(&monte[i].sub.pos, 0, ICON_13F, 1.0f);
            }
        }
    }
}

static USER_FUNC(fall_rtn) {
    MiniGameWork* wp = (MiniGameWork*)event->lwData[0];
    BOOL exists = FALSE;
    RollWork* data = wp->data;
    const char* sfx;
    s32 total;
    s32 i;
    s32 t;
    s32 k;
    s32 j;
    s32 n;
    s32 r;
    s32 retry;
    MonteSub* sub;
    f32 lo;
    f32 hi;
    Vec v;
    Vec a;
    Vec b;
    Vec hit;
    HitCheckQuery query;
    HitCheckQuery* qp;
    Monte* m;

    if (isFirstCall) {
        if (monte_p != NULL) {
            exists = TRUE;
        }
        total = 0;
        for (i = 0; montetable[i].type; i++) {
            total += montetable[i].num;
        }
        monte_p = _mapAlloc(total * sizeof(Monte));
        memset(monte_p, 0, total * sizeof(Monte));
        monte_p->num = total;
        monte_p->wp = wp;
        data->monteCount = 0;
        t = 0;
        k = 0;
        n = 0;
        for (i = 0; i < monte_p->num; i++) {
            sub = &monte_p[i].sub;
            sub->pos.y = 210.0f;
            retry = 0;
            while (TRUE) {
                if (++retry > 500) {
                    sub->pos = (Vec){0.0f, 0.0f, 0.0f};
                    break;
                }
                if (montetable[t].x0 < montetable[t].x1) {
                    lo = montetable[t].x0;
                    hi = montetable[t].x1;
                } else {
                    lo = montetable[t].x1;
                    hi = montetable[t].x0;
                }
                sub->pos.x = lo + irand(100.0f * (hi - lo)) / 100.0f;
                if (montetable[t].z0 < montetable[t].z1) {
                    lo = montetable[t].z0;
                    hi = montetable[t].z1;
                } else {
                    lo = montetable[t].z1;
                    hi = montetable[t].z0;
                }
                hi -= lo;
                sub->pos.z = lo + irand(100.0f * hi) / 100.0f;
                a = sub->pos;
                a.y = 0.0f;
                for (j = 0; j < k; j++) {
                    b = (sub - (j + 1))->pos;
                    b.y = 0.0f;
                    PSVECSubtract(&a, &b, &v);
                    if (PSVECMag(&v) < 20.0f) {
                        break;
                    }
                }
                if (j != k) {
                    continue;
                }
                qp = &query;
                query.targetPos = sub->pos;
                query.targetDistance = 220.0f;
                query.targetDir = (Vec){0.0f, -1.0f, 0.0f};
                if (hitCheckVecFilter(qp, NULL) == NULL) {
                    continue;
                }
                query.targetPos.x -= 9.0f;
                hit = query.hitPos;
                if (hitCheckVecFilter(qp, NULL) != NULL && query.hitPos.y > hit.y + 1.0f) {
                    continue;
                }
                sub->pos.y = query.hitPos.y;
                break;
            }
            n++;
            if (++k >= montetable[t].num) {
                if (montetable[t].type == 2) {
                    for (j = 0; j < n; j++) {
                        monte_p[i - j].hidden = 1;
                        monte_p[i - j].state = 1;
                    }
                    for (j = 0; j < montetable[t].num; j++) {
                        do {
                            r = irand(n);
                        } while (!monte_p[i - r].hidden);
                        monte_p[i - r].hidden = 0;
                        monte_p[i - r].state = 0;
                    }
                    n = 0;
                }
                k = 0;
                t++;
            }
            sub->scale = 1.0f;
        }
    }
    sfx = (0, "SFX_EVT_GAME_MONTE_COUNT1"); /* comma expr: string address into its own saved reg */
    for (i = 0; i < monte_p->num; i++) {
        m = &monte_p[i];
        switch (m->state) {
            case 0:
                if (get_monte_check(&m->sub.pos, &marioGetPtr()->position) == TRUE) {
                    data->monteCount++;
                    psndSFXOn_3D(sfx, &marioGetPtr()->position);
                    m->hidden = 1;
                    m->state = 1;
                }
                break;
            case 1:
                break;
        }
    }
    if (!exists) {
        dispEntry(CAMERA_3D, 0.0f, 1, monte_disp, monte_p);
    }
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(re_fall_rtn) {
    RollWork* data;

    if (monte_p != NULL) {
        data = monte_p->wp->data;
        if (data->viewEvent != NULL) {
            evtDeleteID(data->viewEvent->eventId);
            data->viewEvent = NULL;
            _mapFree(monte_p);
            data->viewEvent = evtEntryType(minigame_view_monte, 0, 0, 0);
            data->viewEvent->lwData[0] = (EvtWord)monte_p->wp;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame3_set_phase) {
    EvtWord* args = event->args;
    RollWork* data = ((MiniGameWork*)evtGetValue(event, args[0]))->data;

    data->phase = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame3_roll_fix_on) {
    marioRollFixOn();
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame3_roll_fix_off) {
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame3_opening_on_off) {
    EvtWord* args = event->args;
    RollWork* data = ((MiniGameWork*)evtGetValue(event, args[0]))->data;

    if (evtGetValue(event, args[1])) {
        data->flags |= 1;
    } else {
        data->flags &= ~1;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame3_opening_chk) {
    EvtWord* args = event->args;
    MiniGameWork* wp = (MiniGameWork*)evtGetValue(event, args[0]);

    if (((RollWork*)wp->data)->flags & 1) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

void yuuminigame_init_3roll(EventEntry* event, BOOL isFirstCall) {
    RollWork* data = ((MiniGameWork*)event->lwData[0])->data;
    EventEntry* e;

    e = evtEntryType(yuu_02_icon_monte_entry_evt, 0, 0, 0xEF);
    e->lwData[0] = event->lwData[0];
    data->viewEvent = evtEntryType(minigame_view_monte, 0, 0, 0);
    data->viewEvent->lwData[0] = event->lwData[0];
    data->flags |= 1;
    marioChgRollMotion();
}

void yuuminigame_main_3roll(EventEntry* event, BOOL isFirstCall) {
    MiniGameWork* wp = (MiniGameWork*)event->lwData[0];
    RollWork* data = wp->data;
    MarioWork* mario = marioGetPtr();

    switch (data->phase) {
        case 0:
            break;
        case 5:
            data->phase = 1;
        case 1:
            wp->subEvent = evtEntry(minigame03_start, 0, 0);
            wp->subEvent->lwData[0] = event->lwData[0];
            data->phase++;
            break;
        case 2:
            data->pos = mario->position;
            data->phase = 3;
            data->result = 0;
        case 3:
            if (yuuminigame3_gamerule(event, isFirstCall, wp, data)) {
                data->phase++;
            }
            break;
        case 4:
            wp->subEvent = evtEntry(minigame03_end, 0, 0);
            wp->subEvent->lwData[0] = data->coins;
            if (data->result + 5 <= 0) {
                data->result = 1;
            } else {
                data->result += 261;
            }
            wp->subEvent->lwData[1] = data->result;
            wp->subEvent->lwData[2] = data->monteCount;
            data->phase++;
            break;
    }
}

void yuuminigame_disp_3roll(CameraId camera, void* param) {
    monteCountDisp();
}

USER_FUNC(yuu_roll_reset) {
    marioResetRollSpd();
    clrRollEvtFlag();
    return EVT_RETURN_DONE;
}

BOOL yuuminigame3_gamerule(EventEntry* event, BOOL isFirstCall, MiniGameWork* wp, RollWork* data) {
    MarioWork* mario = marioGetPtr();

    if (!(data->flags & 2) && mario->position.y <= -50.0f) {
        data->flags |= 2;
        data->jumpEvent = evtEntry(yuu_02_jumpret, 0, 0);
        data->jumpEvent->lwData[0] = (s32)fukkipoint[data->index].x + (fukkipoint[data->index].z >= 0.0f ? 5 : -5);
        data->jumpEvent->lwData[1] = (s32)fukkipoint[data->index].y + 200;
        data->jumpEvent->lwData[2] = (s32)fukkipoint[data->index].z;
        data->jumpEvent->lwData[3] = (s32)fukkipoint[data->index].y;
        data->result--;
    }
    if (!(data->flags & 2)) {
        if (mario->position.z * fukkipoint[data->index + 1].z >= 0.0f && mario->position.y >= fukkipoint[data->index + 1].y - 1.0f) {
            if ((mario->position.z >= 0.0f && data->pos.x < fukkipoint[data->index + 1].x && mario->position.x >= fukkipoint[data->index + 1].x) ||
                (mario->position.z < 0.0f && data->pos.x > fukkipoint[data->index + 1].x && mario->position.x <= fukkipoint[data->index + 1].x)) {
                if (data->index < 14) {
                    data->index++;
                }
                icon_monte_p->hidden = 1;
            }
        }
        if (mario->position.z * fukkipoint[data->index].z >= 0.0f) {
            if ((mario->position.z < 0.0f && data->pos.x < fukkipoint[data->index].x && mario->position.x >= fukkipoint[data->index].x) ||
                (mario->position.z >= 0.0f && data->pos.x > fukkipoint[data->index].x && mario->position.x <= fukkipoint[data->index].x)) {
                if (data->index > 0) {
                    data->index--;
                }
                icon_monte_p->hidden = 0;
            }
        }
        data->pos = mario->position;
        if (yuucounter_get_data() < 0 || data->index >= 13) {
            data->coins = yuucounter_get_data();
            data->index = 0;
            marioKeyOff();
            partyKeyOff();
            yuucounter_flag_on(2);
            icon_monte_p->hidden = 1;
            return TRUE;
        }
    } else {
        data->pos = mario->position;
        if (!evtCheckID(data->jumpEvent->eventId)) {
            data->flags &= ~2;
        }
    }
    return FALSE;
}

EVT_BEGIN(yuu_02_jumpret)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam_letter_box_onoff, 1, 0)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(evt_cam_get_pos, 4, LW(7), LW(8), LW(9))
    CALL(evt_cam_get_at, 4, LW(10), LW(11), LW(12))
    CALL(evt_cam3d_evt_set, LW(7), LW(8), LW(9), LW(10), LW(11), LW(12), 0, 0)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(4), LW(5), LW(6))
        IF_INT_LESS_OR_EQUAL(LW(5), -400)
            CALL(evt_mario_cont_onoff, 0)
            CALL(evt_mario_dispflag_onoff, 1, 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_set_pos, LW(0), LW(3), LW(2))
    CALL(yuu_roll_reset)
    CALL(evt_mario_cam_y_on)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_off, 1000, 0)
    WAIT_MS(1000)
    CALL(evt_cam_get_pos, 4, LW(7), LW(8), LW(9))
    CALL(evt_cam_get_at, 4, LW(10), LW(11), LW(12))
    CALL(evt_cam3d_evt_set, LW(7), LW(8), LW(9), LW(10), LW(11), LW(12), 0, 0)
    CALL(evt_mario_cam_y_off)
    CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_mario_dispflag_onoff, 0, 1)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
        CALL(evt_mario_set_pos, LW(0), LW(4), LW(2))
        CALL(evt_mario_get_name_hitobj_ride, LW(6))
        IF_STR_NOT_EQUAL(LW(6), 0)
            CALL(evt_eff, STRING(""), STRING("kemuri"), 0, LW(0), LW(4), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_cam3d_evt_off, 500, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_onoff, 0, 0)
    CALL(evt_cam_letter_box_disable, 1)
    RETURN
EVT_END()

EVT_BEGIN(yuu_02_kaijo_init)
    CALL(evt_mario_set_pos, 200, 0, 300)
    CALL(evt_mario_set_dir, 90, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(yuu_02_kaijo_init2)
    CALL(evt_mario_set_pos, 200, 0, 300)
    CALL(evt_mario_set_dir, 90, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(minigame03_main)
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(yuu_02_kaijo_init))
    CALL(evt_yuuminigame_alloc, LW(0), 2)
    CALL(yuuminigame_set_dataadr, 0, LW(0))
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mario_kill_party, 1)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE1"), GW(10))
    CALL(yuuminigame_get_dataadr, 0, LW(0))
    CALL(yuuminigame3_set_phase, LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(minigame03_start_cam)
    CALL(evt_cam3d_evt_set, FLOAT(-1600.0f), FLOAT(573.0f), FLOAT(1333.0f), FLOAT(-1600.0f), FLOAT(12.0f), FLOAT(-89.0f), 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_seq_wait, 2)
    CALL(evt_cam3d_evt_set, FLOAT(1600.0f), FLOAT(573.0f), FLOAT(1333.0f), FLOAT(1600.0f), FLOAT(12.0f), FLOAT(-89.0f), 8000, 0)
    WAIT_MS(1000)
    CALL(evt_yuutitle_alloc, 0, 0, STRING("menu_title_minigame3"))
    WAIT_MS(5000)
    CALL(evt_yuutitle_free)
    WAIT_MS(2000)
    RETURN
EVT_END()

EVT_BEGIN(minigame03_start_cam_cancel)
    CALL(evt_seq_wait, 2)
    CALL(evt_yuutitle_free)
    CALL(evt_cam3d_evt_off, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_mario_wait_move_end)
    CALL_CHILD(PTR(yuu_02_kaijo_init2))
    RETURN
EVT_END()

EVT_BEGIN(minigame03_start)
    CALL(yuu_atai_init)
    CALL(yuu_atai_push, LW(0))
    SET_VALUE(LW(0), STRING("S_ugoku"))
    SET_VALUE(LW(1), STRING("ugo1"))
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), -200)
    SET_VALUE(LW(4), 2400)
    SET_VALUE(LW(5), 600)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 1, LW(15))
    SET_VALUE(LW(0), STRING("S_ugoku1"))
    SET_VALUE(LW(1), STRING("ugo2"))
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 200)
    SET_VALUE(LW(4), 2400)
    SET_VALUE(LW(5), 600)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    CALL(yuuminigame_set_dataadr, 2, LW(15))
    CALL(yuu_atai_pop, LW(0))
    CALL(yuuminigame3_opening_chk, LW(0), LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        SPAWN_EVENT_GET_ID(PTR(minigame03_start_cam), LW(8))
        LOOP(0)
            IS_EVENT_RUNNING(LW(8), LW(9))
            IF_INT_EQUAL(LW(9), 0)
                CALL(evt_seq_wait, 2)
                BREAK_LOOP
                ELSE
                CALL(evt_key_get_buttontrg, 0, LW(9))
                IF_BITS_SET(LW(9), 768)
                    DELETE_EVENT(LW(8))
                    CALL(evt_seq_wait, 2)
                    CALL_CHILD(PTR(minigame03_start_cam_cancel))
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(yuuminigame3_opening_on_off, LW(0), 0)
        CALL_CHILD(PTR(yuu_02_kaijo_init2))
        CALL(evt_cam3d_evt_off, 0, 11)
        WAIT_FRAMES(1)
    END_IF
    CALL(evt_mario_mov_pos2, 305, 300, 120)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_MINIGAME1"))
    CALL(eff_call_minigame, 0, 0, 0, 500)
    WAIT_MS(3000)
    CALL(yuuminigame3_roll_fix_on)
    CALL(evt_cam_letter_box_onoff, 0, 0)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_yuucounter_alloc, 0, 70, 1, 180)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(minigame03_end)
    CALL(evt_cam_letter_box_onoff, 1, 0)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE2"), 0)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            IF_INT_LESS(LW(1), -50)
                CALL(evt_mario_jump_pos, 300, 0, 300, 600, 0, -1000)
            END_IF
            IF_INT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(yuuminigame_get_dataadr, 1, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
    CALL(yuuminigame_get_dataadr, 2, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
    CALL(yuuminigame3_roll_fix_off)
    IF_INT_LESS(LW(0), 0)
        CALL(eff_call_minigame, 2, 0, 0, 500)
        WAIT_MS(4000)
        ELSE
        CALL(eff_call_minigame, 1, 0, 0, 500)
        WAIT_MS(4000)
    END_IF
    CALL(evt_yuucounter_free)
    CALL(yuugijou_montemedal_disp_onoff, 1)
    CALL(evt_yuuranking_alloc, 0, 0, 2, 0, LW(0), LW(2), LW(1), 180)
    WAIT_FRAMES(1)
    CALL(yuuranking_wait_button)
    CALL(yuuranking_get_monte, LW(3))
    CALL(yuugijou_add_montemedal, 0, LW(3))
    CALL(yuugijou_montemedal_wait)
    CALL(evt_yuuranking_free)
    WAIT_MS(200)
    CALL(evt_yuuranking_alloc, 0, 0, 2, 1, LW(0), LW(2), LW(1), 180)
    WAIT_FRAMES(1)
    CALL(yuuranking_wait_button)
    CALL(evt_yuuranking_free)
    SET_VALUE(LW(1), 10)
    CALL(yuugijou_get_montemedal, LW(0))
    IF_INT_LESS(LW(0), LW(1))
        SET_VALUE(LW(0), 1)
        ELSE
        CALL(evt_msg_select, 0, STRING("sysyuu_02_00"))
    END_IF
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GSWF(1229), 1)
        CALL(evt_mario_set_prev_party_pos, 0, 0, -1000, 0)
        CALL(evt_mario_set_prev_party_pos, 1, 0, -1000, 0)
        CALL(evt_bero_mapchange, STRING("gor_03"), STRING("A_dokan_02"))
        CALL(evt_snd_bgmoff, 512)
        RETURN
    END_IF
    MULTIPLY(LW(1), -1)
    CALL(yuugijou_add_montemedal, 0, LW(1))
    CALL(yuugijou_montemedal_wait)
    WAIT_MS(200)
    CALL(yuugijou_montemedal_disp_onoff, 0)
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_snd_sfxoff, GW(10))
    CALL(evt_fade_out, 1000)
    CALL(re_fall_rtn)
    SET_VALUE(GF(0), 0)
    CALL_CHILD(PTR(yuu_02_kaijo_init2))
    CALL(yuu_02_cam_shift_reset)
    WAIT_MS(500)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE1"), GW(10))
    CALL(evt_fade_in, 1000)
    RETURN
EVT_END()

EVT_BEGIN(s_en1_turn)
    SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(1), FLOAT(0.75f))
    LOOP(0)
        ADD_FLOAT(LW(0), LW(1))
        IF_INT_GREATER_OR_EQUAL(LW(0), FLOAT(360.0f))
            SUBTRACT_FLOAT(LW(0), FLOAT(360.0f))
        END_IF
        CALL(evt_mapobj_rotate, 1, STRING("S_en1"), FLOAT(0.0f), LW(0), FLOAT(0.0f))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_sub_get_language);
EVT_BEGIN(yuu_02_init_evt)
    CALL(evt_map_playanim, STRING("den1"), 1, 0)
    CALL(evt_map_playanim, STRING("den2"), 1, 0)
    CALL(evt_map_playanim, STRING("kumo"), 1, 0)
    CALL(evt_map_playanim, STRING("wakka"), 1, 0)
    SPAWN_EVENT(s_en1_turn)
    SPAWN_EVENT(minigame03_main)
    CALL_CHILD(evt_yuunpc_minigame3)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_den_mont"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_den_mont"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_den_mont"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_den_mont"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_den_mont"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_den_mont"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_kan"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_kan"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_kan"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_kan"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_kan"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_kan"), 16384)
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_mon"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_den_mont"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_kan"), 16384)
        CASE_EQUAL(1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_mon"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_den_mont"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_kan"), 16384)
        CASE_EQUAL(2)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_mon"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_den_mont"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_kan"), 16384)
        CASE_EQUAL(3)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_mon"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_den_mont"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_kan"), 16384)
        CASE_EQUAL(4)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_mon"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_den_mont"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_kan"), 16384)
        CASE_EQUAL(5)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_mon"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_den_mont"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_kan"), 16384)
    END_SWITCH
    RETURN
EVT_END()
#else
EVT_BEGIN(yuu_02_init_evt)
    CALL(evt_map_playanim, STRING("den1"), 1, 0)
    CALL(evt_map_playanim, STRING("den2"), 1, 0)
    CALL(evt_map_playanim, STRING("kumo"), 1, 0)
    CALL(evt_map_playanim, STRING("wakka"), 1, 0)
    SPAWN_EVENT(s_en1_turn)
    SPAWN_EVENT(minigame03_main)
    CALL_CHILD(evt_yuunpc_minigame3)
    RETURN
EVT_END_PAD()
#endif
