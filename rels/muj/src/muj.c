#include "eff/n64/eff_kameki_tornade_n64.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "mario/mario.h"
#include "system.h"
#include "driver/effdrv.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_npc.h"
#include "party.h"
#include "mario/mariost.h"
#include "dolphin/os.h"
#include "pmario_sound.h"
#include "data/mapdata.h"
#include "muj.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

USER_FUNC(evt_eff64);
USER_FUNC(evt_eff_softdelete);
USER_FUNC(evt_npc_flag_onoff);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_intpl_msec_get_value_para);
USER_FUNC(evt_sub_intpl_msec_init);
USER_FUNC(muj_mario_pos_osreport_func);
USER_FUNC(muj_ship_puka_func);
static USER_FUNC(tornade_alpha_func);
USER_FUNC(muj_npc_buruburu);
USER_FUNC(muj_npc_fuwafuwa);
USER_FUNC(muj_npc_guruguru);
void _unresolved(void);
void _epilog(void);
void _prolog(void);

f32 muj_ship_puka_data[16];
EvtWord muj_ship_puka[21];
EvtWord korutesu_apper[234];
EvtWord korutesu_vanish[227];
EvtWord muj_mario_pos_osreport[4];

f32 muj_ship_puka_data[] = {
    -0.0f, -0.5f, -1.0f, -1.5f, -2.0f, -2.2f, -2.5f, -2.7f,
    -2.7f, -2.7f, -2.5f, -2.2f, -2.0f, -1.5f, -1.0f, -0.5f,
};

EVT_BEGIN(muj_ship_puka)
    SET_VALUE(LW(10), 0)
    LOOP(0)
        CALL(muj_ship_puka_func, LW(10), LW(1), LW(11))
        SET_FLOAT_INDEXED(LW(0), LW(11))
        ADD(LW(10), 1)
        WAIT_MS(130)
    END_LOOP
    RETURN
EVT_END()

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("muj_00", muj_00_init_evt);
    relSetEvtAddr("muj_01", muj_01_init_evt);
    relSetEvtAddr("muj_02", muj_02_init_evt);
    relSetEvtAddr("muj_03", muj_03_init_evt);
    relSetEvtAddr("muj_04", muj_04_init_evt);
    relSetEvtAddr("muj_05", muj_05_init_evt);
    relSetEvtAddr("muj_10", muj_10_init_evt);
    relSetEvtAddr("muj_11", muj_11_init_evt);
    relSetEvtAddr("muj_12", muj_12_init_evt);
    relSetEvtAddr("muj_20", muj_20_init_evt);
    relSetEvtAddr("muj_21", muj_21_init_evt);
    relSetBtlAddr("muj", btlsetup_muj_tbl, btl_muj_setup_no_tbl);
}

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}

void _unresolved(void) {
}

USER_FUNC(muj_ship_puka_func) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    f32 value = evtGetFloat(event, args[1]);

    evtSetFloat(event, args[2], value * muj_ship_puka_data[index & 0xF]);
    return EVT_RETURN_DONE;
}

