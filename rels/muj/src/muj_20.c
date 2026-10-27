#include "version.h"
#include "eff/eff_funemizu.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "eff/eff_mist.h"
#include "eff/eff_splash.h"
#include "eff/n64/eff_water_damage_n64.h"
#include "evt/evt_bero.h"
#include "evt/evt_hit.h"
#include "evt/evt_sub.h"
#include "evt/evt_light.h"
#include "evt/evt_map.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "pmario_sound.h"
#include "mario/mario.h"

#include "driver/npcdrv.h"
#include "mario/mariost.h"
#include "dolphin/os.h"
#include "system.h"
#include "memory.h"
#include "manager/evtmgr_cmd.h"
#include "driver/mapdrv.h"
#include "dolphin/gx.h"
#include "driver/effdrv.h"
#include "dolphin/mtx.h"
#include "driver/camdrv.h"
#include "win/win_main.h"
#include "driver/dispdrv.h"
#include "manager/filemgr.h"
#include "string.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_nannpc.h"
#include "driver/extdrv.h"
#include "party/party_motion.h"
#include "muj.h"
#include "data_tables.h"
#include <dolphin/pad.h>
#include "driver/seqdrv.h"

typedef struct MarcoShip {
    f32 angle;
    f32 speed;
    f32 max;
    s32 stopped;
    s32 state;
    s32 field_0x14;
    OSTime time;
} MarcoShip;
MarcoShip* marcoship;
typedef struct MapAnimRate {
    f32 fune;
    f32 funeTarget;
    f32 nami;
    f32 namiTarget;
} MapAnimRate;
MapAnimRate mapAnimRate;
static u8* _ap;
typedef struct LogoWork {
    FileEntry* tpl;
    f32 y;
    s32 alpha;
    s32 state;
    s32 counter;
} LogoWork;
static void draw(CameraId camera, void* param);

static USER_FUNC(alpha_init_func);
static USER_FUNC(alpha_set_func);
USER_FUNC(elmos1000tai_init);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_nannpc_entry);
USER_FUNC(evt_snd_envoff_f);
USER_FUNC(evt_snd_sfx_dist);
static USER_FUNC(fune_eff);
USER_FUNC(kaizoku_fune_eff);
USER_FUNC(logo);
USER_FUNC(marcoship_shizumu_func);
USER_FUNC(marcoship_yure_func);
USER_FUNC(marcoship_yure_init_func);
USER_FUNC(mist_func);
USER_FUNC(splash_eff);
USER_FUNC(fune_curve_pos);
USER_FUNC(urouro_func);
USER_FUNC(urouro_fuwafuwa_func);
static void pressStartGX(LogoWork* wp, s32 alpha, f32 x, f32 y);
void elmos1000tai_seq00(NanNPC* npc);
USER_FUNC(elmos1000tai_start);
static void alpha_tev(MapTevCallbackInfo* info);
USER_FUNC(evt_set_nami_anim_rate);
USER_FUNC(evt_set_fune_anim_rate);
USER_FUNC(evt_map_anim_rate);
USER_FUNC(marcoship_stop_wait_func);
USER_FUNC(marcoship_stop_func);
extern s32 evt_npc_set_run_anim[];
extern s32 evt_npc_set_stay_anim[];

EvtWord hiru_yoru_event[46];
EvtWord yoru_hiru_event[46];
EvtWord urouro[61];
EvtWord urouro_fuwafuwa[35];
EvtWord buruburu[23];
EvtWord fuwafuwa[15];
EvtWord kirimomi[29];
EvtWord marco_ship_yure[29];
EvtWord moya_effect01[52];
EvtWord moya_effect02[52];
EvtWord moya_effect03[52];
EvtWord moya_effect04[52];
EvtWord moya_effect05[52];
EvtWord nami_alpha[41];
EvtWord moonout_effect[53];
EvtWord moya_effect[19];
EvtWord _kakushuku[32];
EvtWord elmos_a_kakushuku[63];
EvtWord elmos_a_kaku[13];
EvtWord elmos_a_shuku[13];
static ExtTexInfo ext_entry_data[21];
static s16 elmos_animtbl[22];
static EvtWord nannpc_mode_setup[35];
s32 mist_pos[91];
EvtWord mist_setup[45];
EvtWord marcoship_shizumu[156];
EvtWord onryo_event[2250];
EvtWord muj_20_koopa_evt[133];
EvtWord kaizoku_hata[65];
EvtWord gor_muj_ship[195];
static EvtWord muj_gor_ship[195];
EvtWord anm_kaji[61];
EvtWord anm_wakka[40];
EvtWord anm_entotsu[33];
EvtWord anm_neji[33];
EvtWord anm_yure[132];
static EvtWord evt_prologue[182];
static EvtWord epilogue_npc_entry[25];
EvtWord fune_move[136];
static EvtWord epilogue_evt[97];
EvtWord muj_20_init_evt[362];

