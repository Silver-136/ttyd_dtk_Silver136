#include "evt/evt_cmd.h"
#include "driver/mobjdrv.h"
#include "driver/animdrv.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mariost.h"
#include "eff/eff_small_star.h"
#include "mario/mario_motion.h"
#include "memory.h"
#include "mario/mario.h"
#include "party.h"
#include <dolphin/mtx.h>
#include "manager/fontmgr.h"
#include <string.h>
#include "evt/evt_cam.h"
#include "evt/evt_img.h"
#include "evt/evt_memcard.h"
#include "mario/mario_party.h"
#include "mario/mario_pouch.h"
#include "eff/eff_stardust.h"
#include "eff/eff_fire.h"
#include "koopa.h"
#include "kpa_act.h"
#include "driver/mapdrv.h"
#include "driver/msgdrv.h"
#include "driver/windowdrv.h"
#include "driver/effdrv.h"
#include "driver/swdrv.h"
#include "driver/itemdrv.h"
#include "statuswindow.h"
#include "evt/evt_mobj.h"
#include "libc_decls.h"
#include "pmario_sound.h"
#include "sfx_name_word.h"
#include "driver/hitdrv.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
s32 mobj_koopa_pole_evt(MapObjectEntry* mobj);
void kpa_disp_pole_score(CameraId camId, void* param);
static USER_FUNC(kpa_get_level);
s32 mobj_koopa_fireber_dodai_evt(MapObjectEntry* mobj);
s32 mobj_koopa_sango_evt(MapObjectEntry* mobj);
s32 mobj_koopa_dokan_evt(MapObjectEntry* mobj);
s32 mobj_koopa_blk(MapObjectEntry* mobj);
s32 mobj_koopa_badgeblk(MapObjectEntry* mobj);
s32 mobj_koopa_brickblk(MapObjectEntry* mobj);
s32 mobj_koopa_stone_blk_evt(MapObjectEntry* mobj);
s32 mobj_koopa_ojama_blk_evt(MapObjectEntry* mobj);
s32 mobj_kururing_floor(MapObjectEntry* mobj);
void kururing_capture(CameraId camId, MapObjectEntry* mobj);
s32 mobj_breaking_floor(MapObjectEntry* mobj);
s32 mobj_10countblk(MapObjectEntry* mobj);
s32 mobj_brickblk(MapObjectEntry* mobj);
s32 mobj_powerupblk(MapObjectEntry* mobj);
s32 mobj_badgeblk(MapObjectEntry* mobj);
s32 mobj_save_blk(MapObjectEntry* mobj);
s32 mobj_recovery_blk(MapObjectEntry* mobj);
s32 mobj_signboard(MapObjectEntry* mobj);
s32 mobj_itembox(MapObjectEntry* mobj);
s32 mobj_lock(MapObjectEntry* mobj);
s32 mobj_switch_float_blk(MapObjectEntry* mobj);
s32 mobj_float_blk(MapObjectEntry* mobj);
s32 mobj_lv_blk_evt(MapObjectEntry* mobj);
s32 mobj_jumpstand_blue(MapObjectEntry* mobj);
s32 mobj_jumpstand_red(MapObjectEntry* mobj);
s32 mobj_rideswitch_lightblue(MapObjectEntry* mobj);
s32 mobj_timerswitch(MapObjectEntry* mobj);
s32 mobj_switch_red(MapObjectEntry* mobj);
s32 mobj_switch_blue(MapObjectEntry* mobj);
USER_FUNC(evt_mobj_koopa_pole);
USER_FUNC(kpa_clear_jump_data);
USER_FUNC(kpa_disp_pole_init);
USER_FUNC(kpa_disp_pole_score_main);
USER_FUNC(kpa_score_to_str);
USER_FUNC(kpa_get_width);
USER_FUNC(kpa_get_hata_name);
USER_FUNC(kpa_get_pole_name);
USER_FUNC(evt_mobj_koopa_fireber_dodai);
USER_FUNC(evt_mobj_koopa_sango);
USER_FUNC(fire_func);
USER_FUNC(evt_mobj_koopa_dokan);
USER_FUNC(evt_mobj_koopa_blk);
USER_FUNC(evt_mobj_koopa_hidden_blk);
USER_FUNC(evt_mobj_koopa_badgeblk);
USER_FUNC(evt_mobj_koopa_brick);
USER_FUNC(evt_mobj_koopa_stone_blk);
USER_FUNC(evt_mobj_koopa_ojama_blk);
USER_FUNC(kpa_powerup);
USER_FUNC(evt_mobj_breaking_floor);
USER_FUNC(evt_mobj_brick);
USER_FUNC(evt_mobj_powerupblk);
USER_FUNC(evt_mobj_badgeblk);
USER_FUNC(evt_mobj_signboard);
USER_FUNC(evt_mobj_lock);
USER_FUNC(evt_mobj_lock_unlock);
USER_FUNC(evt_mobj_switch_float_blk);
USER_FUNC(evt_mobj_float_blk);
USER_FUNC(evt_mobj_lv_blk);
USER_FUNC(evt_mobj_jumpstand_blue);
USER_FUNC(evt_mobj_jumpstand_red);
USER_FUNC(evt_mobj_rideswitch_lightblue);
USER_FUNC(evt_mobj_timerswitch);
USER_FUNC(evt_mobj_floatswitch_red);
USER_FUNC(evt_mobj_switch_red);
USER_FUNC(evt_mobj_tornadoswitch_blue);
USER_FUNC(evt_mobj_floatswitch_blue);
USER_FUNC(evt_mobj_switch_blue);
USER_FUNC(evt_mobj_hit_onoff);
USER_FUNC(evt_mobj_hitevt_onoff);
USER_FUNC(evt_mobj_set_camid);
USER_FUNC(evt_mobj_wait_animation_end);
USER_FUNC(evt_mobj_set_gravity_bound);
USER_FUNC(evt_mobj_exec_cancel);
USER_FUNC(evt_mobj_set_z_position);
USER_FUNC(evt_mobj_get_z_position);
USER_FUNC(evt_mobj_get_kindname);
USER_FUNC(evt_mobj_check);

extern EventEntry* mobjRunEvent(MapObjectEntry* mobj, EvtWord* eventCode);
extern void kpaPowUp(void);

static inline MapObjectEntry* MOBJ_PTR(EventEntry* event, const char* name) {
    return strcmp(name, "me") ? mobjNameToPtr(name) : event->thisMapObj;
}

static inline MapObjectEntry* MOBJ_PTR2(EventEntry* event, const char* name) {
    return !strcmp(name, "me") ? event->thisMapObj : mobjNameToPtr(name);
}









static EVT_BEGIN(evt_shake)
    CALL(evt_cam_shake, 4, 4064967296, 4064967306, 200)
    RETURN
EVT_END()
const char* _switch_anim[] = {"A_1", "A_2", "A_3", "A_4", "A_5", "S_1"};



// FAKE: the hit pointer is tested as a signed word (cmpwi); a pointer or BOOL test compiles to cmplwi
static inline EvtWord blkHitCheck(MapObjectEntry* m) {
    f32 hx, hy, hz, dist, nx, ny, nz;
    dist = 1.0f + (f32)__fabs(m->velocity.y);
    return (EvtWord)hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, m->position.x, m->position.y + 0.5f, m->position.z, 0.0f, -1.0f, 0.0f);
}

static inline BOOL isAnimA13(const s8* a) {
    if (a[0] == 'A' && a[2] >= '1' && a[2] <= '3') {
        return TRUE;
    }
    return FALSE;
}

extern void* itemEntry(f32 x, f32 y, f32 z, const char* name, s32 id, s32 mode, s32 collect, void* script);

extern void npcKoopaModeMobjBoundDeadCheck(void* hit);
extern void effMObjBrokenEntry(s32 type, f32 x, f32 y, f32 z);


s32 kpa_powerup(EventEntry* event, BOOL isFirstCall);
s32 fire_func(EventEntry* event, BOOL isFirstCall);
EVT_BEGIN(kpa_powerup_evt)
    CALL(kpa_powerup, 4264967296, 4264967297, 4264967298)
    RETURN
EVT_END()
EVT_BEGIN(evt_fire)
    CALL(fire_func, 4264967296, 4264967297, 4264967298, 4264967299)
    RETURN
EVT_END()

#include "driver/imgdrv.h"
#include "driver/offscreendrv.h"
#include "driver/camdrv.h"
#include "system.h"
extern void test_kururing_mapdisp(s32 camId);
#include "driver/npcdrv.h"

USER_FUNC(evt_cam_get_pos);
USER_FUNC(evt_cam_letter_box_disable);
USER_FUNC(evt_mario_cam_y_on);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_set_mov_spd);
USER_FUNC(evt_sub_get_mapname);
USER_FUNC(kpa_chg_pose);
USER_FUNC(kpa_clear_jump_data);
USER_FUNC(kpa_disp_pole_init);
USER_FUNC(kpa_disp_pole_score_main);
USER_FUNC(kpa_get_hata_name);
USER_FUNC(kpa_get_pole_name);
USER_FUNC(kpa_get_width);
USER_FUNC(kpa_powerup);
USER_FUNC(kpa_score_to_str);

// ---- BEGIN tail (defined in reverse of target order) ----

USER_FUNC(evt_mobj_entry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    const char* anim = (const char*)evtGetValue(event, args[1]);
    if (animGroupBaseAsync(anim, gp->inBattle != 0, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_delete) {
    mobjDelete((const char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_check) {
    EvtWord* args = event->args;
    MapObjectEntry* mobj = mobjNameToPtrNoAssert((const char*)evtGetValue(event, args[0]));
    evtSetValue(event, args[1], (EvtWord)mobj);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_flag_onoff) {
    EvtWord* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    s32 mask = args[2];
    MapObjectEntry* mobj = mobjNameToPtr(name);
    if (on) {
        mobj->flags |= mask;
    } else {
        mobj->flags &= ~mask;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_get_kindname) {
    EvtWord* args = event->args;
    MapObjectEntry* mobj = mobjNameToPtr((const char*)evtGetValue(event, args[0]));
    evtSetValue(event, args[1], (EvtWord)mobj->animName);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_set_scale) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    MapObjectEntry* mobj;
    f32 x, y, z;
    Vec scale;
    mobj = MOBJ_PTR2(event, name);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    {
    Vec s2 = {0.0f, 0.0f, 0.0f};
    scale = s2;
    }
    scale.x = x;
    scale.y = y;
    scale.z = z;
    mobj->scale = scale;
    mobj->flags |= 4;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_get_position) {
    EvtWord* args = event->args;
    MapObjectEntry* mobj = MOBJ_PTR(event, (const char*)evtGetValue(event, args[0]));
    evtSetFloat(event, args[1], mobj->position.x);
    evtSetFloat(event, args[2], mobj->position.y);
    evtSetFloat(event, args[3], mobj->position.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_get_x_position) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    MapObjectEntry* mobj = MOBJ_PTR(event, name);
    evtSetFloat(event, args[1], mobj->position.x);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_get_y_position) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    MapObjectEntry* mobj = MOBJ_PTR(event, name);
    evtSetFloat(event, args[1], mobj->position.y);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_get_z_position) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    MapObjectEntry* mobj = MOBJ_PTR(event, name);
    evtSetFloat(event, args[1], mobj->position.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_set_position) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    MapObjectEntry* mobj = MOBJ_PTR(event, name);
    Vec pos = {0.0f, 0.0f, 0.0f};
    pos.x = x;
    pos.y = y;
    pos.z = z;
    mobj->position = pos;
    mobj->flags |= 4;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_set_x_position) {
    EvtWord* args = event->args;
    MapObjectEntry* mobj = MOBJ_PTR(event, (const char*)evtGetValue(event, args[0]));
    Vec pos = mobj->position;
    pos.x = evtGetFloat(event, args[1]);
    mobj->position = pos;
    mobj->flags |= 4;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_set_y_position) {
    EvtWord* args = event->args;
    MapObjectEntry* mobj = MOBJ_PTR(event, (const char*)evtGetValue(event, args[0]));
    Vec pos = mobj->position;
    pos.y = evtGetFloat(event, args[1]);
    mobj->position = pos;
    mobj->flags |= 4;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_set_z_position) {
    EvtWord* args = event->args;
    MapObjectEntry* mobj = MOBJ_PTR(event, (const char*)evtGetValue(event, args[0]));
    Vec pos = mobj->position;
    pos.z = evtGetFloat(event, args[1]);
    mobj->position = pos;
    mobj->flags |= 4;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_exec_cancel) {
    MapObjectEntry* mobj = MOBJ_PTR(event, (const char*)evtGetValue(event, event->args[0]));
    mobj->flags |= 0x1000;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_set_gravity_bound) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 a = evtGetFloat(event, args[1]);
    f32 b = evtGetFloat(event, args[2]);
    MapObjectEntry* mobj = MOBJ_PTR(event, name);
    mobj->gravity = a;
    mobj->bounce = b;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_set_anim) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    const char* anim = (const char*)evtGetValue(event, args[1]);
    MapObjectEntry* mobj = MOBJ_PTR(event, name);
    animPoseSetAnim(mobj->poseId, anim, 1);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_wait_animation_end) {
    const char* name = (const char*)evtGetValue(event, event->args[0]);
    MapObjectEntry* mobj = MOBJ_PTR(event, name);
    if (animPoseGetLoopTimes(mobj->poseId) > 1.0f) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_mobj_set_camid) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    u8 cam = evtGetValue(event, args[1]);
    MapObjectEntry* mobj = MOBJ_PTR(event, name);
    mobj->camId = cam;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_hitevt_onoff) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);
    MapObjectEntry* mobj = MOBJ_PTR(event, name);
    if (on) {
        mobj->flags &= ~0x10;
    } else {
        mobj->flags |= 0x10;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_hit_onoff) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);
    MapObjectEntry* mobj = mobjNameToPtr(name);
    s32 i;
    if (on) {
        for (i = 0; i < 2; i++) {
            if (mobj->hitObj[i].name) {
                hitGrpFlagOff(mobj->hitObj[i].name, 1);
            }
        }
    } else {
        for (i = 0; i < 2; i++) {
            if (mobj->hitObj[i].name) {
                hitGrpFlagOn(mobj->hitObj[i].name, 1);
            }
        }
    }
    return EVT_RETURN_DONE;
}

// ---- switches ----

