#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "cam_shift.h"
#include "libc_decls.h"
#include "party.h"

#include "manager/evtmgr_cmd.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "string.h"
#include "mario/mario.h"
#include "driver/dispdrv.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "driver/effdrv.h"
#include "system.h"
#include "icon_ids.h"
#include "yuu.h"
#ifdef VERSION_G8ME01
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_sub_get_language);
#endif

typedef struct MonteSub { s32 unk0; Vec pos; f32 scale; u8 unk14[0x38 - 0x14]; } MonteSub;
typedef struct Monte { u8 hidden; u8 unk1; s16 num; u8 state; u8 unk5[3]; MonteSub sub; MiniGameWork* wp; } Monte;
void yuu_03_icon_monte_disp(CameraId camera, void* param);
void marioSlitForceCancel(void);
void ext_yuunpc_minigame4_init_sub(void);
void yuunpc_wp_flag_off(s32 flag);
void marioRollTryCancel(void);
void motSlitCancel3(void);
BOOL marioRollKeyDisable(void);
typedef struct RyushiWork { s32 flag; s32 mode; Vec base; Vec pos; s32 alpha; s32 count; f32 scale; s32 r; s32 g; s32 b; s32 r2; s32 g2; s32 b2; } RyushiWork;
static void* yuunpc_wp;
s32 ryuwp;
RyushiWork* xwp;
Monte* yuu_03_monte_p;
static void polygon(f32 x, f32 y, f32 w, f32 h, f32 u, f32 v);
static f32 randf(f32 min, f32 max);
void ryushi_disp(CameraId camera, void* param);
USER_FUNC(evt_ryushi_init);
void mapObjRotate(const char* name, f32 x, f32 y, f32 z);
BOOL yuuminigame4_gamerule(EventEntry* event, BOOL isFirstCall, MiniGameWork* wp, SlitWork* data);
void monteCountDisp(void);
s32 yuucounter_get_data(void);
void yuucounter_flag_on(s32 flag);
#define REVISE360(a) ((3600.0f + (a)) - (s32)((3600.0f + (a)) / 360.0f) * 360)

USER_FUNC(eff_call_minigame);
USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_cam3d_evt_set_now);
USER_FUNC(evt_cam_letter_box_disable);
USER_FUNC(evt_cam_letter_box_onoff);
USER_FUNC(evt_cam_road_reset);
USER_FUNC(evt_fade_in);
USER_FUNC(evt_fade_out);
USER_FUNC(evt_key_get_buttontrg);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mario_get_name_hitobj_ride);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_kill_party);
USER_FUNC(evt_mario_mov_pos2);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_enable_key);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mario_set_pose);
USER_FUNC(evt_mario_set_prev_party_pos);
USER_FUNC(evt_mario_wait_move_end);
USER_FUNC(evt_mario_wait_rideon);
USER_FUNC(evt_msg_select);
USER_FUNC(evt_ryushi);
USER_FUNC(evt_ryushi_clr);
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
USER_FUNC(re_init_yuunpc);
USER_FUNC(yuu_03_icon_monte_entry);
USER_FUNC(yuu_03_cam_shift_reset);
USER_FUNC(yuugijou_add_montemedal);
USER_FUNC(yuugijou_get_montemedal);
USER_FUNC(yuugijou_montemedal_disp_onoff);
USER_FUNC(yuugijou_montemedal_wait);
USER_FUNC(yuuminigame4_opening_chk);
USER_FUNC(yuuminigame4_opening_on_off);
USER_FUNC(yuuminigame4_set_phase);
USER_FUNC(yuuminigame_get_dataadr);
USER_FUNC(yuuminigame_set_dataadr);
USER_FUNC(yuunpc_go);
USER_FUNC(yuuranking_get_monte);
USER_FUNC(yuuranking_wait_button);
BOOL yuuminigame4_gamerule(EventEntry* event, BOOL isFirstCall, MiniGameWork* wp, SlitWork* data);
void yuuminigame_disp_4slit(CameraId camera, void* param);
void yuuminigame_main_4slit(EventEntry* event, BOOL isFirstCall);
void yuuminigame_init_4slit(EventEntry* event, BOOL isFirstCall);
USER_FUNC(evt_ryushi_init);
static f32 randf(f32 min, f32 max);
void ryushi_disp(CameraId camera, void* param);
static void polygon(f32 x, f32 y, f32 w, f32 h, f32 u, f32 v);
static USER_FUNC(evt_nomal_cange);
USER_FUNC(evt_slit_cancel);
USER_FUNC(yuu_03_icon_monte_restart);
void yuu_03_icon_monte_disp(CameraId camera, void* param);