USER_FUNC(muj_npc_guruguru) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    NpcEntry* npc = evtNpcNameToPtr(event, name);
    f32 cx = evtGetFloat(event, args[1]);
    f32 cz = evtGetFloat(event, args[2]);
    s32 dir = evtGetValue(event, args[3]);
    f32 speed = evtGetFloat(event, args[4]);
    s32 duration = evtGetValue(event, args[5]);
    s32 elapsed;
    f32 x;
    f32 z;
    f32 y;
    f32 angle;

    if (isFirstCall) {
        event->savedTime = gp->renderTime;
        event->userdataf[1] = angleABf(cx, cz, npc->position.x, npc->position.z);
        event->userdataf[2] = distABf(cx, cz, npc->position.x, npc->position.z);
    }
    elapsed = OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime);
    angle = speed * (s32)(0.5f + sysMsec2FrameFloat(elapsed)) + event->userdataf[1];
    x = cx;
    y = npc->position.y;
    z = cz;
    movePos(&x, &z, event->userdataf[2], angle);
    if (dir == 0) {
        if (npc->position.x > x) {
            npc->direction = 270.0f;
        } else {
            npc->direction = 90.0f;
        }
    }
    npc->prevPosition = (Vec){x, y, z};
    npc->position = npc->prevPosition;
    if (npc->footstepAccum > npc->footstepStride / npc->moveSpeed) {
        if (npc->footstepToggle != 0) {
            if (npc->footstepSfxA != NULL) {
                psndSFXOnEx_3D(npc->footstepSfxA, &npc->position, 255, 255, 0, 8);
            }
        } else if (npc->footstepSfxB != NULL) {
            psndSFXOnEx_3D(npc->footstepSfxB, &npc->position, 255, 255, 0, 8);
        }
        npc->footstepAccum = 0.0f;
        npc->footstepToggle = (s8)(1 - npc->footstepToggle);
    }
    if (npc->footstepSfxB != NULL || npc->footstepToggle != 0) {
        npc->footstepAccum = 1000.0f * npc->moveDt + npc->footstepAccum;
    }
    if (elapsed < duration) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_YIELD;
}

USER_FUNC(muj_npc_fuwafuwa) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    NpcEntry* npc = evtNpcNameToPtr(event, name);
    s32 axis = evtGetValue(event, args[1]);
    s32 duration = evtGetValue(event, args[2]);
    f32 table[32] = {0.0f, 0.5f, 1.0f, 1.5f, 2.0f, 2.2f, 2.5f, 2.7f, 2.7f, 2.7f, 2.5f, 2.2f, 2.0f, 1.5f, 1.0f, 0.5f,
                     -0.0f, -0.5f, -1.0f, -1.5f, -2.0f, -2.2f, -2.5f, -2.7f, -2.7f, -2.7f, -2.5f, -2.2f, -2.0f, -1.5f, -1.0f, -0.5f};
    Vec* base = (Vec*)&event->userdataf[1];
    s32 elapsed;
    s32 frame;
    f32 x;
    f32 y;
    f32 z;

    if (isFirstCall) {
        event->savedTime = gp->renderTime;
        base->x = npc->position.x;
        base->y = npc->position.y;
        base->z = npc->position.z;
    }
    elapsed = OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime);
    frame = sysMsec2FrameFloat(elapsed) + 0.5f;
    x = base->x;
    y = base->y;
    z = base->z;
    switch (axis) {
        case 0:
            y += table[frame & 31];
            break;
    }
    npc->prevPosition = (Vec){x, y, z};
    npc->position = npc->prevPosition;
    if (elapsed < duration) {
        return EVT_RETURN_BLOCK;
    }
    npc->prevPosition = (Vec){base->x, base->y, base->z};
    npc->position = npc->prevPosition;
    return EVT_RETURN_YIELD;
}

USER_FUNC(muj_npc_buruburu) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    NpcEntry* npc = evtNpcNameToPtr(event, name);
    s32 axis = evtGetValue(event, args[1]);
    f32 amplitude = evtGetFloat(event, args[2]);
    s32 period = evtGetValue(event, args[3]);
    s32 duration = evtGetValue(event, args[4]);
    PartyEntry* party = partyGetPtr(1);
    Vec* base = (Vec*)&event->userdataf[1];
    s32 elapsed;
    f32 offset;
    f32 delta;

    if (isFirstCall) {
        event->savedTime = gp->renderTime;
        event->userdataf[0] = 0.0f;
        if (npc == (NpcEntry*)-2) {
            base->x = party->position.x;
            base->y = party->position.y;
            base->z = party->position.z;
        } else {
            base->x = npc->position.x;
            base->y = npc->position.y;
            base->z = npc->position.z;
        }
    }
    elapsed = OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime);
    if (elapsed % (period * 2) >= period) {
        offset = amplitude;
    } else {
        offset = -amplitude;
    }
    delta = offset - event->userdataf[0];
    switch (axis) {
        case 0:
            if (npc == (NpcEntry*)-2) {
                party->position.x += delta;
            } else {
                npc->position.x += delta;
                npc->prevPosition.x = npc->position.x;
            }
            break;
        case 1:
            if (npc == (NpcEntry*)-2) {
                party->position.y += delta;
            } else {
                npc->position.y += delta;
                npc->prevPosition.y = npc->position.y;
            }
            break;
        case 2:
            if (npc == (NpcEntry*)-2) {
                party->position.z += delta;
            } else {
                npc->position.z += delta;
                npc->prevPosition.z = npc->position.z;
            }
            break;
    }
    event->userdataf[0] = offset;
    if (duration == 0) {
        return EVT_RETURN_BLOCK;
    }
    if (elapsed < duration) {
        return EVT_RETURN_BLOCK;
    }
    if (npc == (NpcEntry*)-2) {
        party->position = (Vec){base->x, base->y, base->z};
    } else {
        npc->prevPosition = (Vec){base->x, base->y, base->z};
        npc->position = npc->prevPosition;
    }
    return EVT_RETURN_YIELD;
}