s32 mobj_switch_blue(MapObjectEntry* mobj) {
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            mobjRunEvent(mobj, (s32*)mobj->event);
            if (mobj->hitFlag == 0x200000 || mobj->hitFlag == 0x1000000) {
                marioChgMot(MARIO_MOTION_JUMP_SW);
            }
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BIKKURI_SWITCH_ON1), &mobj->position);
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            mobj->state++;
        }
        break;
    case 1:
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        effSmallStarCamChg(effSmallStarEntry(0, 4, mobj->position.x, mobj->position.y + 12.5f, mobj->position.z, 0.0f, -1.0f, 0.0f), mobj->camId);
        evtSetValue(NULL, mobj->flagVar, 1);
        mobj->state = 0x63;
    case 0x63:
        mobjDelete(mobj->name);
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_switch_blue) {
    s32 flagVar;
    s32 val;
    EvtWord* args = event->args;
    s32 kind;
    s32 inBattle;
    const char* anim;
    const char* name;
    MapObjectEntry* mobj;
    f32 x, y, z;
    kind = evtGetValue(event, args[0]);
    name = (const char*)evtGetValue(event, args[1]);
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    val = evtGetValue(event, args[5]);
    flagVar = args[6];
    inBattle = gp->inBattle != 0;
    switch (kind) {
    case 0:
        anim = "MOBJ_BlueSwitch";
        break;
    case 1:
        anim = "MOBJ_BlackSwitch";
        break;
    case 2:
    default:
        anim = "HOBJ_BlueSwitch";
        break;
    }
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_switch_blue;
    mobj->event = val;
    mobj->flags |= 0x44;
    mobj->flags |= 0xa10000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (kind == 2) {
        mobj->flags |= 2;
    }
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_floatswitch_blue) {
    EvtWord* args = event->args;
    s32 flagVar;
    f32 x, y, z;
    const char* name;
    MapObjectEntry* mobj;
    s32 val;
    const char* anim;
    s32 inBattle;
    evtGetValue(event, args[0]);
    name = (const char*)evtGetValue(event, args[1]);
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    val = evtGetValue(event, args[5]);
    flagVar = args[6];
    inBattle = gp->inBattle;
    inBattle = inBattle != 0;
    anim = "MOBJ_BlueSwitchBlock";
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_switch_blue;
    mobj->event = val;
    mobj->flags |= 4;
    mobj->flags |= 0x400000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_tornadoswitch_blue) {
    EvtWord* args = event->args;
    s32 flagVar;
    f32 x, y, z;
    const char* name;
    MapObjectEntry* mobj;
    s32 val;
    const char* anim;
    s32 inBattle;
    evtGetValue(event, args[0]);
    name = (const char*)evtGetValue(event, args[1]);
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    val = evtGetValue(event, args[5]);
    flagVar = args[6];
    inBattle = gp->inBattle;
    inBattle = inBattle != 0;
    anim = "MOBJ_BlueTornadoSwitch";
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_switch_blue;
    mobj->event = val;
    mobj->flags |= 4;
    mobj->flags |= 0x1000000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

s32 mobj_switch_red(MapObjectEntry* mobj) {
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            mobjRunEvent(mobj, (s32*)mobj->event);
            if (mobj->hitFlag == 0x200000 || mobj->hitFlag == 0x1000000) {
                marioChgMot(MARIO_MOTION_JUMP_SW);
            }
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BIKKURI_SWITCH_ON1), &mobj->position);
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            mobj->flags &= ~8;
        }
        break;
    case 1:
        if (mobj->flags & 8) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BIKKURI_SWITCH_ON1), &mobj->position);
            if (mobj->hitFlag == 0x200000) {
                marioChgMot(MARIO_MOTION_JUMP_SW);
            }
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            mobj->flags &= ~8;
        }
        if (!(animPoseGetLoopTimes(mobj->poseId) < 1.0f)) {
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            mobj->flags &= ~8;
            mobj->state = 0;
        }
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_switch_red) {
    s32 flagVar;
    s32 val;
    EvtWord* args = event->args;
    s32 kind;
    s32 inBattle;
    const char* anim;
    const char* name;
    MapObjectEntry* mobj;
    f32 x, y, z;
    kind = evtGetValue(event, args[0]);
    name = (const char*)evtGetValue(event, args[1]);
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    val = evtGetValue(event, args[5]);
    flagVar = args[6];
    inBattle = gp->inBattle != 0;
    switch (kind) {
    case 0:
        anim = "MOBJ_RedSwitch";
        break;
    case 1:
    default:
        anim = "MOBJ_WhiteSwitch";
        break;
    }
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_switch_red;
    mobj->event = val;
    mobj->flags |= 0x44;
    mobj->flags |= 0xa10000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_floatswitch_red) {
    EvtWord* args = event->args;
    s32 flagVar;
    f32 x, y, z;
    const char* name;
    MapObjectEntry* mobj;
    s32 val;
    const char* anim;
    s32 inBattle;
    evtGetValue(event, args[0]);
    name = (const char*)evtGetValue(event, args[1]);
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    val = evtGetValue(event, args[5]);
    flagVar = args[6];
    inBattle = gp->inBattle;
    inBattle = inBattle != 0;
    anim = "MOBJ_RedSwitchBlock";
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_switch_red;
    mobj->event = val;
    mobj->flags |= 4;
    mobj->flags |= 0x400000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

s32 mobj_timerswitch(MapObjectEntry* mobj) {
    MapObjectEntry* other = mobjNameToPtr(mobj->linkedName);
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            if (mobj->hitFlag == 0x200000) {
                marioChgMot(MARIO_MOTION_JUMP_SW);
            }
            if (other->state != 2) {
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_TIMER_SWITCH_HIT1), &mobj->position);
            } else {
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_TIMER_SWITCH_LIGHT1), &mobj->position);
            }
            evtEntryType(evt_shake, 0, 0, 0);
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            mobj->state++;
            mobj->flags &= ~8;
        }
        break;
    case 1:
        if (mobj->flags & 8) {
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            animPoseSetAnim(other->poseId, "S_1", 1);
            mobj->state = 0;
            break;
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 0.3f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "A_2", 1);
        animPoseSetAnim(other->poseId, "A_2", 1);
        mobj->state++;
        mobj->timer = mobj->typeWork;
        mobj->flags &= ~8;
        break;
    case 2:
        if (mobj->flags & 8) {
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            animPoseSetAnim(other->poseId, "S_1", 1);
            mobj->state = 0;
            break;
        }
        mobj->timer--;
        if (mobj->timer <= 0) {
            mobj->flags &= ~8;
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            animPoseSetAnim(other->poseId, "S_1", 1);
            mobj->state = 0;
            break;
        }
        if (other->state == 2) {
            if (marioKeyOffChk() || marioCtrlOffChk()) {
                mobj->state = 0;
                break;
            }
            mobjRunEvent(mobj, (s32*)mobj->event);
            if (!strcmp(mobj->animName, "MOBJ_RedTimerSwitch")) {
                mobj->flags &= ~8;
                other->flags &= ~8;
                animPoseSetAnim(mobj->poseId, "S_1", 1);
                animPoseSetAnim(other->poseId, "S_1", 1);
                mobj->state = 0;
                other->state = 0;
                break;
            }
            effSmallStarEntry(0, 8, mobj->position.x, mobj->position.y + 12.5f, mobj->position.z, 0.0f, -1.0f, 0.0f);
            effSmallStarEntry(0, 8, other->position.x, other->position.y + 12.5f, other->position.z, 0.0f, -1.0f, 0.0f);
            evtSetValue(NULL, mobj->flagVar, 1);
            evtSetValue(NULL, other->flagVar, 1);
            mobjDelete(mobj->name);
            mobjDelete(other->name);
        }
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_timerswitch) {
    const char* names[2];
    s32 arg;
    s32 i;
    f32 x[2];
    f32 y[2];
    f32 z[2];
    MapObjectEntry* mobjs[2];
    EvtWord* args = event->args;
    s32 kind = evtGetValue(event, args[0]);
    s32 inBattle = gp->inBattle != 0;
    const char* anims[2] = {"MOBJ_BlueTimerSwitch", "MOBJ_RedTimerSwitch"};
    const char* anim = anims[kind];
    s32 time;
    s32 val;
    names[0] = (const char*)evtGetValue(event, args[1]);
    x[0] = evtGetFloat(event, args[2]);
    y[0] = evtGetFloat(event, args[3]);
    z[0] = evtGetFloat(event, args[4]);
    names[1] = (const char*)evtGetValue(event, args[5]);
    x[1] = evtGetFloat(event, args[6]);
    y[1] = evtGetFloat(event, args[7]);
    z[1] = evtGetFloat(event, args[8]);
    time = evtGetValue(event, args[9]);
    val = evtGetValue(event, args[10]);
    arg = args[11];
    if (kind == 0 && evtGetValue(event, arg)) {
        return EVT_RETURN_DONE;
    }
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    for (i = 0; i < 2; i++) {
        mobjEntry(names[i], anim);
        mobjs[i] = mobjNameToPtr(names[i]);
        strcpy(mobjs[i]->linkedName, i == 0 ? names[1] : names[0]);
        mobjSetPosition(names[i], x[i], y[i], z[i]);
        mobjs[i]->mainFunc = mobj_timerswitch;
        mobjs[i]->event = val;
        mobjs[i]->flags |= 0x44;
        mobjs[i]->flags |= 0xa10000;
        mobjs[i]->flagVar = arg;
        mobjs[i]->state = 0;
        mobjs[i]->typeWork = time;
        if (evtGetValue(event, arg)) {
            mobjs[i]->state = 99;
        }
        mobjCalcMtx(mobjs[i]);
    }
    return EVT_RETURN_DONE;
}

s32 mobj_rideswitch_lightblue(MapObjectEntry* mobj) {
    f32 x, y, z;
    AnimationPoseData* data;
    f32 mag;
    Vec size;
    {
        Vec d1 = (Vec){x, y, z};
        Vec d2 = (Vec){x, y, z};
        Vec d3 = (Vec){x, y, z};
        Vec d4 = (Vec){x, y, z};
    }
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            mobjRunEvent(mobj, (s32*)mobj->event);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_RIDE_SWITCH_ON1), &mobj->position);
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            mobj->state = 1;
        }
        break;
    case 1:
        y = mobj->scale2.y - 0.9f;
        x = mobj->scale2.x;
        z = mobj->scale2.z;
        if (y <= 0.2f) {
            mobj->state = 2;
            y = 0.2f;
            mobj->flags &= ~8;
        }
        mobj->scale2 = (Vec){x, y, z};
        mobjCalcMtx2(mobj);
        break;
    case 2:
        data = animPoseGetAnimBaseDataPtr(mobj->poseId);
        size = (Vec){0.8f * (10.0f * data->bboxMax.x), 10.0f * data->bboxMax.y, 0.8f * (10.0f * data->bboxMax.z)};
        mag = PSVECMag(&size);
        if (PSVECDistance(&mobj->position, &marioGetPtr()->position) > mag) {
            if (!(mobj->flags & 8)) {
                mobj->state = 3;
                animPoseSetAnim(mobj->poseId, "A_2", 1);
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_RIDE_SWITCH_ON2), &mobj->position);
                mobjRunEvent(mobj, (s32*)mobj->extraEvent);
            } else {
                mobj->flags &= ~8;
            }
        }
        break;
    case 3:
        x = mobj->scale2.x;
        y = mobj->scale2.y;
        z = mobj->scale2.z;
        y += 0.9f;
        if (y >= 15.0f) {
            mobj->state = 0;
            y = 15.0f;
        }
        mobj->scale2 = (Vec){x, y, z};
        mobjCalcMtx2(mobj);
        mobj->flags &= ~8;
        break;
    case 99:
        break;
    }
    return 0;
}

void evt_mobj_rideswitch(void) {
    mobjNameToPtr("MOBJ_RideSwitchB");
    mobjNameToPtr("MOBJ_RideSwitchA");
    mobjNameToPtr("MOBJ_RedTornadoSwitch");
}

