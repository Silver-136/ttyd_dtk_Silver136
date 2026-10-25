#include "evt/evt_cmd.h"
#include "evt/evt_npc.h"
#include "evt/evt_snd.h"
#include "driver/camdrv.h"
#include "driver/hitdrv.h"
#include "driver/npcdrv.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "system.h"
#include <dolphin/os.h>

USER_FUNC(evt_npc_wait_anim);

USER_FUNC(evt_gazigazi_move_position);
USER_FUNC(camdir);

#define GAZIGAZI "\x83\x4B\x83\x57\x83\x4B\x83\x57" // "ガジガジ"

EVT_BEGIN(evt_gazigazi_entry)
    CALL(evt_npc_entry, STRING(GAZIGAZI), STRING("c_gazigazi"))
    CALL(evt_npc_set_position, STRING(GAZIGAZI), 0, -1000, 0)
    CALL(evt_npc_pera_onoff, STRING(GAZIGAZI), 0)
    CALL(evt_npc_set_ry, STRING(GAZIGAZI), FLOAT(0.0f))
    CALL(evt_npc_flag_onoff, 1, STRING(GAZIGAZI), 0x02000000)
    RETURN
EVT_END()

EVT_BEGIN(evt_gazigazi_anim_loop)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_GAJIGAJI1"), LW(0), LW(1), LW(2), 0)
    LOOP(0)
        CALL(evt_npc_set_anim, STRING(GAZIGAZI), STRING("GAZ_A_1"))
        CALL(evt_npc_wait_anim, STRING(GAZIGAZI), FLOAT(1.0f))
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_gazigazi_appear)
    SET_VALUE(LW(10), LW(0))
    SET_VALUE(LW(11), LW(1))
    SET_VALUE(LW(12), LW(2))
    SET_VALUE(LW(13), LW(0))
    SET_VALUE(LW(14), LW(1))
    SET_VALUE(LW(15), LW(2))
    ADD(LW(10), 0)
    ADD(LW(11), -50)
    ADD(LW(12), 0)
    ADD(LW(13), 0)
    ADD(LW(14), -10)
    ADD(LW(15), 0)
    CALL(evt_npc_flag_onoff, 1, STRING(GAZIGAZI), 0x40000020)
    CALL(evt_npc_set_rotate_offset, STRING(GAZIGAZI), 0, 20, 0)
    CALL(camdir)
    CALL(evt_npc_set_rotate, STRING(GAZIGAZI), FLOAT(0.0f), LW(3), FLOAT(270.0f))
    CALL(evt_npc_set_scale, STRING(GAZIGAZI), FLOAT(1.0f), FLOAT(1.0f), LW(4))
    CALL(evt_npc_set_anim, STRING(GAZIGAZI), STRING("GAZ_S_1"))
    SPAWN_EVENT_GET_ID(PTR(evt_gazigazi_anim_loop), LW(9))
    BEGIN_INLINE_EVENT
        CALL(evt_gazigazi_move_position, STRING(GAZIGAZI), LW(10), LW(11), LW(12), LW(13), LW(14), LW(15), 300, 1)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_gazigazi_move_position, STRING(GAZIGAZI), LW(13), LW(14), LW(15), LW(10), LW(11), LW(12), 600, 0)
    DELETE_EVENT(LW(9))
    CALL(evt_npc_set_position, STRING(GAZIGAZI), 0, -1000, 0)
    RETURN
EVT_END()

USER_FUNC(camdir) {
    f32 yaw;
    s32 dir;
    CameraEntry* camera;
    MarioWork* mario;
    Vec* pos;

    camera = camGetPtr(CAMERA_3D);
    yaw = camera->viewYaw;
    mario = marioGetPtr();
    dir = 1;
    if (mario->damageHit) {
        pos = hitGetDamageReturnPos(mario->damageHit);
        if (reviseAngle(yaw - angleABf((f32)event->lwData[0], (f32)event->lwData[2], pos->x, pos->z)) > 180.0f) {
            yaw = reviseAngle(180.0f + yaw);
            dir = -1;
        }
    }
    event->lwData[3] = FLOAT(-yaw);
    event->lwData[4] = ((yaw > 90.0f && yaw < 270.0f) ? -dir : dir) * 1024 - EVTDAT_FLOAT_BASE;
    return EVT_RETURN_DONE;
}

// const char* name, s32 x0, s32 y0, s32 z0, s32 x1, s32 y1, s32 z1, s32 msec, s32 mode
USER_FUNC(evt_gazigazi_move_position) {
    EvtWord* args = event->args;
    const char* name;
    f32 y0, x0, z0, x1, y1, z1;
    s32 msec;
    s32 mode;
    NpcEntry* npc;
    s32 elapsed;

    name = (const char*)evtGetValue(event, args[0]);
    x0 = (f32)evtGetValue(event, args[1]);
    y0 = (f32)evtGetValue(event, args[2]);
    z0 = (f32)evtGetValue(event, args[3]);
    x1 = (f32)evtGetValue(event, args[4]);
    y1 = (f32)evtGetValue(event, args[5]);
    z1 = (f32)evtGetValue(event, args[6]);
    msec = evtGetValue(event, args[7]);
    mode = evtGetValue(event, args[8]);
    npc = evtNpcNameToPtr(event, name);
    if (isFirstCall) {
        event->userdata[0] = (s32)gp->renderTime;
    }
    elapsed = OSTicksToMilliseconds((u32)gp->renderTime - event->userdata[0]);
    if (elapsed < msec) {
        x0 = intplGetValue(mode, (s32)elapsed, msec, x0, x1);
        y0 = intplGetValue(mode, (s32)elapsed, msec, y0, y1);
        z0 = intplGetValue(mode, (s32)elapsed, msec, z0, z1);
        {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = x0;
            pos.y = y0;
            pos.z = z0;
            npc->prevPosition = pos;
            npc->position = npc->prevPosition;
        }
        return EVT_RETURN_BLOCK;
    } else {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = x1;
        pos.y = y1;
        pos.z = z1;
        npc->prevPosition = pos;
        npc->position = npc->prevPosition;
        return EVT_RETURN_DONE;
    }
}