EvtWord yuu_03_icon_monte_entry_evt[4];
EvtWord ryushi_event[25];
EvtWord yuu_03_kaijo_init[12];
EvtWord yuu_03_kaijo_init2[12];
EvtWord minigame04_main[33];
EvtWord minigame04_start_cam[40];
EvtWord minigame04_start_cam_cancel[20];
EvtWord minigame04_start[91];
EvtWord minigame04_end[256];
EvtWord yuu_03_init_evt[VERSION_PICK(26, 140)];

USER_FUNC(yuu_03_cam_shift_reset) {
    camShiftReset();
    return EVT_RETURN_DONE;
}

void yuu_03_icon_monte_disp(CameraId camera, void* param) {
    Monte* monte = param;
    s32 i;
    Vec pos;

    if (monte != NULL) {
        for (i = 0; i < monte->num; i++) {
            if (monte[i].hidden == 0) {
                pos = monte[i].sub.pos;
                if ((gp->retraceCount & 0x1F) < 9) {
                    pos.y += 5.0f;
                }
                iconDispGx(&pos, 0, ICON_1B6, 1.0f);
            }
        }
    }
}

USER_FUNC(yuu_03_icon_monte_entry) {
    BOOL exists = FALSE;
    MiniGameWork* wp = (MiniGameWork*)event->lwData[0];
    MarioWork* mario = marioGetPtr();
    s32 total;
    s32 i;
    MonteSub* sub;
    Monte* m;

    if (isFirstCall) {
        if (yuu_03_monte_p != NULL) {
            exists = TRUE;
        }
        total = 0;
        for (i = 0; i < 1; i++) {
            total++;
        }
        yuu_03_monte_p = _mapAlloc(total * sizeof(Monte));
        memset(yuu_03_monte_p, 0, total * sizeof(Monte));
        yuu_03_monte_p->num = total;
        yuu_03_monte_p->wp = wp;
        for (i = 0; i < yuu_03_monte_p->num; i++) {
            sub = &yuu_03_monte_p[i].sub;
            sub->pos = mario->position;
            sub->pos.y += 40.0f;
            sub->scale = 1.0f;
        }
    }
    for (i = 0; i < yuu_03_monte_p->num; i++) {
        m = &yuu_03_monte_p[i];
        switch (m->state) {
            case 0:
                m->sub.pos = mario->position;
                m->sub.pos.y += 40.0f;
                break;
            case 1:
                break;
        }
    }
    if (!exists) {
        dispEntry(CAMERA_3D, 0.0f, 1, yuu_03_icon_monte_disp, yuu_03_monte_p);
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(yuu_03_icon_monte_restart) {
    SlitWork* data;

    if (yuu_03_monte_p != NULL) {
        data = yuu_03_monte_p->wp->data;
        if (data->viewEvent != NULL) {
            evtDeleteID(data->viewEvent->eventId);
            data->viewEvent = NULL;
            _mapFree(yuu_03_monte_p);
            data->viewEvent = evtEntryType(yuu_03_icon_monte_entry_evt, 0, 0, 0);
            data->viewEvent->lwData[0] = (EvtWord)yuu_03_monte_p->wp;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame4_set_phase) {
    EvtWord* args = event->args;
    SlitWork* data = ((MiniGameWork*)evtGetValue(event, args[0]))->data;

    data->phase = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_slit_cancel) {
    MarioWork* mario = marioGetPtr();

    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        marioSlitForceCancel();
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(re_init_yuunpc) {
    ext_yuunpc_minigame4_init_sub();
    return EVT_RETURN_DONE;
}

USER_FUNC(yuunpc_go) {
    yuunpc_wp_flag_off(1);
    return EVT_RETURN_DONE;
}

static USER_FUNC(evt_nomal_cange) {
    MarioWork* mario = marioGetPtr();

    if (isFirstCall) {
        marioRollTryCancel();
        if (mario->flags & MARIO_FLAG_PAPER_MODE) {
            motSlitCancel3();
        }
    }
    if (!marioRollKeyDisable()) {
        return EVT_RETURN_BLOCK;
    }
    return (mario->dispFlags & 0x1000000) ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame4_opening_on_off) {
    EvtWord* args = event->args;
    SlitWork* data = ((MiniGameWork*)evtGetValue(event, args[0]))->data;

    if (evtGetValue(event, args[1])) {
        data->flags |= 1;
    } else {
        data->flags &= ~1;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame4_opening_chk) {
    EvtWord* args = event->args;
    MiniGameWork* wp = (MiniGameWork*)evtGetValue(event, args[0]);

    if (((SlitWork*)wp->data)->flags & 1) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

static void polygon(f32 x, f32 y, f32 w, f32 h, f32 u, f32 v) {
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x, y, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(x + w, y, 0.0f);
    GXTexCoord2f32(u, 0.0f);
    GXPosition3f32(x + w, y - h, 0.0f);
    GXTexCoord2f32(u, v);
    GXPosition3f32(x, y - h, 0.0f);
    GXTexCoord2f32(0.0f, v);
}

void ryushi_disp(CameraId camera, void* param) {
    RyushiWork* hdr = xwp;
    s32 g = hdr->g;
    s32 b = hdr->b;
    RyushiWork* p;
    s32 i;
    s32 n;
    f32 deg;
    f32 scale;
    f32 amp;
    Mtx trans;
    Mtx rot;
    Mtx scl;
    Mtx out;
    Mtx base;
    GXTexObj tex;

    PSMTXScale(base, hdr->scale, hdr->scale, 1.0f);
    PSMTXTransApply(base, base, hdr->pos.x, hdr->pos.y, hdr->pos.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(camera)->viewYaw);
    PSMTXConcat(base, rot, base);
    effGetTexObj(0x1E, &tex);
    GXLoadTexObj(&tex, GX_TEXMAP0);
    GXSetTevColor(GX_TEVREG1, (GXColor){hdr->r2, hdr->g2, hdr->b2});
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_C0, GX_CC_C1, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A0, GX_CA_APREV, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    for (i = 1, p = hdr + 1; i < ryuwp; i++, p++) {
        scale = p->scale;
        if (scale) {
            GXSetTevColor(GX_TEVREG0, (GXColor){amp, g, b, p->alpha});
            deg = 180.0f * ((p->pos.z / 8.0f + 3.14f * i / 4.0f) / 3.14f);
            n = deg / 360.0f;
            if (deg < 0.0f) {
                n--;
            }
            deg -= n * 360;
            amp = 3.0f;
            PSMTXTrans(trans, p->pos.x + amp * yuusintbl[(s32)deg], p->pos.y, p->pos.z);
            PSMTXScale(scl, scale, scale, scale);
            PSMTXConcat(trans, scl, trans);
            PSMTXConcat(base, trans, trans);
            PSMTXConcat(camGetPtr(camera)->view, trans, out);
            GXLoadPosMtxImm(out, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            polygon(-8.0f, -8.0f, 16.0f, 16.0f, 1.0f, 1.0f);
        }
    }
}

static f32 randf(f32 min, f32 max) {
    f32 r = rand() / 32767.0f;

    return r * (max - min) + min;
}

USER_FUNC(evt_ryushi_init) {
    RyushiWork* p;
    RyushiWork* q;
    s32 i;

    ryuwp = 241;
    xwp = p = _mapAlloc(ryuwp * sizeof(RyushiWork));
    p->mode = 0;
    p->count = 0;
    p->alpha = 0;
    p->pos.x = 0.0f;
    p->pos.y = 0.0f;
    p->pos.z = 0.0f;
    p->scale = 1.0f;
    p->r = 255;
    p->g = 0;
    p->b = 255;
    p->r2 = 255;
    p->g2 = 128;
    p->b2 = 255;
    for (i = 1, q = p + 1; i < 241; i++, q++) {
        q->flag = 0;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_ryushi_clr) {
    RyushiWork* p = xwp;
    EvtWord* args = event->args;
    s32 i;
    s32 n = ryuwp;
    s32 j;
    s32 x;
    s32 y;
    s32 z;

    p->mode = evtGetValue(event, *args++);
    p->count = 0;
    p->alpha = 0;
    p->pos.x = 0.0f;
    p->pos.y = 0.0f;
    p->pos.z = 0.0f;
    xwp->base.x = 50.0f;
    xwp->base.y = 50.0f;
    xwp->base.z = 100.0f;
    for (i = 1, p++; i < n; i++, p++) {
        j = i - 1;
        p->flag = 0;
        x = j % 10;
        y = (j / 10) % 6;
        z = j / 60;
        p->base.x = x * xwp->base.x - 4.5f * xwp->base.x + xwp->base.x / 2.0f * ((y + z) & 1);
        p->base.y = y * xwp->base.y - 2.5f * xwp->base.y;
        p->base.z = z * xwp->base.z - 1.5f * xwp->base.z;
        p->pos.x = p->base.x + randf(0.3f * -xwp->base.x, 0.3f * xwp->base.x);
        p->pos.y = p->base.y + randf(0.3f * -xwp->base.y, 0.3f * xwp->base.y);
        p->pos.z = p->base.z + randf(0.3f * -xwp->base.z, 0.3f * xwp->base.z);
        p->scale = 0.5f;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_ryushi) {
    RyushiWork* hdr = xwp;
    Vec pos2;
    Vec pos = {0.0f, 0.0f, 0.0f};
    SlitWork* data;
    RyushiWork* p;
    s32 i;
    s32 n;
    f32 speed;
    f32 alphaScale;
    f32 camX;
    f32 scale;
    f32 w;
    f32 d;

    pos.x = hdr->pos.x;
    pos.y = hdr->pos.y;
    pos.z = hdr->pos.z;
    pos2 = pos;
    speed = 0.0f;
    alphaScale = 0.0f;
    scale = 1.0f;
    data = ((MiniGameWork*)event->lwData[0])->data;
    camX = camGetPtr(CAMERA_3D)->cameraAt.x;
    switch (data->state) {
        case 2:
        case 3:
        case 4:
            speed = 2.9166667f;
            alphaScale = 1.0f;
            break;
        case 6:
        case 7:
        case 8:
            speed = 2.9166667f;
            alphaScale = 1.0f;
            break;
    }
    switch (data->state) {
        case 1:
        case 5:
            alphaScale = (60 - data->timer) / 60.0f;
            scale = alphaScale;
            break;
        case 2:
        case 6:
            speed = speed * (60 - data->timer) / 60.0f;
            break;
        case 4:
        case 8:
            alphaScale = data->timer / 48.0f;
            break;
    }
    hdr->count++;
    n = hdr->count;
    if (n <= 20) {
        hdr->alpha = n * 12;
    } else {
        hdr->alpha = 255;
    }
    if (n & 1) {
        hdr->alpha /= 2;
    }
    scale = 0.5f * scale;
    for (i = 1, p = hdr + 1; i < ryuwp; i++, p++) {
        switch (xwp->mode) {
            case 0:
                p->pos.z -= speed;
                if (p->pos.z < -200.0f) {
                    p->pos.x = p->base.x + randf(0.3f * -xwp->base.x, 0.3f * xwp->base.x);
                    p->pos.y = p->base.y + randf(0.3f * -xwp->base.y, 0.3f * xwp->base.y);
                    p->pos.z = 200.0f + randf(0.3f * -xwp->base.z, 0.3f * xwp->base.z);
                }
                break;
            case 1:
                p->pos.z += speed;
                if (p->pos.z > 200.0f) {
                    p->pos.x = p->base.x + randf(0.3f * -xwp->base.x, 0.3f * xwp->base.x);
                    p->pos.y = p->base.y + randf(0.3f * -xwp->base.y, 0.3f * xwp->base.y);
                    p->pos.z = -200.0f + randf(0.3f * -xwp->base.z, 0.3f * xwp->base.z);
                }
                break;
        }
        n = (p->pos.x - camX) / (10.0f * xwp->base.x / 2.0f);
        if (n != 0) {
            if (n > 0) {
                n++;
            } else {
                n--;
            }
            p->pos.x += (-n / 2) * (10.0f * xwp->base.x);
        }
        if (p->pos.z < 0.0f) {
            d = -p->pos.z;
        } else {
            d = p->pos.z;
        }
        d -= 100.0f;
        if (d < 0.0f) {
            d = 0.0f;
        } else if (d > 100.0f) {
            d = 100.0f;
        }
        p->alpha = 0.7 * (255.0f * ((100.0f - d) / 100.0f) * alphaScale);
        p->scale = scale;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&pos2), 2, ryushi_disp, NULL);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(yuu_03_icon_monte_entry_evt)
    CALL(yuu_03_icon_monte_entry)
    RETURN
EVT_END()

EVT_BEGIN(ryushi_event)
    SET_VALUE(GW(0), 0)
    CALL(evt_ryushi_clr, LW(1))
    LOOP(0)
        CALL(evt_ryushi)
        IF_INT_EQUAL(GW(0), 1)
            BREAK_LOOP
        END_IF
        IF_INT_EQUAL(GW(0), 2)
            RETURN
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(yuu_03_kaijo_init)
    CALL(evt_mario_set_pos, -1650, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(yuu_03_kaijo_init2)
    CALL(evt_mario_set_pos, -1520, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(minigame04_main)
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(yuu_03_kaijo_init))
    CALL(evt_yuuminigame_alloc, LW(0), 3)
    CALL(yuuminigame_set_dataadr, 0, LW(0))
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mario_kill_party, 1)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE1"), GW(10))
    CALL(yuuminigame_get_dataadr, 0, LW(0))
    CALL(yuuminigame4_set_phase, LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(minigame04_start_cam)
    CALL(evt_cam3d_evt_set, FLOAT(-1520.0f), FLOAT(335.0f), FLOAT(850.0f), FLOAT(-1520.0f), FLOAT(65.0f), FLOAT(-25.0f), 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_seq_wait, 2)
    CALL(evt_cam3d_evt_set, FLOAT(1285.0f), FLOAT(334.0f), FLOAT(850.0f), FLOAT(1285.0f), FLOAT(65.0f), FLOAT(-25.0f), 10000, 0)
    WAIT_MS(1000)
    CALL(evt_yuutitle_alloc, 0, 0, STRING("menu_title_minigame4"))
    WAIT_MS(7000)
    CALL(evt_yuutitle_free)
    WAIT_MS(2000)
    RETURN
EVT_END()

EVT_BEGIN(minigame04_start_cam_cancel)
    CALL(evt_seq_wait, 2)
    CALL(evt_yuutitle_free)
    CALL(evt_cam3d_evt_off, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_mario_wait_move_end)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL_CHILD(PTR(yuu_03_kaijo_init2))
    RETURN
EVT_END()

EVT_BEGIN(minigame04_start)
    CALL(yuuminigame4_opening_chk, LW(0), LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        SPAWN_EVENT_GET_ID(PTR(minigame04_start_cam), LW(8))
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
                    CALL_CHILD(PTR(minigame04_start_cam_cancel))
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(yuuminigame4_opening_on_off, LW(0), 0)
        CALL_CHILD(PTR(yuu_03_kaijo_init2))
        CALL(evt_cam3d_evt_off, 0, 11)
        WAIT_FRAMES(1)
    END_IF
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_MINIGAME1"))
    CALL(eff_call_minigame, 0, 0, 0, 500)
    WAIT_MS(3000)
    CALL(evt_mario_set_enable_key, 352)
    CALL(evt_cam_letter_box_onoff, 0, 0)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(yuunpc_go)
    CALL(evt_yuucounter_alloc, 0, 70, 2, 701)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(minigame04_end)
    CALL(evt_cam_letter_box_onoff, 1, 0)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(evt_cam3d_evt_set_now)
    CALL(evt_mario_set_enable_key, 0)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE2"), 0)
    SET_VALUE(GW(0), 1)
    SET_VALUE(GF(0), 1)
    SET_VALUE(LW(3), 1)
    BEGIN_INLINE_EVENT
        IF_INT_LESS(LW(0), 701)
            LOOP(0)
                CALL(evt_mario_get_name_hitobj_ride, LW(0))
                IF_STR_NOT_EQUAL(LW(0), 0)
                    BREAK_LOOP
                END_IF
                IF_INT_EQUAL(GF(0), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            IF_INT_GREATER(LW(1), -100)
                IF_INT_NOT_EQUAL(GF(0), 0)
                    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                    IF_INT_LESS(LW(0), 1650)
                        CALL(evt_mario_normalize)
                        CALL(evt_mario_mov_pos2, 1650, LW(2), 120)
                    END_IF
                END_IF
                ELSE
                CALL(evt_mario_wait_rideon)
                CALL(evt_mario_normalize)
            END_IF
            ELSE
            CALL(evt_mario_wait_rideon)
            CALL(evt_mario_normalize)
        END_IF
    END_INLINE_EVENT
    IF_INT_GREATER_OR_EQUAL(LW(0), 701)
        CALL(eff_call_minigame, 2, 0, 0, 500)
        WAIT_MS(4000)
        ELSE
        CALL(eff_call_minigame, 1, 0, 0, 500)
        WAIT_MS(4000)
    END_IF
    CALL(evt_yuucounter_free)
    CALL(yuugijou_montemedal_disp_onoff, 1)
    CALL(evt_yuuranking_alloc, 0, 0, 1, 0, LW(0), 0, LW(1), 701)
    WAIT_FRAMES(1)
    CALL(yuuranking_wait_button)
    CALL(yuuranking_get_monte, LW(3))
    CALL(yuugijou_add_montemedal, 0, LW(3))
    CALL(yuugijou_montemedal_wait)
    CALL(evt_yuuranking_free)
    WAIT_MS(200)
    CALL(evt_yuuranking_alloc, 0, 0, 1, 1, LW(0), 0, LW(1), 701)
    WAIT_FRAMES(1)
    CALL(yuuranking_wait_button)
    CALL(evt_yuuranking_free)
    SET_VALUE(LW(1), 10)
    CALL(yuugijou_get_montemedal, LW(0))
    IF_INT_LESS(LW(0), LW(1))
        SET_VALUE(LW(0), 1)
        ELSE
        CALL(evt_msg_select, 0, STRING("sysyuu_03_00"))
    END_IF
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GSWF(1229), 1)
        CALL(evt_mario_set_prev_party_pos, 0, 0, -1000, 0)
        CALL(evt_mario_set_prev_party_pos, 1, 0, -1000, 0)
        CALL(evt_snd_bgmoff, 512)
        CALL(evt_bero_mapchange, STRING("gor_03"), STRING("A_dokan_02"))
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
    CALL(evt_cam3d_evt_off, 0, 11)
    SET_VALUE(GF(0), 0)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL_CHILD(PTR(yuu_03_kaijo_init2))
    CALL(evt_cam_road_reset)
    CALL(yuu_03_cam_shift_reset)
    WAIT_MS(500)
    CALL(re_init_yuunpc)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE1"), GW(10))
    CALL(evt_fade_in, 1000)
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(yuu_03_init_evt)
    CALL(evt_map_playanim, STRING("yajirushi"), 1, 0)
    CALL(evt_map_playanim, STRING("tama"), 1, 0)
    CALL(evt_map_playanim, STRING("gurade"), 1, 0)
    CALL(evt_map_playanim, STRING("kyaku"), 1, 0)
    SPAWN_EVENT(PTR(minigame04_main))
    CALL_CHILD(evt_yuunpc_minigame4)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("J_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("E_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("D_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("F_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("Sp_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("I_mon"), 16384)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("mon_10"), 16384)
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("J_mon"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("mon_10"), 16384)
        CASE_EQUAL(1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("E_mon"), 16384)
        CASE_EQUAL(2)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("D_mon"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("mon_10"), 16384)
        CASE_EQUAL(3)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("F_mon"), 16384)
        CASE_EQUAL(4)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("Sp_mon"), 16384)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("mon_10"), 16384)
        CASE_EQUAL(5)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("I_mon"), 16384)
    END_SWITCH
    RETURN
EVT_END()
#else
EVT_BEGIN(yuu_03_init_evt)
    CALL(evt_map_playanim, STRING("yajirushi"), 1, 0)
    CALL(evt_map_playanim, STRING("tama"), 1, 0)
    CALL(evt_map_playanim, STRING("gurade"), 1, 0)
    CALL(evt_map_playanim, STRING("kyaku"), 1, 0)
    SPAWN_EVENT(PTR(minigame04_main))
    CALL_CHILD(PTR(evt_yuunpc_minigame4))
    RETURN
EVT_END()
#endif

void yuuminigame_init_4slit(EventEntry* event, BOOL isFirstCall) {
    SlitWork* data = ((MiniGameWork*)event->lwData[0])->data;

    data->viewEvent = evtEntryType(yuu_03_icon_monte_entry_evt, 0, 0, 0);
    data->viewEvent->lwData[0] = event->lwData[0];
    data->flags |= 1;
    evt_ryushi_init(event, isFirstCall);
}

void yuuminigame_main_4slit(EventEntry* event, BOOL isFirstCall) {
    MiniGameWork* wp = (MiniGameWork*)event->lwData[0];
    SlitWork* data = wp->data;

    switch (data->phase) {
        case 0:
            break;
        case 5:
            data->phase = 1;
        case 1:
            wp->subEvent = evtEntry(minigame04_start, 0, 0);
            wp->subEvent->lwData[0] = event->lwData[0];
            data->phase++;
            break;
        case 2:
            data->state = 0;
            data->timer = (irand(3901) + 100) * 60 / 1000;
            data->result = 0;
            data->phase = 3;
        case 3:
            if (yuuminigame4_gamerule(event, isFirstCall, wp, data)) {
                data->phase++;
            }
            break;
        case 4:
            wp->subEvent = evtEntry(minigame04_end, 0, 0);
            wp->subEvent->lwData[0] = data->coins;
            switch (data->coins) {
                case 0:
                    data->result = 0x105;
                    break;
                case 1:
                    data->result = 0x103;
                    break;
                case 2:
                    data->result = 0x101;
                    break;
                case 10:
                case 21:
                case 32:
                case 43:
                case 54:
                case 65:
                case 76:
                case 87:
                case 98:
                case 110:
                case 221:
                case 332:
                case 443:
                case 554:
                case 665:
                case 776:
                case 887:
                case 998:
                case 1110:
                case 2221:
                case 3332:
                case 4443:
                case 5554:
                case 6665:
                case 7776:
                case 8887:
                case 9998:
                    data->result = 0x103;
                    break;
            }
            wp->subEvent->lwData[1] = data->result;
            data->phase++;
            break;
    }
    mapObjRotate("puro_1", 0.0f, 0.0f, data->rot1);
    mapObjRotate("puro_2", 0.0f, 0.0f, data->rot1);
    mapObjRotate("puro_3", 0.0f, 0.0f, data->rot1);
    mapObjRotate("puro_4", 0.0f, 0.0f, data->rot1);
    mapObjRotate("puro_5", 0.0f, 0.0f, data->rot1);
    mapObjRotate("puro_6", 0.0f, 0.0f, data->rot1);
    mapObjRotate("puro_7", 0.0f, 0.0f, data->rot1);
    mapObjRotate("puro_8", 0.0f, 0.0f, data->rot1);
    mapObjRotate("puro_9", 0.0f, 0.0f, data->rot1);
    mapObjRotate("puro_10", 0.0f, 0.0f, data->rot1);
    mapObjRotate("pera_1", 0.0f, 0.0f, data->rot2);
    mapObjRotate("pera_2", 0.0f, 0.0f, data->rot2);
    mapObjRotate("pera_3", 0.0f, 0.0f, data->rot2);
    mapObjRotate("pera_4", 0.0f, 0.0f, data->rot2);
    mapObjRotate("pera_5", 0.0f, 0.0f, data->rot2);
    mapObjRotate("pera_6", 0.0f, 0.0f, data->rot2);
    mapObjRotate("pera_7", 0.0f, 0.0f, data->rot2);
    mapObjRotate("pera_8", 0.0f, 0.0f, data->rot2);
    mapObjRotate("pera_9", 0.0f, 0.0f, data->rot2);
}

void yuuminigame_disp_4slit(CameraId camera, void* param) {
    monteCountDisp();
}

BOOL yuuminigame4_gamerule(EventEntry* event, BOOL isFirstCall, MiniGameWork* wp, SlitWork* data) {
    MarioWork* mario = marioGetPtr();
    EventEntry* ev;
    f32 speed;

    data->timer--;
    switch (data->state) {
        case 0:
            if (data->timer == 0) {
                if (irand(2)) {
                    data->state = 1;
                    ev = evtEntryType(ryushi_event, 0, 0, 0);
                    ev->lwData[0] = (EvtWord)wp;
                    ev->lwData[1] = 1;
                } else {
                    data->state = 5;
                    ev = evtEntryType(ryushi_event, 0, 0, 0);
                    ev->lwData[0] = (EvtWord)wp;
                    ev->lwData[1] = 0;
                }
                data->timer = 60;
            }
            break;
        case 1:
            if (data->timer == 0) {
                data->state = 2;
                data->timer = 60;
            }
        case 2:
            if (data->timer == 0) {
                data->state = 3;
                data->timer = (irand(2001) + 3000) * 60 / 1000;
            }
            break;
        case 3:
            if (data->timer == 0) {
                data->state = 4;
                data->timer = 48;
            }
            break;
        case 4:
            if (data->timer == 0) {
                data->state = 0;
                data->timer = (irand(3901) + 100) * 60 / 1000;
                evtSetValue(event, GW(0), 1);
            }
            break;
        case 5:
            if (data->timer == 0) {
                data->state = 6;
                data->timer = 60;
            }
            break;
        case 6:
            if (data->timer == 0) {
                data->state = 7;
                data->timer = (irand(2001) + 3000) * 60 / 1000;
            }
            break;
        case 7:
            if (data->timer == 0) {
                data->state = 8;
                data->timer = 48;
            }
            break;
        case 8:
            if (data->timer == 0) {
                data->state = 0;
                data->timer = (irand(3901) + 100) * 60 / 1000;
                evtSetValue(event, GW(0), 1);
            }
            break;
    }
    switch (data->state) {
        case 0:
            break;
        case 1:
        case 2:
            data->rot1 += 36.0f * (120.0f - (data->timer - 1)) / 120.0f;
            data->rot1 = REVISE360(data->rot1);
            break;
        case 3:
            data->rot1 += 36.0f;
            data->rot1 = REVISE360(data->rot1);
            break;
        case 4:
            data->rot1 += 36.0f * (data->timer - 1) / 48.0f;
            data->rot1 = REVISE360(data->rot1);
            break;
        case 5:
        case 6:
            data->rot2 += 36.0f * (120.0f - (data->timer - 1)) / 120.0f;
            data->rot2 = REVISE360(data->rot2);
            break;
        case 7:
            data->rot2 += 36.0f;
            data->rot2 = REVISE360(data->rot2);
            break;
        case 8:
            data->rot2 += 36.0f * (data->timer - 1) / 48.0f;
            data->rot2 = REVISE360(data->rot2);
            break;
    }
    if (mario->position.x < 1500.0f) {
        speed = 0.0f;
        switch (data->state) {
            case 2:
            case 3:
            case 4:
                if (mario->dispFlags & 0x1000000) {
                    if (mario->flags & MARIO_FLAG_PAPER_MODE) {
                        speed = 0.0f;
                    } else {
                        speed = 0.0f;
                    }
                } else {
                    speed = 2.9166667f;
                }
                break;
            case 6:
            case 7:
            case 8:
                if (mario->dispFlags & 0x1000000) {
                    if (mario->flags & MARIO_FLAG_PAPER_MODE) {
                        speed = -0.0f;
                    } else {
                        speed = -0.0f;
                    }
                } else {
                    speed = -2.9166667f;
                }
                break;
        }
        switch (data->state) {
            case 2:
            case 6:
                speed = speed * (60 - data->timer) / 60.0f;
                break;
        }
        mario->floorMove.z += speed;
    }
    if (mario->position.y <= -100.0f || mario->position.x >= 1500.0f) {
        if (mario->position.y <= -100.0f) {
            data->coins = 701;
            yuucounter_flag_on(1);
        } else {
            data->coins = yuucounter_get_data();
        }
        data->state = 0;
        data->timer = (irand(3901) + 100) * 60 / 1000;
        marioKeyOff();
        partyKeyOff();
        yuucounter_flag_on(2);
        return TRUE;
    }
    return FALSE;
}