USER_FUNC(evt_mobj_rideswitch_lightblue) {
    s32 val2;
    s32 val;
    EvtWord* args = event->args;
    const char* name;
    f32 x, y, z;
    s32 flagVar;
    MapObjectEntry* mobj;
    const char* anim;
    s32 inBattle;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    val = evtGetValue(event, args[4]);
    val2 = evtGetValue(event, args[5]);
    flagVar = args[6];
    if (animGroupBaseAsync(anim = "MOBJ_RideSwitchC", gp->inBattle ? 1 : 0, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobj->scale2.x *= 0.8f;
    mobj->scale2.z *= 0.8f;
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_rideswitch_lightblue;
    mobj->event = val;
    mobj->extraEvent = val2;
    mobj->flags |= 4;
    mobj->flags |= 0x200000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx2(mobj);
    return EVT_RETURN_DONE;
}

s32 mobj_jumpstand_red(MapObjectEntry* mobj) {
    MarioWork* mario = marioGetPtr();
    EventEntry* ev;
    partyGetPtr(marioGetPartyId());
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            gp->systemLevelFlags |= 0x10;
            ev = mobjRunEvent(mobj, (s32*)mobj->event);
            if (ev) {
                ev->lwData[0] = mario->fallStartPos.y < mario->position.y ? 0 : (mario->fallStartPos.y < mobj->position.y + mario->wPlayerCollisionBox.y ? 0 : 1);
            }
            if ((s32)(u8)mario->characterId != CHARACTER_BOWSER) {
                if (!strcmp(mobj->animName, "MOBJ_RedJumpStand")) {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_JUMP_STAND1), &mobj->position);
                } else {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_JUMP_STAND2), &mobj->position);
                }
                switch (irand(4)) {
                case 0:
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_AC2_1), &mobj->position);
                    break;
                case 1:
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_AC2_2), &mobj->position);
                    break;
                case 2:
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_AC2_3), &mobj->position);
                    break;
                case 3:
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_AC2_4), &mobj->position);
                    break;
                case 4:
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_AC2_5), &mobj->position);
                    break;
                }
            } else if (!strcmp(mobj->animName, "MOBJ_RedJumpStand")) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_MOBJ_JUMP_STAND1));
                do { } while (0);
            } else {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_MOBJ_JUMP_STAND2));
            }
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            mobj->flags &= ~8;
            mobj->state++;
        }
        break;
    case 1:
        if ((mobj->flags & 8) && evtCheckID(mobj->eventId) == 0) {
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            mobj->state = 0;
            break;
        }
        if (evtCheckID(mobj->eventId) == 1) {
            break;
        }
        gp->systemLevelFlags &= ~0x10;
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        mobj->state = 0;
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_jumpstand_red) {
    MapObjectEntry* mobj;
    EvtWord* args = event->args;
    s32 kind = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    f32 x = evtGetFloat(event, args[2]);
    f32 y = evtGetFloat(event, args[3]);
    f32 z = evtGetFloat(event, args[4]);
    s32 val = evtGetValue(event, args[5]);
    s32 flagVar = args[6];
    s32 inBattle = gp->inBattle ? 1 : 0;
    const char* anims[2] = {"MOBJ_RedJumpStand", "MOBJ_RedJumpStand2"};
    if (animGroupBaseAsync(anims[kind], inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anims[kind]);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_jumpstand_red;
    mobj->event = val;
    mobj->flags |= 0x44;
    mobj->flags |= 0x200000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

s32 mobj_jumpstand_blue(MapObjectEntry* mobj) {
    MarioWork* mario = marioGetPtr();
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            gp->systemLevelFlags |= 0x10;
            mobjRunEvent(mobj, (s32*)mobj->event);
            if (mobj->hitFlag == 0x200000) {
                marioChgJumpStandMotion(mobj->typeDataF[3]);
            }
            if (mario->characterId != CHARACTER_BOWSER) {
                if (!strcmp(mobj->animName, "MOBJ_BlueJumpStand")) {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_JUMP_STAND1), &mobj->position);
                } else {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_JUMP_STAND2), &mobj->position);
                }
                switch (irand(4)) {
                case 0:
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_AC2_1), &mobj->position);
                    break;
                case 1:
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_AC2_2), &mobj->position);
                    break;
                case 2:
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_AC2_3), &mobj->position);
                    break;
                case 3:
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_AC2_4), &mobj->position);
                    break;
                case 4:
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_MARIO_AC2_5), &mobj->position);
                    break;
                }
            } else if (!strcmp(mobj->animName, "MOBJ_BlueJumpStand")) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_MOBJ_JUMP_STAND1));
            } else {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_MOBJ_JUMP_STAND2));
            }
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            mobj->flags &= ~8;
            mobj->state++;
        }
        break;
    case 1:
        if (mobj->flags & 8) {
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            mobj->state = 0;
            break;
        }
        if (evtCheckID(mobj->eventId) == 1) {
            break;
        }
        gp->systemLevelFlags &= ~0x10;
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        mobj->state = 0;
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_jumpstand_blue) {
    MapObjectEntry* mobj;
    EvtWord* args = event->args;
    s32 kind = evtGetValue(event, args[0]);
    const char* name = (const char*)evtGetValue(event, args[1]);
    f32 power = evtGetFloat(event, args[2]);
    f32 x = evtGetFloat(event, args[3]);
    f32 y = evtGetFloat(event, args[4]);
    f32 z = evtGetFloat(event, args[5]);
    s32 val = evtGetValue(event, args[6]);
    s32 inBattle = gp->inBattle ? 1 : 0;
    const char* anims[2] = {"MOBJ_BlueJumpStand", "MOBJ_BlueJumpStand2"};
    s32 flagVar = args[7];
    if (animGroupBaseAsync(anims[kind], inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anims[kind]);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_jumpstand_blue;
    mobj->event = val;
    mobj->flags |= 0x44;
    mobj->flags |= 0x200000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobj->typeDataF[3] = power;
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

s32 mobj_lv_blk_evt(MapObjectEntry* mobj) {
    char oname[16] = "o_";
    char iname[16] = "i_";
    EventEntry ev1;
    EventEntry ev2;
    EvtWord args1[10];
    EvtWord args2[10];
    ImageEntry* img;
    s32 type = gp->inBattle != 0;
    MarioWork* mario = marioGetPtr();
    strcat(oname, mobj->name);
    strcat(iname, mobj->name);
    switch (mobj->state) {
    case 0:
        animGroupBaseAsync("P_box_y", type, NULL);
        if (mobj->flags & 8) {
            if (!strcmp(mobj->animName, "MOBJ_Lv1Block") || !strcmp(mobj->animName, "MOBJ_Lv1BigBlock")) {
            } else if (!strcmp(mobj->animName, "MOBJ_Lv1BigBigBlock")) {
                if (pouchGetHammerLv() < 2 || (u16)mario->currMotionId != MARIO_MOTION_HAMMER_2) {
                    mobj->flags &= ~8;
                    break;
                }
            } else if (!strcmp(mobj->animName, "MOBJ_Lv2Block") || !strcmp(mobj->animName, "MOBJ_Lv2BigBlock")) {
                if (pouchGetHammerLv() < 3) {
                    mobj->flags &= ~8;
                    break;
                }
            } else if (!strcmp(mobj->animName, "MOBJ_Lv2BigBigBlock")) {
                if (pouchGetHammerLv() < 3 || (u16)mario->currMotionId != MARIO_MOTION_HAMMER_2) {
                    mobj->flags &= ~8;
                    break;
                }
            } else {
                mobj->flags &= ~8;
                break;
            }
            mobjRunEvent(mobj, (s32*)mobj->event);
            evtSetValue(NULL, mobj->flagVar, 1);
            mobj->state = 10;
        }
        break;
    case 10:
        animPaperPoseEntry("P_box_y", type);
        offscreenEntry(oname);
        imgEntry(iname, type);
        mobj->offscreenId = offscreenNameToId(oname);
        ev1.args = args1;
        args1[0] = (EvtWord)iname; args1[1] = 0; args1[2] = (EvtWord)oname; args1[3] = 1;
        args1[4] = 0; args1[5] = 0; args1[6] = 0; args1[7] = 0;
        evt_img_alloc_capture(&ev1, TRUE);
        ev2.args = args2;
        args2[0] = (EvtWord)iname; args2[1] = 1; args2[2] = (EvtWord)oname; args2[3] = 1;
        args2[4] = 0; args2[5] = 0; args2[6] = 0; args2[7] = 0;
        evt_img_alloc_capture(&ev2, TRUE);
        img = imgNameToPtr(iname, type);
        img->poseId = animPaperPoseGetId("P_box_y", type);
        img->animName = "A_1";
        img->animTime = animTimeGetTime(type);
        imgSetShadow(img, 2);
        if (!strcmp(mobj->animName, "MOBJ_Lv1Block") || !strcmp(mobj->animName, "MOBJ_Lv2Block")) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_OJAMA_SMALL1), &mobj->position);
        } else if (!strcmp(mobj->animName, "MOBJ_Lv1BigBlock") || !strcmp(mobj->animName, "MOBJ_Lv2BigBlock")) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_OJAMA_MID1), &mobj->position);
        } else if (!strcmp(mobj->animName, "MOBJ_Lv1BigBigBlock") || !strcmp(mobj->animName, "MOBJ_Lv2BigBigBlock")) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_OJAMA_BIG1), &mobj->position);
        }
        mobj->state = 11;
        break;
    case 11:
        img = imgNameToPtr(iname, type);
        if (img->loopTimes >= 1.0f) {
            mobj->state = 12;
        }
        if (img->flags & 0x10000000) {
            mobj->state = 12;
        }
        mobj->position = (Vec){0.0f, -2000.0f, 0.0f};
        mobj->flags |= 4;
        mobj->flags |= 8;
        break;
    case 12:
        img = imgNameToPtr(iname, type);
        imgFreeCapture(img, 0);
        imgFreeCapture(img, 1);
        imgSetShadow(img, 0);
        imgRelease(img, type);
        offscreenDelete(oname);
        mobj->offscreenId = -1;
        mobj->state = 99;
        break;
    case 99:
        mobjDelete(mobj->name);
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_lv_blk) {
    s32 val;
    s32 flagVar;
    EvtWord* args = event->args;
    const char* anim;
    const char* name;
    s32 inBattle;
    MapObjectEntry* mobj;
    f32 x, y, z;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    val = evtGetValue(event, args[4]);
    flagVar = args[5];
    anim = (const char*)evtGetValue(event, args[6]);
    inBattle = gp->inBattle != 0;
    if (evtGetValue(event, flagVar)) {
        return EVT_RETURN_DONE;
    }
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    if (animGroupBaseAsync("P_box_y", inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobj->flags |= 0x44;
    mobj->position = (Vec){x, y, z};
    mobj->mainFunc = mobj_lv_blk_evt;
    mobj->event = val;
    mobj->extraEvent = 0;
    mobj->flags |= 0x10000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    return EVT_RETURN_DONE;
}

s32 mobj_float_blk(MapObjectEntry* mobj) {
    MapObjectEntry* other;
    const char* cur;
    switch (mobj->state) {
    case 99:
        if ((s8)mobj->linkedName[0] == 0) {
            if (blkHitCheck(mobj)) {
                if (strcmp((const char*)animPoseGetCurrentAnim(mobj->poseId), "S_1")) {
                    animPoseSetAnim(mobj->poseId, "S_1", 1);
                    mobj->flags |= 2;
                }
            } else if (strcmp((const char*)animPoseGetCurrentAnim(mobj->poseId), "A_1")) {
                animPoseSetAnim(mobj->poseId, "A_1", 1);
                mobj->flags &= ~2;
            }
        } else {
            other = mobjNameToPtr(mobj->linkedName);
            cur = (const char*)animPoseGetCurrentAnim(other->poseId);
            if ((s8)cur[0] == 'A' && (s8)cur[2] >= '1' && (s8)cur[2] <= '3') {
                animPoseSetAnim(mobj->poseId, "A_1", 0);
            } else {
                animPoseSetAnim(mobj->poseId, "S_1", 1);
            }
        }
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_float_blk) {
    s32 flagVar;
    EvtWord* args = event->args;
    const char* name;
    s32 color;
    s32 size;
    const char* model;
    MapObjectEntry* mobj;
    s32 inBattle;
    f32 x, y, z;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    color = args[4];
    size = args[5];
    evtGetValue(event, args[6]);
    flagVar = args[7];
    inBattle = gp->inBattle != 0;
    switch (color) {
    case 0:
        if (size == 0) {
            model = "MOBJ_green_FlowBlock";
        } else if (size == 1) {
            model = "MOBJ_green_big_FlowBlock";
        } else {
            model = "MOBJ_green_big02_FlowBlock";
        }
        break;
    case 1:
        if (size == 0) {
            model = "MOBJ_purple_FlowBlock";
        } else if (size == 1) {
            model = "MOBJ_purple_big_FlowBlock";
        } else {
            model = "MOBJ_purple_big02_FlowBlock";
        }
        break;
    case 2:
        if (size == 0) {
            model = "MOBJ_orange_FlowBlock";
        } else if (size == 1) {
            model = "MOBJ_orange_big_FlowBlock";
        } else {
            model = "MOBJ_orange_big02_FlowBlock";
        }
        break;
    case 3:
        model = "MOBJ_red_big02_FlowBlock";
        break;
    default:
        if (size == 0) {
            model = "MOBJ_green_FlowBlock";
        } else if (size == 1) {
            model = "MOBJ_green_big_FlowBlock";
        } else {
            model = "MOBJ_green_big02_FlowBlock";
        }
        break;
    }
    if (animGroupBaseAsync(model, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, model);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_float_blk;
    mobj->flags |= 4;
    mobj->flagVar = flagVar;
    mobj->state = 99;
    mobjCalcMtx(mobj);
    if (blkHitCheck(mobj)) {
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        mobj->flags |= 2;
    } else {
        animPoseSetAnim(mobj->poseId, "A_1", 1);
    }
    return EVT_RETURN_DONE;
}

static inline s32 switchAnimIdx(s32 poseId) {
    const char** p;
    const s8* cur;
    s32 idx;
    s32 i;
    cur = (const s8*)animPoseGetCurrentAnim(poseId);
    idx = 0;
    p = (const char**)_switch_anim;
    for (i = idx; i < 6; i++, p++) {
        if (!strcmp(*p, (const char*)cur)) {
            idx = i;
            break;
        }
    }
    return idx;
}

s32 mobj_switch_float_blk(MapObjectEntry* mobj) {
    s32 i;
    s32 idx;
    const s8* cur;
    const char** p;
    s32 anim;
    BOOL a;
    switch (mobj->state) {
    case 0:
        if (!(mobj->flags & 8)) {
            break;
        }
        mobj->typeData[1] = (mobj->typeData[1] & 0x40000000) | switchAnimIdx(mobj->poseId);
        if ((s32)(s8)mobj->linkedName[0]) {
            if (blkHitCheck(mobjNameToPtr(mobj->linkedName))) {
                if (evtCheckID(mobj->eventId)) {
                    if (isAnimA13((const s8*)animPoseGetCurrentAnim(mobj->poseId))) {
                        anim = 0;
                    } else {
                        anim = 3;
                    }
                } else {
                    mobj->typeDataU[1] |= 0x80000000;
                    anim = 0;
                }
            } else if (evtCheckID(mobj->eventId)) {
                cur = (const s8*)animPoseGetCurrentAnim(mobj->poseId);
                if (cur[0] == 'A' && cur[2] >= '1' && cur[2] <= '2') {
                    anim = 0;
                } else if (cur[0] == 'A' && cur[2] == '3') {
                    mobj->typeDataU[1] |= 0x40000000;
                    anim = 0;
                } else if (cur[0] == 'A' && cur[2] >= '4' && cur[2] <= '5') {
                    anim = 3;
                } else {
                    anim = 3;
                }
            } else {
                mobj->typeDataU[1] |= 0x80000000;
                anim = 3;
            }
        } else {
            a = isAnimA13((const s8*)animPoseGetCurrentAnim(mobj->poseId));
            anim = a ? 3 : 0;
            mobj->typeDataU[1] |= 0x80000000;
        }
        mobjRunEvent(mobj, (s32*)mobj->event);
        if (mobj->hitFlag == 0x200000) {
            marioChgMot(MARIO_MOTION_JUMP_SW);
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_SWITCH1), &mobj->position);
        switch (mobj->hitFlag) {
        case 0x400000:
            animPoseSetAnim(mobj->poseId, _switch_anim[anim + 1], 1);
            break;
        default:
            animPoseSetAnim(mobj->poseId, _switch_anim[anim], 1);
            break;
        }
        mobj->flags &= ~8;
        if (mobj->timer) {
            mobj->state = 10;
        } else {
            mobj->state++;
        }
        break;
    case 1:
        if (mobj->flags & 8) {
            mobj->state = 0;
            break;
        }
        if (mobj->flags & 0x1000) {
            mobj->flags &= ~0x1000;
            mobj->state = 10;
            break;
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        if ((s32)(s8)mobj->linkedName[0]) {
            mobjNameToPtr(mobj->linkedName);
        }
        {
            const char* a = isAnimA13((const s8*)animPoseGetCurrentAnim(mobj->poseId)) ? "A_3" : "S_1";
            animPoseSetAnim(mobj->poseId, a, 1);
        }
        mobj->state++;
        break;
    case 2:
        if (mobj->flags & 8) {
            mobj->state = 0;
            break;
        }
        if (mobj->flags & 0x1000) {
            mobj->flags &= ~0x1000;
            mobj->state = 10;
        }
        if (evtCheckID(mobj->eventId) == 0) {
            mobj->state = 0;
            mobj->typeData[1] = 0;
        }
        break;
    case 10:
        if (mobj->flags & 8) {
            mobj->state = 0;
            break;
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        if (isAnimA13((const s8*)animPoseGetCurrentAnim(mobj->poseId))) {
            animPoseSetAnim(mobj->poseId, "A_3", 1);
        } else {
            animPoseSetAnim(mobj->poseId, "S_1", 1);
        }
        mobj->timer = 120;
        mobj->state++;
    case 11:
        if (mobj->flags & 8) {
            mobj->state = 0;
            break;
        }
        mobj->timer--;
        if (mobj->timer != 0) {
            break;
        }
        if ((s32)(s8)mobj->linkedName[0]) {
            if (blkHitCheck(mobjNameToPtr(mobj->linkedName))) {
                animPoseSetAnim(mobj->poseId, "S_1", 0);
            } else {
                animPoseSetAnim(mobj->poseId, "A_3", 0);
            }
        } else if (isAnimA13((const s8*)animPoseGetCurrentAnim(mobj->poseId))) {
            animPoseSetAnim(mobj->poseId, "A_3", 1);
        } else {
            animPoseSetAnim(mobj->poseId, "S_1", 1);
        }
        mobj->state = 0;
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_switch_float_blk) {
    const char* name;
    const char* partner;
    s32 val;
    EvtWord* args = event->args;
    s32 color;
    const char* anim;
    MapObjectEntry* mobj;
    MapObjectEntry* other;
    s32 inBattle;
    s32 flagVar;
    f32 x, y, z;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    partner = (const char*)evtGetValue(event, args[4]);
    color = args[5];
    val = evtGetValue(event, args[6]);
    flagVar = args[7];
    inBattle = gp->inBattle != 0;
    switch (color) {
    case 0:
        anim = "MOBJ_green_FlowBlockSwitch";
        break;
    case 1:
        anim = "MOBJ_purple_FlowBlockSwitch";
        break;
    case 2:
        anim = "MOBJ_orange_FlowBlockSwitch";
        break;
    case 3:
        anim = "MOBJ_red_FlowBlockSwitch";
        break;
    default:
        anim = "MOBJ_green_FlowBlockSwitch";
        break;
    }
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_switch_float_blk;
    mobj->event = val;
    mobj->flags |= 4;
    mobj->flags |= 0xe10000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobj->timer = 0;
    mobjCalcMtx(mobj);
    strcpy(mobj->linkedName, partner);
    if ((s32)(s8)mobj->linkedName[0]) {
        other = mobjNameToPtr(mobj->linkedName);
        strcpy(other->linkedName, mobj->name);
        if (blkHitCheck(other)) {
            animPoseSetAnim(mobj->poseId, "S_1", 1);
        } else {
            animPoseSetAnim(mobj->poseId, "A_3", 1);
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_lock_unlock) {
    MapObjectEntry* mobj = mobjNameToPtr((const char*)evtGetValue(event, event->args[0]));
    if (strcmp(mobj->animName, "MOBJ_Lock")) {
        return EVT_RETURN_DONE;
    }
    if (!mobj->mainFunc) {
        mobj->mainFunc = mobj_lock;
    }
    mobj->flags |= 8;
    marioKeyOff();
    partyKeyOff();
    marioStSystemLevel(1);
    mobj->state = 3;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(evt_lock_sound)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_LOCK_SWING1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(200)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_UNLOCK1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

extern s32 msgWindow_Entry(const char* msg, s32 a, s32 b);

s32 mobj_lock(MapObjectEntry* mobj) {
#ifdef VERSION_G8ME01
    s32 i;
    s32 item;
#else
    s32 item;
    s32 i;
#endif
    EventEntry* ev;
    f32 hx, hy, hz, dist, nx, ny, nz;
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            marioKeyOff();
            partyKeyOff();
            marioStSystemLevel(1);
            mobjRunEvent(mobj, (s32*)mobj->event);
            mobj->state++;
        }
        break;
    case 1:
        if (evtCheckID(mobj->eventId) != 1) {
            if (mobj->flags & 0x1000) {
                mobj->flags &= ~0x1000;
                mobj->flags &= ~8;
                marioKeyOn();
                partyKeyOn();
                marioStSystemLevel(0);
                mobj->eventId = 0;
                mobj->state = 0;
            } else {
                mobj->eventId = 0;
                mobj->state++;
            }
        }
        break;
        if (mobj) {
        }
    case 2:
        if (mobj->typeDataU[0] & 0x80000000) {
            item = 0;
#ifdef VERSION_G8ME01
            for (i = 0; ; i++) {
                s32* list = mobj->typeDataList[0];
                s32 r;
                if (list[i] == -1) {
                    break;
                }
                r = pouchCheckItem(list[i]);
                if (r) {
                    item = list[i];
                    break;
                }
            }
#else
            for (i = 0; ; i++) {
                s32 it = mobj->typeDataList[0][i];
                s32 r;
                if (it == -1) {
                    break;
                }
                r = pouchCheckItem(it);
                if (r) {
                    item = r;
                    break;
                }
            }
#endif
        } else if ((i = pouchCheckItem(mobj->typeData[0])) != 0) {
            item = mobj->typeData[0];
        } else {
            item = 0;
        }
        if (item) {
            pouchRemoveItem((ItemType)item);
            mobj->state++;
        } else {
            mobj->typeData[2] = msgWindow_Entry(msgSearch("sys_no_key"), 0, 0);
            mobj->state = 10;
        }
        break;
    case 3:
        ev = evtEntryType(evt_lock_sound, 0, 0, 0);
        ev->lwData[0] = (s32)(1024.0f * mobj->position.x) + 0xF24A7A80;
        ev->lwData[1] = (s32)(1024.0f * mobj->position.y) + 0xF24A7A80;
        ev->lwData[2] = (s32)(1024.0f * mobj->position.z) + 0xF24A7A80;
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        mobj->state++;
        break;
    case 4:
        if (!(animPoseGetLoopTimes(mobj->poseId) < 1.0f)) {
            mobj->velocity.y = 0.0f;
            mobj->gravity = 0.5f;
            mobj->bounce = 0.0f;
            evtSetValue(NULL, mobj->flagVar, 1);
            mobjRunEvent(mobj, (s32*)mobj->extraEvent);
            mobj->state++;
        }
        break;
    case 5:
        dist = 1.0f + (f32)__fabs(mobj->velocity.y);
        if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, mobj->position.x, 1.0f + mobj->position.y, mobj->position.z, 0.0f, mobj->velocity.y, 0.0f)) {
            mobj->position.y = hy;
            mobj->velocity.y *= -0.8f;
            mobj->bounce += 1.0f;
            if (mobj->bounce < 4.0f) {
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_UNLOCK1_LEAPED1), &mobj->position);
            }
        } else {
            mobj->position.y += mobj->velocity.y;
            mobj->velocity.y -= mobj->gravity;
        }
        if (!(mobj->bounce < 4.0f) && evtCheckID(mobj->eventId) != 1) {
            mobj->state++;
            mobj->timer = 20;
        }
        break;
    case 6:
        mobj->timer--;
        if (mobj->timer & 2) {
            mobj->flags |= 0x4000;
        } else {
            mobj->flags &= ~0x4000;
        }
        if (mobj->timer == 0) {
            mobj->state = 99;
        }
        break;
    case 10:
        if (!windowCheckID(mobj->typeData[2])) {
            mobj->flags &= ~8;
            marioKeyOn();
            partyKeyOn();
            marioStSystemLevel(0);
            mobj->state = 0;
        }
        break;
    case 99:
        marioKeyOn();
        partyKeyOn();
        marioStSystemLevel(0);
        mobjDelete(mobj->name);
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_lock) {
    EvtWord* args = event->args;
    const char* name;
    MapObjectEntry* mobj;
    s32 inBattle;
    const char* anim;
    s32 flagVar;
    s32 val2;
    s32 val;
    s32 item;
    f32 rot;
    f32 x, y, z;
    name = (const char*)evtGetValue(event, ((0, args))[0]);
    item = evtGetValue(event, args[1]);
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    rot = evtGetFloat(event, args[5]);
    val = evtGetValue(event, args[6]);
    val2 = evtGetValue(event, args[7]);
    flagVar = args[8];
    inBattle = gp->inBattle != 0;
    anim = "MOBJ_Lock";
    if (evtGetValue(event, flagVar)) {
        return EVT_RETURN_DONE;
    }
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjHitEntry(mobj, 2);
    mobj->typeData[0] = item;
    mobjSetPosition(name, x, y, z);
    mobj->rotation.y = rot;
    mobj->mainFunc = mobj_lock;
    mobj->event = val;
    mobj->extraEvent = val2;
    mobj->flags |= 0x26;
    mobj->flags |= 0x40000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "Z_1", 1);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(evt_box_sound1)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_TREASURE_BOX_SWING1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(200)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_TREASURE_BOX_OPEN1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_box_sound2)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_TREASURE_BOX_SWING2"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(200)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_TREASURE_BOX_OPEN2"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_box_sound3)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLACKBOX_SWING1"), LW(0), LW(1), LW(2), LW(15))
    WAIT_MS(200)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLACKBOX_OPEN1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

static inline void mobj_itemb(MapObjectEntry* mobj) { \
    AnimationPoseData* data = animPoseGetAnimBaseDataPtr(mobj->poseId); \
    Vec a, b; \
    a = (Vec){data->bboxMin.x, data->bboxMin.y, data->bboxMin.z}; \
    b = (Vec){data->bboxMax.x, data->bboxMax.y, data->bboxMax.z}; \
    a.z = a.z - 0.2f * __fabs(b.y - a.y); \
    PSVECAdd(&b, &a, &mobj->translation); \
    PSVECScale(&mobj->translation, &mobj->translation, 0.5f); \
    PSVECScale(&mobj->translation, &mobj->translation, 10.0f); \
    mobj->translation.y = 0.0f; \
    mobj->scale2.y *= 1.4f; \
    mobj->scale2.z *= 1.2f; \
    mobjCalcMtx2(mobj); \
}

s32 mobj_itembox(MapObjectEntry* mobj) {
    EventEntry* ev;
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            marioKeyOff();
            partyKeyOff();
            marioStSystemLevel(1);
            mobj->eventId = 0;
            mobjRunEvent(mobj, (s32*)mobj->event);
            mobj->state++;
        }
        break;
    case 1:
        if (mobj->eventId) {
            if (evtCheckID(mobj->eventId) == 1) {
                break;
            }
            if (mobj->flags & 0x1000) {
                mobj->flags &= ~0x1000;
                mobj->flags &= ~8;
                marioKeyOn();
                partyKeyOn();
                marioStSystemLevel(0);
                mobj->eventId = 0;
                mobj->state = 0;
                break;
            }
        }
        mobj->state++;
        break;
    case 2:
        if (!strcmp(mobj->animName, "MOBJ_TreasureBox") || !strcmp(mobj->animName, "MOBJ_GrayTreasureBox")) {
            ev = evtEntryType(evt_box_sound1, 0, 0, 0);
            ev->lwData[0] = (s32)(1024.0f * mobj->position.x) + 0xF24A7A80;
            ev->lwData[1] = (s32)(1024.0f * mobj->position.y) + 0xF24A7A80;
            ev->lwData[2] = (s32)(1024.0f * mobj->position.z) + 0xF24A7A80;
        } else if (!strcmp(mobj->animName, "MOBJ_BlackTreasureBox")) {
            ev = evtEntryType(evt_box_sound3, 0, 0, 0);
            ev->lwData[0] = (s32)(1024.0f * mobj->position.x) + 0xF24A7A80;
            ev->lwData[1] = (s32)(1024.0f * mobj->position.y) + 0xF24A7A80;
            ev->lwData[2] = (s32)(1024.0f * mobj->position.z) + 0xF24A7A80;
        } else {
            ev = evtEntryType(evt_box_sound2, 0, 0, 0);
            ev->lwData[0] = (s32)(1024.0f * mobj->position.x) + 0xF24A7A80;
            ev->lwData[1] = (s32)(1024.0f * mobj->position.y) + 0xF24A7A80;
            ev->lwData[2] = (s32)(1024.0f * mobj->position.z) + 0xF24A7A80;
        }
        animPoseSetAnim(mobj->poseId, "A_1", 1);
        mobj->eventId = 0;
        mobjRunEvent(mobj, (s32*)mobj->extraEvent);
        mobj->state++;
        break;
    case 3:
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        {
            if (mobj->eventId && evtCheckID(mobj->eventId) == 1) {
                break;
            }
            mobj->flags &= ~0x20;
            {
                mobj_itemb(mobj);
            }
            marioKeyOn();
            partyKeyOn();
            marioStSystemLevel(0);
            mobj->eventId = 0;
            mobj->state = 99;
        }
        break;
    case 99:
        evtSetValue(NULL, mobj->flagVar, 1);
        mobj->flags |= 8;
        mobj->flags &= ~0x20;
        if (!strcmp(mobj->animName, "MOBJ_BlackTreasureBox")) {
            animPoseSetAnim(mobj->poseId, "S_4", 1);
        } else {
            animPoseSetAnim(mobj->poseId, "S_2", 1);
        }
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_itembox) {
    s32 flagVar;
    EvtWord* args = event->args;
    const char* name;
    MapObjectEntry* mobj;
    s32 val, val2, kind, inBattle;
    const char* anim;
    f32 x, y, z;
    f32 hx, hy, hz, dist, nx, ny, nz;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    kind = evtGetValue(event, args[4]);
    val = evtGetValue(event, args[5]);
    val2 = evtGetValue(event, args[6]);
    flagVar = args[7];
    inBattle = gp->inBattle != 0;
    switch (kind) {
    case 0:
        anim = "MOBJ_TreasureBox";
        break;
    case 1:
        anim = "MOBJ_BigTreasureBox";
        break;
    case 2:
        anim = "MOBJ_GrayTreasureBox";
        break;
    case 3:
        anim = "MOBJ_BlackTreasureBox";
        break;
    case 4:
    default:
        anim = "MOBJ_GoldenTreasureBox";
        break;
    }
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjHitEntry(mobj, 2);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_itembox;
    mobj->event = val;
    mobj->extraEvent = val2;
    mobj->flags |= 0x64;
    mobj->flags |= 0x40000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    if (evtGetValue(event, flagVar)) {
        mobj_itemb(mobj);
        mobj->state = 99;
    }
    dist = 5.0f + (f32)__fabs(mobj->velocity.y);
    if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, mobj->position.x, 1.0f + mobj->position.y, mobj->position.z, 0.0f, -1.0f, 0.0f)) {
        mobj->flags |= 2;
        mobj->flags &= ~0x40;
    }
    return EVT_RETURN_DONE;
}