static USER_FUNC(tornade_alpha_func) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 alpha = evtGetFloat(event, args[1]);

    ((EffKamekiTornadeN64Work*)effNameToPtr(name)->userdata)->alphaScale = alpha;
    return EVT_RETURN_DONE;
}

USER_FUNC(muj_mario_pos_osreport_func) {
    marioGetPtr();
    keyGetButtonTrg(0);
    return EVT_RETURN_BLOCK;
}

EVT_BEGIN(korutesu_apper)
    CALL(evt_npc_get_position, STRING("コルテス"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_KOR_APPEAR1"), LW(0), LW(1), LW(2), LW(10))
        WAIT_MS(1000)
        CALL(evt_snd_sfxoff, LW(10))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_KOR_APPEAR2"), LW(0), LW(1), LW(2), LW(10))
        WAIT_MS(2000)
        CALL(evt_snd_sfxoff, LW(10))
    END_INLINE_EVENT
    CALL(evt_eff64, STRING("tornade"), STRING("kameki_tornade_n64"), 0, LW(0), LW(1), LW(2), FLOAT(2.5f), 0, 0, 0, 0, 0, 0, 0)
    CALL(tornade_alpha_func, STRING("tornade"), FLOAT(0.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 0, FLOAT(0.0f), FLOAT(100.0f), 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(tornade_alpha_func, STRING("tornade"), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        CALL(evt_eff_softdelete, STRING("tornade"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(700)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(600)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(500)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(400)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(300)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(200)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(100)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(50)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(30)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(30)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
    END_INLINE_EVENT
    WAIT_MS(3000)
    RETURN
EVT_END()

EVT_BEGIN(korutesu_vanish)
    CALL(evt_npc_get_position, STRING("コルテス"), LW(0), LW(1), LW(2))
    CALL(evt_eff64, STRING("tornade"), STRING("kameki_tornade_n64"), 0, LW(0), LW(1), LW(2), FLOAT(2.5f), 0, 0, 0, 0, 0, 0, 0)
    CALL(tornade_alpha_func, STRING("tornade"), FLOAT(0.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 0, FLOAT(0.0f), FLOAT(100.0f), 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(tornade_alpha_func, STRING("tornade"), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_STG5_KOR_GOOUT1"), LW(10))
        WAIT_MS(3000)
        CALL(evt_snd_sfxoff, LW(10))
        CALL(evt_eff_softdelete, STRING("tornade"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(30)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(30)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(50)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(100)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(200)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(300)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(400)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(500)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(600)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(700)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), 128)
        WAIT_MS(800)
        CALL(evt_npc_flag_onoff, 0, STRING("コルテス"), 128)
        WAIT_FRAMES(1)
    END_INLINE_EVENT
    RETURN
EVT_END()

EVT_BEGIN(muj_mario_pos_osreport)
    CALL(muj_mario_pos_osreport_func)
    RETURN
EVT_END()