EVT_BEGIN(hiru_yoru_event)
    CALL(evt_map_playanim, STRING("hiru_yoru"), MAP_ANIM_PLAY_ONCE, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_map_set_playrate, STRING("hiru_yoru"), FLOAT(4.0f))
        WAIT_MS(750)
        CALL(evt_map_set_playrate, STRING("hiru_yoru"), FLOAT(3.399414f))
        WAIT_MS(200)
        CALL(evt_map_set_playrate, STRING("hiru_yoru"), FLOAT(2.7998047f))
        WAIT_MS(100)
        CALL(evt_map_set_playrate, STRING("hiru_yoru"), FLOAT(1.5f))
    END_INLINE_EVENT
    LOOP(0)
        CALL(evt_map_checkanim, STRING("hiru_yoru"), LW(0), LW(1))
        IF_INT_EQUAL(LW(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(yoru_hiru_event)
    CALL(evt_map_playanim, STRING("yoru_hiru"), MAP_ANIM_PLAY_ONCE, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_map_set_playrate, STRING("yoru_hiru"), FLOAT(4.0f))
        WAIT_MS(750)
        CALL(evt_map_set_playrate, STRING("yoru_hiru"), FLOAT(3.399414f))
        WAIT_MS(100)
        CALL(evt_map_set_playrate, STRING("yoru_hiru"), FLOAT(2.7998047f))
        WAIT_MS(100)
        CALL(evt_map_set_playrate, STRING("yoru_hiru"), FLOAT(1.5f))
    END_INLINE_EVENT
    LOOP(0)
        CALL(evt_map_checkanim, STRING("yoru_hiru"), LW(0), LW(1))
        IF_INT_EQUAL(LW(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

USER_FUNC(urouro_func) {
    int t;
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 sx = evtGetValue(event, args[1]);
    f32 sz = evtGetValue(event, args[2]);
    f32 ex = evtGetValue(event, args[3]);
    f32 ez = evtGetValue(event, args[4]);
    s32 noDir = evtGetValue(event, args[5]);
    s32 duration = evtGetValue(event, args[6]);
    s32 mode = evtGetValue(event, args[7]);
    NpcEntry* npc = evtNpcNameToPtr(event, name);

    if (isFirstCall) {
        event->savedTime = gp->renderTime;
    }
    t = OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime);
    if (t < duration) {
        sx = intplGetValue(mode, sx, ex, t, duration);
        sz = intplGetValue(mode, sz, ez, t, duration);
        if (!noDir) {
            if (npc->position.x > sx) {
                npc->direction = 270.0f;
            } else {
                npc->direction = 90.0f;
            }
        }
        npc->position.x = npc->prevPosition.x = sx;
        npc->position.z = npc->prevPosition.z = sz;
        return EVT_RETURN_BLOCK;
    }
    if (!noDir) {
        if (npc->position.x > ex) {
            npc->direction = 270.0f;
        } else {
            npc->direction = 90.0f;
        }
    }
    npc->position.x = npc->prevPosition.x = ex;
    npc->position.z = npc->prevPosition.z = ez;
    return EVT_RETURN_DONE;
}

USER_FUNC(urouro_fuwafuwa_func) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 sx = evtGetValue(event, args[1]);
    f32 sy = evtGetValue(event, args[2]);
    f32 sz = evtGetValue(event, args[3]);
    f32 ex = evtGetValue(event, args[4]);
    f32 ey = evtGetValue(event, args[5]);
    f32 ez = evtGetValue(event, args[6]);
    s32 noDir = evtGetValue(event, args[7]);
    s32 duration = evtGetValue(event, args[8]);
    s32 mode = evtGetValue(event, args[9]);
    NpcEntry* npc = evtNpcNameToPtr(event, name);
    f32 tbl[32] = {
        0.0f, 0.5f, 1.0f, 1.5f, 2.0f, 2.2f, 2.5f, 2.7f, 2.7f, 2.7f, 2.5f, 2.2f, 2.0f, 1.5f, 1.0f, 0.5f,
        -0.0f, -0.5f, -1.0f, -1.5f, -2.0f, -2.2f, -2.5f, -2.7f, -2.7f, -2.7f, -2.5f, -2.2f, -2.0f, -1.5f, -1.0f, -0.5f,
    };
    s32 t;
    f32 dy;

    if (isFirstCall) {
        event->savedTime = gp->renderTime;
    }
    t = OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime);
    if (t < duration) {
        sx = intplGetValue(mode, sx, ex, t, duration);
        sy = intplGetValue(mode, sy, ey, t, duration);
        sz = intplGetValue(mode, sz, ez, t, duration);
        if (!noDir) {
            if (npc->position.x > sx) {
                npc->direction = 270.0f;
            } else {
                npc->direction = 90.0f;
            }
        }
        dy = tbl[sysMsec2Frame(t) & 31];
        npc->prevPosition = (Vec){sx, sy + dy, sz};
        npc->position = npc->prevPosition;
        return EVT_RETURN_BLOCK;
    }
    if (!noDir) {
        if (npc->position.x > ex) {
            npc->direction = 270.0f;
        } else {
            npc->direction = 90.0f;
        }
    }
    npc->prevPosition = (Vec){ex, ey, ez};
    npc->position = npc->prevPosition;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(urouro)
    CALL(evt_npc_get_position, LW(0), LW(11), LW(12), LW(13))
    LOOP(0)
        CALL(urouro_func, LW(0), LW(11), LW(13), LW(1), LW(3), LW(4), LW(5), EASING_LINEAR)
        BEGIN_INLINE_EVENT
            CALL(evt_npc_get_position, LW(0), LW(11), LW(12), LW(13))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_NPC_ESCAPE1"), LW(11), LW(12), LW(13), 0)
        END_INLINE_EVENT
        CALL(urouro_func, LW(0), LW(1), LW(3), LW(11), LW(13), LW(4), LW(5), EASING_LINEAR)
        BEGIN_INLINE_EVENT
            CALL(evt_npc_get_position, LW(0), LW(11), LW(12), LW(13))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_NPC_ESCAPE1"), LW(11), LW(12), LW(13), 0)
        END_INLINE_EVENT
    END_LOOP
    RETURN
EVT_END()

USER_FUNC(marcoship_yure_init_func) {
    MarcoShip* ms;

    marcoship = ms = _mapAlloc(sizeof(MarcoShip));
    ms->angle = 0.0f;
    ms->speed = 0.02f;
    ms->max = 1.5f;
    ms->stopped = 0;
    ms->state = 0;
    ms->time = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(marcoship_yure_func) {
    MarcoShip* ms = marcoship;
    EvtWord* args = event->args;

    switch (ms->state) {
        case 0:
            if (ms->speed > 0.0f) {
                if (ms->angle > ms->max) {
                    ms->speed *= -1.0f;
                    ms->state = 1;
                    ms->time = gp->renderTime;
                    break;
                }
            } else if (ms->angle < -ms->max) {
                ms->speed *= -1.0f;
                ms->state = 1;
                ms->time = gp->renderTime;
                break;
            }
            ms->angle += ms->speed;
            break;
        case 1:
            if ((s32)OSTicksToMilliseconds((u32)gp->renderTime - (u32)ms->time) > 500) {
                ms->state = 0;
            }
            break;
        case 2:
            if (ms->speed > 0.0f) {
                if (ms->angle > 0.0f) {
                    ms->state = 3;
                    break;
                }
            } else if (ms->angle < 0.0f) {
                ms->state = 3;
                break;
            }
            ms->angle += ms->speed;
            break;
        case 3:
            ms->angle = 0.0f;
            ms->stopped = 1;
            break;
    }
    evtSetFloat(event, args[0], ms->angle);
    evtSetValue(event, args[1], ms->stopped);
    return EVT_RETURN_DONE;
}

USER_FUNC(marcoship_stop_func) {
    marcoship->state = 2;
    return EVT_RETURN_DONE;
}

USER_FUNC(marcoship_stop_wait_func) {
    return marcoship->stopped ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

USER_FUNC(evt_map_anim_rate) {
    MapAnimRate* rate = &mapAnimRate;
    f32 d;

    if (isFirstCall) {
        rate->fune = 1.0f;
        rate->nami = 1.0f;
        rate->funeTarget = 1.0f;
        rate->namiTarget = 1.0f;
    }
    d = rate->funeTarget - rate->fune;
    if (d >= 0.005f) {
        d = 0.005f;
    } else if (d <= -0.005f) {
        d = -0.005f;
    }
    rate->fune += d;
    d = rate->namiTarget - rate->nami;
    if (d >= 0.005f) {
        d = 0.005f;
    } else if (d <= -0.005f) {
        d = -0.005f;
    }
    rate->nami += d;
    mapSetPlayRate("fune", rate->fune);
    mapSetPlayRate("fune_01", rate->fune);
    mapSetPlayRate("nami", rate->nami);
    mapSetPlayRate("nami_02", rate->nami);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_set_fune_anim_rate) {
    mapAnimRate.funeTarget = evtGetFloat(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_set_nami_anim_rate) {
    mapAnimRate.namiTarget = evtGetFloat(event, event->args[0]);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(urouro_fuwafuwa)
    CALL(evt_npc_get_position, LW(0), LW(11), LW(12), LW(13))
    LOOP(0)
        CALL(urouro_fuwafuwa_func, LW(0), LW(11), LW(12), LW(13), LW(1), LW(2), LW(3), LW(4), LW(5), EASING_LINEAR)
        CALL(urouro_fuwafuwa_func, LW(0), LW(1), LW(2), LW(3), LW(11), LW(12), LW(13), LW(4), LW(5), EASING_LINEAR)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(buruburu)
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_sub_random, 200, LW(10))
    ADD(LW(10), 100)
    WAIT_MS(LW(10))
    CALL(muj_npc_buruburu, LW(0), 0, FLOAT(1.0f), 10, 0)
    RETURN
EVT_END()

EVT_BEGIN(fuwafuwa)
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_AUTO_MOVE_ANIM)
    LOOP(0)
        CALL(muj_npc_fuwafuwa, LW(0), 0, 10000)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(kirimomi)
    LOOP(0)
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 360, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_npc_set_ry, LW(10), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(marco_ship_yure)
    CALL(marcoship_yure_init_func)
    CALL(evt_hit_bind_mapobj, STRING("A_a1"), STRING("S_a1"))
    LOOP(0)
        CALL(marcoship_yure_func, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 1, STRING("S_a1"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(moya_effect01)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hai_l_01"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hai_r_01"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 1500, 0, 7000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(2), 0)
        SUBTRACT(LW(2), LW(0))
        CALL(evt_mapobj_trans, 1, STRING("S_hai_l_01"), LW(0), 0, 0)
        CALL(evt_mapobj_trans, 1, STRING("S_hai_r_01"), LW(2), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(moya_effect02)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hai_l_02"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hai_r_02"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 1500, 0, 7000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(2), 0)
        SUBTRACT(LW(2), LW(0))
        CALL(evt_mapobj_trans, 1, STRING("S_hai_l_02"), LW(0), 0, 0)
        CALL(evt_mapobj_trans, 1, STRING("S_hai_r_02"), LW(2), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(moya_effect03)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hai_l_03"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hai_r_03"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 1500, 0, 7000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(2), 0)
        SUBTRACT(LW(2), LW(0))
        CALL(evt_mapobj_trans, 1, STRING("S_hai_l_03"), LW(0), 0, 0)
        CALL(evt_mapobj_trans, 1, STRING("S_hai_r_03"), LW(2), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(moya_effect04)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hai_l_04"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hai_r_04"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 1500, 0, 7000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(2), 0)
        SUBTRACT(LW(2), LW(0))
        CALL(evt_mapobj_trans, 1, STRING("S_hai_l_04"), LW(0), 0, 0)
        CALL(evt_mapobj_trans, 1, STRING("S_hai_r_04"), LW(2), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(moya_effect05)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hai_l_05"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hai_r_05"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 1500, 0, 7000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(2), 0)
        SUBTRACT(LW(2), LW(0))
        CALL(evt_mapobj_trans, 1, STRING("S_hai_l_05"), LW(0), 0, 0)
        CALL(evt_mapobj_trans, 1, STRING("S_hai_r_05"), LW(2), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static void alpha_tev(MapTevCallbackInfo* info) {
    s32 stage = info->tevStage;
    u8* alpha = _ap;

    GXSetTevOrder(stage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, *alpha});
    GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    info->tevStage++;
}

static USER_FUNC(alpha_init_func) {
    _ap = _mapAlloc(1);
    *_ap = 255;
    return EVT_RETURN_DONE;
}

static USER_FUNC(alpha_set_func) {
    *_ap = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(elmos1000tai_init) {
    NanNPCWork* wp = nanNPCWork;
    s32 i;
    NanNPC* npc;
    s32 deg;
    s32 a;
    s32 r;
    f32 sn;
    f32 rad;
    f32 cs;
    f32 dist;
    f32 fdeg;

    wp->flags |= 1;
    for (i = 0; i < wp->num; i++) {
        npc = &wp->npc[i];
        npc->pose->flags |= 4;
        npc->color.a = 0;
        npc->dir = 90.0f;
        npc->index = i;
        if (i % 3) {
            deg = irand(140) + 110;
        } else {
            deg = irand(360);
        }
        fdeg = deg;
        npc->dist = 250;
        r = 330;
        a = __abs(180 - deg);
        if (a < 70) {
            r = (70 - a) * 1000 / 70 + 330;
        }
        npc->dist += irand(r);
        rad = 6.2831855f * fdeg / 360.0f;
        dist = npc->dist;
        sn = sin(rad);
        cs = cos(rad);
        npc->pos.x = 0.9f * (dist * sn);
        npc->pos.y = 100.0f;
        npc->pos.z = 0.5f * (dist * cs) - 100.0f;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(elmos1000tai_start) {
    s32 i;
    NanNPC* npc;
    NanNPCWork* wp;

    wp = nanNPCWork;

    for (i = 0; i < wp->num; i++) {
        npc = &wp->npc[i];
        npc->func = elmos1000tai_seq00;
        npc->state = 0;
    }
    return EVT_RETURN_DONE;
}

void elmos1000tai_seq00(NanNPC* npc) {
    s32 t;

    switch (npc->state) {
        case 0:
            npc->startTime = gp->renderTime;
            npc->duration = irand(1000) + npc->dist * 3;
            npc->state = 1;
        case 1:
            if ((s32)OSTicksToMilliseconds((u32)gp->renderTime - (u32)npc->startTime) >= npc->duration) {
                npc->startTime = gp->renderTime;
                npc->duration = 500;
                npc->state = 2;
            }
            break;
        case 2:
            t = OSTicksToMilliseconds((u32)gp->renderTime - (u32)npc->startTime);
            if (t < npc->duration) {
                npc->color.a = intplGetValue(EASING_LINEAR, 0.0f, 255.0f, t, npc->duration);
            } else {
                npc->state = 3;
            }
            break;
        case 3:
            break;
    }
}

USER_FUNC(mist_func) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    EffectEntry* eff;
    BOOL done = FALSE;

    if (isFirstCall) {
        x += irand(150) - 75;
        y += irand(150) - 75;
        z += irand(150) - 75;
        event->userdatap[1] = effMistEntry(0, 0, x, y, z, 0.0f);
        event->userdata[0] = 0;
    }
    eff = event->userdatap[1];
    switch (event->userdata[0]) {
        case 0:
            event->savedTime = gp->renderTime;
            event->userdata[2] = irand(1000) + 3000;
            event->userdata[0] = 1;
            break;
        case 1:
            if ((s32)OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime) > event->userdata[2]) {
                event->userdata[0] = 2;
            }
            break;
        case 2:
            done = TRUE;
            break;
    }
    if (!done) {
        return EVT_RETURN_BLOCK;
    }
    effSoftDelete(eff);
    return EVT_RETURN_DONE;
}

USER_FUNC(marcoship_shizumu_func) {
    f32 y = evtGetFloat(event, event->args[0]);

    mapObjTranslate("S_a1", 0.0f, y, 0.0f);
    return EVT_RETURN_DONE;
}

USER_FUNC(splash_eff) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    effSplashEntry(0, x, y, z, 3.0f);
    return EVT_RETURN_DONE;
}

USER_FUNC(kaizoku_fune_eff) {
    EffectEntry* eff;
    Vec pos;
    f32 oldX, oldZ, speed, angle, dir;

    if (isFirstCall) {
        event->userdatap[0] = effFunemizuEntry(0, 0.0f, -1000.0f, 0.0f, 0.0f, 0.0f);
        mapObjGetPos("S_b", &pos);
        event->userdataf[2] = pos.x;
        event->userdata3f = pos.z;
    }
    eff = event->userdatap[0];
    oldX = event->userdataf[2];
    oldZ = event->userdata3f;
    mapObjGetPos("S_b", &pos);
    event->userdataf[2] = pos.x;
    event->userdata3f = pos.z;
    speed = distABf(oldX, oldZ, pos.x, pos.z);
    angle = angleABf(oldX, oldZ, pos.x, pos.z);
    if (angle < 180.0f) {
        dir = 1.0f;
    } else {
        dir = -1.0f;
    }
    ((EffFunemizuWork*)eff->userdata)->scale = 5.0f;
    ((EffFunemizuWork*)eff->userdata)->position.x = 140.0f * dir + pos.x;
    ((EffFunemizuWork*)eff->userdata)->position.y = pos.y - 160.0f;
    ((EffFunemizuWork*)eff->userdata)->position.z = 40.0f + pos.z;
    ((EffFunemizuWork*)eff->userdata)->size = speed;
    if (angle < 180.0f) {
        ((EffFunemizuWork*)eff->userdata)->yaw = 135.0f + angle;
    } else {
        ((EffFunemizuWork*)eff->userdata)->yaw = 45.0f + angle;
    }
    return EVT_RETURN_BLOCK;
}

EVT_BEGIN(nami_alpha)
    CALL(alpha_init_func)
    CALL(evt_map_set_tevcallback, 1, PTR(alpha_tev))
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("S_umi_l_siro_b"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_umi_l_siro_b"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(alpha_set_func, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(moonout_effect)
    CALL(alpha_init_func)
    CALL(evt_map_set_tevcallback, 1, PTR(alpha_tev))
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("S_hai02"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("S_hai03_moon"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hai02"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hai03_moon"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 0, 1500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(alpha_set_func, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(moya_effect)
    SPAWN_EVENT(PTR(moya_effect01))
    SPAWN_EVENT(PTR(moya_effect02))
    SPAWN_EVENT(PTR(moya_effect03))
    SPAWN_EVENT(PTR(moya_effect04))
    SPAWN_EVENT(PTR(moya_effect05))
    CALL(evt_map_playanim, STRING("onryou"), MAP_ANIM_PLAY_ONCE, 0)
    SPAWN_EVENT(PTR(moonout_effect))
    RETURN
EVT_END()

EVT_BEGIN(_kakushuku)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(0), LW(1), LW(2))
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_FLOAT_VALUE(LW(2), LW(0))
        DIVIDE_FLOAT(LW(2), FLOAT(100.0f))
        CALL(evt_npc_set_scale, STRING("エルモスA"), LW(2), LW(2), LW(2))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(elmos_a_kakushuku)
    SET_VALUE(LW(0), 100)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 150)
    SPAWN_EVENT(PTR(_kakushuku))
    WAIT_MS(500)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 100)
    SET_VALUE(LW(2), 150)
    SPAWN_EVENT(PTR(_kakushuku))
    LOOP(0)
        SET_VALUE(LW(0), 100)
        SET_VALUE(LW(1), 150)
        SET_VALUE(LW(2), 100)
        SPAWN_EVENT(PTR(_kakushuku))
        WAIT_MS(100)
        SET_VALUE(LW(0), 150)
        SET_VALUE(LW(1), 100)
        SET_VALUE(LW(2), 100)
        SPAWN_EVENT(PTR(_kakushuku))
        WAIT_MS(100)
        IF_INT_EQUAL(GF(10), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GF(11), 0)
    RETURN
EVT_END()

EVT_BEGIN(elmos_a_kaku)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 100)
    SET_VALUE(LW(2), 150)
    CALL_CHILD(PTR(_kakushuku))
    RETURN
EVT_END()

EVT_BEGIN(elmos_a_shuku)
    SET_VALUE(LW(0), 100)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 150)
    CALL_CHILD(PTR(_kakushuku))
    RETURN
EVT_END()

static ExtTexInfo ext_entry_data[] = {
    {"c_elmos", "S_1", 0.0f},
    {"c_elmos", "S_1", 1.0f},
    {"c_elmos", "S_1", 2.0f},
    {"c_elmos", "S_1", 3.0f},
    {"c_elmos", "S_1", 4.0f},
    {"c_elmos", "S_1", 5.0f},
    {"c_elmos", "S_1", 6.0f},
    {"c_elmos", "S_1", 7.0f},
    {"c_elmos", "S_1", 8.0f},
    {"c_elmos", "S_1", 9.0f},
    {"c_elmos", "S_1", 10.0f},
    {"c_elmos", "S_1", 11.0f},
    {"c_elmos", "S_1", 12.0f},
    {"c_elmos", "S_1", 13.0f},
    {"c_elmos", "S_1", 14.0f},
    {"c_elmos", "S_1", 15.0f},
    {"c_elmos", "S_1", 16.0f},
    {"c_elmos", "S_1", 17.0f},
    {"c_elmos", "S_1", 18.0f},
    {"c_elmos", "S_1", 19.0f},
    {0},
};

static s16 elmos_animtbl[] = {
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, -1, 0,
};

static EVT_BEGIN(nannpc_mode_setup)
    CALL(evt_nannpc_init, PTR(ext_entry_data), 0, 0, 0, 1000)
    CALL(evt_nannpc_set_subfunc, 1, PTR(nannpc_ext_main_sub_fast))
    SET_VALUE(LW(0), 0)
    LOOP(1000)
        CALL(evt_nannpc_entry, LW(0))
        CALL(evt_nannpc_set_func, LW(0), 0)
        CALL(evt_nannpc_set_animtbl, LW(0), PTR(elmos_animtbl))
        ADD(LW(0), 1)
    END_LOOP
    CALL(elmos1000tai_init)
    RETURN
EVT_END()

s32 mist_pos[] = {
    -400, 30, -200, -400, 30, -400, -400, 30,
    -600, -400, 30, -800, -400, 30, -1000, -400,
    30, -1200, -200, 30, -200, -200, 30, -400,
    -200, 30, -600, -200, 30, -800, -200, 30,
    -1000, -200, 30, -1200, 0, 30, -200, 0,
    30, -400, 0, 30, -600, 0, 30, -800,
    0, 30, -1000, 0, 30, -1200, 200, 30,
    -200, 200, 30, -400, 200, 30, -600, 200,
    30, -800, 200, 30, -1000, 200, 30, -1200,
    400, 30, -200, 400, 30, -400, 400, 30,
    -600, 400, 30, -800, 400, 30, -1000, 400,
    30, -1200, -10000,
};

EVT_BEGIN(mist_setup)
    LOOP(0)
        SET_VALUE(LW(10), 0)
        LOOP(0)
            SET_INT_READ_ADDR(PTR(mist_pos))
            READ_INT_INDEXED(LW(0), LW(10))
            IF_INT_EQUAL(LW(0), -10000)
                BREAK_LOOP
            END_IF
            ADD(LW(10), 1)
            READ_INT_INDEXED(LW(1), LW(10))
            ADD(LW(10), 1)
            READ_INT_INDEXED(LW(2), LW(10))
            ADD(LW(10), 1)
            BEGIN_INLINE_EVENT
                CALL(mist_func, LW(0), LW(1), LW(2))
            END_INLINE_EVENT
        END_LOOP
        WAIT_MS(2000)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(marcoship_shizumu)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_SHIP_QUAKE1"), LW(10))
    SET_FLOAT_VALUE(LW(2), FLOAT(0.0f))
    CALL(evt_hit_bind_mapobj, STRING("A_a1"), STRING("S_a1"))
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, 0, -300, 8000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(marcoship_shizumu_func, LW(0))
        SET_FLOAT_VALUE(LW(3), LW(0))
        SUBTRACT_FLOAT(LW(3), LW(2))
        CALL(evt_npc_get_position, STRING("マルコ"), LW(4), LW(5), LW(6))
        ADD_FLOAT(LW(5), LW(3))
        CALL(evt_npc_set_position, STRING("マルコ"), LW(4), LW(5), LW(6))
        CALL(evt_npc_get_position, STRING("ガラの悪い水夫"), LW(4), LW(5), LW(6))
        ADD_FLOAT(LW(5), LW(3))
        CALL(evt_npc_set_position, STRING("ガラの悪い水夫"), LW(4), LW(5), LW(6))
        CALL(evt_npc_get_position, STRING("オドオド水夫"), LW(4), LW(5), LW(6))
        ADD_FLOAT(LW(5), LW(3))
        CALL(evt_npc_set_position, STRING("オドオド水夫"), LW(4), LW(5), LW(6))
        CALL(evt_npc_get_position, STRING("水夫A"), LW(4), LW(5), LW(6))
        ADD_FLOAT(LW(5), LW(3))
        CALL(evt_npc_set_position, STRING("水夫A"), LW(4), LW(5), LW(6))
        CALL(evt_npc_get_position, STRING("水夫ボム兵"), LW(4), LW(5), LW(6))
        ADD_FLOAT(LW(5), LW(3))
        CALL(evt_npc_set_position, STRING("水夫ボム兵"), LW(4), LW(5), LW(6))
        CALL(evt_npc_get_position, STRING("店員"), LW(4), LW(5), LW(6))
        ADD_FLOAT(LW(5), LW(3))
        CALL(evt_npc_set_position, STRING("店員"), LW(4), LW(5), LW(6))
        CALL(evt_npc_get_position, STRING("宿店員"), LW(4), LW(5), LW(6))
        ADD_FLOAT(LW(5), LW(3))
        CALL(evt_npc_set_position, STRING("宿店員"), LW(4), LW(5), LW(6))
        CALL(evt_hit_bind_update, STRING("A_a1"))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        SET_FLOAT_VALUE(LW(2), LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(10))
    RETURN
EVT_END()

EVT_BEGIN(onryo_event)
    BEGIN_INLINE_EVENT
        CALL(evt_map_anim_rate)
    END_INLINE_EVENT
    SPAWN_EVENT(marco_ship_yure)
    SPAWN_EVENT(nannpc_mode_setup)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_b"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("A_b"), 1, 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_00_LOGBOOK)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_cont_onoff, 0)
        CALL(evt_party_cont_onoff, 0, 0)
        CALL(evt_mario_set_pos, 5, 90, -100)
        CALL(evt_party_set_pos, 0, 0, -1000, 0)
        GOTO(100)
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
#ifdef VERSION_G8ME01
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
#endif
    CALL(evt_cam3d_evt_set, 0, 600, 2155, 0, 350, -108, 0, EASING_SINE_IN_OUT)
    CALL(evt_map_playanim, STRING("yoru_hiru"), MAP_ANIM_PLAY_AT_END, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(4000)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg5_sea_00"), 0, 0)
    CALL_CHILD(hiru_yoru_event)
    WAIT_MS(3000)
    CALL_CHILD(yoru_hiru_event)
    WAIT_MS(1000)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg5_sea_28"), 0, 0)
    CALL_CHILD(hiru_yoru_event)
    WAIT_MS(3000)
    CALL_CHILD(yoru_hiru_event)
    WAIT_MS(1000)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg5_sea_29"), 0, 0)
    CALL_CHILD(hiru_yoru_event)
    WAIT_MS(1000)
    SET_VALUE(GSW_STORY_PROGRESS, STG5_00_LOGBOOK)
    LABEL(100)
    CALL(evt_mario_cont_onoff, 1)
#ifdef VERSION_G8ME01
#else
    CALL(evt_party_cont_onoff, 0, 0)
#endif
    CALL(evt_mario_set_pos, 5, 90, -100)
    CALL(evt_mario_set_dir, 90, 0, 0)
#ifdef VERSION_G8ME01
#else
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
#endif
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_a1"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_npc_set_position, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), 116, 116, -100)
    CALL(evt_npc_set_ry, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), 90)
    CALL(evt_npc_set_position, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), 25, 90, -163)
    CALL(evt_npc_set_ry, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), 90)
    CALL(evt_npc_set_position, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), 21, 90, -55)
    CALL(evt_npc_set_ry, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), 90)
    CALL(evt_npc_set_position, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"), -11, 90, -163)
    CALL(evt_npc_set_ry, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"), 90)
    CALL(evt_npc_set_position, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), -15, 90, -40)
    CALL(evt_npc_set_ry, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), 90)
    CALL(evt_npc_set_position, STRING(/*"水夫A"*/"\x90\x85\x95\x76\x41"), -65, 114, -145)
    CALL(evt_npc_set_ry, STRING(/*"水夫A"*/"\x90\x85\x95\x76\x41"), 90)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"水夫A"*/"\x90\x85\x95\x76\x41"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_stay_anim, STRING(/*"水夫A"*/"\x90\x85\x95\x76\x41"))
    CALL(evt_npc_set_position, STRING(/*"水夫ボム兵"*/"\x90\x85\x95\x76\x83\x7B\x83\x80\x95\xBA"), -65, 114, -44)
    CALL(evt_npc_set_ry, STRING(/*"水夫ボム兵"*/"\x90\x85\x95\x76\x83\x7B\x83\x80\x95\xBA"), 90)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"水夫ボム兵"*/"\x90\x85\x95\x76\x83\x7B\x83\x80\x95\xBA"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_stay_anim, STRING(/*"水夫ボム兵"*/"\x90\x85\x95\x76\x83\x7B\x83\x80\x95\xBA"))
    CALL(evt_npc_set_position, STRING(/*"店員"*/"\x93\x58\x88\xF5"), -115, 163, -153)
    CALL(evt_npc_set_ry, STRING(/*"店員"*/"\x93\x58\x88\xF5"), 90)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"店員"*/"\x93\x58\x88\xF5"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_stay_anim, STRING(/*"店員"*/"\x93\x58\x88\xF5"))
    CALL(evt_npc_set_position, STRING(/*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5"), -119, 163, -62)
    CALL(evt_npc_set_ry, STRING(/*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5"), 90)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_stay_anim, STRING(/*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5"))
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"水夫A"*/"\x90\x85\x95\x76\x41"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"水夫ボム兵"*/"\x90\x85\x95\x76\x83\x7B\x83\x80\x95\xBA"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"店員"*/"\x93\x58\x88\xF5"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_sound_data_set, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), STRING("SFX_STG5_MARCO_MOVE1L"), STRING("SFX_STG5_MARCO_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(evt_cam3d_evt_set, -10, 340, 558, -11, 131, -108, 0, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"水夫A"*/"\x90\x85\x95\x76\x41"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"水夫ボム兵"*/"\x90\x85\x95\x76\x83\x7B\x83\x80\x95\xBA"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"店員"*/"\x93\x58\x88\xF5"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5"), NPC_FLAG_INSTANT_TURN)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_sea_01"), 0, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 480, 352, 214, 58, 150, -96, 0, EASING_SINE_IN_OUT)
    WAIT_MS(1500)
    CALL(evt_npc_set_ry, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), 270)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), 107, -100, 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_sea_02"), 0, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"))
    CALL(evt_cam3d_evt_set, 0, 300, 985, 0, 175, -108, 0, EASING_SINE_IN_OUT)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
    SET_VALUE(GW(0), 0)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_set_anim, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), STRING("S_2"))
    BEGIN_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("small_01"), 0, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"))
        LOOP(0)
            IF_INT_EQUAL(GW(0), 1)
                BREAK_LOOP
            END_IF
            CALL(evt_msg_repeat)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_set_fune_anim_rate, FLOAT(0.69921875f))
    CALL(evt_set_nami_anim_rate, FLOAT(0.69921875f))
    WAIT_MS(1000)
    WAIT_MS(1500)
    CALL(evt_msg_print, 0, STRING("stg5_sea_03"), 0, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_sea_04"), 0, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"))
    CALL(marcoship_stop_func)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
        WAIT_MS(2000)
        CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_EVT_STG5_GHOST1"), 2000)
        CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    END_INLINE_EVENT
    CALL(evt_set_fune_anim_rate, FLOAT(0.39941406f))
    CALL(evt_set_nami_anim_rate, FLOAT(0.39941406f))
    CALL(evt_msg_print_add, 0, STRING("stg5_sea_04_01"))
    CALL(evt_cam3d_evt_set, 0, 295, 370, 0, 145, -108, 0, EASING_SINE_IN_OUT)
    SPAWN_EVENT(nami_alpha)
    CALL(evt_set_fune_anim_rate, FLOAT(0.0f))
    CALL(evt_set_nami_anim_rate, FLOAT(0.099609375f))
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("S_fne_nami"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_sea_05"), 0, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"))
    WAIT_MS(1000)
    SET_VALUE(GW(0), 1)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), NPC_FLAG_TALK_POSE)
    CALL(evt_msg_close, STRING("small_01"))
    CALL(evt_npc_set_anim, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), STRING("T_1"))
    WAIT_MS(2000)
    CALL(evt_npc_set_anim, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), STRING("S_1"))
    CALL(marcoship_stop_wait_func)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), 68, -71, 0, FLOAT(120.0f), 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_sea_06"), 0, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_sea_07"), 0, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"))
    CALL(evt_cam3d_evt_set, -50, 470, 588, -50, 390, -107, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), STRING("R_1"))
    CALL(evt_npc_set_position, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), -12, 90, -80)
    CALL(evt_npc_set_ry, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), 270)
    CALL(evt_npc_glide_position, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), -52, 315, -75, 0, FLOAT(120.0f), 0, 0, 0)
    CALL(evt_npc_set_anim, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), STRING("S_1"))
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), 270)
    CALL(evt_npc_set_anim, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), STRING("R_1"))
    CALL(evt_npc_glide_position, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), -78, 315, -79, 0, FLOAT(120.0f), 0, 0, 0)
    CALL(evt_npc_set_anim, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), STRING("S_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_position, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), -23, 315, -79)
    CALL(evt_npc_set_rotate, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), 0, 180, 0)
    WAIT_MS(1200)
    CALL(evt_npc_set_position, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), -78, 315, -79)
    CALL(evt_npc_set_rotate, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), 0, 0, 0)
    WAIT_MS(1800)
    CALL(evt_npc_set_position, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), -23, 315, -79)
    CALL(evt_npc_set_rotate, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), 0, 180, 0)
    WAIT_MS(1000)
    CALL(evt_npc_set_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), -45, 336, -73)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ELMOS_APPEAR1_2"), -45, 336, -73, 0)
    CALL(evt_npc_set_ry, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), 90)
    WAIT_MS(1000)
    CALL(evt_npc_get_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), LW(10), LW(11), LW(12))
    SET_VALUE(LW(13), LW(10))
    SET_VALUE(LW(14), LW(11))
    SET_VALUE(LW(15), LW(12))
    ADD(LW(13), 5)
    ADD(LW(14), -5)
    ADD(LW(15), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ELMOS_MOVE1"), LW(10), LW(11), LW(12), 0)
    CALL(evt_npc_glide_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), LW(13), LW(14), LW(15), 0, FLOAT(120.0f), 0, 0, 0)
    CALL(evt_npc_glide_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), LW(10), LW(11), LW(12), 0, FLOAT(120.0f), 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ELMOS_MOVE1"), LW(10), LW(11), LW(12), 0)
    CALL(evt_npc_glide_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), LW(13), LW(14), LW(15), 0, FLOAT(120.0f), 0, 0, 0)
    CALL(evt_npc_glide_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), LW(10), LW(11), LW(12), 0, FLOAT(120.0f), 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_sea_08"), 0, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"))
    CALL(evt_npc_set_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), 0, -1000, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 180, 0, 200)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_npc_set_rotate, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(200)
    SPAWN_EVENT(moya_effect)
    WAIT_MS(1500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_NPC_QUESTION_MARK1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_eff_fukidashi, FUKIDASHI_AT_POS, STRING(""), 0, 1, 0, LW(0), LW(1), LW(2), 30, 0, 90)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), 90)
    WAIT_MS(500)
    CALL(evt_npc_set_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ELMOS_APPEAR1_2"), LW(10), LW(11), LW(12), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ELMOS_MOVE1"), LW(10), LW(11), LW(12), 0)
    CALL(evt_npc_glide_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), LW(13), LW(14), LW(15), 0, FLOAT(120.0f), 0, 0, 0)
    CALL(evt_npc_glide_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), LW(10), LW(11), LW(12), 0, FLOAT(120.0f), 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ELMOS_MOVE1"), LW(10), LW(11), LW(12), 0)
    CALL(evt_npc_glide_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), LW(13), LW(14), LW(15), 0, FLOAT(120.0f), 0, 0, 0)
    CALL(evt_npc_glide_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), LW(10), LW(11), LW(12), 0, FLOAT(120.0f), 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_sea_09"), 0, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"))
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), 270)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -50, 412, 340, -50, 360, -108, 100, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_NPC_BIKKURI_MARK1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_eff_fukidashi, FUKIDASHI_AT_POS, STRING(""), 0, 0, 0, LW(0), LW(1), LW(2), 30, 0, 60)
    END_INLINE_EVENT
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER2"))
    END_INLINE_EVENT
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        LOOP(5)
            CALL(evt_cam3d_evt_set, -50, 412, 340, -50, 360, -108, 100, EASING_SINE_IN_OUT)
            WAIT_MS(100)
            CALL(evt_cam3d_evt_set, -50, 423, 438, -50, 360, -108, 100, EASING_SINE_IN_OUT)
            WAIT_MS(100)
        END_LOOP
        CALL(evt_cam3d_evt_set, -50, 412, 340, -50, 360, -108, 100, EASING_SINE_IN_OUT)
        WAIT_MS(100)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg5_sea_11"), 0, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"))
    WAIT_WHILE(GF(0))
    CALL(evt_npc_set_anim, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), STRING("R_1"))
    CALL(muj_npc_guruguru, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), -52, -90, 0, FLOAT(-12.0f), 3000)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), 270)
        CALL(evt_npc_glide_position, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), -52, 315, -70, 0, FLOAT(180.0f), 0, 0, 0)
        CALL(evt_npc_set_ry, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), 90)
        CALL(evt_npc_glide_position, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), -43, 90, -72, 0, FLOAT(180.0f), 0, 0, 0)
        CALL(evt_npc_set_ry, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), 90)
        CALL(evt_npc_glide_position, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), 20, 90, -55, 0, FLOAT(180.0f), 0, 0, 0)
        CALL(evt_npc_jump_position_nohit, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), 20, 90, -55, 100, 30)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -10, 340, 558, -11, 131, -108, 1000, EASING_SINE_IN_OUT)
    WAIT_WHILE(GF(0))
    CALL(evt_msg_print, 0, STRING("stg5_sea_12"), 0, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_sea_13"), 0, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"))
    WAIT_MS(500)
    SPAWN_EVENT(mist_setup)
    CALL(evt_npc_set_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), 168, 160, -84)
    CALL(evt_npc_set_position, STRING(/*"エルモスB"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x42"), 105, 100, -200)
    CALL(evt_npc_set_position, STRING(/*"エルモスC"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x43"), 10, 110, -200)
    CALL(evt_npc_set_position, STRING(/*"エルモスD"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x44"), -70, 105, -200)
    CALL(evt_npc_set_position, STRING(/*"エルモスE"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x45"), 125, 105, 0)
    CALL(evt_npc_set_position, STRING(/*"エルモスF"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x46"), 10, 100, 0)
    CALL(evt_npc_set_position, STRING(/*"エルモスG"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x47"), -80, 110, 0)
    CALL(evt_npc_set_ry, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), 270)
    CALL(evt_npc_set_ry, STRING(/*"エルモスB"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x42"), 270)
    CALL(evt_npc_set_ry, STRING(/*"エルモスC"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x43"), 270)
    CALL(evt_npc_set_ry, STRING(/*"エルモスD"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x44"), 90)
    CALL(evt_npc_set_ry, STRING(/*"エルモスE"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x45"), 270)
    CALL(evt_npc_set_ry, STRING(/*"エルモスF"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x46"), 270)
    CALL(evt_npc_set_ry, STRING(/*"エルモスG"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x47"), 90)
    CALL(evt_npc_set_color, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), 255, 255, 255, 0)
    CALL(evt_npc_set_color, STRING(/*"エルモスB"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x42"), 255, 255, 255, 0)
    CALL(evt_npc_set_color, STRING(/*"エルモスC"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x43"), 255, 255, 255, 0)
    CALL(evt_npc_set_color, STRING(/*"エルモスD"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x44"), 255, 255, 255, 0)
    CALL(evt_npc_set_color, STRING(/*"エルモスE"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x45"), 255, 255, 255, 0)
    CALL(evt_npc_set_color, STRING(/*"エルモスF"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x46"), 255, 255, 255, 0)
    CALL(evt_npc_set_color, STRING(/*"エルモスG"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x47"), 255, 255, 255, 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_APPEAR1_2"), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 255, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_npc_set_color, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), 255, 255, 255, LW(0))
        CALL(evt_npc_set_color, STRING(/*"エルモスB"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x42"), 255, 255, 255, LW(0))
        CALL(evt_npc_set_color, STRING(/*"エルモスC"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x43"), 255, 255, 255, LW(0))
        CALL(evt_npc_set_color, STRING(/*"エルモスD"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x44"), 255, 255, 255, LW(0))
        CALL(evt_npc_set_color, STRING(/*"エルモスE"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x45"), 255, 255, 255, LW(0))
        CALL(evt_npc_set_color, STRING(/*"エルモスF"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x46"), 255, 255, 255, LW(0))
        CALL(evt_npc_set_color, STRING(/*"エルモスG"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x47"), 255, 255, 255, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    SET_VALUE(LW(0), STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"))
    SPAWN_EVENT_GET_ID(PTR(fuwafuwa), GW(1))
    SET_VALUE(LW(0), STRING(/*"エルモスB"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x42"))
    CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(1), -20)
    SET_VALUE(LW(4), 1)
    SET_VALUE(LW(5), 850)
    SPAWN_EVENT(urouro_fuwafuwa)
    SET_VALUE(LW(0), STRING(/*"エルモスC"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x43"))
    CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(1), -25)
    SET_VALUE(LW(4), 1)
    SET_VALUE(LW(5), 1000)
    SPAWN_EVENT(urouro_fuwafuwa)
    SET_VALUE(LW(0), STRING(/*"エルモスD"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x44"))
    CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(1), -22)
    SET_VALUE(LW(4), 1)
    SET_VALUE(LW(5), 900)
    SPAWN_EVENT(urouro_fuwafuwa)
    SET_VALUE(LW(0), STRING(/*"エルモスE"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x45"))
    CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(1), -30)
    SET_VALUE(LW(4), 1)
    SET_VALUE(LW(5), 1050)
    SPAWN_EVENT(urouro_fuwafuwa)
    SET_VALUE(LW(0), STRING(/*"エルモスF"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x46"))
    CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(1), -20)
    SET_VALUE(LW(4), 1)
    SET_VALUE(LW(5), 950)
    SPAWN_EVENT(urouro_fuwafuwa)
    SET_VALUE(LW(0), STRING(/*"エルモスG"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x47"))
    CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(1), -24)
    SET_VALUE(LW(4), 1)
    SET_VALUE(LW(5), 900)
    SPAWN_EVENT(urouro_fuwafuwa)
    CALL(evt_cam3d_evt_set, 2, 700, 1614, -1, 261, -108, 0, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_position, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), 116, 116, -100)
    WAIT_MS(500)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_VOICE1"), 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_KORTESS_VOICE1"), 0)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg5_sea_14"), 0, 0)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 0, 358, 572, -1, 145, -108, 0, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg5_sea_15"), 0, 0)
    WAIT_MS(500)
    SET_VALUE(LW(0), STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"))
    CALL(evt_npc_set_anim, LW(0), STRING("R_2"))
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(1), -20)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 300)
    SPAWN_EVENT(urouro)
    SET_VALUE(LW(0), STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"))
    CALL(evt_npc_set_run_anim, LW(0))
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(1), -25)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 330)
    SPAWN_EVENT(urouro)
    SET_VALUE(LW(0), STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"))
    CALL(evt_npc_set_run_anim, LW(0))
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(1), -30)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 350)
    SPAWN_EVENT_GET_ID(PTR(urouro), GW(2))
    SET_VALUE(LW(0), STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"))
    CALL(evt_npc_set_run_anim, LW(0))
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(1), -22)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 340)
    SPAWN_EVENT_GET_ID(PTR(urouro), GW(3))
    SET_VALUE(LW(0), STRING(/*"店員"*/"\x93\x58\x88\xF5"))
    CALL(evt_npc_set_run_anim, LW(0))
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(1), -22)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 300)
    SPAWN_EVENT(urouro)
    SET_VALUE(LW(0), STRING(/*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5"))
    CALL(evt_npc_set_run_anim, LW(0))
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(1), -24)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 340)
    SPAWN_EVENT(urouro)
    SET_VALUE(LW(0), STRING(/*"水夫A"*/"\x90\x85\x95\x76\x41"))
    SPAWN_EVENT(buruburu)
    SET_VALUE(LW(0), STRING(/*"水夫ボム兵"*/"\x90\x85\x95\x76\x83\x7B\x83\x80\x95\xBA"))
    SPAWN_EVENT(buruburu)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_anim, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), STRING("O_1"))
    CALL(evt_mario_set_pose, STRING("M_I_O"))
    CALL(evt_npc_flag_onoff, 0, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"), NPC_FLAG_TALK_POSE)
    CALL(evt_msg_print, 0, STRING("stg5_sea_16"), 0, STRING(/*"ガラの悪い水夫"*/"\x83\x4B\x83\x89\x82\xCC\x88\xAB\x82\xA2\x90\x85\x95\x76"))
    CALL(evt_msg_print, 0, STRING("stg5_sea_17"), 0, STRING(/*"オドオド水夫"*/"\x83\x49\x83\x68\x83\x49\x83\x68\x90\x85\x95\x76"))
    CALL(evt_cam3d_evt_set, 115, 300, 323, 115, 165, -105, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_sea_18"), 0, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"))
    CALL(elmos1000tai_start)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_APPEAR2"), 0)
    END_INLINE_EVENT
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_KORTESS_VOICE1"), 0)
    CALL(evt_msg_print, 0, STRING("stg5_sea_19"), 0, 0)
    DELETE_EVENT(GW(1))
    SET_VALUE(GF(10), 1)
    SET_VALUE(GF(11), 1)
    SPAWN_EVENT(elmos_a_kakushuku)
    WAIT_MS(500)
    CALL(evt_npc_set_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), 80, 126, -100)
    CALL(evt_npc_set_ry, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), 90)
    WAIT_MS(500)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_NPC_BIKKURI_MARK1"), LW(0), LW(1), LW(2), 0)
        ADD(LW(1), 20)
        CALL(evt_eff_fukidashi, FUKIDASHI_AT_POS, STRING(""), 0, 0, 0, LW(0), LW(1), LW(2), 30, 0, 90)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_sea_20"), 0, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"))
    CALL(evt_npc_set_anim, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), STRING("B_1"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_sea_21"), 0, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"))
    SET_VALUE(GF(10), 0)
    WAIT_WHILE(GF(11))
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_STG5_SHIP_QUAKE1"), LW(10))
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.001953125f), FLOAT(0.0f), 200)
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.00390625f), FLOAT(0.0f), 200)
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.005859375f), FLOAT(0.0f), 200)
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0078125f), FLOAT(0.0f), 200)
        CALL(evt_snd_sfxoff, LW(10))
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 0)
    END_INLINE_EVENT
    CALL_CHILD(elmos_a_shuku)
    CALL(evt_npc_set_position, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), 168, 160, -84)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ELMOS_APPEAR1"), 168, 160, -84, 0)
    CALL(evt_npc_set_ry, STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"), 270)
    WAIT_MS(300)
    CALL_CHILD(elmos_a_kaku)
    SET_VALUE(LW(0), STRING(/*"エルモスA"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x41"))
    SPAWN_EVENT(fuwafuwa)
    SET_VALUE(LW(0), STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"))
    CALL(evt_npc_set_anim, LW(0), STRING("R_2"))
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(1), -20)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 340)
    SPAWN_EVENT_GET_ID(PTR(urouro), GW(0))
    CALL(evt_cam3d_evt_set, 0, 358, 572, -1, 145, -108, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_sea_22"), 0, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"))
    CALL(evt_msg_print, 0, STRING("stg5_sea_23"), 0, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"))
    CALL(evt_msg_print, 0, STRING("stg5_sea_24"), 0, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"))
    DELETE_EVENT(GW(2))
    SET_VALUE(LW(10), STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"))
    SPAWN_EVENT(kirimomi)
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_npc_get_position, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_NPC_FALL1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING(/*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58"), -20, -100, -300, 600, 100)
    CALL(evt_eff, 0, STRING("splash"), 0, -20, 30, -150, FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg5_sea_25"), 0, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"))
    DELETE_EVENT(GW(3))
    SET_VALUE(LW(10), STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"))
    SPAWN_EVENT(kirimomi)
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_npc_get_position, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_NPC_FALL1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING(/*"めがね水夫"*/"\x82\xDF\x82\xAA\x82\xCB\x90\x85\x95\x76"), -30, -100, 100, 600, 100)
    CALL(evt_eff, 0, STRING("splash"), 0, -30, 30, 0, FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg5_sea_26"), 0, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"))
    BEGIN_INLINE_EVENT
        DELETE_EVENT(GW(0))
        CALL(evt_npc_set_ry, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), 270)
        CALL(evt_npc_jump_position_nohit, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), 20, 90, -90, 500, 50)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_mapobj_get_position, STRING("S_a1"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_SHIP_SINK1"), LW(0), LW(1), LW(2), LW(10))
    SPAWN_EVENT(marcoship_shizumu)
    WAIT_MS(4500)
    CALL(evt_fade_entry, 2, 500, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    SET_VALUE(GSW_STORY_PROGRESS, STG5_10_HAUNTED)
    CALL(evt_fade_set_mapchange_type, 0, 10, 0, 9, 300)
    CALL(evt_bero_mapchange, STRING("muj_00"), STRING(""))
    RETURN
EVT_END()

EVT_BEGIN(muj_20_koopa_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_npc_set_position, STRING("飛クッパ"), 0, 500, -700)
    CALL(evt_npc_set_rotate, STRING("飛クッパ"), 0, 0, 180)
    CALL(evt_npc_set_anim, STRING("飛クッパ"), STRING("HKPA_O_3"))
    CALL(evt_cam3d_evt_set, 0, 322, 480, 0, 218, -113, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(2000)
    SET_VALUE(GF(0), 0)
    CALL(evt_snd_sfxon, STRING("SFX_KUPPA_AIRSHIP_FALL1"), GW(0))
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_npc_get_position, STRING("飛クッパ"), LW(0), LW(1), LW(2))
            IF_INT_LESS_OR_EQUAL(LW(1), 50)
                CALL(evt_snd_sfxoff, GW(0))
                CALL(evt_snd_sfxon, STRING("SFX_KUPPA_AIRSHIP_DOWN_SEA1"), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_npc_get_position, STRING("飛クッパ"), LW(0), LW(1), LW(2))
            IF_INT_LESS_OR_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(splash_eff, LW(0), LW(1), LW(2))
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, 500, -100, 1000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_position, STRING("飛クッパ"), 0, LW(0), -700)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    SET_VALUE(GF(0), 1)
    WAIT_MS(2000)
    SET_VALUE(GSW_STORY_PROGRESS, STG4_15_06_BOWSER_BLIMP_AREA)
    CALL(evt_bero_mapchange, STRING("kpa_03"), STRING(""))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(kaizoku_hata)
    LOOP(0)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -4000, 4000, 2500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(evt_mapobj_rotate, 1, STRING("S_b_02_hata"), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(500)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 4000, -4000, 2500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(evt_mapobj_rotate, 1, STRING("S_b_02_hata"), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(500)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(gor_muj_ship)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_party_set_pos, 1, 0, -1000, 0)
    CALL(evt_cam3d_evt_set, 0, 600, 2155, 0, 350, -108, 0, EASING_SINE_IN_OUT)
    CALL(evt_map_playanim, STRING("yoru_hiru"), MAP_ANIM_PLAY_AT_END, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_a1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_trans, 1, STRING("S_b"), -1000, 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_b"), 0, 0, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_umi_l_siro_b"), -1000, 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_umi_l_siro_b"), 0, 0, 0)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_umi_l_siro_b"), MAP_OBJ_FLAG_HIDDEN)
    BEGIN_INLINE_EVENT
        CALL(kaizoku_fune_eff)
    END_INLINE_EVENT
    CALL(evt_seq_wait, SEQ_GAME)
    SPAWN_EVENT(PTR(kaizoku_hata))
    BEGIN_INLINE_EVENT
        WAIT_MS(3500)
        CALL_CHILD(PTR(hiru_yoru_event))
        WAIT_MS(3500)
        CALL_CHILD(PTR(yoru_hiru_event))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_key_get_buttontrg, 0, LW(0))
            IF_INT_EQUAL(LW(0), 256)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_bero_mapchange, STRING("muj_00"), STRING(""))
    END_INLINE_EVENT
    SET_RAW_VALUE(LW(0), GW(1))
    SET_FLOAT_VALUE(LW(1), FLOAT(5.0f))
    SPAWN_EVENT(PTR(muj_ship_puka))
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, -1000, 1000, 15000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, STRING("S_b"), LW(0), GW(1), 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_b"), 0, 0, 0)
        CALL(evt_mapobj_trans, 1, STRING("S_umi_l_siro_b"), LW(0), 0, 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_umi_l_siro_b"), 0, 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_bero_mapchange, STRING("muj_00"), STRING(""))
    RETURN
EVT_END()

static EVT_BEGIN(muj_gor_ship)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_party_set_pos, 1, 0, -1000, 0)
    CALL(evt_cam3d_evt_set, 0, 600, 2155, 0, 350, -108, 0, EASING_SINE_IN_OUT)
    CALL(evt_map_playanim, STRING("yoru_hiru"), MAP_ANIM_PLAY_AT_END, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_a1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_trans, 1, STRING("S_b"), -1000, 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_b"), 0, 180, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_umi_l_siro_b"), -1000, 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_umi_l_siro_b"), 0, 180, 0)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_umi_l_siro_b"), MAP_OBJ_FLAG_HIDDEN)
    BEGIN_INLINE_EVENT
        CALL(kaizoku_fune_eff)
    END_INLINE_EVENT
    CALL(evt_seq_wait, SEQ_GAME)
    SPAWN_EVENT(PTR(kaizoku_hata))
    BEGIN_INLINE_EVENT
        WAIT_MS(3500)
        CALL_CHILD(PTR(hiru_yoru_event))
        WAIT_MS(3500)
        CALL_CHILD(PTR(yoru_hiru_event))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_key_get_buttontrg, 0, LW(0))
            IF_INT_EQUAL(LW(0), 256)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_bero_mapchange, STRING("gor_00"), STRING(""))
    END_INLINE_EVENT
    SET_RAW_VALUE(LW(0), GW(1))
    SET_FLOAT_VALUE(LW(1), FLOAT(5.0f))
    SPAWN_EVENT(PTR(muj_ship_puka))
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 1000, -1000, 15000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, STRING("S_b"), LW(0), GW(1), 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_b"), 0, 180, 0)
        CALL(evt_mapobj_trans, 1, STRING("S_umi_l_siro_b"), LW(0), 0, 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_umi_l_siro_b"), 0, 180, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_bero_mapchange, STRING("gor_00"), STRING(""))
    RETURN
EVT_END()

EVT_BEGIN(anm_kaji)
    LOOP(0)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 500, -500, 2000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(evt_mapobj_rotate, 1, STRING("S_f_drin"), LW(0), 0, 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -500, 500, 2000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(evt_mapobj_rotate, 1, STRING("S_f_drin"), LW(0), 0, 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(anm_wakka)
    SET_FLOAT_VALUE(LW(1), FLOAT(0.0f))
    LOOP(0)
        SET_FLOAT_VALUE(LW(0), FLOAT(2.0f))
        MULTIPLY_FLOAT(LW(0), GW(10))
        ADD_FLOAT(LW(1), LW(0))
        CALL(evt_mapobj_rotate, 1, STRING("S_f_wa_l"), 0, 0, LW(1))
        CALL(evt_mapobj_rotate, 1, STRING("S_f_wa_r"), 0, 0, LW(1))
        IF_FLOAT_GREATER(LW(1), FLOAT(360.0f))
            SUBTRACT_FLOAT(LW(1), FLOAT(360.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(anm_entotsu)
    SET_FLOAT_VALUE(LW(1), FLOAT(0.0f))
    LOOP(0)
        SET_FLOAT_VALUE(LW(0), FLOAT(2.0f))
        MULTIPLY_FLOAT(LW(0), GW(10))
        ADD_FLOAT(LW(1), LW(0))
        CALL(evt_mapobj_rotate, 1, STRING("S_f_en01"), 0, LW(1), 0)
        IF_FLOAT_GREATER(LW(1), FLOAT(360.0f))
            SUBTRACT_FLOAT(LW(1), FLOAT(360.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(anm_neji)
    SET_FLOAT_VALUE(LW(1), FLOAT(0.0f))
    LOOP(0)
        SET_FLOAT_VALUE(LW(0), FLOAT(2.0f))
        MULTIPLY_FLOAT(LW(0), GW(10))
        ADD_FLOAT(LW(1), LW(0))
        CALL(evt_mapobj_rotate, 1, STRING("S_skr"), LW(1), 0, 0)
        IF_FLOAT_GREATER(LW(1), FLOAT(360.0f))
            SUBTRACT_FLOAT(LW(1), FLOAT(360.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static USER_FUNC(fune_eff) {
    EffectEntry* eff1;
    EffectEntry* eff2;
    Vec pos;
    f32 oldX, oldZ, off85, speed;

    if (isFirstCall) {
        event->userdatap[0] = effFunemizuEntry(0, 0.0f, -1000.0f, 0.0f, 0.0f, 0.0f);
        event->userdatap[1] = effFunemizuEntry(2, 0.0f, -1000.0f, 0.0f, 0.0f, 0.0f);
        mapObjGetPos("S_fune_m", &pos);
        event->userdataf[2] = pos.x;
        event->userdata3f = pos.z;
    }
    eff1 = event->userdatap[0];
    eff2 = event->userdatap[1];
    oldX = event->userdataf[2];
    oldZ = event->userdata3f;
    mapObjGetPos("S_fune_m", &pos);
    event->userdataf[2] = pos.x;
    event->userdata3f = pos.z;
    speed = distABf(oldX, oldZ, pos.x, pos.z);
    angleABf(oldX, oldZ, pos.x, pos.z);
    off85 = 85.0f;
    if (evtGetValue(event, GSW_STORY_PROGRESS) == STG0_02_MARIOS_HOUSE) {
        speed = 2.0f;
    }
    ((EffFunemizuWork*)eff1->userdata)->scale = 3.0f;
    ((EffFunemizuWork*)eff1->userdata)->position.x = pos.x + off85;
    ((EffFunemizuWork*)eff1->userdata)->position.y = pos.y - 51.0f;
    ((EffFunemizuWork*)eff1->userdata)->position.z = 30.0f + pos.z;
    ((EffFunemizuWork*)eff1->userdata)->size = speed;
    ((EffFunemizuWork*)eff1->userdata)->yaw = 225.0f;
    ((EffFunemizuWork*)eff2->userdata)->position.x = pos.x - 15.0f;
    ((EffFunemizuWork*)eff2->userdata)->position.y = pos.y - 51.0f;
    ((EffFunemizuWork*)eff2->userdata)->position.z = 90.0f + pos.z;
    ((EffFunemizuWork*)eff2->userdata)->size = speed;
    ((EffFunemizuWork*)eff2->userdata)->yaw = 180.0f;
    if (speed > 1.0f && gp->retraceCount % 15 == 0) {
        effWaterDamageN64Entry(5, 30, pos.x - 150.0f, pos.y - 50.0f, pos.z, 2.0f);
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(fune_curve_pos) {
    f32 x = evtGetFloat(event, GW(0));
    f32 y = evtGetFloat(event, GW(1));
    f32 z = evtGetFloat(event, GW(2));
    f32 rot = evtGetFloat(event, LW(0));
    f32 angle;
    Vec out;
    Mtx trans, rotm, m;

    out = (Vec){x, y, z};
    angle = 5.0f * x / -6000.0f;
    MTXTrans(trans, 0.0f, -6000.0f, 0.0f);
    MTXRotRad(rotm, 'z', angle * 0.017453292f);
    MTXConcat(trans, rotm, m);
    MTXTrans(trans, 0.0f, 6000.0f, 0.0f);
    MTXConcat(m, trans, m);
    MTXMultVec(m, &out, &out);
    evtSetFloat(event, LW(7), out.x);
    evtSetFloat(event, LW(8), out.y);
    evtSetFloat(event, LW(9), out.z);
    evtSetFloat(event, LW(0), rot + angle);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(anm_yure)
    CALL(evt_mapobj_trans, 1, STRING("S_fune_m"), GW(0), GW(1), GW(2))
    CALL(evt_mapobj_trans, 1, STRING("S_umi_l_siro_b"), GW(0), GW(1), GW(2))
    WAIT_MS(1500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_OPN_SHIP_MOVE1"), GW(0), GW(1), GW(2), GW(4))
    CALL(evt_snd_sfx_dist, GW(4), 60)
    LOOP(0)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 500, -500, 2000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(fune_curve_pos)
            CALL(evt_mapobj_trans, 1, STRING("S_fune_m"), LW(7), LW(8), LW(9))
            CALL(evt_mapobj_rotate, 1, STRING("S_fune_m"), 0, 180, LW(0))
            CALL(evt_snd_sfx_pos, GW(4), LW(7), LW(8), LW(9))
            CALL(evt_mapobj_trans, 1, STRING("S_umi_l_siro_b"), LW(7), LW(8), LW(9))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -500, 500, 2000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(fune_curve_pos)
            CALL(evt_mapobj_trans, 1, STRING("S_fune_m"), LW(7), LW(8), LW(9))
            CALL(evt_mapobj_rotate, 1, STRING("S_fune_m"), 0, 180, LW(0))
            CALL(evt_snd_sfx_pos, GW(4), LW(7), LW(8), LW(9))
            CALL(evt_mapobj_trans, 1, STRING("S_umi_l_siro_b"), LW(7), LW(8), LW(9))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

static void pressStartGX(LogoWork* wp, s32 alpha, f32 x, f32 y) {
    CameraEntry* camera = camGetCurPtr();
    GXTexObj startText, gradient;
    Mtx v17, v16;

    TEXGetGXTexObjFromPalette(*wp->tpl->data, &startText, VERSION_PICK(8, 15));
    GXLoadTexObj(&startText, GX_TEXMAP0);
    TEXGetGXTexObjFromPalette(*wp->tpl->data, &gradient, VERSION_PICK(9, 6));
    GXLoadTexObj(&gradient, GX_TEXMAP1);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    GXSetTevColor(GX_TEVREG0, (GXColor){ 0x00, 0x00, 0x00, alpha });
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 30);
    MTXScale(v16, 1.0f, 1.0f, 1.0f);
    MTXTrans(v17, (f32)gp->retraceCount / -200.0f, 0.0f, 0.0f);
    MTXConcat(v16, v17, v17);
    GXLoadTexMtxImm(v17, 30, GX_MTX2x4);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    MTXTrans(v17, x, y, 0.0f);
    MTXScale(v16, (f32)GXGetTexObjWidth(&startText), (f32)GXGetTexObjHeight(&startText), 1.0f);
    MTXConcat(v17, v16, v17);
    MTXConcat(camera->view, v17, v17);
    GXLoadPosMtxImm(v17, 0);
    GXSetCurrentMtx(0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-0.5f, -0.5f, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXPosition3f32(0.5f, -0.5f, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(0.5f, 0.5f, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(-0.5f, 0.5f, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
}

static void draw(CameraId camera, void* param) {
    LogoWork* wp = param;
    CameraEntry saved;
    f32 viewport[6];
    f32 projection[7];
    CameraEntry* cam;
#ifdef VERSION_G8ME01
    // US: logo texture per language (indexed by gp->language)
    s32 logoTex[6] = {0x15, 0x15, 0x16, 0x17, 0x18, 0x19};
#endif

    GXGetViewportv(viewport);
    GXGetProjectionv(projection);
    cam = camGetPtr(CAMERA_2D);
    GXSetProjection(camGetPtr(CAMERA_2D)->projection, cam->projType);
    saved = *camGetCurPtr();
    cam = camGetPtr(CAMERA_2D);
    *camGetCurPtr() = *cam;
    winTexInit(*(TEXPalette**)wp->tpl->data);
#ifdef VERSION_G8ME01
    winTexSetNearest(logoTex[gp->language], (Vec){0.0f, 100.0f + wp->y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, wp->alpha});
#else
    winTexSetNearest(5, (Vec){0.0f, 100.0f + wp->y, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, (GXColor){0xFF, 0xFF, 0xFF, wp->alpha});
#endif
    *camGetCurPtr() = saved;
    GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
    GXSetProjectionv(projection);
}

static EVT_BEGIN(evt_prologue)
    WAIT_MS(500)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_OPENING1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_OPN_MUJ1"))
    WAIT_MS(3500)
    SET_VALUE(GF(0), 0)
    SET_VALUE(GF(1), 0)
    SET_VALUE(GF(2), 0)
    BEGIN_INLINE_EVENT
        CALL(logo)
    END_INLINE_EVENT
    SET_FLOAT_VALUE(GW(10), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(2), FLOAT(-1800.0f))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -1800, 0, 10000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_FLOAT_VALUE(LW(3), LW(0))
        SUBTRACT_FLOAT(LW(3), LW(2))
        SET_FLOAT_VALUE(GW(10), LW(3))
        SET_FLOAT_VALUE(LW(2), LW(0))
        SET_FLOAT_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        LOOP(5)
            WAIT_MS(1000)
            IF_INT_NOT_EQUAL(GF(1), 0)
                RETURN
            END_IF
        END_LOOP
        IF_INT_EQUAL(GF(1), 0)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_SAILING1"))
        END_IF
    END_INLINE_EVENT
    LOOP(0)
        IF_INT_EQUAL(GF(1), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxon, STRING("SFX_PRESS_START1"), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
        CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 6000)
        CALL(evt_snd_envoff_f, PSND_FADEOUT, 6000)
        CALL(evt_fade_set_mapchange_type, 1, 17, 800, 16, 800)
        CALL(evt_bero_mapchange, STRING("sys_01"), STRING("prologue"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(5000)
        CALL(evt_snd_sfxoff, GW(4))
    END_INLINE_EVENT
    SET_FLOAT_VALUE(GW(10), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(2), FLOAT(0.0f))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 1800, 10000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_FLOAT_VALUE(LW(3), LW(0))
        SUBTRACT_FLOAT(LW(3), LW(2))
        SET_FLOAT_VALUE(GW(10), LW(3))
        SET_FLOAT_VALUE(LW(2), LW(0))
        SET_FLOAT_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    LOOP(0)
        IF_INT_EQUAL(GF(2), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_npc_entry)
    CALL(evt_npc_entry, STRING("コルテス"), STRING("c_korutesu3"))
    CALL(evt_npc_set_tribe, STRING("コルテス"), STRING("コルテス"))
    CALL(evt_npc_set_position, STRING("コルテス"), FLOAT(-830.0f), FLOAT(147.0f), FLOAT(-98.0f))
    CALL(evt_npc_set_ry, STRING("コルテス"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), NPC_FLAG_INSTANT_TURN)
    RETURN
EVT_END()

EVT_BEGIN(fune_move)
    BEGIN_INLINE_EVENT
        CALL(kaizoku_fune_eff)
    END_INLINE_EVENT
    CALL(evt_mapobj_get_position, STRING("S_b"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_EPILOGUE_KOR_SHIP_MOVE1"), LW(0), LW(1), LW(2), LW(10))
    CALL(evt_snd_sfx_dist, LW(10), 30)
    SET_FLOAT_VALUE(LW(0), FLOAT(-700.0f))
    SET_FLOAT_VALUE(LW(1), FLOAT(0.0f))
    LOOP(0)
        ADD_FLOAT(LW(0), FLOAT(2.0f))
        CALL(evt_mapobj_trans, 1, STRING("S_b"), LW(0), 0, 0)
        CALL(evt_mapobj_get_position, STRING("S_b"), LW(3), LW(4), LW(5))
        CALL(evt_snd_sfx_pos, LW(10), LW(3), LW(4), LW(5))
        ADD_FLOAT(LW(1), FLOAT(1.0f))
        IF_INT_GREATER_OR_EQUAL(LW(1), FLOAT(360.0f))
            SUBTRACT_FLOAT(LW(1), FLOAT(360.0f))
        END_IF
        CALL(evt_sub_get_sincos, LW(1), LW(2), LW(3))
        MULTIPLY_FLOAT(LW(2), FLOAT(3.0f))
        CALL(evt_mapobj_rotate, 1, STRING("S_b"), 0, 0, LW(2))
        CALL(evt_mapobj_trans, 1, STRING("S_umi_l_siro_b"), LW(0), 0, 0)
        SET_FLOAT_VALUE(LW(2), LW(0))
        ADD_FLOAT(LW(2), FLOAT(130.0f))
        CALL(evt_npc_set_position, STRING("コルテス"), LW(2), 147, -98)
        SET_FLOAT_VALUE(LW(2), LW(0))
        ADD_FLOAT(LW(2), FLOAT(45.0f))
        CALL(evt_npc_set_position, STRING("サンダース"), LW(2), 90, -50)
        SET_FLOAT_VALUE(LW(2), LW(0))
        ADD_FLOAT(LW(2), FLOAT(17.0f))
        CALL(evt_npc_set_position, STRING("水夫A"), LW(2), 90, -169)
        SET_FLOAT_VALUE(LW(2), LW(0))
        ADD_FLOAT(LW(2), FLOAT(-106.0f))
        CALL(evt_npc_set_position, STRING("水夫ボム兵"), LW(2), 160, -18)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_MUJ1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_END_MUJ3"))
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_npc_set_position, STRING("サンダース"), FLOAT(-655.0f), FLOAT(90.0f), FLOAT(-50.0f))
    CALL(evt_npc_set_ry, STRING("サンダース"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("サンダース"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_position, STRING("水夫A"), FLOAT(-683.0f), FLOAT(90.0f), FLOAT(-169.0f))
    CALL(evt_npc_set_ry, STRING("水夫A"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("水夫A"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_position, STRING("水夫ボム兵"), FLOAT(-806.0f), FLOAT(160.0f), FLOAT(-18.0f))
    CALL(evt_npc_set_ry, STRING("水夫ボム兵"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("水夫ボム兵"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 25, 305, 1091, 25, 150, -108, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    SPAWN_EVENT(PTR(fune_move))
    WAIT_MS(10000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 900, 9, 900)
    CALL(evt_bero_mapchange, STRING("muj_01"), STRING(""))
    RETURN
EVT_END()

EVT_BEGIN(muj_20_init_evt)
    BEGIN_INLINE_EVENT
        CALL(evt_light_bind_mapobj, STRING("fune_b_pointLight1"), STRING("S_b"), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_light_bind_mapobj, STRING("fune_b_pointLight2"), STRING("S_b"), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_light_bind_mapobj, STRING("fune_b_pointLight3"), STRING("S_b"), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_light_bind_mapobj, STRING("fune_b_pointLight4"), STRING("S_b"), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_light_bind_mapobj, STRING("fune_b_pointLight5"), STRING("S_b"), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_light_bind_mapobj, STRING("fune_b_pointLight6"), STRING("S_b"), 1)
    END_INLINE_EVENT
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hai_l_01"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hai_l_02"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hai_l_03"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hai_l_04"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hai_l_05"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hai_r_01"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hai_r_02"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hai_r_03"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hai_r_04"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hai_r_05"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fune_m"), MAP_OBJ_FLAG_HIDDEN)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_mario_set_mode, 2)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_a1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_b"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_umi_l_siro_b"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_map_playanim, STRING("nami"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_playanim, STRING("nami_02"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_playanim, STRING("fune_01"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_playanim, STRING("kumo"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_map_playanim, STRING("yoru_hiru"), MAP_ANIM_PLAY_AT_END, 0)
        SPAWN_EVENT(PTR(muj_20_koopa_evt))
        RETURN
    END_IF
    CALL(evt_map_playanim, STRING("nami"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("nami_02"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("fune_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("kumo"), MAP_ANIM_PLAY_LOOP, 0)
    IF_INT_EQUAL(GSWF_GOR_SAILING_TO_KEELHAUL, 1)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG5_GHOST_SHIP1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ1_1"))
        SPAWN_EVENT(PTR(gor_muj_ship))
    END_IF
    IF_INT_EQUAL(GSWF_MUJ_SAILING_TO_ROGUEPORT, 1)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG5_GHOST_SHIP1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ1_1"))
        SPAWN_EVENT(PTR(muj_gor_ship))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_END_OFF_TO_THE_KEYS)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_SAILING1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ1_1"))
        SPAWN_EVENT(PTR(onryo_event))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_00_LOGBOOK)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_SAILING1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ1_1"))
        SPAWN_EVENT(PTR(onryo_event))
    END_IF
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("prologue"))
        CALL(evt_cam3d_evt_set, 0, 367, 2093, 0, 343, -115, 0, EASING_SINE_IN_OUT)
        CALL(evt_mario_cont_onoff, 0)
        CALL(evt_party_cont_onoff, 0, 0)
        CALL(evt_mario_set_pos, 0, -1000, 0)
        CALL(evt_party_set_pos, 0, 0, -1000, 0)
        CALL(evt_map_playanim, STRING("yoru_hiru"), MAP_ANIM_PLAY_AT_END, 0)
        CALL(evt_mapobj_trans, 1, STRING("S_hai03_sun"), 0, -260, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_umi_l_siro_b"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_a1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_b"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_fune_m"), MAP_OBJ_FLAG_HIDDEN)
        BEGIN_INLINE_EVENT
            CALL(fune_eff)
        END_INLINE_EVENT
        SET_FLOAT_VALUE(GW(0), FLOAT(-2000.0f))
        SET_FLOAT_VALUE(GW(1), FLOAT(0.0f))
        SET_FLOAT_VALUE(GW(2), FLOAT(-3000.0f))
        SPAWN_EVENT(PTR(anm_kaji))
        SPAWN_EVENT(PTR(anm_wakka))
        SPAWN_EVENT(PTR(anm_entotsu))
        SPAWN_EVENT(PTR(anm_neji))
        SPAWN_EVENT(PTR(anm_yure))
        SPAWN_EVENT(PTR(evt_prologue))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_a1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_a1"), 1, 0)
        CALL(evt_map_playanim, STRING("yoru_hiru"), MAP_ANIM_PLAY_AT_END, 0)
        CALL(evt_mario_kill_party, 0)
        CALL_CHILD(PTR(epilogue_npc_entry))
        SPAWN_EVENT(PTR(epilogue_evt))
    END_IF
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
/* FAKE: in the US build the step-count argument (FPS_FRAMES) of logo's second
 * intplGetValue call is evaluated after the float arguments. Calling through a
 * prototype with `steps` last reproduces that evaluation order; the registers
 * (r3 mode, r4 step, r5 steps, f1 start, f2 end) are the same under the EABI,
 * so the call is behaviour-identical. */


#endif

USER_FUNC(logo) {
    LogoWork* wp;

    if (isFirstCall) {
        wp = _mapAlloc(sizeof(LogoWork));
        memset(wp, 0, sizeof(LogoWork));
        event->userdatap[0] = wp;
    }
    wp = event->userdatap[0];
    if ((u32)fileAsyncf(FILE_TYPE_TPL, NULL, "%s/mariost.tpl", getMarioStDvdRoot()) == 0) {
        return EVT_RETURN_BLOCK;
    }
    switch (wp->state) {
        case 0:
            wp->tpl = fileAllocf(FILE_TYPE_TPL, "%s/mariost.tpl", getMarioStDvdRoot());
            wp->alpha = 0;
            wp->y = 0.0f;
            wp->counter = 0;
            wp->state++;
            break;
        case 1:
            if (wp->counter < VERSION_PICK(180, FPS_FRAMES(180))) {
                wp->alpha = intplGetValue(EASING_SINE_IN_OUT, 0.0f, 255.0f, ++wp->counter, VERSION_PICK(180, FPS_FRAMES(180)));
#ifdef VERSION_G8ME01
                wp->y = intplGetValue(EASING_SINE_IN_OUT, -50.0f, 0.0f, ++wp->counter, FPS_FRAMES(180)); /* FAKE */
#else
                wp->y = intplGetValue(EASING_SINE_IN_OUT, -50.0f, 0.0f, ++wp->counter, 180);
#endif
            } else if (gp->retraceCount % 15 == 0) {
                wp->state++;
            }
            break;
        case 2:
            if (evtGetValue(event, GF(0)) && (keyGetButtonTrg(0) & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START))) {
                wp->counter = 0;
                wp->state++;
                evtSetValue(event, GF(1), 1);
            }
            break;
        case 3:
            if (wp->counter < VERSION_PICK(120, FPS_FRAMES(120))) {
                wp->alpha = intplGetValue(EASING_SINE_IN_OUT, 255.0f, 0.0f, ++wp->counter, VERSION_PICK(120, FPS_FRAMES(120)));
#ifdef VERSION_G8ME01
                wp->y = intplGetValue(EASING_SINE_IN_OUT, 0.0f, 100.0f, ++wp->counter, FPS_FRAMES(120)); /* FAKE */
#else
                wp->y = intplGetValue(EASING_SINE_IN_OUT, 0.0f, 100.0f, ++wp->counter, 120);
#endif
            } else {
                wp->state++;
            }
            break;
        case 4:
            fileFree(wp->tpl);
            wp->tpl = NULL;
            _mapFree(wp);
            evtSetValue(event, GF(2), 1);
            return EVT_RETURN_DONE;
    }
    if (wp->tpl) {
        dispEntry(CAMERA_2D, 1000.0f, 0, draw, wp);
    }
    return EVT_RETURN_BLOCK;
}