s32 mobj_signboard(MapObjectEntry* mobj) {
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            marioKeyOff();
            mobjRunEvent(mobj, (s32*)mobj->event);
            mobj->state++;
        }
        break;
    case 1:
        if (evtCheckID(mobj->eventId) != 1) {
            marioKeyOn();
            mobj->eventId = 0;
            mobj->flags &= ~8;
            mobj->state = 0;
        }
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_signboard) {
    s32 val;
    EvtWord* args = event->args;
    const char* name;
    s32 flagVar;
    MapObjectEntry* mobj;
    const char* anim;
    s32 inBattle;
    f32 x, y, z;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    val = evtGetValue(event, args[4]);
    inBattle = gp->inBattle;
    inBattle = inBattle != 0;
    flagVar = args[5];
    anim = "MOBJ_Signboard";
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_signboard;
    mobj->event = val;
    mobj->flags |= 0x24;
    mobj->flags |= 0x40000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    return EVT_RETURN_DONE;
}

void mobj_sign(void) {
    mobjNameToPtr("MOBJ_Arrow");
}

extern EffectEntry* effCoinFukidashiEntry(f32 x, f32 y, f32 z, s32 type, s32 number, s32 duration);

// case 3 heal sequence; case 4 spells the same steps out inline (only this split reproduces the target registers)
static inline void recoveryHeal(void) {
    PouchPartyData* party;
    MarioWork* mario;
    s32 i;
    mario = marioGetPtr();
    party = (PouchPartyData*)pouchGetPtr();
    effStardustEntry(2, 50, 100, mario->position.x, mario->position.y, mario->position.z, 50.0f, 50.0f);
    marioChgPose("M_I_1");
    ((PouchData*)party)->currentHP = ((PouchData*)party)->maximumHP;
    ((PouchData*)party)->currentFP = ((PouchData*)party)->maximumFP;
    for (i = 0; i < 8; party++, i++) {
        if ((party->flags & PARTY_FLAG_IS_ACTIVE) && !(party->flags & PARTY_FLAG_IS_GROUNDED)) {
            party->currentHP = party->maximumHP;
        }
    }
}

s32 mobj_recovery_blk(MapObjectEntry* mobj) {
    MarioWork* mario;
    PouchData* pouch;
    PouchPartyData* party;
    s32 i;
    s32 kind;
    switch (mobj->state) {
    case -1:
        mobj->effect = effCoinFukidashiEntry(mobj->position.x, 25.0f + mobj->position.y, mobj->position.z - 10.0f, 0, mobj->typeData[1], 0);
        mobj->state++;
        break;
    case 0:
        if (!(mobj->flags & 8)) {
            break;
        }
        marioKeyOff();
        camGetPtr((CameraId)8)->flags |= 0x200;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_HP_RECOVER_HIT1), &mobj->position);
        mobj->state++;
    case 1:
        if (!((u32)gp->flags & 0x1000) && swGet(0xea) == 0) {
            swSet(0xea);
            mobj->state = 20;
            break;
        }
        mobj->state++;
    case 2:
        kind = pouchGetCoin(); // kind reused for the coin count (matches the target codegen)
        if (kind < mobj->typeData[1]) {
            mobj->state = 10;
            break;
        }
        if (mobj->effect) {
            effSoftDelete(mobj->effect);
            mobj->effect = NULL;
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_HP_RECOVER_HEART1), &mobj->position);
        animPoseSetAnim(mobj->poseId, "A_2", 1);  // correctness: target str_8041A11C
        if (mobj->typeData[1] == 0) {
            mobj->state = 3;
        } else {
            mobj->state = 4;
            statusWinForceOpen();
        }
        mobj->timer = 120;
        break;
    case 3:
        if (mobj->timer == 100) {
            effStardustEntry(1, 30, 60, mobj->position.x, 45.0f + mobj->position.y, mobj->position.z, 50.0f, 50.0f);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_HP_RECOVER_SHINE1), &mobj->position);
        }
        if (mobj->timer == 100) {
            recoveryHeal();
            pouchAddCoin(0);
        }
        mobj->timer--;
        if (mobj->timer <= 0) {
            mobj->state = 5;
        }
        break;
    case 4:
        if (mobj->timer == 100) {
            effStardustEntry(1, 30, 60, mobj->position.x, 45.0f + mobj->position.y, mobj->position.z, 50.0f, 50.0f);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_HP_RECOVER_SHINE1), &mobj->position);
        }
        if (mobj->timer == 100) {
            kind = mobj->typeData[1];
            mario = marioGetPtr();
            party = (PouchPartyData*)pouchGetPtr();
            effStardustEntry(2, 50, 100, mario->position.x, mario->position.y, mario->position.z, 50.0f, 50.0f);
            marioChgPose("M_I_1");
            ((PouchData*)party)->currentHP = ((PouchData*)party)->maximumHP;
            ((PouchData*)party)->currentFP = ((PouchData*)party)->maximumFP;
            for (i = 0; i < 8; party++, i++) {
                if ((party->flags & PARTY_FLAG_IS_ACTIVE) && !(party->flags & PARTY_FLAG_IS_GROUNDED)) {
                    party->currentHP = party->maximumHP;
                }
            }
            ((s32 (*)(s32))pouchAddCoin)(-kind);
        }
        mobj->timer--;
        if (mobj->timer <= 0) {
            mobj->state = 5;
        }
        break;
    case 5:
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "A_1", 1);  // correctness: target str_8041A118
        marioChgPose("M_S_1");
        marioKeyOn();
        statusWinForceOff();
        camGetPtr((CameraId)8)->flags &= ~0x200;
        mobj->state++;
        break;
    case 6:
        if (mobj->effect == NULL) {
            mobj->effect = effCoinFukidashiEntry(mobj->position.x, 25.0f + mobj->position.y, mobj->position.z - 10.0f, 0, mobj->typeData[1], 0);
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_1", 1);  // correctness: target str_8041A12C
        mobj->flags &= ~8;
        mobj->state = 0;
        break;
    case 10:
        mobj->typeData[2] = msgWindow_Entry(msgSearch("sys_no_coin"), 0, 0);
        mobj->state++;
        break;
    case 11:
        if (windowCheckID(mobj->typeData[2]) == 0) {
            marioKeyOn();
            camGetPtr((CameraId)8)->flags &= ~0x200;
            mobj->flags &= ~8;
            mobj->state = 0;
        }
        break;
    case 20:
        mobj->typeData[2] = msgWindow_Entry(msgSearch("kaifuku_first_hit"), 0, 0);
        mobj->state++;
        break;
    case 21:
        if (windowCheckID(mobj->typeData[2]) == 0) {
            marioKeyOn();
            camGetPtr((CameraId)8)->flags &= ~0x200;
            mobj->flags &= ~8;
            mobj->state = 0;
        }
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_recovery_blk) {
    BOOL inBattle;
    s32 flagVar;
    EvtWord* args = event->args;
    const char* anim;
    s32 kind;
    const char* name;
    MapObjectEntry* mobj;
    f32 x, y, z;
    name = (const char*)evtGetValue(event, args[0]);
    kind = evtGetValue(event, args[1]);
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    evtGetValue(event, args[5]);
    flagVar = args[6];
    inBattle = gp->inBattle != 0;
    anim = kind == 0 ? "MOBJ_RecoveryBlock" : "MOBJ_CoinTakenRecoveryBlock";
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobj->typeData[1] = kind;
    mobjSetPosition(name, x, y, z);
    mobj->flags |= 4;
    mobj->mainFunc = mobj_recovery_blk;
    mobj->event = 0;
    mobj->extraEvent = 0;
    mobj->flags |= 4;
    mobj->flags |= 0x400000;
    mobj->flagVar = flagVar;
    if (kind == 0) {
        mobj->state = 0;
    } else {
        mobj->state = -1;
    }
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(mobj_save_blk_sysevt)
    SET_TYPE(0)
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(LF(10), 0)
    IF_INT_EQUAL(GSWF_SAVE_BLOCK_TUTORIAL_SEEN, 0)
        SET_VALUE(GSWF_SAVE_BLOCK_TUTORIAL_SEEN, 1)
        CALL(evt_msg_print, 0, STRING("save_first_hit"), 0, 0)
        SET_VALUE(LF(10), 1)
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 7)
        SET_VALUE(GSWF(236), 1)
    END_IF
    SET_VALUE(LW(0), 0)
#ifdef VERSION_G8ME01
    CALL_CHILD(PTR(evt_memcard_save_wrapper))
#else
    CALL_CHILD(PTR(evt_memcard_save))
#endif
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

s32 mobj_save_blk(MapObjectEntry* mobj) {
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            marioKeyOff();
            marioStSystemLevel(1);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_SAVE1), &mobj->position);
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            camGetPtr((CameraId)8)->flags |= 0x200;
            mobj->state++;
        }
        break;
    case 1:
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        if ((void*)mobj->event) {
            mobjRunEvent(mobj, (s32*)mobj->event);
            mobj->state++;
        } else {
            mobj->state = 3;
        }
        break;
    case 2:
        if (evtCheckID(mobj->eventId) != 1) {
            mobj->eventId = 0;
            mobj->state++;
        }
        break;
    case 3:
        mobjRunEvent(mobj, (s32*)mobj->extraEvent);
        mobj->state++;
        break;
    case 4:
        if (evtCheckID(mobj->eventId) != 1) {
            marioStSystemLevel(0);
            camGetPtr((CameraId)8)->flags &= ~0x200;
            mobj->flags &= ~8;
            marioKeyOn();
            mobj->state = 0;
        }
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_save_blk) {
    s32 flagVar;
    s32 val;
    EvtWord* args = event->args;
    const char* name;
    const char* anim;
    MapObjectEntry* mobj;
    s32 inBattle;
    f32 x, y, z;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    val = evtGetValue(event, args[4]);
    flagVar = args[5];
    anim = "MOBJ_SaveBlock";
    inBattle = gp->inBattle != 0;
    if ((u32)gp->flags & 0x20) {
        return EVT_RETURN_DONE;
    }
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_save_blk;
    mobj->event = val;
    mobj->extraEvent = (EvtWord)mobj_save_blk_sysevt;
    mobj->flags |= 0x84;
    mobj->flags |= 0x400000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    return EVT_RETURN_DONE;
}

#ifdef VERSION_G8ME01
// US-only plain block (MOBJ_Block / MOBJ_PinkBlock) that runs an event when hit.
s32 mobj_blk(MapObjectEntry* mobj) {
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_UNBREAK1), &mobj->position);
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            mobj->flags &= ~8;
            mobj->state++;
        }
        break;
    case 1:
        if ((void*)mobj->event) {
            mobjRunEvent(mobj, (s32*)mobj->event);
            mobj->state++;
        } else {
            mobj->state = 3;
        }
        break;
    case 2:
        if (evtCheckID(mobj->eventId) != 1) {
            mobj->eventId = 0;
            mobj->state++;
        }
        break;
    case 3:
        if (!(mobj->flags & 8)) {
            mobj->state = 0;
            break;
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        mobj->state = 0;
        mobj->flags &= ~8;
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_blk) {
    EvtWord* args = event->args;
    const char* name;
    f32 x, y, z;
    s32 kind;
    s32 val;
    s32 flagVar;
    const char* anim;
    MapObjectEntry* mobj;
    BOOL inBattle;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    kind = args[4];
    val = evtGetValue(event, args[5]);
    flagVar = args[6];
    inBattle = gp->inBattle != 0;
    switch (kind) {
    case 0:
        anim = "MOBJ_Block";
        break;
    case 1:
        anim = "MOBJ_PinkBlock";
        break;
    default:
        anim = "MOBJ_Block";
        break;
    }
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_blk;
    mobj->event = val;
    mobj->flags |= 4;
    mobj->flags |= 0x400000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    return EVT_RETURN_DONE;
}
#endif

s32 mobj_badgeblk(MapObjectEntry* mobj) {
    ItemEntry* item;
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            if (!strcmp(mobj->animName, "MOBJ_BadgeBlock") || !strcmp(mobj->animName, "MOBJ_BrickBadgeBlock") ||
                !strcmp(mobj->animName, "MOBJ_HiddenBadgeBlock")) {
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_BADGE1), &mobj->position);
            } else {
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_QUESTION1), &mobj->position);
            }
            if (mobj->flags & 0x800) {
                mobj->flags &= ~0x800;
            }
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            if ((u32)mobj->event != 0) {
                mobjRunEvent(mobj, (s32*)mobj->event);
            } else if (mobj->typeData[0] != 0x79) {
                itemEntry(mobj->position.x, 40.0f + mobj->position.y, mobj->position.z, mobj->name, mobj->typeData[0], 4, mobj->flagVar, NULL);
                item = itemNameToPtr(mobj->name);
                itemFlagOn(item, 0x10);
                item->bounceFactor = 0.7f;
                item->gravityScale = 1.0f;
                item->maxBounces = 0;
                item->velocityY = 360.0f;
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_ITEM_APPEAR1), &mobj->position);
            } else {
                pouchAddCoin(1);
                itemEntry(mobj->position.x, mobj->position.y, mobj->position.z, NULL, ITEM_COIN, 0xf, -1, NULL);
                evtSetValue(NULL, mobj->flagVar, 1);
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_COUNT_COIN1), &mobj->position);
            }
            mobj->state++;
        }
        break;
    case 1:
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_2", 1);
        mobj->flags &= ~8;
        mobj->state = 90;
        if (!strcmp(mobj->animName, "MOBJ_HatenaBlock") || !strcmp(mobj->animName, "MOBJ_BrickHatenaBlock") ||
            !strcmp(mobj->animName, "MOBJ_HiddenHatenaBlock")) {
            strcpy(mobj->animName, "MOBJ_Block");
        } else {
            strcpy(mobj->animName, "MOBJ_PinkBlock");
        }
        break;
    case 90:
        if (mobj->flags & 8) {
            mobj->flags &= ~8;
            animPoseSetAnim(mobj->poseId, "A_2", 1);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_UNBREAK1), &mobj->position);
            mobj->state++;
        }
        break;
    case 91:
        if (!(mobj->flags & 8)) {
            mobj->state = 90;
            break;
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_2", 1);
        mobj->state = 90;
        mobj->flags &= ~8;
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_badgeblk) {
    EvtWord* args = event->args;
    const char* anim;
    f32 x, y, z;
    const char* name;
    s32 val;
    s32 item;
    s32 flagVar;
    s32 kind;
    MapObjectEntry* mobj;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    item = evtGetValue(event, args[4]);
    val = evtGetValue(event, args[5]);
    kind = args[7];
    flagVar = args[6];
    {
        BOOL b = gp->inBattle != 0;
        anim = kind != 0 ? "MOBJ_BadgeBlock" : "MOBJ_HatenaBlock";
        if (animGroupBaseAsync(anim, b, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }}
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_badgeblk;
    mobj->event = val;
    mobj->flags |= 4;
    mobj->flags |= 0xc10000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobj->typeData[0] = item;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    if (evtGetValue(event, flagVar)) {
        mobj->state = 90;
        animPoseSetAnim(mobj->poseId, "S_2", 1);
        strcpy(mobj->animName, kind != 0 ? "MOBJ_PinkBlock" : "MOBJ_Block");
    }
    return EVT_RETURN_DONE;
}

s32 mobj_powerupblk(MapObjectEntry* mobj) {
    switch (mobj->state) {
    case 0:
        if (!(mobj->flags & 8)) {
            break;
        }
        if (marioKeyOffChk() || marioCtrlOffChk()) {
            mobj->flags &= ~8;
            break;
        }
        marioKeyOff();
        if (!strcmp(gp->currentMapName, "dou_02")) {
            marioCtrlOff();
        } else if ((!strcmp(gp->currentMapName, "dou_05") || !strcmp(gp->currentMapName, "dou_10")) && mobj->hitFlag == 0x800000) {
            marioCtrlOff();
        }
        gp->systemLevelFlags |= 3;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_POWER_SHINE1 | PSND_SFX_BYPASS_MUTE), &mobj->position);
        animPoseSetAnim(mobj->poseId, "A_1", 1);
        itemEntry(mobj->position.x, mobj->position.y, mobj->position.z, "pwrblk1", ITEM_SHINE_SPRITE, 0xf, -1, NULL);
        mobj->state++;
        break;
    case 1:
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "A_2", 1);
        mobj->flags &= ~8;
        mobj->typeData[1] = 1;
        mobj->state++;
        break;
    case 2:
        if (mobj->typeData[1]) {
            mobj->typeData[1]--;
            break;
        }
        itemForceGet(itemEntry(mobj->position.x, 40.0f + mobj->position.y, mobj->position.z, "pwrblk2", ITEM_SHINE_SPRITE, 0x11, -1, NULL));
        evtSetValue(NULL, mobj->flagVar, 1);
        marioKeyOn();
        if (!strcmp(gp->currentMapName, "dou_02")) {
            marioCtrlOn();
        } else if ((!strcmp(gp->currentMapName, "dou_05") || !strcmp(gp->currentMapName, "dou_10")) && mobj->hitFlag == 0x800000) {
            marioCtrlOn();
        }
        mobj->state = 90;
        break;
    case 90:
        mobjDelete(mobj->name);
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_powerupblk) {
    MapObjectEntry* mobj;
    const char* name;
    const char* anim;
    f32 rot;
    f32 x, y, z;
    s32 flagVar;
    s32 val;
    EvtWord* args = event->args;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    val = evtGetValue(event, args[4]);
    flagVar = args[5];
    rot = evtGetFloat(event, args[6]);
    anim = "MOBJ_PowerUpBlock";
    if (animGroupBaseAsync(anim, gp->inBattle != 0, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_powerupblk;
    mobj->event = val;
    mobj->flags |= 4;
    mobj->flags |= 0xc10000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobj->rotation.y = rot;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    if (evtGetValue(event, flagVar)) {
        mobj->state = 90;
        animPoseSetAnim(mobj->poseId, "A_2", 1);
    }
    return EVT_RETURN_DONE;
}

s32 mobj_brickblk(MapObjectEntry* mobj) {
    char oname[16] = "o_";
    char iname[16] = "i_";
    EventEntry ev1;
    EventEntry ev2;
    EvtWord args1[10];
    EvtWord args2[10];
    ImageEntry* img;
    s32 type = gp->inBattle != 0;
    strcat(oname, mobj->name);
    strcat(iname, mobj->name);
    switch (mobj->state) {
    case 0:
        animGroupBaseAsync("P_box_y", type, NULL);
        if (mobj->flags & 8) {
            mobjRunEvent(mobj, (s32*)mobj->event);
            evtSetValue(NULL, mobj->flagVar, 1);
            mobj->state = 10;
        }
        break;
    case 10:
        animPaperPoseEntry("P_box_y", type);
        offscreenEntry(oname);
        imgEntry(iname, type);
        mobj->offscreenId = offscreenNameToId(oname);
        ev1.args = args1;
        args1[0] = (EvtWord)iname; args1[1] = 0; args1[2] = (EvtWord)oname; args1[3] = 1;
        args1[4] = 0; args1[5] = 0; args1[6] = 0; args1[7] = 0;
        evt_img_alloc_capture(&ev1, TRUE);
        ev2.args = args2;
        args2[0] = (EvtWord)iname; args2[1] = 1; args2[2] = (EvtWord)oname; args2[3] = 1;
        args2[4] = 0; args2[5] = 0; args2[6] = 0; args2[7] = 0;
        evt_img_alloc_capture(&ev2, TRUE);
        img = imgNameToPtr(iname, type);
        img->poseId = animPaperPoseGetId("P_box_y", type);
        img->animName = "A_1";
        img->animTime = animTimeGetTime(type);
        imgSetShadow(img, 2);
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_BREAK1), &mobj->position);
        mobj->state = 11;
        break;
    case 11:
        img = imgNameToPtr(iname, type);
        if (img->loopTimes >= 1.0f) {
            mobj->state = 12;
        }
        if (img->flags & 0x10000000) {
            mobj->state = 12;
        }
        mobj->position = (Vec){0.0f, -2000.0f, 0.0f};
        mobj->flags |= 4;
        mobj->flags |= 8;
        break;
    case 12:
        img = imgNameToPtr(iname, type);
        imgFreeCapture(img, 0);
        imgFreeCapture(img, 1);
        imgSetShadow(img, 0);
        imgRelease(img, type);
        offscreenDelete(oname);
        mobj->offscreenId = -1;
        mobj->state = 99;
        break;
    case 99:
        mobjDelete(mobj->name);
        break;
    }
    return 0;
}

s32 mobj_10countblk(MapObjectEntry* mobj) {
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            if (mobj->flags & 0x800) {
                mobj->flags &= ~0x800;
                strcpy(mobj->animName, "MOBJ_Block");
            }
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_TEN_COUNT_HIT1), &mobj->position);
            evtSetValue(NULL, mobj->flagVar, 1);
            if ((u32)mobj->event != 0) {
                mobjRunEvent(mobj, (s32*)mobj->event);
            } else {
                pouchAddCoin(1);
                itemEntry(mobj->position.x, mobj->position.y, mobj->position.z, NULL, ITEM_COIN, 0xf, -1, NULL);
                mobj->timer = 200;
            }
            mobj->state++;
            mobj->flags &= ~8;
        }
        break;
    case 1:
        if (mobj->timer > 0) {
            mobj->timer--;
        }
        if (mobj->timer <= 0) {
            mobj->state = 90;
            animPoseSetAnim(mobj->poseId, "S_2", 1);
            strcpy(mobj->animName, "MOBJ_Block");
            break;
        }
        if (mobj->flags & 8) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_TEN_COUNT_COIN1), &mobj->position);
            pouchAddCoin(1);
            itemEntry(mobj->position.x, mobj->position.y, mobj->position.z, NULL, ITEM_COIN, 0xf, -1, NULL);
            mobj->flags &= ~8;
            animPoseSetAnim(mobj->poseId, "A_1", 1);
        }
        break;
    case 90:
        if (mobj->flags & 8) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_UNBREAK1), &mobj->position);
            animPoseSetAnim(mobj->poseId, "A_3", 1);
            mobj->flags &= ~8;
            mobj->state++;
        }
        break;
    case 91:
        if (!(mobj->flags & 8)) {
            mobj->state = 90;
            break;
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_2", 1);
        mobj->state = 90;
        mobj->flags &= ~8;
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_brick) {
    f32 x, y, z;
    EvtWord* args = event->args;
    const char* name;
    s32 item;
    s32 inBattle;
    s32 val;
    s32 kind;
    s32 flagVar;
    MapObjectEntry* mobj;
    const char* anim;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    item = evtGetValue(event, args[4]);
    kind = args[5];
    val = evtGetValue(event, args[6]);
    flagVar = args[7];
    inBattle = gp->inBattle != 0;
    switch (kind) {
    case 0:
        anim = "MOBJ_FlowBrickBlock";
        break;
    case 1:
        anim = "MOBJ_BrickHatenaBlock";
        break;
    case 2:
        anim = "MOBJ_BrickBadgeBlock";
        break;
    case 3:
        anim = "MOBJ_10CountBlock";
        break;
    case 10:
        anim = "MOBJ_HiddenHatenaBlock";
        break;
    case 11:
        anim = "MOBJ_HiddenHatenaBlock";
        break;
    case 12:
        anim = "MOBJ_HiddenBadgeBlock";
        break;
    case 13:
        anim = "MOBJ_Hidden10CountBlock";
        break;
    }
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    if (animGroupBaseAsync("P_box_y", inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    if ((s32)kind >= 10) {
        mobj->flags |= 0x800;
    }
    switch (kind) {
    case 0:
    case 10:
        mobj->mainFunc = mobj_brickblk;
        break;
    case 1:
    case 11:
        mobj->mainFunc = mobj_badgeblk;
        break;
    case 2:
    case 12:
        mobj->mainFunc = mobj_badgeblk;
        break;
    case 3:
    case 13:
        mobj->mainFunc = mobj_10countblk;
        break;
    }
    mobj->event = val;
    mobj->flags |= 4;
    mobj->flags |= 0xc10000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobj->typeData[0] = item;
    mobjCalcMtx(mobj);
    if ((s32)kind < 10) {
        animPoseSetAnim(mobj->poseId, "S_1", 1);
    } else if (kind == 13) {
        animPoseSetAnim(mobj->poseId, "S_3", 1);
    } else {
        animPoseSetAnim(mobj->poseId, "S_1", 1);
    }
    if (evtGetValue(event, flagVar)) {
        const char* anim2 = NULL;
        switch (kind) {
        case 0:
        case 10:
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            mobj->state = 99;
            break;
        case 1:
        case 11:
            animPoseSetAnim(mobj->poseId, "S_2", 1);
            anim2 = "MOBJ_Block";
            mobj->state = 90;
            break;
        case 2:
        case 12:
            animPoseSetAnim(mobj->poseId, "S_2", 1);
            anim2 = "MOBJ_PinkBlock";
            mobj->state = 90;
            break;
        case 3:
        case 13:
            animPoseSetAnim(mobj->poseId, "S_2", 1);
            anim2 = "MOBJ_Block";
            mobj->state = 90;
            break;
        }
        if (anim2) {
            strcpy(mobj->animName, anim2);
        }
        mobj->flags &= ~0x800;
    }
    return EVT_RETURN_DONE;
}

s32 mobj_breaking_floor(MapObjectEntry* mobj) {
    MarioWork* mario = marioGetPtr();
    GXColor c3;
    GXColor c2;
    GXColor c1 = {0xff, 0xff, 0xff, 0xff};
    c2 = c1;
    c3 = c1;
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            psndSFXOn_3D((const char*)0x401e8, &mobj->position);
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            if ((u32)mobj->event != 0U) {
                mobjRunEvent(mobj, (s32*)mobj->event);
            }
            if (mobj->hitObj[0].hitObject) {
                mobj->hitObj[0].hitObject->flags |= 1;
            }
            mobj->flags |= 2;
            evtSetValue(NULL, mobj->flagVar, 1);
            mobj->state = 1;
            animPoseSetMaterialFlagOn(mobj->poseId, 0x40);
            animPoseSetMaterialEvtColor(mobj->poseId, c2);
            mobj->timer = 64;
        }
        break;
    case 1:
        c3 = animPoseGetMaterialEvtColor(mobj->poseId);
        if (((0, c3)).a >= 4) {
            c3.a -= 4;
        } else {
            c3.a = 0;
            mobj->state = 99;
        }
        animPoseSetMaterialEvtColor(mobj->poseId, c3);
        mobj->timer--;
        if (mobj->timer <= 0) {
            mobj->state = 99;
        }
        break;
    case 99:
        mobjDelete(mobj->name);
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_breaking_floor) {
    char anim[128];
    s32 flagVar;
    s32 val;
    EvtWord* args = event->args;
    s32 kind;
    const char* name;
    s32 inBattle;
    f32 x, y, z;
    MapObjectEntry* mobj;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    kind = evtGetValue(event, args[4]);
    val = evtGetValue(event, args[5]);
    flagVar = args[6];
    inBattle = gp->inBattle != 0;
    switch (kind) {
    case 0:
        strcpy(anim, "MOBJ_BreakingFloor");
        break;
    case 1:
        strcpy(anim, "MOBJ_BlackBreakingFloor");
        break;
    default:
        strcpy(anim, "MOBJ_BreakingFloor");
        break;
    }
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_breaking_floor;
    mobj->event = val;
    mobj->flags |= 4;
    mobj->flags |= 0x1000000;
    mobj->flagVar = flagVar;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    } else {
        mobj->state = 0;
    }
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    return EVT_RETURN_DONE;
}

// ---- kpa ----

typedef struct KpaPoleScoreWork {
    char str[8];
    f32 x;
    f32 y;
    f32 z;
} KpaPoleScoreWork;

static KpaPoleScoreWork* psw;

void kururing_capture(CameraId camId, MapObjectEntry* mobj) {
    u8 saved[0x260];
    CameraEntry* cam;
    AnimationPoseData* data;
    Vec min, max;
    s32 h, w;
    s32 ww, hh;
    smartEntry* e;
    f32 t;
    memcpy(saved, camGetPtr(camId), 0x260);
    data = animPoseGetAnimBaseDataPtr(mobj->poseId);
    min = (Vec){data->bboxMin.x, data->bboxMin.y, data->bboxMin.z};
    max = (Vec){data->bboxMax.x, data->bboxMax.y, data->bboxMax.z};
    ww = 10.0f * (max.x - min.x);
    hh = 10.0f * (max.z - min.z);
    cam = camGetPtr(camId);
    h = (u16)(ww + 1) & ~1;
    w = (u16)(hh + 1) & ~1;
    cam->cameraUp = (Vec){0.0f, 0.0f, -1.0f};
    cam->cameraAt = mobj->position;
    cam->cameraPos = mobj->position;
    t = tan(0.21816615760326385);
    cam->cameraPos.y += (f32)(u32)w / 2.0f / t;
    C_MTXPerspective(cam->projection, 25.0f, (f32)(h / w), 1.0f, 32767.0f);
    cam->projType = GX_PERSPECTIVE;
    C_MTXLookAt(cam->view, &cam->cameraPos, &cam->cameraUp, &cam->cameraAt);
    GXSetProjection(cam->projection, cam->projType);
    GXSetViewport(0.0f, 0.0f, (u32)h, (u32)w, 0.0f, 1.0f);
    GXSetScissor(0, 0, h, w);
    test_kururing_mapdisp(camId);
    sysWaitDrawSync();
    hh = ((u32)h >> 1) & 0x7fff;
    ww = ((u32)w >> 1) & 0x7fff;
    mobj->paperTexData = smartAlloc(GXGetTexBufferSize(hh, ww, 6, 0, 0), 3);
    GXSetTexCopySrc(0, 0, h, w);
    GXSetTexCopyDst(hh, ww, (GXTexFmt)6, 1);
    GXSetZMode(1, (GXCompare)3, 1);
    GXCopyTex(mobj->paperTexData->address, 1);
    GXInvalidateTexAll();
    GXPixModeSync();
    GXTexModeSync();
    GXInitTexObj(&mobj->paperTexObj, mobj->paperTexData->address, hh, ww, (GXTexFmt)6, (GXTexWrapMode)0, (GXTexWrapMode)0, 0);
    GXInitTexObjLOD(&mobj->paperTexObj, (GXTexFilter)0, (GXTexFilter)0, 0.0f, 0.0f, 0.0f, 0, 0, (GXAnisotropy)0);
    PSMTXIdentity(mobj->paperTexMtx);
    memcpy(camGetPtr(camId), saved, 0x260);
}


static inline BOOL kururing_in_box(f32 x, f32 z, f32 px, f32 pz, f32 r) {
    if (x - r <= px && px <= x + r && z - r <= pz && pz <= z + r) {
        return TRUE;
    }
    return FALSE;
}

#define KURU_POSE_FLAGS(m) animGetPtr()->poses[(m)->poseId].flags

s32 mobj_kururing_floor(MapObjectEntry* mobj) {
    PartyEntry* party;
    MarioWork* mario = marioGetPtr();
    s32 mode;
    f32 dist;
    void* item;
    void* p;
    void* cur2;
    party = partyGetPtr(marioGetPartyId());
    switch (mobj->state) {
    case 0:
        if (!(mobj->flags & 8)) {
            break;
        }
        dist = distABf(mario->position.x, mario->position.z, mobj->position.x, mobj->position.z);
        if (__fabsf(mario->position.y - mobj->position.y) > 10.0f) {
            dist = 100000.0f;
        }
        if (mobj->hitFlag == 0x1000000) {
            if (pouchGetJumpLv() < 2) {
                mode = 0;
            } else if (dist < 60.0f) {
                mode = 2;
            } else if (dist < 100.0f) {
                mode = 1;
            } else {
                mode = 0;
            }
        }
        if (mobj->hitFlag == 0x10000) {
            mode = 1;
        }
        if (mode == 0) {
            mobj->flags &= ~8;
            break;
        }
        if (kururing_in_box(mobj->position.x, mobj->position.z, mario->position.x, mario->position.z, 30.0f)) {
            mode = 1;
        }
        if (party) {
            if (kururing_in_box(mobj->position.x, mobj->position.z, party->position.x, party->position.z, 30.0f)) {
                mode = 1;
            }
        }
        if (itemHitCheck(mobj->position.x, mobj->position.y, mobj->position.z, 42.420002f)) {
            mode = 1;
        }
        if (npcNearDistCheck(mobj->position.x, mobj->position.y, mobj->position.z, 42.420002f)) {
            mode = 1;
        }
        if (mode == 0) {
            mobj->flags &= ~8;
            break;
        }
        if (mode == 1) {
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            psndSFXOnEx_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_KURURIN_FLOOR_HOP1), &mobj->position, 0xff, 0xff, 100, 0x10);
            mobj->state = 10;
        } else if (mode == 2) {
            animPoseSetAnim(mobj->poseId, "A_2", 1);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_KURURIN_FLOOR_LEAP1), &mobj->position);
            mobj->state = 1;
            item = itemNameToPtr(mobj->name);
            if (item == NULL || !(((ItemEntry*)item)->flags & 1)) {
                if (itemEntry(mobj->position.x, 30.0f + mobj->position.y, mobj->position.z, mobj->name, ITEM_STAR_PIECE, 4, mobj->flagVar, NULL)) {
                    item = itemNameToPtr(mobj->name);
                    itemFlagOn(item, 0x10);
                    ((ItemEntry*)item)->bounceFactor = 0.6f;
                    ((ItemEntry*)item)->gravityScale = 1.0f;
                    ((ItemEntry*)item)->maxBounces = 5;
                    ((ItemEntry*)item)->velocityY = 300.0f;
                }
            }
        }
        KURU_POSE_FLAGS(mobj) |= 0x10;
        mobj->flags |= 0x400;
        dispEntry(CAMERA_SHADOW, 0.0f, 0, (DispCallback)kururing_capture, mobj);
        break;
    case 1:
        KURU_POSE_FLAGS(mobj) |= 0x10;
        mobj->flags |= 0x400;
        dispEntry(CAMERA_SHADOW, 0.0f, 0, (DispCallback)kururing_capture, mobj);
        if (!(animPoseGetLoopTimes(mobj->poseId) < 1.0f)) {
            KURU_POSE_FLAGS(mobj) &= ~0x10;
            mobj->flags &= ~0x400;
            mobj->flags &= ~8;
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_KURURIN_FLOOR_CLOSE1), &mobj->position);
            mobj->state = 0;
        }
        break;
    case 10:
    {
        void* joint;
        KURU_POSE_FLAGS(mobj) |= 0x10;
        mobj->flags |= 0x400;
        dispEntry(CAMERA_SHADOW, 0.0f, 0, (DispCallback)kururing_capture, mobj);
        // FAKE: joint points at worldPositions[0][1].y (the y read below) so its web is the unnamed add temp, not a named load (colours before mario's call temp)
        joint = &animPoseGetAnimPosePtr(mobj->poseId)->worldPositions[0][1].y;
        item = marioGetPtr();
        if (kururing_in_box(mobj->position.x, mobj->position.z, ((MarioWork*)item)->position.x, ((MarioWork*)item)->position.z, 30.0f)) {
            ((MarioWork*)item)->position.y = *(f32*)joint;
        }
        p = partyGetPtr(marioGetPartyId());
#ifdef VERSION_G8ME01
        if (p && !(((PartyEntry*)p)->flags & 0x100)) {
#else
        if (p) {
#endif
            if (kururing_in_box(mobj->position.x, mobj->position.z, ((PartyEntry*)p)->position.x, ((PartyEntry*)p)->position.z, 30.0f)) {
                ((PartyEntry*)p)->position.y = *(f32*)joint;
            }
        }
        p = partyGetPtr(marioGetExtraPartyId());
#ifdef VERSION_G8ME01
        if (p && !(((PartyEntry*)p)->flags & 0x100)) {
#else
        if (p) {
#endif
            if (kururing_in_box(mobj->position.x, mobj->position.z, ((PartyEntry*)p)->position.x, ((PartyEntry*)p)->position.z, 30.0f)) {
                ((PartyEntry*)p)->position.y = *(f32*)joint;
            }
        }
        {
            s32 i;
            NpcWork* wp = npcGetWorkPtr();
            NpcEntry* npc = wp->entries;
            for (i = 0; i < wp->total; i++, npc++) {
                if ((u32)npc->flags & 1) {
                    if (kururing_in_box(mobj->position.x, mobj->position.z, npc->position.x, npc->position.z, 42.420002f)) {
                        npc->position.y = *(f32*)joint;
                    }
                }
            }
        }
        p = itemNearDistCheck(mobj->position.x, mobj->position.y, mobj->position.z, 42.420002f);
        if (p) {
            ((ItemEntry*)p)->position.y = *(f32*)joint;
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 2.0f) {
            break;
        }
        KURU_POSE_FLAGS(mobj) &= ~0x10;
        mobj->flags &= ~0x400;
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_KURURIN_FLOOR_CLOSE1), &mobj->position);
        mobj->timer = 20;
        mobj->state++;
    }
    case 11:
        mobj->timer--;
        if (mobj->timer == 0) {
            mobj->state++;
        }
        break;
    case 12:
        mobj->flags &= ~8;
        mobj->state = 0;
        break;
    }
    return 0;
}
USER_FUNC(evt_mobj_kururing_floor) {
    const char* grp;
    s32 flagVar;
    EvtWord* args = event->args;
    const char* name;
    const char* anim;
    MapObjectEntry* mobj;
    f32 x, y, z;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    grp = (const char*)evtGetValue(event, args[4]);
    evtGetValue(event, args[5]);
    flagVar = args[6];
    anim = "MOBJ_KururinFloor";
    if (animGroupBaseAsync(anim, gp->inBattle != 0, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobj->position = (Vec){x, 0.1f + y, z};
    mobj->mainFunc = mobj_kururing_floor;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobj->flags |= 0x1010000;
    mobj->flags |= 2;
    mobjCalcMtx(mobj);
    mobj->scale2 = (Vec){200.0f, mobj->scale2.y, 200.0f};
    mobjCalcMtx2(mobj);
    {
        Vec dz = (Vec){x, y, z};
        char oname[16] = "o_";
        char iname[16] = "i_";
    }
    mapGrpFlagOn(grp, 0x400000);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    animPoseWorldPositionEvalOn(mobj->poseId);
    return EVT_RETURN_DONE;
}

USER_FUNC(kpa_powerup) {
    EvtWord* args = event->args;
    Vec pos;
    pos.x = evtGetFloat(event, args[0]);
    pos.y = evtGetFloat(event, args[1]);
    pos.z = evtGetFloat(event, args[2]);
    kpaAddScorePos(500, &pos);
    if (kpaGetLevel() == 3) {
        kpaAddScore(1000);
    }
    kpaPowUp();
    return EVT_RETURN_DONE;
}

s32 mobj_koopa_ojama_blk_evt(MapObjectEntry* mobj) {
    BOOL ok = FALSE;
    ItemEntry* item;
    {
        Vec dv = (Vec){0.0f, -2000.0f, 0.0f};
    }
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            switch (kpaGetLevel()) {
            case 0:
            case 1:
                if (mobj->hitFlag == 0x20000000 || mobj->hitFlag == 0x4000000) {
                    ok = TRUE;
                }
                break;
            case 2:
                if (mobj->hitFlag == 0x20000000 || mobj->hitFlag == 0x10000000 || mobj->hitFlag == 0x4000000) {
                    ok = TRUE;
                }
                break;
            case 3:
                ok = TRUE;
                break;
            }
            if (!ok) {
                mobj->flags &= ~8;
                break;
            }
            mobjRunEvent(mobj, (s32*)mobj->event);
            evtSetValue(NULL, mobj->flagVar, 1);
            if (mobj->hitFlag == 0x4000000) {
                npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
            }
            if (mobj->hitObj[0].hitObject) {
                mobj->hitObj[0].hitObject->flags |= 1;
            }
            mobj->flags |= 2;
            kpaAddScore(kpaMutekiCheck() ? 500 : 400);
            mobj->state = 10;
        }
        break;
    case 10:
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_OJAMA1), &mobj->position);
        effMObjBrokenEntry(5, mobj->position.x, 25.0f + mobj->position.y, 25.0f + mobj->position.z);
        item = itemEntry(mobj->position.x, 25.0f + mobj->position.y, mobj->position.z, mobj->name, ITEM_BOWSER_BROS_MEAT, 4, -1, kpa_powerup_evt);
        if (item) {
            itemStatusOn(item, 0x10);
            itemFlagOn(item, 0x14);
            item->timer = 10000;
            item->bounceFactor = 0.45f;
            item->gravityScale = 0.7f;
            item->maxBounces = 0;
            item->velocityY = 500.0f;
            item->speed = 120.0f;
            item->direction = 90;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_MEAT_APPEAR1), &mobj->position);
        }
        mobj->state = 99;
        break;
    case 99:
        mobjDelete(mobj->name);
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_koopa_ojama_blk) {
    s32 val;
    s32 flagVar;
    EvtWord* args = event->args;
    const char* anim;
    const char* name;
    MapObjectEntry* mobj;
    f32 x, y, z;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    val = evtGetValue(event, args[4]);
    flagVar = args[5];
    anim = (const char*)evtGetValue(event, args[6]);
    if (animGroupBaseAsync(anim, gp->inBattle != 0, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobj->flags |= 4;
    mobj->position = (Vec){x, y, z};
    mobj->mainFunc = mobj_koopa_ojama_blk_evt;
    mobj->event = val;
    mobj->extraEvent = 0;
    mobj->flags |= 0x3c000000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    return EVT_RETURN_DONE;
}

void mobj_koopa_stone_blk(void) {
    mobjNameToPtr("MOBJ_BombRock");
    mobjNameToPtr("OFF_d_bom");
    mobjNameToPtr("MOBJ_trap");
}

s32 mobj_koopa_stone_blk_evt(MapObjectEntry* mobj) {
    "MOBJ_BombRock";
    "OFF_d_bom";
    "MOBJ_trap";
    return 0;
}

USER_FUNC(evt_mobj_koopa_stone_blk) {
    s32 val;
    s32 flagVar;
    EvtWord* args = event->args;
    const char* anim;
    const char* name;
    MapObjectEntry* mobj;
    f32 x, y, z;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    val = evtGetValue(event, args[4]);
    flagVar = args[5];
    anim = (const char*)evtGetValue(event, args[6]);
    if (animGroupBaseAsync(anim, gp->inBattle != 0, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobj->flags |= 4;
    mobj->position = (Vec){x, y, z};
    mobj->mainFunc = mobj_koopa_stone_blk_evt;
    mobj->event = val;
    mobj->extraEvent = 0;
    mobj->flagVar = flagVar;
    mobj->state = 99;
    return EVT_RETURN_DONE;
}

s32 mobj_koopa_brickblk(MapObjectEntry* mobj) {
    BOOL ok = FALSE;
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            switch (kpaGetLevel()) {
            case 0:
                if (mobj->hitFlag == 0x20000000) {
                    ok = TRUE;
                }
                break;
            case 1:
                if (mobj->hitFlag == 0x20000000 || mobj->hitFlag == 0x4000000) {
                    ok = TRUE;
                }
                break;
            case 2:
                if (mobj->hitFlag == 0x20000000 || mobj->hitFlag == 0x10000000 || mobj->hitFlag == 0x4000000) {
                    ok = TRUE;
                }
                break;
            case 3:
                ok = TRUE;
                break;
            }
            if (!ok) {
                if (mobj->hitFlag == 0x4000000) {
                    npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
                    mobj->state = 20;
                    break;
                }
                mobj->flags &= ~8;
                break;
            }
            mobjRunEvent(mobj, (s32*)mobj->event);
            evtSetValue(NULL, mobj->flagVar, 1);
            if (mobj->hitFlag == 0x4000000) {
                npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
            }
            if (mobj->hitObj[0].hitObject) {
                mobj->hitObj[0].hitObject->flags |= 1;
            }
            mobj->flags |= 2;
            kpaAddScore(kpaMutekiCheck() ? 500 : 400);
            mobj->state = 10;
        }
        break;
    case 10:
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_BREAK1), &mobj->position);
        if (!strcmp(mobj->animName, "MOBJ_FlowBrickBlock")) {
            effMObjBrokenEntry(0, mobj->position.x, 12.5f + mobj->position.y, 12.5f + mobj->position.z);
        } else if (!strcmp(mobj->animName, "MOBJ_block_ky")) {
            effMObjBrokenEntry(1, mobj->position.x, 12.5f + mobj->position.y, 12.5f + mobj->position.z);
        } else if (!strcmp(mobj->animName, "MOBJ_bigblock_01")) {
            effMObjBrokenEntry(3, mobj->position.x, 25.0f + mobj->position.y, 25.0f + mobj->position.z);
        } else if (!strcmp(mobj->animName, "MOBJ_bigblock_02")) {
            effMObjBrokenEntry(4, mobj->position.x, 25.0f + mobj->position.y, 25.0f + mobj->position.z);
        }
        mobj->state = 99;
        break;
    case 20:
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_UNBREAK1), &mobj->position);
        animPoseSetAnim(mobj->poseId, "A_1", 1);
        mobj->state = 21;
        break;
    case 21:
        if (!(mobj->flags & 8)) {
            mobj->state = 0;
            break;
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        mobj->state = 0;
        mobj->flags &= ~8;
        break;
    case 99:
        mobjDelete(mobj->name);
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_koopa_brick) {
    const char* name;
    s32 flagVar;
    s32 val;
    EvtWord* args = event->args;
    s32 item;
    s32 kind;
    const char* anim;
    s32 inBattle;
    f32 x, y, z;
    MapObjectEntry* mobj;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    item = evtGetValue(event, args[4]);
    kind = args[5];
    val = evtGetValue(event, args[6]);
    flagVar = args[7];
    inBattle = 0;
    inBattle = gp->inBattle != inBattle;
    switch (kind) {
    case 0:
        anim = "MOBJ_FlowBrickBlock";
        break;
    case 1:
        anim = "MOBJ_block_ky";
        break;
    case 2:
        anim = "MOBJ_bigblock_01";
        break;
    case 3:
    default:
        anim = "MOBJ_bigblock_02";
        break;
    }
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_koopa_brickblk;
    mobj->event = val;
    mobj->flags |= 4;
    mobj->flags |= 0x3c000000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobj->typeData[0] = item;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    if (evtGetValue(event, flagVar)) {
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        mobj->state = 99;
    }
    return EVT_RETURN_DONE;
}

s32 mobj_koopa_badgeblk(MapObjectEntry* mobj) {
    ItemEntry* item;
    BOOL ok;
    marioGetPtr();
    ok = FALSE;
    switch (mobj->state) {
    case 0:
        if (!(mobj->flags & 8)) {
            break;
        }
        if (mobj->hitFlag != 0x4000000) {
            mobj->flags &= ~8;
            break;
        }
        if (mobj->flags & 0x800) {
            mobjHitEntry(mobj, 0);
        }
        animPoseSetAnim(mobj->poseId, "A_1", 1);
        if ((s32*)mobj->event != NULL) {
            mobjRunEvent(mobj, (s32*)mobj->event);
        } else if (mobj->typeData[0] == 0x59) {
            itemEntry(mobj->position.x, 12.5f + mobj->position.y, mobj->position.z, mobj->name, mobj->typeData[0], 4, -1, kpa_powerup_evt);
            item = itemNameToPtr(mobj->name);
            itemStatusOn(item, 0x10);
            itemFlagOn(item, 0x14);
            item->timer = 10000;
            item->bounceFactor = 0.45f;
            item->gravityScale = 0.7f;
            item->maxBounces = 0;
            item->velocityY = 500.0f;
            item->speed = 120.0f;
            item->direction = 90;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_MEAT_APPEAR1), &mobj->position);
        } else {
            kpaAddCoin(1);
            itemEntry(mobj->position.x, mobj->position.y, mobj->position.z, NULL, ITEM_COIN, 0xf, -1, NULL);
            evtSetValue(NULL, mobj->flagVar, 1);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_COUNT_COIN1), &mobj->position);
        }
        if (mobj->hitFlag == 0x4000000) {
            npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
        }
        mobj->state++;
        break;
    case 1:
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_2", 1);
        mobj->flags &= ~8;
        mobj->state = 90;
        strcpy(mobj->animName, "MOBJ_Block");
        break;
    case 90:
        if (mobj->flags & 8) {
            switch (kpaGetLevel()) {
            case 3:
                ok = TRUE;
                break;
            case 2: break;
            }
            if (!ok) {
                mobj->flags &= ~8;
                break;
            }
            if (mobj->hitFlag == 0x4000000) {
                npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
            }
            if (mobj->hitObj[0].hitObject) {
                mobj->hitObj[0].hitObject->flags |= 1;
            }
            mobj->flags |= 2;
            mobj->state++;
        }
        break;
    case 91:
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_BREAK1), &mobj->position);
        effMObjBrokenEntry(2, mobj->position.x, 12.5f + mobj->position.y, 12.5f + mobj->position.z);
        mobj->state = 99;
        break;
    case 99:
        mobjDelete(mobj->name);
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_koopa_badgeblk) {
    s32 val;
    s32 item;
    EvtWord* args = event->args;
    s32 flagVar;
    int battleFlag; // FAKE: separate inBattle temp needed for the target register allocation
    const char* name;
    MapObjectEntry* mobj;
    const char* anim;
    f32 x, y, z;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    item = evtGetValue(event, args[4]);
    val = evtGetValue(event, args[5]);
    flagVar = args[6];
    battleFlag = gp->inBattle;
    battleFlag = battleFlag != 0;
    anim = "MOBJ_HatenaBlock";
    if (animGroupBaseAsync(anim, battleFlag, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_koopa_badgeblk;
    mobj->event = val;
    mobj->flags |= 4;
    mobj->flags |= 0x3c000000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobj->typeData[0] = item;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    if (evtGetValue(event, flagVar)) {
        mobj->state = 90;
        animPoseSetAnim(mobj->poseId, "S_2", 1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mobj_koopa_hidden_blk) {
    const char* name;
    const char* anim;
    s32 item;
    EvtWord* args = event->args;
    s32 inBattle;
    s32 flagVar;
    f32 x, y, z;
    s32 val;
    MapObjectEntry* mobj;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    item = evtGetValue(event, args[4]);
    val = evtGetValue(event, args[6]);
    flagVar = args[7];
    inBattle = gp->inBattle;
    inBattle = inBattle != 0;
    anim = "MOBJ_HiddenHatenaBlock";
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->flags |= 0x800;
    mobjHitEntry(mobj, 1);
    mobj->mainFunc = mobj_koopa_badgeblk;
    mobj->event = val;
    mobj->flags |= 4;
    mobj->flags |= 0x3c000000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobj->typeData[0] = item;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    if (evtGetValue(event, flagVar)) {
        mobj->state = 90;
        animPoseSetAnim(mobj->poseId, "S_2", 1);
    }
    return EVT_RETURN_DONE;
}

s32 mobj_koopa_blk(MapObjectEntry* mobj) {
    BOOL ok = FALSE;
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            switch (kpaGetLevel()) {
            case 3:
                ok = TRUE;
                break;
            case 2:
                break;
            }
            if (!ok) {
                mobj->flags &= ~8;
                break;
            }
            mobjRunEvent(mobj, (s32*)mobj->event);
            evtSetValue(NULL, mobj->flagVar, 1);
            if (mobj->hitFlag == 0x4000000) {
                npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
            }
            if (mobj->hitObj[0].hitObject) {
                mobj->hitObj[0].hitObject->flags |= 1;
            }
            mobj->flags |= 2;
            mobj->state = 10;
        }
        break;
    case 10:
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_BREAK1), &mobj->position);
        effMObjBrokenEntry(2, mobj->position.x, 12.5f + mobj->position.y, 12.5f + mobj->position.z);
        mobj->state = 99;
    case 99:
        mobjDelete(mobj->name);
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_koopa_blk) {
    s32 val;
    EvtWord* args = event->args;
    s32 flagVar;
    const char* name;
    MapObjectEntry* mobj;
    f32 x, y, z;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    val = evtGetValue(event, args[5]);
    flagVar = args[6];
    if (animGroupBaseAsync("MOBJ_Block", gp->inBattle != 0, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, "MOBJ_Block");
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_koopa_blk;
    mobj->event = val;
    mobj->flags |= 4;
    mobj->flags |= 0x3c000000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    return EVT_RETURN_DONE;
}

s32 mobj_koopa_dokan_evt(MapObjectEntry* mobj) {
    BOOL ok = FALSE;
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            switch (kpaGetLevel()) {
            case 0:
                break;
            case 1:
            case 2:
                if (mobj->hitFlag == 0x20000000) {
                    ok = TRUE;
                }
                break;
            case 3:
                ok = TRUE;
                break;
            }
            if (!ok) {
                mobj->flags &= ~8;
                break;
            }
            mobjRunEvent(mobj, (s32*)mobj->event);
            evtSetValue(NULL, mobj->flagVar, 1);
            if (mobj->hitFlag == 0x4000000) {
                npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
            }
            if (mobj->hitObj[0].hitObject) {
                mobj->hitObj[0].hitObject->flags |= 1;
            }
            mobj->flags |= 2;
            kpaAddScore(kpaMutekiCheck() ? 500 : 400);
            mobj->state = 10;
        }
        break;
    case 10:
        if (!strcmp(mobj->animName, "MOBJ_GreenPipe_ks")) {
            effMObjBrokenEntry(6, mobj->position.x, 25.0f + mobj->position.y, 25.0f + mobj->position.z);
        } else if (!strcmp(mobj->animName, "MOBJ_GreenPipe_kl")) {
            effMObjBrokenEntry(6, mobj->position.x, 7.5f + (25.0f + mobj->position.y), 25.0f + mobj->position.z);
        } else {
            effMObjBrokenEntry(6, mobj->position.x - 25.0f, 25.0f + mobj->position.y, 25.0f + mobj->position.z);
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_DOKAN_BREAK1), &mobj->position);
        mobj->state = 99;
        break;
    case 99:
        mobjDelete(mobj->name);
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_koopa_dokan) {
    s32 kind;
    s32 val;
    EvtWord* args = event->args;
    s32 flagVar;
    const char* name;
    const char* anim;
    MapObjectEntry* mobj;
    s32 inBattle;
    f32 x, y, z;
    kind = evtGetValue(event, args[0]);
    name = (const char*)evtGetValue(event, args[1]);
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    val = evtGetValue(event, args[5]);
    flagVar = args[6];
    inBattle = gp->inBattle != 0;
    switch (kind) {
    case 0:
        anim = "MOBJ_GreenPipe_ks";
        break;
    case 1:
        anim = "MOBJ_GreenPipe_kl";
        break;
    case 2:
        anim = "MOBJ_GreenPipe_ky";
        break;
    }
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobj->flags |= 6;
    mobj->position = (Vec){x, y, z};
    mobj->mainFunc = mobj_koopa_dokan_evt;
    mobj->event = val;
    mobj->extraEvent = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    mobj->flags |= 0x3c000000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(fire_func) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    s32 dur = evtGetValue(event, args[3]);
    EffectEntry* eff;
    f32 a, b;
    if (isFirstCall) {
        f32 t = (f32)irand(10) / 10.0f;
        event->userdatap[0] = effFireEntry(5, dur, x, y, z, 0.8f + t);
    }
    a = event->userdataf[1];
    b = event->userdataf[2];
    a = a + (1.0f - a) / 8.0f;
    b = b + (1.0f - b) / 8.0f;
    eff = event->userdatap[0];
    ((EffFireWork*)eff->userdata)->scaleX = a;
    ((EffFireWork*)eff->userdata)->scaleY = b;
    if (a >= 0.99f) {
        ((EffFireWork*)eff->userdata)->scaleX = 1.0f;
        ((EffFireWork*)eff->userdata)->scaleY = 1.0f;
        return EVT_RETURN_DONE;
    }
    event->userdataf[1] = a;
    event->userdataf[2] = b;
    return EVT_RETURN_BLOCK;
}

s32 mobj_koopa_sango_evt(MapObjectEntry* mobj) {
    BOOL ok = FALSE;
    EventEntry* ev;
    switch (mobj->state) {
    case 0:
        if (mobj->flags & 8) {
            switch (kpaGetLevel()) {
            case 0:
            case 1:
            case 2:
                if (mobj->hitFlag == 0x20000000) {
                    ok = TRUE;
                }
                break;
            case 3:
                ok = TRUE;
                break;
            }
            if (!ok) {
                mobj->flags &= ~8;
                break;
            }
            mobjRunEvent(mobj, (s32*)mobj->event);
            evtSetValue(NULL, mobj->flagVar, 1);
            if (mobj->hitFlag == 0x4000000) {
                npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
            }
            if (mobj->hitObj[0].hitObject) {
                mobj->hitObj[0].hitObject->flags |= 1;
            }
            mobj->flags |= 2;
            kpaAddScore(kpaMutekiCheck() ? 500 : 400);
            mobj->state = 10;
        }
        break;
    case 10:
        if (mobj->hitFlag == 0x20000000) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_BTL_DAMAGE_FIRE1), &mobj->position);
        }
        ev = evtEntry(evt_fire, 0, 0);
        ev->lwData[0] = (s32)(1024.0f * mobj->position.x) + 0xF24A7A80;
        ev->lwData[1] = (s32)(1024.0f * mobj->position.y) + 0xF24A7A80;
        ev->lwData[2] = (s32)(1024.0f * (10.0f + mobj->position.z)) + 0xF24A7A80;
        ev->lwData[3] = 90;
        mobj->state++;
        break;
    case 20:
        ev = evtEntry(evt_fire, 0, 0);
        ev->lwData[0] = (s32)(1024.0f * (10.0f + mobj->position.x)) + 0xF24A7A80;
        ev->lwData[1] = (s32)(1024.0f * (30.0f + mobj->position.y)) + 0xF24A7A80;
        ev->lwData[2] = (s32)(1024.0f * (10.0f + mobj->position.z)) + 0xF24A7A80;
        ev->lwData[3] = 80;
        mobj->state++;
        break;
    case 30:
        ev = evtEntry(evt_fire, 0, 0);
        ev->lwData[0] = (s32)(1024.0f * (mobj->position.x - 10.0f)) + 0xF24A7A80;
        ev->lwData[1] = (s32)(1024.0f * (60.0f + mobj->position.y)) + 0xF24A7A80;
        ev->lwData[2] = (s32)(1024.0f * (10.0f + mobj->position.z)) + 0xF24A7A80;
        ev->lwData[3] = 70;
        mobj->state++;
        break;
    case 98:
        effMObjBrokenEntry(7, mobj->position.x, mobj->position.y, 10.0f + mobj->position.z);
        mobj->state++;
        break;
    case 99:
        mobjDelete(mobj->name);
        break;
    default:
        mobj->state++;
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_koopa_sango) {
    s32 val;
    s32 flagVar;
    EvtWord* args = event->args;
    const char* name;
    MapObjectEntry* mobj;
    f32 x, y, z;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    val = evtGetValue(event, args[4]);
    flagVar = args[5];
    if (animGroupBaseAsync("MOBJ_sango", gp->inBattle != 0, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, "MOBJ_sango");
    mobj = mobjNameToPtr(name);
    mobj->flags |= 0x84;
    mobj->position = (Vec){x, y, z};
    mobj->mainFunc = mobj_koopa_sango_evt;
    mobj->event = val;
    mobj->extraEvent = 0;
    mobj->flags |= 0x3c000000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    return EVT_RETURN_DONE;
}

extern s32 psndSFXChk(s32 id);
extern void psndSFXOff(s32 id);

s32 mobj_koopa_fireber_dodai_evt(MapObjectEntry* mobj) {
    Mtx mtx;
    Mtx rot;
    char buf[0x20];
    char buf2[0x20];
    BOOL ok = FALSE;
    NpcEntry* npc;
    s32 i;
    MarioWork* mario = marioGetPtr();
    if (mobj->state < 10) {
        if (distABf(mario->position.x, mario->position.y, mobj->position.x, mobj->position.y) < 150.0f) {
            if (psndSFXChk(mobj->typeWork)) {
                mobj->typeWork = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_FIREBAR_ROUND1), &mobj->position);
            }
        } else if (psndSFXChk(mobj->typeWork) == 0) {
            psndSFXOff(mobj->typeWork);
            mobj->typeWork = -1;
        }
        PSMTXIdentity(mtx);
        PSMTXTransApply(mtx, mtx, -mobj->position.x, -mobj->position.y, -mobj->position.z);
        PSMTXRotRad(rot, 'Z', 0.017453292f * mobj->typeDataF[3]);
        PSMTXConcat(rot, mtx, mtx);
        PSMTXTransApply(mtx, mtx, mobj->position.x, mobj->position.y, mobj->position.z);
        for (i = 0; i < mobj->typeData[1]; i++) {
            sprintf(buf, "%s%2x", mobj->name, i);
            npc = npcNameToPtr(buf);
            PSMTXMultVec(mtx, &npc->position, &npc->position);
        }
    }
    switch (mobj->state) {
    case 0:
        if (!(mobj->flags & 8)) {
            break;
        }
        switch (kpaGetLevel()) {
        case 0:
        case 1:
        case 2:
            if (mobj->hitFlag == 0x20000000) {
                ok = TRUE;
            }
            break;
        case 3:
            ok = TRUE;
            break;
        }
        if (!ok) {
            mobj->flags &= ~8;
            break;
        }
        mobjRunEvent(mobj, (s32*)mobj->event);
        evtSetValue(NULL, mobj->flagVar, 1);
        if (mobj->hitObj[0].hitObject) {
            mobj->hitObj[0].hitObject->flags |= 1;
        }
        mobj->flags |= 2;
        kpaAddScore(kpaMutekiCheck() ? 500 : 400);
        mobj->state = 10;
        break;
    case 10:
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_BREAK1), &mobj->position);
        effMObjBrokenEntry(0, mobj->position.x, 12.5f + mobj->position.y, 12.5f + mobj->position.z);
        mobj->state++;
        break;
    case 11:
        mobj->position.y = -1000.0f;
        for (i = 0; i < mobj->typeData[1]; i++) {
            sprintf(buf, "%s%2x", mobj->name, i);
            npc = npcNameToPtr(buf);
            npc->moveTicks = 0;
            npc->jumpStartPos = npc->position;
            npc->jumpTargetPos.x = irand(101) / 200.0f;
            npc->jumpTargetPos.y = 6.0f - irand(101) / 75.0f;
            npc->jumpTargetPos.z = 4.8f;
            npc->moveTime = 2.0f;
            npc->jumpFlags |= 4;
        }
        if (psndSFXChk(mobj->typeWork) == 0) {
            psndSFXOff(mobj->typeWork);
            mobj->typeWork = -1;
        }
        mobj->state++;
        break;
    case 12:
        for (i = 0; i < mobj->typeData[1]; i++) {
            sprintf(buf, "%s%2x", mobj->name, i);
            npc = npcNameToPtr(buf);
            npc->moveOffset.x += npc->jumpTargetPos.x;
            npc->position.y += npc->jumpTargetPos.y;
            npc->moveOffset.z += npc->jumpTargetPos.z;
            npc->scale.x += 0.04f;
            if (npc->scale.x > 3.0f) {
                npc->scale.x = 3.0f;
            }
            npc->scale.z = npc->scale.y = npc->scale.x;
            npc->jumpTargetPos.y -= 0.2f;
        }
        if (npc->moveTime <= npc->moveElapsed) {
            mobj->state = 99;
        }
        break;
    case 99: {
        s32 j;
        for (j = 0; j < mobj->typeData[1]; j++) {
            sprintf(buf2, "%s%2x", mobj->name, j);
            npcDelete(npcNameToPtr(buf2));
        }
        mobjDelete(mobj->name);
        break;
    }
    }
    return 0;
}

USER_FUNC(evt_mobj_koopa_fireber_dodai) {
    const char* name;
    s32 dir, count, script, flagVar, i;
    EvtWord* args = event->args;
    MapObjectEntry* mobj;
    NpcEntry* npc;
    f32 x, y, z, spd, base, step;
    char buf[32];
    s32 inBattle;
    dir = evtGetValue(event, args[0]);
    name = (const char*)evtGetValue(event, args[1]);
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    count = evtGetValue(event, args[5]);
    spd = evtGetFloat(event, args[6]);
    script = evtGetValue(event, args[7]);
    flagVar = args[8];
    inBattle = gp->inBattle != 0;
    if (animGroupBaseAsync("MOBJ_fire_dodai", inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    if (animGroupBaseAsync("c_fireber", inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, "MOBJ_fire_dodai");
    mobj = mobjNameToPtr(name);
    mobj->flags |= 4;
    mobj->position = (Vec){x, y, z};
    mobj->mainFunc = mobj_koopa_fireber_dodai_evt;
    mobj->event = script;
    mobj->extraEvent = 0;
    mobj->flags |= 0x3c000000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    if (spd == 0.0f) {
        spd = 1.0f;
    }
    mobj->typeData[1] = count;
    mobj->typeDataF[3] = dir ? __fabsf(spd) : -__fabsf(spd);
    mobj->typeWork = 0;
    for (i = 0; i < count; i++) {
        sprintf(buf, "%s%2x", name, i);
        npcEntry(buf, "c_fireber");
        npc = npcNameToPtr(buf);
        npc->flags |= 0x8000000;
        npc->hazardFlags |= 0xf;
        npc->position.x = x;
        npc->position.y = 25.0f + y + 25.0f * i;
#ifdef VERSION_G8ME01
        npc->position.z = 175.0f + z + i;
#else
        npc->position.z = 175.0f + z;
#endif
        animPoseSetAnim(npc->poseId, "FRB_S_1", 1);
        npc->flags |= 0x40000;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(kpa_get_pole_name) {
    MapObjectEntry* mobj = event->thisMapObj;
    EvtWord* args = event->args;
    if (mobj != NULL) {
        evtSetValue(event, args[0], (EvtWord)mobj->name);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(kpa_get_hata_name) {
    MapObjectEntry* mobj = event->thisMapObj;
    EvtWord* args = event->args;
    if (mobj != NULL) {
        evtSetValue(event, args[0], (EvtWord)mobj->linkedName);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(kpa_get_level) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], kpaGetLevel());
    return EVT_RETURN_DONE;
}

USER_FUNC(kpa_get_width) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], (s32)marioGetPtr()->wPlayerCollisionBox.x);
    return EVT_RETURN_DONE;
}

USER_FUNC(kpa_score_to_str) {
    s32 score = evtGetValue(event, event->args[0]);
    s32 d[4];
    s32 i, j;
    d[0] = score / 1000;
    score = score % 1000;
    d[1] = score / 100;
    score = score % 100;
    d[2] = score / 10;
    d[3] = score % 10;
    i = 0;
    for (j = 0; j < 4; j++) {
        if (i != 0 || d[j] != 0) {
            psw->str[i] = d[j] + '0';
            i++;
        }
    }
    psw->str[4] = 0;
    return EVT_RETURN_DONE;
}

void kpa_disp_pole_score(CameraId camId, void* param) {
    Mtx trans;
    Mtx scale;
    PSMTXTrans(trans, psw->x, psw->y, 300.0f);
    PSMTXScale(scale, 0.5f, 0.5f, 0.5f);
    PSMTXConcat(trans, scale, trans);
    FontDrawStart();
    FontDrawColor((GXColor){255, 255, 255, 255});
    FontDrawStringMtx(trans, psw->str);
}

USER_FUNC(kpa_disp_pole_score_main) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    f32 spd = evtGetFloat(event, args[3]);
    if (isFirstCall) {
        psw->x = x;
        psw->y = y;
        psw->z = z;
    }
    if (psw->y < 230.0f) {
        psw->y += spd;
    } else {
        psw->y = 230.0f;
    }
    dispEntry(CAMERA_3D, 0.0f, 2, kpa_disp_pole_score, NULL);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(kpa_disp_pole_init) {
    psw = _mapAlloc(0x14);
    psw->str[0] = 0;
    psw->z = 0.0f;
    psw->y = 0.0f;
    psw->x = 0.0f;
    return EVT_RETURN_DONE;
}

USER_FUNC(kpa_clear_jump_data) {
    MarioWork* mario = marioGetPtr();
    mario->airTimer = 0;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(kpa_pole)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_cam_letter_box_onoff, 0, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_cam_y_on)
    CALL(kpa_disp_pole_init)
    CALL(kpa_clear_jump_data)
    CALL(kpa_get_hata_name, LW(15))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(kpa_chg_pose, STRING("KPA2_G_1"))
    CALL(evt_mario_get_pos, 0, LW(1), LW(2), LW(3))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_POST_JUMP1"), LW(1), LW(2), LW(3), 0)
    CALL(evt_mobj_get_x_position, LW(15), LW(4))
    CALL(kpa_get_width, LW(5))
    MULTIPLY_FLOAT(LW(5), FLOAT(0.5996094f))
    SUBTRACT_FLOAT(LW(4), LW(5))
    CALL(evt_mario_set_pos, LW(4), LW(2), LW(3))
    CALL(kpa_score_to_str, LW(0))
    SET_FLOAT_VALUE(LW(14), FLOAT(2.0f))
    SET_FLOAT_VALUE(LW(13), FLOAT(2.0f))
    SET_FLOAT_VALUE(LW(7), FLOAT(50.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_mobj_get_position, LW(15), LW(3), LW(4), LW(5))
        ADD_FLOAT(LW(3), FLOAT(7.5f))
        CALL(kpa_disp_pole_score_main, LW(3), LW(7), LW(5), LW(13))
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(1), LW(2), LW(3))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_POST_SLIDE1"), LW(1), LW(2), LW(3), LW(10))
    BEGIN_INLINE_EVENT_GET_ID(LW(6))
        LOOP(0)
            CALL(evt_mobj_get_y_position, LW(15), LW(4))
            SUBTRACT_FLOAT(LW(4), LW(14))
            CALL(evt_mobj_set_y_position, LW(15), LW(4))
            WAIT_FRAMES(1)
            IF_INT_LESS_OR_EQUAL(LW(4), 25)
                CALL(evt_mobj_set_y_position, LW(15), FLOAT(25.0f))
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SUBTRACT_FLOAT(LW(1), FLOAT(2.0f))
        CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(1), FLOAT(25.0f))
            CALL(evt_mario_set_pos, LW(0), FLOAT(25.0f), LW(2))
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(0)
        IS_EVENT_RUNNING(LW(6), LW(7))
        IF_INT_EQUAL(LW(7), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(10))
    WAIT_MS(200)
    CALL(evt_cam_get_pos, 4, LW(4), LW(5), LW(6))
    CALL(evt_cam_get_at, 4, LW(7), LW(8), LW(9))
    CALL(evt_cam3d_evt_set, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9), 0, 11)
    WAIT_FRAMES(1)
    CALL(kpa_get_width, LW(3))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    MULTIPLY_FLOAT(LW(3), FLOAT(1.1992188f))
    ADD_FLOAT(LW(0), LW(3))
    CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_cam3d_event_from_road, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
    CALL(evt_cam3d_evt_set, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9), 300, 0)
    WAIT_MS(200)
    CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_mario_set_mov_spd, 0)
    WAIT_MS(200)
    ADD(LW(0), 20)
    CALL(evt_mario_set_pos, LW(0), 0, 10)
    WAIT_FRAMES(1)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_cam3d_evt_off, 200, 11)
    WAIT_MS(200)
    RETURN
EVT_END()

EVT_BEGIN(kpa_pole_move)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_cam_letter_box_onoff, 0, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(kpa_get_pole_name, LW(14))
    CALL(kpa_get_hata_name, LW(15))
    CALL(evt_mobj_get_x_position, LW(14), LW(0))
    CALL(evt_mobj_get_y_position, LW(14), LW(1))
    CALL(evt_mobj_get_z_position, LW(14), LW(2))
    SET_FLOAT_VALUE(LW(6), FLOAT(2.0f))
    SET_FLOAT_VALUE(LW(7), FLOAT(9.0f))
    SET_FLOAT_VALUE(LW(8), FLOAT(5.7998047f))
    CALL(evt_mobj_get_x_position, LW(15), LW(3))
    CALL(evt_mobj_get_y_position, LW(15), LW(4))
    CALL(evt_mobj_get_z_position, LW(15), LW(5))
    SET_FLOAT_VALUE(LW(9), FLOAT(2.0f))
    SET_FLOAT_VALUE(LW(11), FLOAT(5.7998047f))
    SET_FLOAT_VALUE(LW(12), FLOAT(1.0f))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_POST_THROW1"), LW(0), LW(1), LW(2), LW(13))
    LOOP(100)
        ADD_FLOAT(LW(0), LW(6))
        ADD_FLOAT(LW(1), LW(7))
        ADD_FLOAT(LW(2), LW(8))
        ADD_FLOAT(LW(3), LW(9))
        SET_FLOAT_VALUE(LW(4), FLOAT(180.0f))
        MULTIPLY_FLOAT(LW(4), LW(12))
        ADD_FLOAT(LW(4), LW(1))
        ADD_FLOAT(LW(5), LW(11))
        CALL(evt_mobj_set_position, LW(14), LW(0), LW(1), LW(2))
        CALL(evt_mobj_set_scale, LW(14), LW(12), LW(12), LW(12))
        CALL(evt_mobj_set_position, LW(15), LW(3), LW(4), LW(5))
        CALL(evt_mobj_set_scale, LW(15), LW(12), LW(12), LW(12))
        ADD_FLOAT(LW(6), FLOAT(0.0f))
        ADD_FLOAT(LW(7), FLOAT(-0.7998047f))
        ADD_FLOAT(LW(8), FLOAT(0.0f))
        ADD_FLOAT(LW(9), FLOAT(0.0f))
        ADD_FLOAT(LW(11), FLOAT(0.0f))
        ADD_FLOAT(LW(12), FLOAT(0.0390625f))
        IF_INT_GREATER(LW(12), 3)
            SET_FLOAT_VALUE(LW(12), FLOAT(3.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(13))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(kpa_move_to_toride)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_wait_landon)
    SET_VALUE(LSWF(5), 1)
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("kpa_06"))
        CALL(evt_mapobj_get_position, STRING("toride"), LW(0), LW(1), LW(2))
        ELSE
        CALL(evt_mapobj_get_position, STRING("toride_"), LW(0), LW(1), LW(2))
    END_IF
    CALL(kpa_get_level, LW(6))
    SWITCH(LW(6))
        CASE_LESS_OR_EQUAL(1)
        ADD(LW(0), 100)
        CASE_EQUAL(2)
        CALL(kpa_get_width, LW(7))
        MULTIPLY_FLOAT(LW(7), FLOAT(1.1992188f))
        SUBTRACT_FLOAT(LW(0), LW(7))
        CASE_EQUAL(3)
        CALL(kpa_get_width, LW(7))
        MULTIPLY_FLOAT(LW(7), FLOAT(1.1992188f))
        SUBTRACT_FLOAT(LW(0), LW(7))
    END_SWITCH
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_mario_mov_pos2, LW(0), LW(5), 90)
    IF_INT_GREATER_OR_EQUAL(LW(6), 2)
        CALL(evt_mario_get_pos, 0, LW(1), LW(2), LW(3))
        CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_STRUGGLE1"), LW(1), LW(2), LW(3), LW(10))
        CALL(evt_mario_dispflag_onoff, 1, 2)
        WAIT_MS(2000)
        CALL(evt_mario_dispflag_onoff, 0, 2)
        CALL(evt_snd_sfxoff, LW(10))
        CALL(kpa_chg_pose, STRING("KPA2_S_1"))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

s32 mobj_koopa_pole_evt(MapObjectEntry* mobj);

s32 mobj_koopa_pole_evt(MapObjectEntry* mobj) {
    MarioWork* mario = marioGetPtr();
    s32 score;
    s32 next = 0;
    s32 off;
    s32 i;
    switch (mobj->state) {
    case 0:
        if (!(mobj->flags & 8)) {
            break;
        }
        if (mario->characterId != CHARACTER_BOWSER) {
            mobj->flags &= ~8;
            break;
        }
        switch (kpaGetLevel()) {
        case 0:
        case 1:
            if (mario->position.y < 25.0f) {
                mobj->flags &= ~8;
                break;
            }
            next = 10;
            break;
        case 2:
        case 3:
            next = 50;
            break;
        }
        if (next == 0) {
            break;
        }
        for (i = 0, off = 0; i < npcGetWorkPtr()->total; off += 0x340, i++) {
            NpcEntry* npc = (NpcEntry*)((char*)npcGetWorkPtr()->entries + off);
            u32 flags = (u32)npc->flags;
            if ((flags & 1) && !(npc->jumpFlags & 4) && (flags & 0x8000000) && (flags & 8)) {
                npcGroupDead(npc, 0x20);
            }
        }
        marioKeyOff();
        evtSetValue(NULL, mobj->flagVar, 1);
        mario->kpaNoHitTimer = 0;
        mobj->state = next;
        break;
    case 10:
        mario = marioGetPtr();
        score = 0;
        if (kpaGetLevel() == 3) {
            kpaAddScore(2000);
        } else if (kpaGetLevel() >= 2) {
            kpaAddScore(1000);
        } else {
            if (mario->position.y >= 170.0f) {
                score = 1000;
            } else if (mario->position.y >= 100.0f) {
                score = 500;
            } else if (mario->position.y >= 50.0f) {
                score = 200;
            } else {
                score = 100;
            }
            kpaAddScore(score);
        }
        mobjRunEvent(mobj, kpa_pole)->lwData[0] = score;
        mobj->state++;
        break;
    case 11:
        if (evtCheckID(mobj->eventId) != 1) {
            mobj->state = 90;
        }
        break;
    case 50:
        mario = marioGetPtr();
        if (kpaGetLevel() == 3) {
            kpaAddScore(2000);
        } else if (kpaGetLevel() >= 2) {
            kpaAddScore(1000);
        } else {
            if (mario->position.y >= 170.0f) {
                score = 1000;
            } else if (mario->position.y >= 100.0f) {
                score = 500;
            } else if (mario->position.y >= 50.0f) {
                score = 200;
            } else {
                score = 100;
            }
            kpaAddScore(score);
        }
        mobjRunEvent(mobj, kpa_pole_move);
        mobj->state++;
        break;
    case 51:
        if (evtCheckID(mobj->eventId) != 1) {
            mobj->state = 90;
        }
        break;
    case 90:
        mobjRunEvent(mobj, kpa_move_to_toride);
        mobj->state++;
        break;
    case 91:
        if (evtCheckID(mobj->eventId) != 1) {
            mobj->eventId = 0;
            mobj->state++;
        }
        break;
    case 92:
        mobjRunEvent(mobj, (s32*)mobj->extraEvent);
        mobj->state++;
        break;
    case 93:
        if (evtCheckID(mobj->eventId) == 1) {
            break;
        }
        mobj->eventId = 0;
        mobj->state++;
    case 94:
        mobjRunEvent(mobj, (s32*)mobj->event);
        mobj->state++;
        break;
    case 95:
        break;
    }
    return 0;
}

USER_FUNC(evt_mobj_koopa_pole) {
    s32 inBattle;
    s32 val2;
    s32 val;
    const char* name;
    EvtWord* args = event->args;
    s32 flagVar;
    MapObjectEntry* mobj;
    MapObjectEntry* hata;
    f32 x, y, z;
    inBattle = gp->inBattle != 0;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    val = evtGetValue(event, args[4]);
    val2 = evtGetValue(event, args[5]);
    flagVar = args[6];
    inBattle = gp->inBattle != 0;
    if (animGroupBaseAsync("MOBJ_goal_pole", inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    if (animGroupBaseAsync("MOBJ_goal_hata", inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, "MOBJ_goal_pole");
    mobj = mobjNameToPtr(name);
    mobj->flags |= 0x84;
    mobj->position = (Vec){x, y, z};
    mobj->mainFunc = mobj_koopa_pole_evt;
    mobj->event = val;
    mobj->extraEvent = val2;
    mobj->flags |= 0x1c280000;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    strcpy(mobj->linkedName, name);
    strcat(mobj->linkedName, "_h");
    mobjEntry(mobj->linkedName, "MOBJ_goal_hata");
    hata = mobjNameToPtr(mobj->linkedName);
    hata->position = (Vec){x, 180.0f + y, z};
    hata->mainFunc = NULL;
    hata->event = 0;
    hata->extraEvent = 0;
    hata->flagVar = 0;
    hata->state = 0;
    strcpy(hata->linkedName, name);
    if (hata->hitObj[0].hitObject) {
        hata->hitObj[0].hitObject->flags |= 1;
    }
    return EVT_RETURN_DONE;
}

