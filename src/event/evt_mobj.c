// Script commands and main functions for map objects (mobj): blocks, switches, save blocks,
// item boxes, jump stands, locks and Bowser's side-scrolling stage objects.

#include "version.h"
#include "evt/evt_cmd.h"
#include "evt/evt_story.h"
#include "evt/evt_sub.h"
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
#include "driver/imgdrv.h"
#include "driver/offscreendrv.h"
#include "driver/camdrv.h"
#include "system.h"
#include "driver/npcdrv.h"
#include "koopa_motion.h"
#include "eff/eff_mobj_broken.h"
static s32 mobj_brickblk(MapObjectEntry* mobj);
static s32 mobj_signboard(MapObjectEntry* mobj);
static s32 mobj_lock(MapObjectEntry* mobj);


// The objects JP, US and the US September demo strip (the A/B ride switches, red tornado switch, arrow,
// trap floor and breaking rock) stay in the early demos, the PAL demo and PAL.
#if VERSION >= VERSION_DEMO_EU
#define MOBJ_EXTRA_OBJS 1
#else
#if VERSION <= VERSION_DEMO_JP
#define MOBJ_EXTRA_OBJS 1
#else
#define MOBJ_EXTRA_OBJS 0
#endif
#endif

/**
 * Resolves a map object name for the script commands below;
 * "me" means the object whose event is running (event->thisMapObj).
 */
static inline MapObjectEntry* mobjPtrOrSelf(EventEntry* event, const char* name) {
    return strcmp(name, "me") ? mobjNameToPtr(name) : event->thisMapObj;
}

/** Same lookup as mobjPtrOrSelf with the test written the other way round; evt_mobj_set_scale needs this form. */
static inline MapObjectEntry* mobjPtrOrSelf2(EventEntry* event, const char* name) {
    return !strcmp(name, "me") ? event->thisMapObj : mobjNameToPtr(name);
}

/** Shakes the 3D camera vertically (0.01) for 200 ms; started when Mario hits a timer switch. */
static EVT_BEGIN(evt_shake)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.009765625f), 200)
    RETURN
EVT_END()
/** Animation names of the coloured switch blocks, in the order mobj_switch_float_blk steps through them. */
const char* _switch_anim[] = {"A_1", "A_2", "A_3", "A_4", "A_5", "S_1"};

// FAKE: the hit pointer is tested as a signed word (cmpwi); a pointer or BOOL test compiles to cmplwi
static inline EvtWord blkHitCheck(MapObjectEntry* m) {
    f32 hx, hy, hz, dist, nx, ny, nz;
    dist = 1.0f + (f32)__fabs(m->velocity.y);
    return (EvtWord)hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, m->position.x, m->position.y + 0.5f, m->position.z, 0.0f, -1.0f, 0.0f);
}

/**
 * True when the animation name is "A_1" to "A_3" (an 'A' and a digit 1-3 in the third character);
 * used by the switch-block code to tell its hit animations from the others.
 */
static inline BOOL isAnimA13(const s8* a) {
    if (a[0] == 'A' && a[2] >= '1' && a[2] <= '3') {
        return TRUE;
    }
    return FALSE;
}

// FAKE: itemEntry declared with the position first: the original calls load x/y/z before the other arguments
// (same registers by the EABI); calls in the definition's order and an inline wrapper both load them last
extern ItemEntry* itemEntry(f32 x, f32 y, f32 z, const char* name, s32 itemId, u16 type, s32 collectFlag,
                            EvtWord* evtCode);


/** Event that runs kpa_powerup with LW(0)-LW(2); started when Bowser grabs a power-up in his side-scrolling stages. */
#if VERSION >= VERSION_JP
                            EVT_BEGIN(kpa_powerup_evt)
                                CALL(kpa_powerup, LW(0), LW(1), LW(2))
                                RETURN
                            EVT_END()
#endif

/** Event that runs fire_func with LW(0)-LW(3) to spawn a fire effect for Bowser's stage objects. */
#if VERSION >= VERSION_JP
EVT_BEGIN(evt_fire)
    CALL(fire_func, LW(0), LW(1), LW(2), LW(3))
    RETURN
EVT_END()
#endif


// ---- entry, delete and property functions ----

#if VERSION <= VERSION_DEMO_JP
extern EvtWord kpa_powerup_evt[];
extern EvtWord evt_fire[];
#endif

/** Creates a bare map object. args: name, animation group. Waits (blocks) until the animation group has loaded. */
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

/** Deletes the map object named by args[0]. */
USER_FUNC(evt_mobj_delete) {
    mobjDelete((const char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

/**
 * Looks up the map object named by args[0] without asserting;
 * writes its pointer, or 0 if it does not exist, to args[1].
 */
USER_FUNC(evt_mobj_check) {
    EvtWord* args = event->args;
    MapObjectEntry* mobj = mobjNameToPtrNoAssert((const char*)evtGetValue(event, args[0]));
    evtSetValue(event, args[1], (EvtWord)mobj);
    return EVT_RETURN_DONE;
}

/** Sets (args[0] nonzero) or clears the flag bits args[2] (a literal mask) on the map object named by args[1]. */
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

/** Writes the animation group name of the map object args[0] (its kind, e.g. "MOBJ_SaveBlock") to args[1]. */
USER_FUNC(evt_mobj_get_kindname) {
    EvtWord* args = event->args;
    MapObjectEntry* mobj = mobjNameToPtr((const char*)evtGetValue(event, args[0]));
    evtSetValue(event, args[1], (EvtWord)mobj->animName);
    return EVT_RETURN_DONE;
}

/** Sets the scale of the map object args[0] ("me" allowed) to args[1..3] and marks its matrix for recalculation. */
USER_FUNC(evt_mobj_set_scale) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    MapObjectEntry* mobj = mobjPtrOrSelf2(event, name);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    Vec scale = {0.0f, 0.0f, 0.0f};
    scale.x = x;
    scale.y = y;
    scale.z = z;
    mobj->scale = scale;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    return EVT_RETURN_DONE;
}

/** Reads the position of the map object args[0] ("me" allowed) into the float variables args[1..3]. */
USER_FUNC(evt_mobj_get_position) {
    EvtWord* args = event->args;
    MapObjectEntry* mobj = mobjPtrOrSelf(event, (const char*)evtGetValue(event, args[0]));
    evtSetFloat(event, args[1], mobj->position.x);
    evtSetFloat(event, args[2], mobj->position.y);
    evtSetFloat(event, args[3], mobj->position.z);
    return EVT_RETURN_DONE;
}

/** Reads the X position of the map object args[0] ("me" allowed) into args[1]. */
USER_FUNC(evt_mobj_get_x_position) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    MapObjectEntry* mobj = mobjPtrOrSelf(event, name);
    evtSetFloat(event, args[1], mobj->position.x);
    return EVT_RETURN_DONE;
}

/** Reads the Y position of the map object args[0] ("me" allowed) into args[1]. */
USER_FUNC(evt_mobj_get_y_position) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    MapObjectEntry* mobj = mobjPtrOrSelf(event, name);
    evtSetFloat(event, args[1], mobj->position.y);
    return EVT_RETURN_DONE;
}

/** Reads the Z position of the map object args[0] ("me" allowed) into args[1]. */
USER_FUNC(evt_mobj_get_z_position) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    MapObjectEntry* mobj = mobjPtrOrSelf(event, name);
    evtSetFloat(event, args[1], mobj->position.z);
    return EVT_RETURN_DONE;
}

/** Moves the map object args[0] ("me" allowed) to (args[1], args[2], args[3]) and recalculates its matrix. */
USER_FUNC(evt_mobj_set_position) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    MapObjectEntry* mobj = mobjPtrOrSelf(event, name);
    Vec pos = {0.0f, 0.0f, 0.0f};
    pos.x = x;
    pos.y = y;
    pos.z = z;
    mobj->position = pos;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    return EVT_RETURN_DONE;
}

/** Sets only the X position of the map object args[0] ("me" allowed) to args[1]. */
USER_FUNC(evt_mobj_set_x_position) {
    EvtWord* args = event->args;
    MapObjectEntry* mobj = mobjPtrOrSelf(event, (const char*)evtGetValue(event, args[0]));
    Vec pos = mobj->position;
    pos.x = evtGetFloat(event, args[1]);
    mobj->position = pos;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    return EVT_RETURN_DONE;
}

/** Sets only the Y position of the map object args[0] ("me" allowed) to args[1]. */
USER_FUNC(evt_mobj_set_y_position) {
    EvtWord* args = event->args;
    MapObjectEntry* mobj = mobjPtrOrSelf(event, (const char*)evtGetValue(event, args[0]));
    Vec pos = mobj->position;
    pos.y = evtGetFloat(event, args[1]);
    mobj->position = pos;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    return EVT_RETURN_DONE;
}

/** Sets only the Z position of the map object args[0] ("me" allowed) to args[1]. */
USER_FUNC(evt_mobj_set_z_position) {
    EvtWord* args = event->args;
    MapObjectEntry* mobj = mobjPtrOrSelf(event, (const char*)evtGetValue(event, args[0]));
    Vec pos = mobj->position;
    pos.z = evtGetFloat(event, args[1]);
    mobj->position = pos;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    return EVT_RETURN_DONE;
}

/**
 * Sets MOBJ_FLAG_EXEC_CANCEL on the map object args[0] ("me" allowed): a cancel request that the switch-block,
 * lock and treasure-chest main functions read and clear.
 */
USER_FUNC(evt_mobj_exec_cancel) {
    MapObjectEntry* mobj = mobjPtrOrSelf(event, (const char*)evtGetValue(event, event->args[0]));
    mobj->flags |= MOBJ_FLAG_EXEC_CANCEL;
    return EVT_RETURN_DONE;
}

/**
 * Sets the gravity (args[1]) and bounce factor (args[2]) used when the map object args[0] falls,
 * e.g. an item block or switch dropped onto the floor.
 */
USER_FUNC(evt_mobj_set_gravity_bound) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 a = evtGetFloat(event, args[1]);
    f32 b = evtGetFloat(event, args[2]);
    MapObjectEntry* mobj = mobjPtrOrSelf(event, name);
    mobj->gravity = a;
    mobj->bounce = b;
    return EVT_RETURN_DONE;
}

/** Plays the animation args[1] (e.g. "A_1", "S_1") on the map object args[0] ("me" allowed). */
USER_FUNC(evt_mobj_set_anim) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    const char* anim = (const char*)evtGetValue(event, args[1]);
    MapObjectEntry* mobj = mobjPtrOrSelf(event, name);
    animPoseSetAnim(mobj->poseId, anim, 1);
    return EVT_RETURN_DONE;
}

/** Blocks the script until the current animation of the map object args[0] has played through once. */
USER_FUNC(evt_mobj_wait_animation_end) {
    const char* name = (const char*)evtGetValue(event, event->args[0]);
    MapObjectEntry* mobj = mobjPtrOrSelf(event, name);
    if (animPoseGetLoopTimes(mobj->poseId) > 1.0f) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/** Moves the map object args[0] to camera args[1] (CameraId), e.g. to draw it in a 2D/overlay camera. */
USER_FUNC(evt_mobj_set_camid) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    u8 cam = evtGetValue(event, args[1]);
    MapObjectEntry* mobj = mobjPtrOrSelf(event, name);
    mobj->camId = cam;
    return EVT_RETURN_DONE;
}

/** Enables (args[1] nonzero) or disables the hit event of the map object args[0], so hitting it does nothing. */
USER_FUNC(evt_mobj_hitevt_onoff) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);
    MapObjectEntry* mobj = mobjPtrOrSelf(event, name);
    if (on) {
        mobj->flags &= ~MOBJ_FLAG_HITEVT_OFF;
    } else {
        mobj->flags |= MOBJ_FLAG_HITEVT_OFF;
    }
    return EVT_RETURN_DONE;
}

/** Enables (args[1] nonzero) or disables the collision of the map object args[0] by switching its hit groups. */
USER_FUNC(evt_mobj_hit_onoff) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);
    MapObjectEntry* mobj = mobjNameToPtr(name);
    s32 i;

    // The original tests the address of the name array, which is never NULL, so
    // both hit groups are switched even when a slot is unused.
    if (on) {
        for (i = 0; i < 2; i++) {
            if (mobj->hitObj[i].name) {
                hitGrpFlagOff(mobj->hitObj[i].name, HIT_FLAG_OFF);
            }
        }
    } else {
        for (i = 0; i < 2; i++) {
            if (mobj->hitObj[i].name) {
                hitGrpFlagOn(mobj->hitObj[i].name, HIT_FLAG_OFF);
            }
        }
    }
    return EVT_RETURN_DONE;
}

// ---- switches ----

/**
 * Main function of the one-shot blue switches: when hit it runs the object's event,
 * plays the press animation and sound, then sparkles, sets the save flag (flagVar) to 1 and deletes itself.
 * Already-set switches start at state 99 and vanish at once.
 */
static s32 mobj_switch_blue(MapObjectEntry* mobj) {
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            mobjRunEvent(mobj, (s32*)mobj->event);
            if (mobj->hitFlag == MOBJ_FLAG_HIT_STAND || mobj->hitFlag == MOBJ_FLAG_HIT_SPIN_JUMP) {
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
        mobj->state = 99;
    case 99:
        mobjDelete(mobj->name);
        break;
    }
    return 0;
}

/**
 * Places a one-shot floor switch hit by hammer, stomp or partner.
 * args: kind (0 blue, 1 black, 2 blue without shadow, HOBJ model), name, x, y, z, event to run,
 * flag variable set when pressed (already set: not placed).
 */
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
    mobj->flags |= (MOBJ_FLAG_UPDATE_MTX | MOBJ_FLAG_GRAVITY);
    mobj->flags |= (MOBJ_FLAG_HIT_HAMMER | MOBJ_FLAG_HIT_STAND | MOBJ_FLAG_HIT_PARTY);
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (kind == 2) {
        mobj->flags |= MOBJ_FLAG_NO_SHADOW;
    }
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

/**
 * Places a blue switch block that is pressed by hitting it from below.
 * Same args as evt_mobj_switch_blue; args[0] is ignored.
 */
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

    // FAKE: gp->inBattle is loaded into inBattle before the test; `inBattle = gp->inBattle != 0` changes the object
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= MOBJ_FLAG_HIT_HEAD;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

/**
 * Places a blue tornado switch, pressed only by a spin jump (Paper Mode tornado jump).
 * Same args as evt_mobj_switch_blue; args[0] is ignored.
 */
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

    // FAKE: gp->inBattle is loaded into inBattle before the test; `inBattle = gp->inBattle != 0` changes the object
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= MOBJ_FLAG_HIT_SPIN_JUMP;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

/**
 * Main function of the red (reusable) switches:
 * every hit runs the object's event and replays the press animation; the switch stays and can be pressed again.
 */
static s32 mobj_switch_red(MapObjectEntry* mobj) {
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            mobjRunEvent(mobj, (s32*)mobj->event);
            if (mobj->hitFlag == MOBJ_FLAG_HIT_STAND || mobj->hitFlag == MOBJ_FLAG_HIT_SPIN_JUMP) {
                marioChgMot(MARIO_MOTION_JUMP_SW);
            }
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BIKKURI_SWITCH_ON1), &mobj->position);
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            mobj->flags &= ~MOBJ_FLAG_HIT;
        }
        break;
    case 1:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BIKKURI_SWITCH_ON1), &mobj->position);
            if (mobj->hitFlag == MOBJ_FLAG_HIT_STAND) {
                marioChgMot(MARIO_MOTION_JUMP_SW);
            }
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            mobj->flags &= ~MOBJ_FLAG_HIT;
        }
        if (!(animPoseGetLoopTimes(mobj->poseId) < 1.0f)) {
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            mobj->flags &= ~MOBJ_FLAG_HIT;
            mobj->state = 0;
        }
        break;
    }
    return 0;
}

/**
 * Places a reusable floor switch.
 * args: kind (0 red, 1 white), name, x, y, z, event to run each press, flag variable (stored but not read).
 */
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
    mobj->flags |= (MOBJ_FLAG_UPDATE_MTX | MOBJ_FLAG_GRAVITY);
    mobj->flags |= (MOBJ_FLAG_HIT_HAMMER | MOBJ_FLAG_HIT_STAND | MOBJ_FLAG_HIT_PARTY);
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

/** Places a reusable red switch block pressed from below. Same args as evt_mobj_switch_red; args[0] is ignored. */
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

    // FAKE: gp->inBattle is loaded into inBattle before the test; `inBattle = gp->inBattle != 0` changes the object
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= MOBJ_FLAG_HIT_HEAD;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

#if MOBJ_EXTRA_OBJS
/** Places a reusable red tornado switch pressed by a spin jump; only built in the early demos, the PAL demo and PAL. args[0] ignored. */
USER_FUNC(evt_mobj_tornadoswitch_red) {
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
    anim = "MOBJ_RedTornadoSwitch";
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_switch_red;
    mobj->event = val;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= MOBJ_FLAG_HIT_SPIN_JUMP;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

#endif

/**
 * Main function of the paired timer switches: pressing one starts a countdown of typeWork frames;
 * if the other switch of the pair is pressed before it runs out, the event runs.
 * The blue pair then sparkles, sets both flags and deletes itself; the red pair resets for another try.
 * The early demos do not stop on timeout first.
 */
static s32 mobj_timerswitch(MapObjectEntry* mobj) {
    MapObjectEntry* other = mobjNameToPtr(mobj->linkedName);
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            if (mobj->hitFlag == MOBJ_FLAG_HIT_STAND) {
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
            mobj->flags &= ~MOBJ_FLAG_HIT;
        }
        break;
    case 1:
        if (mobj->flags & MOBJ_FLAG_HIT) {
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
        mobj->flags &= ~MOBJ_FLAG_HIT;
        break;
    case 2:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            animPoseSetAnim(other->poseId, "S_1", 1);
            mobj->state = 0;
            break;
        }
        mobj->timer--;
        if (mobj->timer <= 0) {
            mobj->flags &= ~MOBJ_FLAG_HIT;
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            animPoseSetAnim(other->poseId, "S_1", 1);
            mobj->state = 0;
#if VERSION >= VERSION_JP
            break;
#endif
        }
        if (other->state == 2) {
#if VERSION >= VERSION_JP
            if (marioKeyOffChk() || marioCtrlOffChk()) {
                mobj->state = 0;
                break;
            }
#endif
            mobjRunEvent(mobj, (s32*)mobj->event);
            if (!strcmp(mobj->animName, "MOBJ_RedTimerSwitch")) {
                mobj->flags &= ~MOBJ_FLAG_HIT;
                other->flags &= ~MOBJ_FLAG_HIT;
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

/**
 * Places a pair of timer switches linked to each other.
 * args: kind (0 blue, 1 red), name1, x1, y1, z1, name2, x2, y2, z2, time in frames, event,
 * flag variable (a blue pair already set is not placed).
 */
USER_FUNC(evt_mobj_timerswitch) {
    const char* names[2];
    s32 flagVar;
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
    flagVar = args[11];
    if (kind == 0 && evtGetValue(event, flagVar)) {
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
        mobjs[i]->flags |= (MOBJ_FLAG_UPDATE_MTX | MOBJ_FLAG_GRAVITY);
        mobjs[i]->flags |= (MOBJ_FLAG_HIT_PARTY | MOBJ_FLAG_HIT_STAND | MOBJ_FLAG_HIT_HAMMER);
        mobjs[i]->flagVar = flagVar;
        mobjs[i]->state = 0;
        mobjs[i]->typeWork = time;
        if (evtGetValue(event, flagVar)) {
            mobjs[i]->state = 99;
        }
        mobjCalcMtx(mobjs[i]);
    }
    return EVT_RETURN_DONE;
}

#if MOBJ_EXTRA_OBJS
/**
 * Main function of the orange ride switch (early demos, PAL demo and PAL):
 * standing on it runs the event and squashes it flat, where it stays.
 */
static s32 mobj_rideswitch_orange(MapObjectEntry* mobj) {
    f32 x, y, z;
    switch (mobj->state) {
        case 0:
            if (mobj->flags & MOBJ_FLAG_HIT) {
                mobjRunEvent(mobj, (s32*)mobj->event);
                psndSFXOn_3D((const char*)PSND_SFX_MOBJ_RIDE_SWITCH_ON5, &mobj->position);
                animPoseSetAnim(mobj->poseId, "A_1", 1);
                mobj->state = 1;
            }
            break;
        case 1:
            y = mobj->scale2.y - 0.3f;
            x = mobj->scale2.x;
            z = mobj->scale2.z;
            if (y <= 0.2f) {
                mobj->state = 99;
                y = 0.2f;
            }
            mobj->scale2 = (Vec){x, y, z};
            mobjCalcMtx2(mobj);
            break;
        case 99:
            break;
    }
    return 0;
}

/** Places an orange ride switch (MOBJ_RideSwitchA). args: name, x, y, z, event, flag variable. */
USER_FUNC(evt_mobj_rideswitch_orange) {
    s32 val;
    EvtWord* args = event->args;
    const char* name;
    f32 x, y, z;
    s32 flagVar;
    MapObjectEntry* mobj;
    const char* anim;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    val = evtGetValue(event, args[4]);
    flagVar = args[5];
    if (animGroupBaseAsync(anim = "MOBJ_RideSwitchA", gp->inBattle ? 1 : 0, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobj->scale2.x *= 0.8f;
    mobj->scale2.z *= 0.8f;
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_rideswitch_orange;
    mobj->event = val;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= MOBJ_FLAG_HIT_STAND;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx2(mobj);
    return EVT_RETURN_DONE;
}

/**
 * Main function of the green ride switch (early demos, PAL demo and PAL): stepping on it runs the event and flattens it;
 * once Mario walks away it rises back up and can be pressed again.
 */
static s32 mobj_rideswitch_green(MapObjectEntry* mobj) {
    f32 x, y, z;
    AnimationPoseData* data;
    f32 mag;
    Vec size;
    switch (mobj->state) {
        case 0:
            if (mobj->flags & MOBJ_FLAG_HIT) {
                mobjRunEvent(mobj, (s32*)mobj->event);
                psndSFXOn_3D((const char*)PSND_SFX_MOBJ_RIDE_SWITCH_ON3, &mobj->position);
                animPoseSetAnim(mobj->poseId, "A_1", 1);
                mobj->state = 1;
            }
            break;
        case 1:
            y = mobj->scale2.y - 0.3f;
            x = mobj->scale2.x;
            z = mobj->scale2.z;
            if (y <= 0.2f) {
                mobj->state = 2;
                y = 0.2f;
                mobj->flags &= ~MOBJ_FLAG_HIT;
            }
            mobj->scale2 = (Vec){x, y, z};
            mobjCalcMtx2(mobj);
            break;
        case 2:
            data = animPoseGetAnimBaseDataPtr(mobj->poseId);
            size = (Vec){0.8f * (10.0f * data->bboxMax.x), 10.0f * data->bboxMax.y, 0.8f * (10.0f * data->bboxMax.z)};
            mag = PSVECMag(&size);
            if (PSVECDistance(&mobj->position, &marioGetPtr()->position) > mag) {
                if (!(mobj->flags & MOBJ_FLAG_HIT)) {
                    mobj->state = 3;
                    animPoseSetAnim(mobj->poseId, "A_2", 1);
                    psndSFXOn_3D((const char*)PSND_SFX_MOBJ_RIDE_SWITCH_ON4, &mobj->position);
                } else {
                    mobj->flags &= ~MOBJ_FLAG_HIT;
                }
            }
            break;
        case 3:
            y = mobj->scale2.y;
            y += 0.3f;
            x = mobj->scale2.x;
            z = mobj->scale2.z;
            if (y >= 15.0f) {
                mobj->state = 0;
                y = 15.0f;
            }
            mobj->scale2 = (Vec){x, y, z};
            mobjCalcMtx2(mobj);
            mobj->flags &= ~MOBJ_FLAG_HIT;
            break;
        case 99:
            break;
    }
    return 0;
}

/** Places a green ride switch (MOBJ_RideSwitchB). args: name, x, y, z, event, flag variable. */
USER_FUNC(evt_mobj_rideswitch_green) {
    s32 val;
    EvtWord* args = event->args;
    const char* name;
    f32 x, y, z;
    s32 flagVar;
    MapObjectEntry* mobj;
    const char* anim;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    val = evtGetValue(event, args[4]);
    flagVar = args[5];
    if (animGroupBaseAsync(anim = "MOBJ_RideSwitchB", gp->inBattle ? 1 : 0, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobj->scale2.x *= 0.8f;
    mobj->scale2.z *= 0.8f;
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_rideswitch_green;
    mobj->event = val;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= MOBJ_FLAG_HIT_STAND;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx2(mobj);
    return EVT_RETURN_DONE;
}

#endif

/**
 * Main function of the light blue ride switch: stepping on it runs the event and flattens it;
 * when Mario leaves it runs the second (release) event and springs back up.
 */
static s32 mobj_rideswitch_lightblue(MapObjectEntry* mobj) {
    f32 x, y, z;
    AnimationPoseData* data;
    f32 mag;
    Vec size;
    Vec d1;
    Vec d2;
    Vec d3;
    Vec d4;
#if !MOBJ_EXTRA_OBJS
    // Four Vec literals never read (they read the uninitialized x/y/z), in the original too: their stack slots
    // and initializer templates are in the retail code
    d1 = (Vec){x, y, z};
    d2 = (Vec){x, y, z};
    d3 = (Vec){x, y, z};
    d4 = (Vec){x, y, z};
#endif
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
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
            mobj->flags &= ~MOBJ_FLAG_HIT;
        }
        mobj->scale2 = (Vec){x, y, z};
        mobjCalcMtx2(mobj);
        break;
    case 2:
        data = animPoseGetAnimBaseDataPtr(mobj->poseId);
        size = (Vec){0.8f * (10.0f * data->bboxMax.x), 10.0f * data->bboxMax.y, 0.8f * (10.0f * data->bboxMax.z)};
        mag = PSVECMag(&size);
        if (PSVECDistance(&mobj->position, &marioGetPtr()->position) > mag) {
            if (!(mobj->flags & MOBJ_FLAG_HIT)) {
                mobj->state = 3;
                animPoseSetAnim(mobj->poseId, "A_2", 1);
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_RIDE_SWITCH_ON2), &mobj->position);
                mobjRunEvent(mobj, (s32*)mobj->extraEvent);
            } else {
                mobj->flags &= ~MOBJ_FLAG_HIT;
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
        mobj->flags &= ~MOBJ_FLAG_HIT;
        break;
    case 99:
        break;
    }
    return 0;
}

#if !MOBJ_EXTRA_OBJS
/** Not called: it only places the model-name strings that stripped code referenced. */
void evt_mobj_rideswitch(void) {
    mobjNameToPtr("MOBJ_RideSwitchB");
    mobjNameToPtr("MOBJ_RideSwitchA");
    mobjNameToPtr("MOBJ_RedTornadoSwitch");
}
#endif

/**
 * Places a light blue ride switch (MOBJ_RideSwitchC).
 * args: name, x, y, z, press event, release event, flag variable.
 */
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= MOBJ_FLAG_HIT_STAND;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx2(mobj);
    return EVT_RETURN_DONE;
}

static s32 mobj_jumpstand_red(MapObjectEntry* mobj) {
    MarioWork* mario = marioGetPtr();
    EventEntry* ev;
    partyGetPtr(marioGetPartyId());
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            gp->systemLevelFlags |= SYSTEM_LEVEL_NO_FIELD_BATTLE;
            ev = mobjRunEvent(mobj, (s32*)mobj->event);
            if (ev) {
                ev->lwData[0] = mario->fallStartPos.y < mario->position.y ? 0 : (mario->fallStartPos.y < mobj->position.y + mario->wPlayerCollisionBox.y ? 0 : 1);
            }
            if (mario->characterId != CHARACTER_BOWSER) {
                if (!strcmp(mobj->animName, "MOBJ_RedJumpStand")) {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_JUMP_STAND1), &mobj->position);
                } else {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_JUMP_STAND2), &mobj->position);
                }

                // BUG: irand(4) returns 0..3, so case 4 (the fifth voice) never plays.
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
            } else {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_MOBJ_JUMP_STAND2));
            }
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            mobj->flags &= ~MOBJ_FLAG_HIT;
            mobj->state++;
        }
        break;
    case 1:
        if ((mobj->flags & MOBJ_FLAG_HIT) && evtCheckID(mobj->eventId) == 0) {
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            mobj->state = 0;
            break;
        }
        if (evtCheckID(mobj->eventId) == 1) {
            break;
        }
        gp->systemLevelFlags &= ~SYSTEM_LEVEL_NO_FIELD_BATTLE;
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        mobj->state = 0;
        break;
    }
    return 0;
}

/**
 * Places a red jump stand that runs a script when stood on.
 * args: kind (0 normal, 1 MOBJ_RedJumpStand2), name, x, y, z, event, flag variable.
 */
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
    mobj->flags |= (MOBJ_FLAG_UPDATE_MTX | MOBJ_FLAG_GRAVITY);
    mobj->flags |= MOBJ_FLAG_HIT_STAND;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

/**
 * Main function of the blue jump stands: landing on one launches Mario upwards with the stored power
 * (marioChgJumpStandMotion), runs the event and plays the spring sounds; field battles stay off
 * (SYSTEM_LEVEL_NO_FIELD_BATTLE) until the event ends.
 */
static s32 mobj_jumpstand_blue(MapObjectEntry* mobj) {
    MarioWork* mario = marioGetPtr();
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            gp->systemLevelFlags |= SYSTEM_LEVEL_NO_FIELD_BATTLE;
            mobjRunEvent(mobj, (s32*)mobj->event);
            if (mobj->hitFlag == MOBJ_FLAG_HIT_STAND) {
                marioChgJumpStandMotion(mobj->typeDataF[3]);
            }
            if (mario->characterId != CHARACTER_BOWSER) {
                if (!strcmp(mobj->animName, "MOBJ_BlueJumpStand")) {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_JUMP_STAND1), &mobj->position);
                } else {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_JUMP_STAND2), &mobj->position);
                }

                // BUG: irand(4) returns 0..3, so case 4 (the fifth voice) never plays.
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
            mobj->flags &= ~MOBJ_FLAG_HIT;
            mobj->state++;
        }
        break;
    case 1:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            mobj->state = 0;
            break;
        }
        if (evtCheckID(mobj->eventId) == 1) {
            break;
        }
        gp->systemLevelFlags &= ~SYSTEM_LEVEL_NO_FIELD_BATTLE;
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        mobj->state = 0;
        break;
    }
    return 0;
}

/**
 * Places a blue jump stand that bounces Mario.
 * args: kind (0 normal, 1 MOBJ_BlueJumpStand2), name, jump power, x, y, z, event, flag variable.
 */
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
    mobj->flags |= (MOBJ_FLAG_UPDATE_MTX | MOBJ_FLAG_GRAVITY);
    mobj->flags |= MOBJ_FLAG_HIT_STAND;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobj->typeDataF[3] = power;
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

/**
 * Main function of the hammer-level blocks: a hit only counts with a strong enough hammer (Lv1 any,
 * Lv1 huge block needs Super Hammer and a spin-charged hammer, Lv2 needs Ultra Hammer).
 * It then runs the event, sets the flag variable
 * and plays the "P_box_y" paper-peel animation through an offscreen capture before deleting itself.
 */
static s32 mobj_lv_blk_evt(MapObjectEntry* mobj) {
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
#if VERSION >= VERSION_JP
        animGroupBaseAsync("P_box_y", type, NULL);
#endif
        if (mobj->flags & MOBJ_FLAG_HIT) {
            if (!strcmp(mobj->animName, "MOBJ_Lv1Block") || !strcmp(mobj->animName, "MOBJ_Lv1BigBlock")) {
            } else if (!strcmp(mobj->animName, "MOBJ_Lv1BigBigBlock")) {
                if (pouchGetHammerLv() < 2 || (u16)mario->currMotionId != MARIO_MOTION_HAMMER_2) {
                    mobj->flags &= ~MOBJ_FLAG_HIT;
                    break;
                }
            } else if (!strcmp(mobj->animName, "MOBJ_Lv2Block") || !strcmp(mobj->animName, "MOBJ_Lv2BigBlock")) {
                if (pouchGetHammerLv() < 3) {
                    mobj->flags &= ~MOBJ_FLAG_HIT;
                    break;
                }
            } else if (!strcmp(mobj->animName, "MOBJ_Lv2BigBigBlock")) {
                if (pouchGetHammerLv() < 3 || (u16)mario->currMotionId != MARIO_MOTION_HAMMER_2) {
                    mobj->flags &= ~MOBJ_FLAG_HIT;
                    break;
                }
            } else {
                mobj->flags &= ~MOBJ_FLAG_HIT;
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
        args1[0] = (EvtWord)iname;
        args1[1] = 0;
        args1[2] = (EvtWord)oname;
        args1[3] = 1;
        args1[4] = 0;
        args1[5] = 0;
        args1[6] = 0;
        args1[7] = 0;
        evt_img_alloc_capture(&ev1, TRUE);
        ev2.args = args2;
        args2[0] = (EvtWord)iname;
        args2[1] = 1;
        args2[2] = (EvtWord)oname;
        args2[3] = 1;
        args2[4] = 0;
        args2[5] = 0;
        args2[6] = 0;
        args2[7] = 0;
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
#if VERSION >= VERSION_JP
            mobj->state = 12;
        }
        if (img->flags & IMAGE_FLAG_NO_CAPTURE) {
#endif
            mobj->state = 12;
        }
        mobj->position = (Vec){0.0f, -2000.0f, 0.0f};
        mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
        mobj->flags |= MOBJ_FLAG_HIT;
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

/**
 * Places a block that only a strong enough hammer breaks (MOBJ_Lv1Block etc.).
 * args: name, x, y, z, event, flag variable (already set: not placed), animation group.
 * Also loads the "P_box_y" paper-peel animation.
 */
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
    mobj->flags |= (MOBJ_FLAG_UPDATE_MTX | MOBJ_FLAG_GRAVITY);
    mobj->position = (Vec){x, y, z};
    mobj->mainFunc = mobj_lv_blk_evt;
    mobj->event = val;
    mobj->extraEvent = 0;
    mobj->flags |= MOBJ_FLAG_HIT_HAMMER;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    return EVT_RETURN_DONE;
}

/**
 * Main function of the floating (flow) blocks toggled by a switch block:
 * on their own they show "S_1" (solid, no shadow) when they rest on a floor just below them and "A_1" otherwise;
 * when linked they follow the switch's A_1-A_3 animation.
 */
static s32 mobj_float_blk(MapObjectEntry* mobj) {
    MapObjectEntry* other;
    const char* cur;
    switch (mobj->state) {
    case 99:
        if (mobj->linkedName[0] == 0) {
            if (blkHitCheck(mobj)) {
                if (strcmp((const char*)animPoseGetCurrentAnim(mobj->poseId), "S_1")) {
                    animPoseSetAnim(mobj->poseId, "S_1", 1);
                    mobj->flags |= MOBJ_FLAG_NO_SHADOW;
                }
            } else if (strcmp((const char*)animPoseGetCurrentAnim(mobj->poseId), "A_1")) {
                animPoseSetAnim(mobj->poseId, "A_1", 1);
                mobj->flags &= ~MOBJ_FLAG_NO_SHADOW;
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

/**
 * Places a floating colour block.
 * args: name, x, y, z, colour (0 green, 1 purple, 2 orange, 3 red big), size (0 small, 1 big, 2 big02), unused,
 * flag variable.
 */
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flagVar = flagVar;
    mobj->state = 99;
    mobjCalcMtx(mobj);
    if (blkHitCheck(mobj)) {
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        mobj->flags |= MOBJ_FLAG_NO_SHADOW;
    } else {
        animPoseSetAnim(mobj->poseId, "A_1", 1);
    }
    return EVT_RETURN_DONE;
}

/** Returns the index in _switch_anim of the switch block's current animation (0 when not found). */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
s32 _mobj_get_switch_anim_index(s32 poseId) {
    const char** p;
    const s8* cur;
    s32 idx;
    s32 i;
    cur = (const s8*)animPoseGetCurrentAnim(poseId);
    idx = 0;
    p = _switch_anim;
    for (i = idx; i < 6; i++, p++) {
        if (!strcmp(*p, (const char*)cur)) {
            idx = i;
            break;
        }
    }
    return idx;
}

/**
 * Main function of the coloured switch blocks that raise and lower their linked floating block:
 * each hit runs the event and plays the next raise/lower animation;
 * with a nonzero timer it springs back after 120 frames.
 * typeData[1] records the animation index and which way it was switched for the event.
 */
static s32 mobj_switch_float_blk(MapObjectEntry* mobj) {
    const s8* cur;
    s32 animIndex;
    BOOL a;
    switch (mobj->state) {
    case 0:
        if (!(mobj->flags & MOBJ_FLAG_HIT)) {
            break;
        }
        mobj->typeData[1] = (mobj->typeData[1] & 0x40000000) | _mobj_get_switch_anim_index(mobj->poseId);
        if (mobj->linkedName[0]) {
            if (blkHitCheck(mobjNameToPtr(mobj->linkedName))) {
                if (evtCheckID(mobj->eventId)) {
                    if (isAnimA13((const s8*)animPoseGetCurrentAnim(mobj->poseId))) {
                        animIndex = 0;
                    } else {
                        animIndex = 3;
                    }
                } else {
                    mobj->typeDataU[1] |= 0x80000000;
                    animIndex = 0;
                }
            } else if (evtCheckID(mobj->eventId)) {
                cur = (const s8*)animPoseGetCurrentAnim(mobj->poseId);
                if (cur[0] == 'A' && cur[2] >= '1' && cur[2] <= '2') {
                    animIndex = 0;
                } else if (cur[0] == 'A' && cur[2] == '3') {
                    mobj->typeDataU[1] |= 0x40000000;
                    animIndex = 0;
                } else if (cur[0] == 'A' && cur[2] >= '4' && cur[2] <= '5') {
                    animIndex = 3;
                } else {
                    animIndex = 3;
                }
            } else {
                mobj->typeDataU[1] |= 0x80000000;
                animIndex = 3;
            }
        } else {
            a = isAnimA13((const s8*)animPoseGetCurrentAnim(mobj->poseId));
            animIndex = a ? 3 : 0;
            mobj->typeDataU[1] |= 0x80000000;
        }
        mobjRunEvent(mobj, (s32*)mobj->event);
        if (mobj->hitFlag == MOBJ_FLAG_HIT_STAND) {
            marioChgMot(MARIO_MOTION_JUMP_SW);
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_SWITCH1), &mobj->position);
        switch (mobj->hitFlag) {
        case MOBJ_FLAG_HIT_HEAD:
            animPoseSetAnim(mobj->poseId, _switch_anim[animIndex + 1], 1);
            break;
        default:
            animPoseSetAnim(mobj->poseId, _switch_anim[animIndex], 1);
            break;
        }
        mobj->flags &= ~MOBJ_FLAG_HIT;
        if (mobj->timer) {
            mobj->state = 10;
        } else {
            mobj->state++;
        }
        break;
    case 1:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            mobj->state = 0;
            break;
        }
        if (mobj->flags & MOBJ_FLAG_EXEC_CANCEL) {
            mobj->flags &= ~MOBJ_FLAG_EXEC_CANCEL;
            mobj->state = 10;
            break;
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        if (mobj->linkedName[0]) {
            mobjNameToPtr(mobj->linkedName);
        }
        {
            const char* nextAnim = isAnimA13((const s8*)animPoseGetCurrentAnim(mobj->poseId)) ? "A_3" : "S_1";
            animPoseSetAnim(mobj->poseId, nextAnim, 1);
        }
        mobj->state++;
        break;
    case 2:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            mobj->state = 0;
            break;
        }
        if (mobj->flags & MOBJ_FLAG_EXEC_CANCEL) {
            mobj->flags &= ~MOBJ_FLAG_EXEC_CANCEL;
            mobj->state = 10;
        }
        if (evtCheckID(mobj->eventId) == 0) {
            mobj->state = 0;
            mobj->typeData[1] = 0;
        }
        break;
    case 10:
        if (mobj->flags & MOBJ_FLAG_HIT) {
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
        if (mobj->flags & MOBJ_FLAG_HIT) {
            mobj->state = 0;
            break;
        }
        mobj->timer--;
        if (mobj->timer != 0) {
            break;
        }
        if (mobj->linkedName[0]) {
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= (MOBJ_FLAG_HIT_HAMMER | MOBJ_FLAG_HIT_STAND | MOBJ_FLAG_HIT_HEAD | MOBJ_FLAG_HIT_PARTY);
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobj->timer = 0;
    mobjCalcMtx(mobj);
    strcpy(mobj->linkedName, partner);
    if (mobj->linkedName[0]) {
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

/**
 * Opens the lock map object args[0] from a script:
 * locks Mario and the partner's controls and starts the lock's unlock sequence (state 3).
 * Does nothing if the object is not a MOBJ_Lock.
 */
USER_FUNC(evt_mobj_lock_unlock) {
    MapObjectEntry* mobj = mobjNameToPtr((const char*)evtGetValue(event, event->args[0]));
    if (strcmp(mobj->animName, "MOBJ_Lock")) {
        return EVT_RETURN_DONE;
    }
    if (!mobj->mainFunc) {
        mobj->mainFunc = mobj_lock;
    }
    mobj->flags |= MOBJ_FLAG_HIT;
    marioKeyOff();
    partyKeyOff();
    marioStSystemLevel(1);
    mobj->state = 3;
    return EVT_RETURN_DONE;
}

/** Lock swing and unlock sounds at the position LW(0)-LW(2), started as the lock opens. */
EVT_BEGIN(evt_lock_sound)
    WAIT_MS(100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_LOCK_SWING1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(200)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_UNLOCK1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()


static s32 mobj_lock(MapObjectEntry* mobj) {
#if VERSION >= VERSION_US
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
        if (mobj->flags & MOBJ_FLAG_HIT) {
            marioKeyOff();
            partyKeyOff();
            marioStSystemLevel(1);
            mobjRunEvent(mobj, (s32*)mobj->event);
            mobj->state++;
        }
        break;
    case 1:
        if (evtCheckID(mobj->eventId) != 1) {
            if (mobj->flags & MOBJ_FLAG_EXEC_CANCEL) {
                mobj->flags &= ~MOBJ_FLAG_EXEC_CANCEL;
                mobj->flags &= ~MOBJ_FLAG_HIT;
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
    case 2:
        if (mobj->typeDataU[0] & 0x80000000) {
            item = 0;
            for (i = 0; ; i++) {
#if VERSION >= VERSION_US
                s32* list = mobj->typeDataList[0];
                s32 r;
                if (list[i] == -1) {
                    break;
                }
                r = pouchCheckItem(list[i]);
                if (r) {
                    item = list[i];
#else
                s32 it = mobj->typeDataList[0][i];
                s32 r;
                if (it == -1) {
                    break;
                }
                r = pouchCheckItem(it);
                if (r) {
                    // BUG (early demos and JP, fixed in US and later above): pouchCheckItem returns how many of the key the player holds,
                    // not its id, so this stores the count and the count is passed to pouchRemoveItem below as an item
                    // id. Holding one key gives item 1 (ITEM_STRANGE_SACK), so a lock that takes a list of keys tries
                    // to remove that item.
                    item = r;
#endif
                    break;
                }
            }
        } else if (pouchCheckItem(mobj->typeData[0])) {
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
        ev->lwData[0] = FLOAT(mobj->position.x);
        ev->lwData[1] = FLOAT(mobj->position.y);
        ev->lwData[2] = FLOAT(mobj->position.z);
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
            mobj->flags |= MOBJ_FLAG_NO_DISP;
        } else {
            mobj->flags &= ~MOBJ_FLAG_NO_DISP;
        }
        if (mobj->timer == 0) {
            mobj->state = 99;
        }
        break;
    case 10:
        if (!windowCheckID(mobj->typeData[2])) {
            mobj->flags &= ~MOBJ_FLAG_HIT;
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

/**
 * Places a padlock.
 * args: name, key item (or a key list with bit 31 set), x, y, z, Y rotation in degrees, event run when checked,
 * event run once unlocked, flag variable (already set: not placed).
 */
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
    name = (const char*)evtGetValue(event, args[0]);
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
    mobj->flags |= (MOBJ_FLAG_NO_SHADOW | MOBJ_FLAG_UPDATE_MTX | MOBJ_FLAG_SHOW_HINT);
    mobj->flags |= MOBJ_FLAG_HIT_A_BUTTON;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "Z_1", 1);
    return EVT_RETURN_DONE;
}

/** Opening sounds for the normal and grey treasure chests at LW(0)-LW(2). */
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

// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void itemboxChgHitSize(MapObjectEntry* mobj) {
    AnimationPoseData* data = animPoseGetAnimBaseDataPtr(mobj->poseId);
    Vec a, b;
    a = (Vec){data->bboxMin.x, data->bboxMin.y, data->bboxMin.z};
    b = (Vec){data->bboxMax.x, data->bboxMax.y, data->bboxMax.z};
    a.z = a.z - 0.2f * __fabs(b.y - a.y);
    PSVECAdd(&b, &a, &mobj->translation);
    PSVECScale(&mobj->translation, &mobj->translation, 0.5f);
    PSVECScale(&mobj->translation, &mobj->translation, 10.0f);
    mobj->translation.y = 0.0f;
    mobj->scale2.y *= 1.4f;
    mobj->scale2.z *= 1.2f;
    mobjCalcMtx2(mobj);
}

/**
 * Main function of the treasure chests: when Mario opens one it locks the controls,
 * runs the check event (which can cancel with MOBJ_FLAG_EXEC_CANCEL), plays the opening sound and animation,
 * runs the open event (item get), then sets the flag variable and stays open.
 */
static s32 mobj_itembox(MapObjectEntry* mobj) {
    EventEntry* ev;
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
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
            if (mobj->flags & MOBJ_FLAG_EXEC_CANCEL) {
                mobj->flags &= ~MOBJ_FLAG_EXEC_CANCEL;
                mobj->flags &= ~MOBJ_FLAG_HIT;
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
            ev->lwData[0] = FLOAT(mobj->position.x);
            ev->lwData[1] = FLOAT(mobj->position.y);
            ev->lwData[2] = FLOAT(mobj->position.z);
        } else if (!strcmp(mobj->animName, "MOBJ_BlackTreasureBox")) {
            ev = evtEntryType(evt_box_sound3, 0, 0, 0);
            ev->lwData[0] = FLOAT(mobj->position.x);
            ev->lwData[1] = FLOAT(mobj->position.y);
            ev->lwData[2] = FLOAT(mobj->position.z);
        } else {
            ev = evtEntryType(evt_box_sound2, 0, 0, 0);
            ev->lwData[0] = FLOAT(mobj->position.x);
            ev->lwData[1] = FLOAT(mobj->position.y);
            ev->lwData[2] = FLOAT(mobj->position.z);
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
        if (mobj->eventId && evtCheckID(mobj->eventId) == 1) {
            break;
        }
        mobj->flags &= ~MOBJ_FLAG_SHOW_HINT;
        itemboxChgHitSize(mobj);
        marioKeyOn();
        partyKeyOn();
        marioStSystemLevel(0);
        mobj->eventId = 0;
        mobj->state = 99;
        break;
    case 99:
        evtSetValue(NULL, mobj->flagVar, 1);
        mobj->flags |= MOBJ_FLAG_HIT;
        mobj->flags &= ~MOBJ_FLAG_SHOW_HINT;
        if (!strcmp(mobj->animName, "MOBJ_BlackTreasureBox")) {
            animPoseSetAnim(mobj->poseId, "S_4", 1);
        } else {
            animPoseSetAnim(mobj->poseId, "S_2", 1);
        }
        break;
    }
    return 0;
}

/**
 * Places a treasure chest.
 * args: name, x, y, z, kind (0 normal, 1 big, 2 grey, 3 black, 4 golden), check event, open event,
 * flag variable (already set: placed open). Chests with a floor within 5 below them do not fall or cast a shadow.
 */
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
    case MOBJ_ITEMBOX_NORMAL:
        anim = "MOBJ_TreasureBox";
        break;
    case MOBJ_ITEMBOX_BIG:
        anim = "MOBJ_BigTreasureBox";
        break;
    case MOBJ_ITEMBOX_GRAY:
        anim = "MOBJ_GrayTreasureBox";
        break;
    case MOBJ_ITEMBOX_BLACK:
        anim = "MOBJ_BlackTreasureBox";
        break;
    case MOBJ_ITEMBOX_GOLDEN:
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
    mobj->flags |= (MOBJ_FLAG_UPDATE_MTX | MOBJ_FLAG_SHOW_HINT | MOBJ_FLAG_GRAVITY);
    mobj->flags |= MOBJ_FLAG_HIT_A_BUTTON;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    if (evtGetValue(event, flagVar)) {
        itemboxChgHitSize(mobj);
        mobj->state = 99;
    }
    dist = 5.0f + (f32)__fabs(mobj->velocity.y);
    if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, mobj->position.x, 1.0f + mobj->position.y, mobj->position.z, 0.0f, -1.0f, 0.0f)) {
        mobj->flags |= MOBJ_FLAG_NO_SHADOW;
        mobj->flags &= ~MOBJ_FLAG_GRAVITY;
    }
    return EVT_RETURN_DONE;
}

/**
 * Main function of the signboards: reading one runs its event and waits for it to finish.
 * JP and later lock Mario's controls while it runs; the early demos' sign scripts do that themselves.
 */
static s32 mobj_signboard(MapObjectEntry* mobj) {
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
#if VERSION >= VERSION_JP
            marioKeyOff();
#endif
            mobjRunEvent(mobj, (s32*)mobj->event);
            mobj->state++;
        }
        break;
    case 1:
        if (evtCheckID(mobj->eventId) != 1) {
#if VERSION >= VERSION_JP
            marioKeyOn();
#endif
            mobj->eventId = 0;
            mobj->flags &= ~MOBJ_FLAG_HIT;
            mobj->state = 0;
        }
        break;
    }
    return 0;
}

/** Places a signboard (MOBJ_Signboard). args: name, x, y, z, read event, flag variable. */
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

    // FAKE: gp->inBattle is loaded into inBattle before the test; `inBattle = gp->inBattle != 0` changes the object
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
    mobj->flags |= (MOBJ_FLAG_UPDATE_MTX | MOBJ_FLAG_SHOW_HINT);
    mobj->flags |= MOBJ_FLAG_HIT_A_BUTTON;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    return EVT_RETURN_DONE;
}

#if MOBJ_EXTRA_OBJS
/** Main function of the arrow sign; it does nothing (the arrow is only decoration). */
static s32 mobj_arrow(MapObjectEntry* mobj) {
    return 0;
}

/** Places a decorative arrow sign (MOBJ_Arrow); only built in the early demos, the PAL demo and PAL. args: name, x, y, z, flag variable. */
USER_FUNC(evt_mobj_arrow) {
    EvtWord* args = event->args;
    s32 inBattle;
    const char* name;
    f32 x, y, z;
    s32 flagVar;
    MapObjectEntry* mobj;
    const char* anim;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    flagVar = args[4];
    inBattle = gp->inBattle;
    inBattle = inBattle != 0;
    anim = "MOBJ_Arrow";
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_arrow;
    mobj->event = 0;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flagVar = flagVar;
    mobj->state = 99;
    return EVT_RETURN_DONE;
}
#else
/** Not called: it only places the model-name string that stripped code referenced. */
void mobj_sign(void) {
    mobjNameToPtr("MOBJ_Arrow");
}
#endif

// FAKE: private float-first prototype; eff_batfukidashi.h (and the definition) order the arguments (type, number,
// duration, x, y, z), and calls through it change the object.
extern EffectEntry* effCoinFukidashiEntry(f32 x, f32 y, f32 z, s32 type, s32 number, s32 duration);

/** case 3 heal sequence; case 4 spells the same steps out inline (only this split reproduces the target registers) */
// Restores Mario's HP and FP and the HP of every active partner that is not PARTY_FLAG_IS_GROUNDED (case 3).
// FAKE: case 4 repeats these steps inline instead of calling this helper; calling it there changes the object
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void mobj_recovery_blk_recover(void) {
    PouchData* pouch;
    PouchPartyData* party;
    MarioWork* mario;
    s32 i;
    mario = marioGetPtr();
    pouch = pouchGetPtr();
    party = pouch->partyData;
    effStardustEntry(2, 50, 100, mario->position.x, mario->position.y, mario->position.z, 50.0f, 50.0f);
    marioChgPose("M_I_1");
    pouch->currentHP = pouch->maximumHP;
    pouch->currentFP = pouch->maximumFP;
    for (i = 0; i < 8; party++, i++) {
        if ((party->flags & PARTY_FLAG_IS_ACTIVE) && !(party->flags & PARTY_FLAG_IS_GROUNDED)) {
            party->currentHP = party->maximumHP;
        }
    }
}

/**
 * Main function of the heart (recovery) blocks:
 * hitting one from below restores Mario's HP and FP and every active partner's HP.
 * The coin-taking version first checks the price (typeData[1]), shows "sys_no_coin" when Mario is short,
 * charges the coins and keeps a price bubble above it.
 * The first hit outside E3 mode (GW_FLAG_E3) shows the "kaifuku_first_hit" tutorial instead
 * and sets GSWF_HEART_BLOCK_TUTORIAL_SEEN.
 */
static s32 mobj_recovery_blk(MapObjectEntry* mobj) {
    PouchData* pouch;
    MarioWork* mario;
    PouchPartyData* party;
    s32 i;
    s32 coins;
    switch (mobj->state) {
    case -1:
        mobj->effect = effCoinFukidashiEntry(mobj->position.x, 25.0f + mobj->position.y, mobj->position.z - 10.0f, 0, mobj->typeData[1], 0);
        mobj->state++;
        break;
    case 0:
        if (!(mobj->flags & MOBJ_FLAG_HIT)) {
            break;
        }
        marioKeyOff();
        camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_HP_RECOVER_HIT1), &mobj->position);
        mobj->state++;
    case 1:
        if (!(gp->flags & GW_FLAG_E3) && swGet(GSWF_ID(GSWF_HEART_BLOCK_TUTORIAL_SEEN)) == 0) {
            swSet(GSWF_ID(GSWF_HEART_BLOCK_TUTORIAL_SEEN));
            mobj->state = 20;
            break;
        }
        mobj->state++;
    case 2:
        coins = pouchGetCoin();
        if (coins < mobj->typeData[1]) {
            mobj->state = 10;
            break;
        }
        if (mobj->effect) {
            effSoftDelete(mobj->effect);
            mobj->effect = NULL;
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_HP_RECOVER_HEART1), &mobj->position);
        animPoseSetAnim(mobj->poseId, "A_2", 1);
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
            mobj_recovery_blk_recover();
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
            coins = mobj->typeData[1];
            mario = marioGetPtr();
            pouch = pouchGetPtr();
            party = pouch->partyData;
            effStardustEntry(2, 50, 100, mario->position.x, mario->position.y, mario->position.z, 50.0f, 50.0f);
            marioChgPose("M_I_1");
            pouch->currentHP = pouch->maximumHP;
            pouch->currentFP = pouch->maximumFP;
            for (i = 0; i < 8; party++, i++) {
                if ((party->flags & PARTY_FLAG_IS_ACTIVE) && !(party->flags & PARTY_FLAG_IS_GROUNDED)) {
                    party->currentHP = party->maximumHP;
                }
            }
            pouchAddCoin(-coins);
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
        animPoseSetAnim(mobj->poseId, "A_1", 1);
        marioChgPose("M_S_1");
        marioKeyOn();
        statusWinForceOff();
        camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX_OFF;
        mobj->state++;
        break;
    case 6:
        if (mobj->effect == NULL) {
            mobj->effect = effCoinFukidashiEntry(mobj->position.x, 25.0f + mobj->position.y, mobj->position.z - 10.0f, 0, mobj->typeData[1], 0);
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        mobj->flags &= ~MOBJ_FLAG_HIT;
        mobj->state = 0;
        break;
    case 10:
        mobj->typeData[2] = msgWindow_Entry(msgSearch("sys_no_coin"), 0, 0);
        mobj->state++;
        break;
    case 11:
        if (windowCheckID(mobj->typeData[2]) == 0) {
            marioKeyOn();
            camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX_OFF;
            mobj->flags &= ~MOBJ_FLAG_HIT;
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
            camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX_OFF;
            mobj->flags &= ~MOBJ_FLAG_HIT;
            mobj->state = 0;
        }
        break;
    }
    return 0;
}

/**
 * Places a heart block.
 * args: name, price in coins (0 = free MOBJ_RecoveryBlock, otherwise the coin-taking block), x, y, z, unused,
 * flag variable.
 */
USER_FUNC(evt_mobj_recovery_blk) {
    BOOL inBattle;
    s32 flagVar;
    EvtWord* args = event->args;
    const char* anim;
    s32 coinCost;
    const char* name;
    MapObjectEntry* mobj;
    f32 x, y, z;
    name = (const char*)evtGetValue(event, args[0]);
    coinCost = evtGetValue(event, args[1]);
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    evtGetValue(event, args[5]);
    flagVar = args[6];
    inBattle = gp->inBattle != 0;
    anim = coinCost == 0 ? "MOBJ_RecoveryBlock" : "MOBJ_CoinTakenRecoveryBlock";
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobj->typeData[1] = coinCost;
    mobjSetPosition(name, x, y, z);
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->mainFunc = mobj_recovery_blk;
    mobj->event = 0;
    mobj->extraEvent = 0;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= MOBJ_FLAG_HIT_HEAD;
    mobj->flagVar = flagVar;
    if (coinCost == 0) {
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
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG0_07_GOOMBELLA_JOINS)
        SET_VALUE(GSWF_MAIN_SAVED_SINCE_GOOMBELLA_JOINED, 1)
    END_IF
    SET_VALUE(LW(0), 0)
#if VERSION >= VERSION_US
    CALL_CHILD(evt_memcard_save_wrapper)
#else
    CALL_CHILD(evt_memcard_save)
#endif
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static s32 mobj_save_blk(MapObjectEntry* mobj) {
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            marioKeyOff();
            marioStSystemLevel(1);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_SAVE1), &mobj->position);
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
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
            camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX_OFF;
            mobj->flags &= ~MOBJ_FLAG_HIT;
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
    if (gp->flags & GW_FLAG_NO_SAVE_BLOCK) {
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
    mobj->flags |= (MOBJ_FLAG_UPDATE_MTX | MOBJ_FLAG_NO_CULL);
    mobj->flags |= MOBJ_FLAG_HIT_HEAD;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    return EVT_RETURN_DONE;
}

#if VERSION >= VERSION_US
/** Plain block (MOBJ_Block / MOBJ_PinkBlock) that runs an event when hit; not in JP. */
// US and later: plain block (MOBJ_Block / MOBJ_PinkBlock) that runs an event when hit.
static s32 mobj_blk(MapObjectEntry* mobj) {
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_UNBREAK1), &mobj->position);
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            mobj->flags &= ~MOBJ_FLAG_HIT;
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
        if (!(mobj->flags & MOBJ_FLAG_HIT)) {
            mobj->state = 0;
            break;
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        mobj->state = 0;
        mobj->flags &= ~MOBJ_FLAG_HIT;
        break;
    }
    return 0;
}

/**
 * Places a plain block that runs an event when hit from below.
 * args: name, x, y, z, kind (0 yellow MOBJ_Block, 1 MOBJ_PinkBlock), event, flag variable. Not in JP.
 */
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= MOBJ_FLAG_HIT_HEAD;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    return EVT_RETURN_DONE;
}
#endif

#if VERSION <= VERSION_DEMO_JP
/** Plain block (MOBJ_Block / MOBJ_PinkBlock) that runs an event when hit; not in JP. */
static s32 mobj_blk(MapObjectEntry* mobj) {
    switch (mobj->state) {
        case 0:
            if (mobj->flags & MOBJ_FLAG_HIT) {
                psndSFXOn_3D((const char*)PSND_SFX_MOBJ_BLOCK_UNBREAK1, &mobj->position);
                animPoseSetAnim(mobj->poseId, "A_1", 1);
                mobj->flags &= ~MOBJ_FLAG_HIT;
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
            if (!(mobj->flags & MOBJ_FLAG_HIT)) {
                mobj->state = 0;
                break;
            }
            if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
                break;
            }
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            mobj->state = 0;
            mobj->flags &= ~MOBJ_FLAG_HIT;
            break;
    }
    return 0;
}

/**
 * Places a plain block that runs an event when hit from below.
 * args: name, x, y, z, kind (0 yellow MOBJ_Block, 1 MOBJ_PinkBlock), event, flag variable. Not in JP.
 */
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= MOBJ_FLAG_HIT_HEAD;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    return EVT_RETURN_DONE;
}
#endif
/**
 * Main function of the ?
 * blocks and badge (pink) blocks: the first hit pops out the stored item (or a coin,
 * which is collected at once and sets the flag), or runs the event instead if one is set.
 * The block then turns into an empty block that just bounces when hit again.
 */
static s32 mobj_badgeblk(MapObjectEntry* mobj) {
    ItemEntry* item;
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            if (!strcmp(mobj->animName, "MOBJ_BadgeBlock") || !strcmp(mobj->animName, "MOBJ_BrickBadgeBlock") ||
                !strcmp(mobj->animName, "MOBJ_HiddenBadgeBlock")) {
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_BADGE1), &mobj->position);
            } else {
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_QUESTION1), &mobj->position);
            }
            if (mobj->flags & MOBJ_FLAG_HIDDEN) {
                mobj->flags &= ~MOBJ_FLAG_HIDDEN;
            }
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            if ((u32)mobj->event != 0) {
                mobjRunEvent(mobj, (s32*)mobj->event);
            } else if (mobj->typeData[0] != ITEM_COIN) {
                itemEntry(mobj->position.x, 40.0f + mobj->position.y, mobj->position.z, mobj->name, mobj->typeData[0], ITEM_TYPE_SCATTER_NO_PICKUP, mobj->flagVar, NULL);
                item = itemNameToPtr(mobj->name);
                itemFlagOn(item, ITEM_FLAG_CUSTOM_BOUND);
                item->bounceFactor = 0.7f;
                item->gravityScale = 1.0f;
                item->maxBounces = 0;
                item->velocityY = 360.0f;
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_ITEM_APPEAR1), &mobj->position);
            } else {
                pouchAddCoin(1);
                itemEntry(mobj->position.x, mobj->position.y, mobj->position.z, NULL, ITEM_COIN, ITEM_TYPE_POP, -1, NULL);
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
        mobj->flags &= ~MOBJ_FLAG_HIT;
        mobj->state = 90;
        if (!strcmp(mobj->animName, "MOBJ_HatenaBlock") || !strcmp(mobj->animName, "MOBJ_BrickHatenaBlock") ||
            !strcmp(mobj->animName, "MOBJ_HiddenHatenaBlock")) {
            strcpy(mobj->animName, "MOBJ_Block");
        } else {
            strcpy(mobj->animName, "MOBJ_PinkBlock");
        }
        break;
    case 90:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            mobj->flags &= ~MOBJ_FLAG_HIT;
            animPoseSetAnim(mobj->poseId, "A_2", 1);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_UNBREAK1), &mobj->position);
            mobj->state++;
        }
        break;
    case 91:
        if (!(mobj->flags & MOBJ_FLAG_HIT)) {
            mobj->state = 90;
            break;
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_2", 1);
        mobj->state = 90;
        mobj->flags &= ~MOBJ_FLAG_HIT;
        break;
    }
    return 0;
}

/**
 * Places a ? block or badge block.
 * args: name, x, y, z, item to give, event (0 to give the item), flag variable (already set: placed empty),
 * kind (literal: 0 ? block, 1 badge block).
 */
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
    BOOL group;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    item = evtGetValue(event, args[4]);
    val = evtGetValue(event, args[5]);
    kind = args[7];
    flagVar = args[6];
    group = gp->inBattle != 0;
    anim = kind != 0 ? "MOBJ_BadgeBlock" : "MOBJ_HatenaBlock";
    if (animGroupBaseAsync(anim, group, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_badgeblk;
    mobj->event = val;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= (MOBJ_FLAG_HIT_HAMMER | MOBJ_FLAG_HIT_HEAD | MOBJ_FLAG_HIT_PARTY);
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

/**
 * Main function of the Shine Sprite blocks: when hit it pops a Shine Sprite, gives it to Mario,
 * sets the flag variable and deletes itself.
 * In Pirate's Grotto (dou_02, dou_05/dou_10 when hit by a partner) it also stops Mario
 * while the Shine is collected; the early demos have no such maps.
 */
static s32 mobj_powerupblk(MapObjectEntry* mobj) {
    switch (mobj->state) {
    case 0:
        if (!(mobj->flags & MOBJ_FLAG_HIT)) {
            break;
        }
        if (marioKeyOffChk() || marioCtrlOffChk()) {
            mobj->flags &= ~MOBJ_FLAG_HIT;
            break;
        }
        marioKeyOff();
#if VERSION <= VERSION_DEMO_JP
        gp->systemLevelFlags |= SYSTEM_LEVEL_STOP_LV0 | SYSTEM_LEVEL_STOP_LV1;
#else
        if (!strcmp(gp->currentMapName, "dou_02")) {
            marioCtrlOff();
        } else if ((!strcmp(gp->currentMapName, "dou_05") || !strcmp(gp->currentMapName, "dou_10")) && mobj->hitFlag == MOBJ_FLAG_HIT_PARTY) {
            marioCtrlOff();
        }
        gp->systemLevelFlags |= (SYSTEM_LEVEL_STOP_LV0 | SYSTEM_LEVEL_STOP_LV1);
#endif
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_POWER_SHINE1 | PSND_SFX_BYPASS_MUTE), &mobj->position);
        animPoseSetAnim(mobj->poseId, "A_1", 1);
        itemEntry(mobj->position.x, mobj->position.y, mobj->position.z, "pwrblk1", ITEM_SHINE_SPRITE, ITEM_TYPE_POP, -1, NULL);
        mobj->state++;
        break;
    case 1:
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "A_2", 1);
        mobj->flags &= ~MOBJ_FLAG_HIT;
        mobj->typeData[1] = 1;
        mobj->state++;
        break;
    case 2:
        if (mobj->typeData[1]) {
            mobj->typeData[1]--;
            break;
        }
        itemForceGet(itemEntry(mobj->position.x, 40.0f + mobj->position.y, mobj->position.z, "pwrblk2", ITEM_SHINE_SPRITE, ITEM_TYPE_STILL_NO_TOUCH, -1, NULL));
        evtSetValue(NULL, mobj->flagVar, 1);
        marioKeyOn();
#if VERSION >= VERSION_JP
        if (!strcmp(gp->currentMapName, "dou_02")) {
            marioCtrlOn();
        } else if ((!strcmp(gp->currentMapName, "dou_05") || !strcmp(gp->currentMapName, "dou_10")) && mobj->hitFlag == MOBJ_FLAG_HIT_PARTY) {
            marioCtrlOn();
        }
#endif
        mobj->state = 90;
        break;
    case 90:
        mobjDelete(mobj->name);
        break;
    }
    return 0;
}

/**
 * Places a Shine Sprite block (MOBJ_PowerUpBlock).
 * args: name, x, y, z, event (stored, not run), flag variable (already set: placed empty),
 * Y rotation in degrees.
 */
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= (MOBJ_FLAG_HIT_HAMMER | MOBJ_FLAG_HIT_HEAD | MOBJ_FLAG_HIT_PARTY);
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

/**
 * Main function of the brick blocks: when hit it runs the event,
 * sets the flag variable and breaks apart with the "P_box_y" paper-peel animation drawn through an offscreen
 * capture, then deletes itself.
 */
static s32 mobj_brickblk(MapObjectEntry* mobj) {
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
#if VERSION >= VERSION_JP
        animGroupBaseAsync("P_box_y", type, NULL);
#endif
        if (mobj->flags & MOBJ_FLAG_HIT) {
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
        args1[0] = (EvtWord)iname;
        args1[1] = 0;
        args1[2] = (EvtWord)oname;
        args1[3] = 1;
        args1[4] = 0;
        args1[5] = 0;
        args1[6] = 0;
        args1[7] = 0;
        evt_img_alloc_capture(&ev1, TRUE);
        ev2.args = args2;
        args2[0] = (EvtWord)iname;
        args2[1] = 1;
        args2[2] = (EvtWord)oname;
        args2[3] = 1;
        args2[4] = 0;
        args2[5] = 0;
        args2[6] = 0;
        args2[7] = 0;
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
#if VERSION >= VERSION_JP
            mobj->state = 12;
        }
        if (img->flags & IMAGE_FLAG_NO_CAPTURE) {
#endif
            mobj->state = 12;
        }
        mobj->position = (Vec){0.0f, -2000.0f, 0.0f};
        mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
        mobj->flags |= MOBJ_FLAG_HIT;
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

/**
 * Main function of the coin blocks that pay out several coins (MOBJ_10CountBlock):
 * the first hit sets the flag variable and gives a coin (or runs the event instead)
 * and starts a 200-frame window in which every further hit gives another coin; then it becomes an empty block.
 */
static s32 mobj_10countblk(MapObjectEntry* mobj) {
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            if (mobj->flags & MOBJ_FLAG_HIDDEN) {
                mobj->flags &= ~MOBJ_FLAG_HIDDEN;
#if VERSION >= VERSION_JP
                strcpy(mobj->animName, "MOBJ_Block");
#endif
            }
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_TEN_COUNT_HIT1), &mobj->position);
            evtSetValue(NULL, mobj->flagVar, 1);
            if ((u32)mobj->event != 0) {
                mobjRunEvent(mobj, (s32*)mobj->event);
            } else {
                pouchAddCoin(1);
                itemEntry(mobj->position.x, mobj->position.y, mobj->position.z, NULL, ITEM_COIN, ITEM_TYPE_POP, -1, NULL);
                mobj->timer = 200;
            }
            mobj->state++;
            mobj->flags &= ~MOBJ_FLAG_HIT;
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
        if (mobj->flags & MOBJ_FLAG_HIT) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_TEN_COUNT_COIN1), &mobj->position);
            pouchAddCoin(1);
            itemEntry(mobj->position.x, mobj->position.y, mobj->position.z, NULL, ITEM_COIN, ITEM_TYPE_POP, -1, NULL);
            mobj->flags &= ~MOBJ_FLAG_HIT;
            animPoseSetAnim(mobj->poseId, "A_1", 1);
        }
        break;
    case 90:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_UNBREAK1), &mobj->position);
            animPoseSetAnim(mobj->poseId, "A_3", 1);
            mobj->flags &= ~MOBJ_FLAG_HIT;
            mobj->state++;
        }
        break;
    case 91:
        if (!(mobj->flags & MOBJ_FLAG_HIT)) {
            mobj->state = 90;
            break;
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_2", 1);
        mobj->state = 90;
        mobj->flags &= ~MOBJ_FLAG_HIT;
        break;
    }
    return 0;
}

/**
 * Places a brick-style block.
 * args: name, x, y, z, item, kind (literal: 0 brick, 1 brick ? block, 2 brick badge block, 3 coin block;
 * 10-13 the same but hidden until hit), event, flag variable (already set: broken or empty).
 */
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
    case MOBJ_BRICK_PLAIN:
        anim = "MOBJ_FlowBrickBlock";
        break;
    case MOBJ_BRICK_ITEM:
        anim = "MOBJ_BrickHatenaBlock";
        break;
    case MOBJ_BRICK_BADGE:
        anim = "MOBJ_BrickBadgeBlock";
        break;
    case MOBJ_BRICK_TEN_COIN:
        anim = "MOBJ_10CountBlock";
        break;
    case MOBJ_BRICK_HIDDEN_PLAIN:
        anim = "MOBJ_HiddenHatenaBlock";
        break;
    case MOBJ_BRICK_HIDDEN_ITEM:
        anim = "MOBJ_HiddenHatenaBlock";
        break;
    case MOBJ_BRICK_HIDDEN_BADGE:
        anim = "MOBJ_HiddenBadgeBlock";
        break;
    case MOBJ_BRICK_HIDDEN_TEN_COIN:
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
    if (kind >= MOBJ_BRICK_HIDDEN_PLAIN) {
        mobj->flags |= MOBJ_FLAG_HIDDEN;
    }
    switch (kind) {
    case MOBJ_BRICK_PLAIN:
    case MOBJ_BRICK_HIDDEN_PLAIN:
        mobj->mainFunc = mobj_brickblk;
        break;
    case MOBJ_BRICK_ITEM:
    case MOBJ_BRICK_HIDDEN_ITEM:
        mobj->mainFunc = mobj_badgeblk;
        break;
    case MOBJ_BRICK_BADGE:
    case MOBJ_BRICK_HIDDEN_BADGE:
        mobj->mainFunc = mobj_badgeblk;
        break;
    case MOBJ_BRICK_TEN_COIN:
    case MOBJ_BRICK_HIDDEN_TEN_COIN:
        mobj->mainFunc = mobj_10countblk;
        break;
    }
    mobj->event = val;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= (MOBJ_FLAG_HIT_HAMMER | MOBJ_FLAG_HIT_HEAD | MOBJ_FLAG_HIT_PARTY);
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobj->typeData[0] = item;
    mobjCalcMtx(mobj);
    if (kind < MOBJ_BRICK_HIDDEN_PLAIN) {
        animPoseSetAnim(mobj->poseId, "S_1", 1);
    } else if (kind == MOBJ_BRICK_HIDDEN_TEN_COIN) {
        animPoseSetAnim(mobj->poseId, "S_3", 1);
    } else {
        animPoseSetAnim(mobj->poseId, "S_1", 1);
    }
    if (evtGetValue(event, flagVar)) {
        const char* anim2 = NULL;
        switch (kind) {
        case MOBJ_BRICK_PLAIN:
        case MOBJ_BRICK_HIDDEN_PLAIN:
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            mobj->state = 99;
            break;
        case MOBJ_BRICK_ITEM:
        case MOBJ_BRICK_HIDDEN_ITEM:
            animPoseSetAnim(mobj->poseId, "S_2", 1);
            anim2 = "MOBJ_Block";
            mobj->state = 90;
            break;
        case MOBJ_BRICK_BADGE:
        case MOBJ_BRICK_HIDDEN_BADGE:
            animPoseSetAnim(mobj->poseId, "S_2", 1);
            anim2 = "MOBJ_PinkBlock";
            mobj->state = 90;
            break;
        case MOBJ_BRICK_TEN_COIN:
        case MOBJ_BRICK_HIDDEN_TEN_COIN:
            animPoseSetAnim(mobj->poseId, "S_2", 1);
            anim2 = "MOBJ_Block";
            mobj->state = 90;
            break;
        }
        if (anim2) {
            strcpy(mobj->animName, anim2);
        }
        mobj->flags &= ~MOBJ_FLAG_HIDDEN;
    }
    return EVT_RETURN_DONE;
}

/**
 * Main function of the breakable floor panels: a spin jump onto one (Super Boots) runs the event,
 * sets the flag variable, turns off the panel's collision and fades it out over 64 frames before deleting it.
 */
static s32 mobj_breaking_floor(MapObjectEntry* mobj) {
    MarioWork* mario = marioGetPtr();
    GXColor c3;
    GXColor c2;
    GXColor c1 = {0xff, 0xff, 0xff, 0xff};
    c2 = c1;
    c3 = c1;
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
#if VERSION <= VERSION_DEMO_JP
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BREAK_FLOOR1), &mobj->position);
#else
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BREAK_FLOOR1 | PSND_SFX_KEEP), &mobj->position);
#endif
            animPoseSetAnim(mobj->poseId, "A_1", 1);
            if ((u32)mobj->event != 0U) {
                mobjRunEvent(mobj, (s32*)mobj->event);
            }
            if (mobj->hitObj[0].hitObject) {
                mobj->hitObj[0].hitObject->flags |= HIT_FLAG_OFF;
            }
            mobj->flags |= MOBJ_FLAG_NO_SHADOW;
            evtSetValue(NULL, mobj->flagVar, 1);
            mobj->state = 1;
            animPoseSetMaterialFlagOn(mobj->poseId, MAP_OBJ_FLAG_COLOR);
            animPoseSetMaterialEvtColor(mobj->poseId, c2);
            mobj->timer = 64;
        }
        break;
    case 1:
        c3 = animPoseGetMaterialEvtColor(mobj->poseId);

        // FAKE: comma expression `(0, X)`: X becomes a separate compiler temp; the plain X changes the object
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

/**
 * Places a breakable floor panel.
 * args: name, x, y, z, kind (0 MOBJ_BreakingFloor, 1 black), event,
 * flag variable (already set: removed at once).
 */
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= MOBJ_FLAG_HIT_SPIN_JUMP;
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

/**
 * Draw callback for the flipping floor panels: renders the map below the panel from straight above into a
 * texture (the panel's paper texture), so the flipped panel shows the floor; restores the camera afterwards.
 */
static void kururing_capture(CameraId camId, MapObjectEntry* mobj) {
    u8 saved[sizeof(CameraEntry)];
    CameraEntry* cam;
    AnimationPoseData* data;
    Vec min, max;
    s32 width, height;
    s32 ww, hh;
    f32 t;
    memcpy(saved, camGetPtr(camId), sizeof(CameraEntry));
    data = animPoseGetAnimBaseDataPtr(mobj->poseId);
    min = (Vec){data->bboxMin.x, data->bboxMin.y, data->bboxMin.z};
    max = (Vec){data->bboxMax.x, data->bboxMax.y, data->bboxMax.z};
    ww = 10.0f * (max.x - min.x);
    hh = 10.0f * (max.z - min.z);
    cam = camGetPtr(camId);
    width = (u16)(ww + 1) & ~1;
    height = (u16)(hh + 1) & ~1;
    cam->cameraUp = (Vec){0.0f, 0.0f, -1.0f};
    cam->cameraAt = mobj->position;
    cam->cameraPos = mobj->position;
    t = tan(0.21816615760326385);
    cam->cameraPos.y += (f32)(u32)height / 2.0f / t;

    // The aspect is an integer division (width / height) converted to float afterwards.
    C_MTXPerspective(cam->projection, 25.0f, (f32)(width / height), 1.0f, 32767.0f);
    cam->projType = GX_PERSPECTIVE;
    C_MTXLookAt(cam->view, &cam->cameraPos, &cam->cameraUp, &cam->cameraAt);
    GXSetProjection(cam->projection, cam->projType);
    GXSetViewport(0.0f, 0.0f, (u32)width, (u32)height, 0.0f, 1.0f);
    GXSetScissor(0, 0, width, height);
    test_kururing_mapdisp(camId);
    sysWaitDrawSync();
    hh = ((u32)width >> 1) & 0x7fff;
    ww = ((u32)height >> 1) & 0x7fff;
    mobj->paperTexData = smartAlloc(GXGetTexBufferSize(hh, ww, GX_TF_RGBA8, 0, 0), SMART_GROUP_TEMP);
    GXSetTexCopySrc(0, 0, width, height);
    GXSetTexCopyDst(hh, ww, GX_TF_RGBA8, GX_TRUE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXCopyTex(mobj->paperTexData->address, GX_TRUE);
    GXInvalidateTexAll();
    GXPixModeSync();
    GXTexModeSync();
    GXInitTexObj(&mobj->paperTexObj, mobj->paperTexData->address, hh, ww, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, 0);
    GXInitTexObjLOD(&mobj->paperTexObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    PSMTXIdentity(mobj->paperTexMtx);
    memcpy(camGetPtr(camId), saved, sizeof(CameraEntry));
}

/** True when (px, pz) lies inside the square of half-width r around (x, z). */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
BOOL mobj_kf_ride_check(f32 x, f32 z, f32 px, f32 pz, f32 r) {
    if (x - r <= px && px <= x + r && z - r <= pz && pz <= z + r) {
        return TRUE;
    }
    return FALSE;
}


/**
 * Main function of the flipping floor panels (MOBJ_KururinFloor) that hide Star Pieces:
 * a Super Boots spin jump next to it (within 60) with nothing standing on it flips it over
 * and pops out the Star Piece; a hammer hit, a farther spin jump
 * or anything on it gives the half flip that carries Mario, partners, NPCs and items with it.
 * US and later skip a partner with flag 0x100 set.
 */
static s32 mobj_kururing_floor(MapObjectEntry* mobj) {
    PartyEntry* party;
    MarioWork* mario = marioGetPtr();
    s32 mode;
    f32 dist;
    ItemEntry* item;
    PartyEntry* member;
    MarioWork* mp;
    s32 i;
    NpcWork* wp;
    NpcEntry* npc;
    f32* joint;
    party = partyGetPtr(marioGetPartyId());
    switch (mobj->state) {
    case 0:
        if (!(mobj->flags & MOBJ_FLAG_HIT)) {
            break;
        }
        dist = distABf(mario->position.x, mario->position.z, mobj->position.x, mobj->position.z);
        if (__fabsf(mario->position.y - mobj->position.y) > 10.0f) {
            dist = 100000.0f;
        }
        if (mobj->hitFlag == MOBJ_FLAG_HIT_SPIN_JUMP) {
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
        if (mobj->hitFlag == MOBJ_FLAG_HIT_HAMMER) {
            mode = 1;
        }
        if (mode == 0) {
            mobj->flags &= ~MOBJ_FLAG_HIT;
            break;
        }
        if (mobj_kf_ride_check(mobj->position.x, mobj->position.z, mario->position.x, mario->position.z, 30.0f)) {
            mode = 1;
        }
        if (party) {
            if (mobj_kf_ride_check(mobj->position.x, mobj->position.z, party->position.x, party->position.z, 30.0f)) {
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
            mobj->flags &= ~MOBJ_FLAG_HIT;
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
            if (item == NULL || !(item->flags & ITEM_FLAG_ACTIVE)) {
                if (itemEntry(mobj->position.x, 30.0f + mobj->position.y, mobj->position.z, mobj->name, ITEM_STAR_PIECE, ITEM_TYPE_SCATTER_NO_PICKUP, mobj->flagVar, NULL)) {
                    item = itemNameToPtr(mobj->name);
                    itemFlagOn(item, ITEM_FLAG_CUSTOM_BOUND);
                    item->bounceFactor = 0.6f;
                    item->gravityScale = 1.0f;
                    item->maxBounces = 5;
                    item->velocityY = 300.0f;
                }
            }
        }
        animGetPtr()->poses[mobj->poseId].flags |= ANIM_POSE_FLAG_CHANGE_TEXTURE;
        mobj->flags |= MOBJ_FLAG_PAPER_TEX;
        dispEntry(CAMERA_SHADOW, 0.0f, DISP_MODE_OPAQUE, (DispCallback)kururing_capture, mobj);
        break;
    case 1:
        animGetPtr()->poses[mobj->poseId].flags |= ANIM_POSE_FLAG_CHANGE_TEXTURE;
        mobj->flags |= MOBJ_FLAG_PAPER_TEX;
        dispEntry(CAMERA_SHADOW, 0.0f, DISP_MODE_OPAQUE, (DispCallback)kururing_capture, mobj);
        if (!(animPoseGetLoopTimes(mobj->poseId) < 1.0f)) {
            animGetPtr()->poses[mobj->poseId].flags &= ~ANIM_POSE_FLAG_CHANGE_TEXTURE;
            mobj->flags &= ~MOBJ_FLAG_PAPER_TEX;
            mobj->flags &= ~MOBJ_FLAG_HIT;
            animPoseSetAnim(mobj->poseId, "S_1", 1);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_KURURIN_FLOOR_CLOSE1), &mobj->position);
            mobj->state = 0;
        }
        break;
    case 10:
        animGetPtr()->poses[mobj->poseId].flags |= ANIM_POSE_FLAG_CHANGE_TEXTURE;
        mobj->flags |= MOBJ_FLAG_PAPER_TEX;
        dispEntry(CAMERA_SHADOW, 0.0f, DISP_MODE_OPAQUE, (DispCallback)kururing_capture, mobj);

        // FAKE: joint points at worldPositions[0][1].y (the y read below) so its web is the unnamed add temp, not a named load (colours before mario's call temp)
        joint = &((Vec*)animPoseGetAnimPosePtr(mobj->poseId)->worldPositions->address)[1].y;
        mp = marioGetPtr();
        if (mobj_kf_ride_check(mobj->position.x, mobj->position.z, mp->position.x, mp->position.z, 30.0f)) {
            mp->position.y = *joint;
        }
        member = partyGetPtr(marioGetPartyId());
#if VERSION >= VERSION_US
        if (member && !(member->flags & PARTY_FLAG_IS_BEING_USED)) {
#else
        if (member) {
#endif
            if (mobj_kf_ride_check(mobj->position.x, mobj->position.z, member->position.x, member->position.z, 30.0f)) {
                member->position.y = *joint;
            }
        }
        member = partyGetPtr(marioGetExtraPartyId());
#if VERSION >= VERSION_US
        if (member && !(member->flags & PARTY_FLAG_IS_BEING_USED)) {
#else
        if (member) {
#endif
            if (mobj_kf_ride_check(mobj->position.x, mobj->position.z, member->position.x, member->position.z, 30.0f)) {
                member->position.y = *joint;
            }
        }
        wp = npcGetWorkPtr();
        npc = wp->entries;
        for (i = 0; i < wp->total; i++, npc++) {
            if (npc->flags & NPC_FLAG_IN_USE) {
                if (mobj_kf_ride_check(mobj->position.x, mobj->position.z, npc->position.x, npc->position.z, 42.420002f)) {
                    npc->position.y = *joint;
                }
            }
        }
        item = itemNearDistCheck(mobj->position.x, mobj->position.y, mobj->position.z, 42.420002f);
        if (item) {
            item->position.y = *joint;
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 2.0f) {
            break;
        }
        animGetPtr()->poses[mobj->poseId].flags &= ~ANIM_POSE_FLAG_CHANGE_TEXTURE;
        mobj->flags &= ~MOBJ_FLAG_PAPER_TEX;
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_KURURIN_FLOOR_CLOSE1), &mobj->position);
        mobj->timer = 20;
        mobj->state++;
    case 11:
        mobj->timer--;
        if (mobj->timer == 0) {
            mobj->state++;
        }
        break;
    case 12:
        mobj->flags &= ~MOBJ_FLAG_HIT;
        mobj->state = 0;
        break;
    }
    return 0;
}

/**
 * Places a flipping floor panel.
 * args: name, x, y, z, map group under it (flagged 0x400000), unused,
 * flag variable of the Star Piece underneath.
 */
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
    mobj->flags |= (MOBJ_FLAG_HIT_HAMMER | MOBJ_FLAG_HIT_SPIN_JUMP);
    mobj->flags |= MOBJ_FLAG_NO_SHADOW;
    mobjCalcMtx(mobj);
    mobj->scale2 = (Vec){200.0f, mobj->scale2.y, 200.0f};
    mobjCalcMtx2(mobj);
#if !MOBJ_EXTRA_OBJS
    {
        Vec dz = (Vec){x, y, z};
        char oname[16] = "o_";
        char iname[16] = "i_";
    }
#endif
    mapGrpFlagOn(grp, MAP_OBJ_FLAG_KURURING);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    animPoseWorldPositionEvalOn(mobj->poseId);
    return EVT_RETURN_DONE;
}

#if MOBJ_EXTRA_OBJS
/**
 * Main function of the trap door panels (early demos, PAL demo and PAL):
 * standing or spin-jumping on one sets the flag variable and flips it open with the paper texture,
 * then deletes it.
 */
static s32 mobj_trap_floor(MapObjectEntry* mobj) {
    switch (mobj->state) {
        case 0:
            if (mobj->flags & MOBJ_FLAG_HIT) {
                evtSetValue(NULL, mobj->flagVar, 1);
                animPoseSetAnim(mobj->poseId, "A_1", 1);
                psndSFXOn_3D((const char*)PSND_SFX_MOBJ_FLOOR_TRAP1, &mobj->position);
                mobj->state++;
            }
            break;
        case 1:
            animGetPtr()->poses[mobj->poseId].flags |= ANIM_POSE_FLAG_CHANGE_TEXTURE;
            mobj->flags |= MOBJ_FLAG_PAPER_TEX;
            dispEntry(CAMERA_SHADOW, 0.0f, DISP_MODE_OPAQUE, (DispCallback)kururing_capture, mobj);
            if (!(animPoseGetLoopTimes(mobj->poseId) < 1.0f)) {
                animGetPtr()->poses[mobj->poseId].flags &= ~ANIM_POSE_FLAG_CHANGE_TEXTURE;
                mobj->flags &= ~MOBJ_FLAG_PAPER_TEX;
                mobj->state++;
            }
            break;
        case 2:
            mobjDelete(mobj->name);
            break;
    }
    return 0;
}

/**
 * Places a trap door panel (MOBJ_trap; early demos, PAL demo and PAL).
 * args: name, x, y, z, map group under it, unused, flag variable (already set: not placed).
 */
USER_FUNC(evt_mobj_trap_floor) {
    EvtWord* args = event->args;
    const char* name;
    MapObjectEntry* mobj;
    f32 x, y, z;
    s32 inBattle;
    const char* grp;
    s32 flagVar;
    const char* anim;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    grp = (const char*)evtGetValue(event, args[4]);
    evtGetValue(event, args[5]);
    inBattle = gp->inBattle != 0;
    flagVar = args[6];
    anim = "MOBJ_trap";
    if (evtGetValue(event, flagVar)) {
        return EVT_RETURN_DONE;
    }
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobj->position = (Vec){x, 0.1f + y, z};
    mobj->mainFunc = mobj_trap_floor;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobj->flags |= MOBJ_FLAG_HIT_STAND | MOBJ_FLAG_HIT_SPIN_JUMP;
    mobj->flags |= MOBJ_FLAG_NO_SHADOW;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobjCalcMtx(mobj);
    mapGrpFlagOn(grp, MAP_OBJ_FLAG_KURURING);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    return EVT_RETURN_DONE;
}

/** Early demos, PAL demo and PAL: a bomb rock that peels away as a paper sheet when hit */
static s32 mobj_breaking_rock(MapObjectEntry* mobj) {
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
            if (mobj->flags & MOBJ_FLAG_HIT) {
                if ((s32*)mobj->event != NULL) {
                    mobjRunEvent(mobj, (s32*)mobj->event);
                }
                if (mobj->flagVar) {
                    evtSetValue(NULL, mobj->flagVar, 1);
                }
                mobj->state++;
            }
            break;
        case 1:
            animPaperPoseEntry("OFF_d_bom", type);
            offscreenEntry(oname);
            imgEntry(iname, type);
            mobj->offscreenId = offscreenNameToId(oname);
            ev1.args = args1;
            args1[0] = (EvtWord)iname;
            args1[1] = 0;
            args1[2] = (EvtWord)oname;
            args1[3] = 1;
            args1[4] = 0;
            args1[5] = 0;
            args1[6] = 0;
            args1[7] = 0;
            evt_img_alloc_capture(&ev1, TRUE);
            ev2.args = args2;
            args2[0] = (EvtWord)iname;
            args2[1] = 1;
            args2[2] = (EvtWord)oname;
            args2[3] = 1;
            args2[4] = 0;
            args2[5] = 0;
            args2[6] = 0;
            args2[7] = 0;
            evt_img_alloc_capture(&ev2, TRUE);
            img = imgNameToPtr(iname, type);
            img->poseId = animPaperPoseGetId("OFF_d_bom", type);
            img->animName = "A_1";
            img->animTime = animTimeGetTime(type);
            imgSetShadow(img, 2);
            psndSFXOn_3D((const char*)PSND_SFX_MOBJ_WALL_BREAK1, &mobj->position);
            mobj->state++;
            break;
        case 2:
            img = imgNameToPtr(iname, type);
            if (img->loopTimes >= 1.0f) {
                mobj->state++;
            }
            mobj->position = (Vec){0.0f, -2000.0f, 0.0f};
            mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
            mobj->flags |= MOBJ_FLAG_HIT;
            break;
        case 3:
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

/**
 * Places a rock that a Bobbery bomb blows away (MOBJ_BombRock; early demos, PAL demo and PAL).
 * args: name, x, y, z, event, flag variable (already set: not placed).
 */
USER_FUNC(evt_mobj_breaking_rock) {
    EvtWord* args = event->args;
    const char* name;
    s32 val;
    s32 flagVar;
    s32 inBattle;
    MapObjectEntry* mobj;
    f32 x, y, z;
    const char* anim;
    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    val = evtGetValue(event, args[4]);
    inBattle = gp->inBattle != 0;
    flagVar = args[5];
    anim = "MOBJ_BombRock";
    if (evtGetValue(event, flagVar)) {
        return EVT_RETURN_DONE;
    }
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->mainFunc = mobj_breaking_rock;
    mobj->event = val;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= MOBJ_FLAG_HIT_BOBBERY;
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    return EVT_RETURN_DONE;
}

#endif

/**
 * Bowser stage power-up pickup. args: x, y, z of the pickup.
 * Adds 500 points there (1000 more at level 3) and powers Bowser up.
 */
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

/**
 * Main function of the obstacle blocks in Bowser's side-scrolling stages:
 * they only break for a fire breath or head bump (plus a hip drop from level 2, anything at level 3).
 * Breaking one runs the event, sets the flag, scores 400 (500 while invincible),
 * shatters it and launches a Bros. Meat power-up.
 */
static s32 mobj_koopa_ojama_blk_evt(MapObjectEntry* mobj) {
    BOOL ok = FALSE;
    ItemEntry* item;
    Vec dv;
#if !MOBJ_EXTRA_OBJS
    // FAKE: unused Vec local; removing the block changes the object
    dv = (Vec){0.0f, -2000.0f, 0.0f};
#endif
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            switch (kpaGetLevel()) {
            case 0:
            case 1:
                if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_FLAME || mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_HEAD) {
                    ok = TRUE;
                }
                break;
            case 2:
                if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_FLAME || mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_HIP || mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_HEAD) {
                    ok = TRUE;
                }
                break;
            case 3:
                ok = TRUE;
                break;
            }
            if (!ok) {
                mobj->flags &= ~MOBJ_FLAG_HIT;
                break;
            }
            mobjRunEvent(mobj, (s32*)mobj->event);
            evtSetValue(NULL, mobj->flagVar, 1);
            if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_HEAD) {
                npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
            }
            if (mobj->hitObj[0].hitObject) {
                mobj->hitObj[0].hitObject->flags |= HIT_FLAG_OFF;
            }
            mobj->flags |= MOBJ_FLAG_NO_SHADOW;
            kpaAddScore(kpaMutekiCheck() ? 500 : 400);
            mobj->state = 10;
        }
        break;
    case 10:
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_BLOCK_OJAMA1), &mobj->position);
        effMObjBrokenEntry(5, mobj->position.x, 25.0f + mobj->position.y, 25.0f + mobj->position.z);
        item = itemEntry(mobj->position.x, 25.0f + mobj->position.y, mobj->position.z, mobj->name, ITEM_BOWSER_BROS_MEAT, ITEM_TYPE_SCATTER_NO_PICKUP, -1, kpa_powerup_evt);
        if (item) {
            itemStatusOn(item, ITEM_FLAG2_SKIP_BOUND_INIT);
            itemFlagOn(item, ITEM_FLAG_QUICK_GET | ITEM_FLAG_CUSTOM_BOUND);
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

/**
 * Places a Bowser-stage obstacle block.
 * args: name, x, y, z, event, flag variable (already set: removed), animation group.
 */
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->position = (Vec){x, y, z};
    mobj->mainFunc = mobj_koopa_ojama_blk_evt;
    mobj->event = val;
    mobj->extraEvent = 0;
    mobj->flags |= (MOBJ_FLAG_HIT_KOOPA_HEAD | MOBJ_FLAG_HIT_KOOPA_BODY_LV3 | MOBJ_FLAG_HIT_KOOPA_HIP | MOBJ_FLAG_HIT_KOOPA_FLAME);
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    return EVT_RETURN_DONE;
}

#if !MOBJ_EXTRA_OBJS
/** Not called: it only places the model-name strings that stripped code referenced. */
void mobj_koopa_stone_blk(void) {
    mobjNameToPtr("MOBJ_BombRock");
    mobjNameToPtr("OFF_d_bom");
    mobjNameToPtr("MOBJ_trap");
}
#endif

/**
 * Main function of the Bowser-stage stone blocks:
 * they never react (the strings only keep stripped names in .rodata).
 */
static s32 mobj_koopa_stone_blk_evt(MapObjectEntry* mobj) {
    return 0;
}

/**
 * Places an unbreakable Bowser-stage stone block.
 * args: name, x, y, z, event (stored, never run), flag variable, animation group.
 */
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->position = (Vec){x, y, z};
    mobj->mainFunc = mobj_koopa_stone_blk_evt;
    mobj->event = val;
    mobj->extraEvent = 0;
    mobj->flagVar = flagVar;
    mobj->state = 99;
    return EVT_RETURN_DONE;
}

/**
 * Main function of the Bowser-stage brick blocks:
 * at level 0 only fire breaks them, level 1 adds the head bump, level 2 the hip drop, level 3 anything.
 * A head bump that cannot break one just bounces it.
 * Breaking one runs the event, sets the flag, scores 400 (500 while invincible) and shatters it.
 */
static s32 mobj_koopa_brickblk(MapObjectEntry* mobj) {
    BOOL ok = FALSE;
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            switch (kpaGetLevel()) {
            case 0:
                if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_FLAME) {
                    ok = TRUE;
                }
                break;
            case 1:
                if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_FLAME || mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_HEAD) {
                    ok = TRUE;
                }
                break;
            case 2:
                if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_FLAME || mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_HIP || mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_HEAD) {
                    ok = TRUE;
                }
                break;
            case 3:
                ok = TRUE;
                break;
            }
            if (!ok) {
                if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_HEAD) {
                    npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
                    mobj->state = 20;
                    break;
                }
                mobj->flags &= ~MOBJ_FLAG_HIT;
                break;
            }
            mobjRunEvent(mobj, (s32*)mobj->event);
            evtSetValue(NULL, mobj->flagVar, 1);
            if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_HEAD) {
                npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
            }
            if (mobj->hitObj[0].hitObject) {
                mobj->hitObj[0].hitObject->flags |= HIT_FLAG_OFF;
            }
            mobj->flags |= MOBJ_FLAG_NO_SHADOW;
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
        if (!(mobj->flags & MOBJ_FLAG_HIT)) {
            mobj->state = 0;
            break;
        }
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_1", 1);
        mobj->state = 0;
        mobj->flags &= ~MOBJ_FLAG_HIT;
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
    inBattle = gp->inBattle != 0;
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= (MOBJ_FLAG_HIT_KOOPA_HEAD | MOBJ_FLAG_HIT_KOOPA_BODY_LV3 | MOBJ_FLAG_HIT_KOOPA_HIP | MOBJ_FLAG_HIT_KOOPA_FLAME);
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

/**
 * Main function of the Bowser-stage ?
 * blocks: a head bump gives the item (a Bros. Meat power-up), a coin to Bowser's tally, or runs the event;
 * the empty block left behind only breaks at level 3. Hidden blocks get their collision when first bumped.
 */
static s32 mobj_koopa_badgeblk(MapObjectEntry* mobj) {
    ItemEntry* item;
    BOOL ok;
    marioGetPtr();
    ok = FALSE;
    switch (mobj->state) {
    case 0:
        if (!(mobj->flags & MOBJ_FLAG_HIT)) {
            break;
        }
        if (mobj->hitFlag != MOBJ_FLAG_HIT_KOOPA_HEAD) {
            mobj->flags &= ~MOBJ_FLAG_HIT;
            break;
        }
        if (mobj->flags & MOBJ_FLAG_HIDDEN) {
            mobjHitEntry(mobj, 0);
        }
        animPoseSetAnim(mobj->poseId, "A_1", 1);
        if ((s32*)mobj->event != NULL) {
            mobjRunEvent(mobj, (s32*)mobj->event);
        } else if (mobj->typeData[0] == ITEM_BOWSER_BROS_MEAT) {
            itemEntry(mobj->position.x, 12.5f + mobj->position.y, mobj->position.z, mobj->name, mobj->typeData[0], ITEM_TYPE_SCATTER_NO_PICKUP, -1, kpa_powerup_evt);
            item = itemNameToPtr(mobj->name);
            itemStatusOn(item, ITEM_FLAG2_SKIP_BOUND_INIT);
            itemFlagOn(item, ITEM_FLAG_QUICK_GET | ITEM_FLAG_CUSTOM_BOUND);
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
            itemEntry(mobj->position.x, mobj->position.y, mobj->position.z, NULL, ITEM_COIN, ITEM_TYPE_POP, -1, NULL);
            evtSetValue(NULL, mobj->flagVar, 1);
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_COUNT_COIN1), &mobj->position);
        }
        if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_HEAD) {
            npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
        }
        mobj->state++;
        break;
    case 1:
        if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
            break;
        }
        animPoseSetAnim(mobj->poseId, "S_2", 1);
        mobj->flags &= ~MOBJ_FLAG_HIT;
        mobj->state = 90;
        strcpy(mobj->animName, "MOBJ_Block");
        break;
    case 90:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            switch (kpaGetLevel()) {
            case 3:
                ok = TRUE;
                break;
            case 2: break;
            }
            if (!ok) {
                mobj->flags &= ~MOBJ_FLAG_HIT;
                break;
            }
            if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_HEAD) {
                npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
            }
            if (mobj->hitObj[0].hitObject) {
                mobj->hitObj[0].hitObject->flags |= HIT_FLAG_OFF;
            }
            mobj->flags |= MOBJ_FLAG_NO_SHADOW;
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

/**
 * Places a Bowser-stage ? block.
 * args: name, x, y, z, item (ITEM_BOWSER_BROS_MEAT or a coin), event, flag variable (already set: placed empty).
 */
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= (MOBJ_FLAG_HIT_KOOPA_HEAD | MOBJ_FLAG_HIT_KOOPA_BODY_LV3 | MOBJ_FLAG_HIT_KOOPA_HIP | MOBJ_FLAG_HIT_KOOPA_FLAME);
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

/**
 * Places a hidden Bowser-stage ? block that appears when head-bumped from below.
 * args: name, x, y, z, item, unused, event, flag variable (already set: placed empty).
 */
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

    // FAKE: gp->inBattle is loaded into inBattle before the test; `inBattle = gp->inBattle != 0` changes the object
    inBattle = gp->inBattle;
    inBattle = inBattle != 0;
    anim = "MOBJ_HiddenHatenaBlock";
    if (animGroupBaseAsync(anim, inBattle, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, anim);
    mobj = mobjNameToPtr(name);
    mobjSetPosition(name, x, y, z);
    mobj->flags |= MOBJ_FLAG_HIDDEN;
    mobjHitEntry(mobj, 1);
    mobj->mainFunc = mobj_koopa_badgeblk;
    mobj->event = val;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= (MOBJ_FLAG_HIT_KOOPA_HEAD | MOBJ_FLAG_HIT_KOOPA_BODY_LV3 | MOBJ_FLAG_HIT_KOOPA_HIP | MOBJ_FLAG_HIT_KOOPA_FLAME);
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

/**
 * Main function of the plain Bowser-stage blocks:
 * they break only at level 3, running the event and setting the flag variable.
 */
static s32 mobj_koopa_blk(MapObjectEntry* mobj) {
    BOOL ok = FALSE;
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            switch (kpaGetLevel()) {
            case 3:
                ok = TRUE;
                break;
            case 2:
                break;
            }
            if (!ok) {
                mobj->flags &= ~MOBJ_FLAG_HIT;
                break;
            }
            mobjRunEvent(mobj, (s32*)mobj->event);
            evtSetValue(NULL, mobj->flagVar, 1);
            if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_HEAD) {
                npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
            }
            if (mobj->hitObj[0].hitObject) {
                mobj->hitObj[0].hitObject->flags |= HIT_FLAG_OFF;
            }
            mobj->flags |= MOBJ_FLAG_NO_SHADOW;
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

/** Places a plain Bowser-stage block (MOBJ_Block). args: name, x, y, z, unused, event, flag variable. */
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->flags |= (MOBJ_FLAG_HIT_KOOPA_HEAD | MOBJ_FLAG_HIT_KOOPA_BODY_LV3 | MOBJ_FLAG_HIT_KOOPA_HIP | MOBJ_FLAG_HIT_KOOPA_FLAME);
    mobj->flagVar = flagVar;
    mobj->state = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    return EVT_RETURN_DONE;
}

/**
 * Main function of the green pipes in Bowser's stages: fire breath breaks them at levels 1-2, anything at level
 * 3. Breaking one runs the event, sets the flag, scores 400 (500 while invincible) and shatters it.
 */
static s32 mobj_koopa_dokan_evt(MapObjectEntry* mobj) {
    BOOL ok = FALSE;
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            switch (kpaGetLevel()) {
            case 0:
                break;
            case 1:
            case 2:
                if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_FLAME) {
                    ok = TRUE;
                }
                break;
            case 3:
                ok = TRUE;
                break;
            }
            if (!ok) {
                mobj->flags &= ~MOBJ_FLAG_HIT;
                break;
            }
            mobjRunEvent(mobj, (s32*)mobj->event);
            evtSetValue(NULL, mobj->flagVar, 1);
            if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_HEAD) {
                npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
            }
            if (mobj->hitObj[0].hitObject) {
                mobj->hitObj[0].hitObject->flags |= HIT_FLAG_OFF;
            }
            mobj->flags |= MOBJ_FLAG_NO_SHADOW;
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
    mobj->flags |= (MOBJ_FLAG_NO_SHADOW | MOBJ_FLAG_UPDATE_MTX);
    mobj->position = (Vec){x, y, z};
    mobj->mainFunc = mobj_koopa_dokan_evt;
    mobj->event = val;
    mobj->extraEvent = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "S_1", 1);
    mobj->flags |= (MOBJ_FLAG_HIT_KOOPA_HEAD | MOBJ_FLAG_HIT_KOOPA_BODY_LV3 | MOBJ_FLAG_HIT_KOOPA_HIP | MOBJ_FLAG_HIT_KOOPA_FLAME);
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    return EVT_RETURN_DONE;
}

/**
 * Spawns a fire effect at args[0..2] lasting args[3] frames and grows it to full size over the following frames;
 * blocks until it is fully grown. Used by evt_fire when Bowser burns a coral.
 */
USER_FUNC(fire_func) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    s32 dur = evtGetValue(event, args[3]);
    EffectEntry* eff;
    f32 scaleX, scaleY;
    if (isFirstCall) {
        f32 t = (f32)irand(10) / 10.0f;
        event->userdatap[0] = effFireEntry(5, dur, x, y, z, 0.8f + t);
    }
    scaleX = event->userdataf[1];
    scaleY = event->userdataf[2];
    scaleX = scaleX + (1.0f - scaleX) / 8.0f;
    scaleY = scaleY + (1.0f - scaleY) / 8.0f;
    eff = event->userdatap[0];
    ((EffFireWork*)eff->userdata)->scaleX = scaleX;
    ((EffFireWork*)eff->userdata)->scaleY = scaleY;
    if (scaleX >= 0.99f) {
        ((EffFireWork*)eff->userdata)->scaleX = 1.0f;
        ((EffFireWork*)eff->userdata)->scaleY = 1.0f;
        return EVT_RETURN_DONE;
    }
    event->userdataf[1] = scaleX;
    event->userdataf[2] = scaleY;
    return EVT_RETURN_BLOCK;
}

/**
 * Main function of the coral in Bowser's stages:
 * fire breath burns it (anything at level 3), running the event,
 * setting the flag and scoring 400 (500 while invincible);
 * it then lights three staggered evt_fire flames up the coral before crumbling and deleting itself.
 */
static s32 mobj_koopa_sango_evt(MapObjectEntry* mobj) {
    BOOL ok = FALSE;
    EventEntry* ev;
    switch (mobj->state) {
    case 0:
        if (mobj->flags & MOBJ_FLAG_HIT) {
            switch (kpaGetLevel()) {
            case 0:
            case 1:
            case 2:
                if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_FLAME) {
                    ok = TRUE;
                }
                break;
            case 3:
                ok = TRUE;
                break;
            }
            if (!ok) {
                mobj->flags &= ~MOBJ_FLAG_HIT;
                break;
            }
            mobjRunEvent(mobj, (s32*)mobj->event);
            evtSetValue(NULL, mobj->flagVar, 1);
            if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_HEAD) {
                npcKoopaModeMobjBoundDeadCheck(mobj->hitObj[0].hitObject);
            }
            if (mobj->hitObj[0].hitObject) {
                mobj->hitObj[0].hitObject->flags |= HIT_FLAG_OFF;
            }
            mobj->flags |= MOBJ_FLAG_NO_SHADOW;
            kpaAddScore(kpaMutekiCheck() ? 500 : 400);
            mobj->state = 10;
        }
        break;
    case 10:
        if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_FLAME) {
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_BTL_DAMAGE_FIRE1), &mobj->position);
        }
        ev = evtEntry(evt_fire, 0, 0);
        ev->lwData[0] = FLOAT(mobj->position.x);
        ev->lwData[1] = FLOAT(mobj->position.y);
        ev->lwData[2] = FLOAT(10.0f + mobj->position.z);
        ev->lwData[3] = 90;
        mobj->state++;
        break;
    case 20:
        ev = evtEntry(evt_fire, 0, 0);
        ev->lwData[0] = FLOAT(10.0f + mobj->position.x);
        ev->lwData[1] = FLOAT(30.0f + mobj->position.y);
        ev->lwData[2] = FLOAT(10.0f + mobj->position.z);
        ev->lwData[3] = 80;
        mobj->state++;
        break;
    case 30:
        ev = evtEntry(evt_fire, 0, 0);
        ev->lwData[0] = FLOAT(mobj->position.x - 10.0f);
        ev->lwData[1] = FLOAT(60.0f + mobj->position.y);
        ev->lwData[2] = FLOAT(10.0f + mobj->position.z);
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

/**
 * Places a burnable coral (MOBJ_sango) in a Bowser stage.
 * args: name, x, y, z, event, flag variable (already set: removed).
 */
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
    mobj->flags |= (MOBJ_FLAG_UPDATE_MTX | MOBJ_FLAG_NO_CULL);
    mobj->position = (Vec){x, y, z};
    mobj->mainFunc = mobj_koopa_sango_evt;
    mobj->event = val;
    mobj->extraEvent = 0;
    mobj->flags |= (MOBJ_FLAG_HIT_KOOPA_HEAD | MOBJ_FLAG_HIT_KOOPA_BODY_LV3 | MOBJ_FLAG_HIT_KOOPA_HIP | MOBJ_FLAG_HIT_KOOPA_FLAME);
    mobj->flagVar = flagVar;
    mobj->state = 0;
    if (evtGetValue(event, flagVar)) {
        mobj->state = 99;
    }
    return EVT_RETURN_DONE;
}

/**
 * Main function of the Fire Bar platforms in Bowser's stages:
 * rotates its column of c_fireber NPCs around the base every frame (typeDataF[3] degrees) with a crackling sound
 * while Mario is near.
 * Fire breath (anything at level 3) breaks the base:
 * event, flag, 400/500 points, and the fireballs fly off towards the camera and are deleted.
 */
static s32 mobj_koopa_fireber_dodai_evt(MapObjectEntry* mobj) {
    Mtx mtx;
    Mtx rot;
    char buf[0x20];
    char buf2[0x20];
    BOOL ok = FALSE;
#if VERSION >= VERSION_JP
    NpcEntry* npc;
#endif
    s32 i;
#if VERSION <= VERSION_DEMO_JP
    NpcEntry* npc;
#endif
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
        if (!(mobj->flags & MOBJ_FLAG_HIT)) {
            break;
        }
        switch (kpaGetLevel()) {
        case 0:
        case 1:
        case 2:
            if (mobj->hitFlag == MOBJ_FLAG_HIT_KOOPA_FLAME) {
                ok = TRUE;
            }
            break;
        case 3:
            ok = TRUE;
            break;
        }
        if (!ok) {
            mobj->flags &= ~MOBJ_FLAG_HIT;
            break;
        }
        mobjRunEvent(mobj, (s32*)mobj->event);
        evtSetValue(NULL, mobj->flagVar, 1);
        if (mobj->hitObj[0].hitObject) {
            mobj->hitObj[0].hitObject->flags |= HIT_FLAG_OFF;
        }
        mobj->flags |= MOBJ_FLAG_NO_SHADOW;
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
            npc->statusFlags |= NPC_STATUS_DEAD;
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

        // npc is the last fire bar NPC from the loop above; with typeData[1] == 0 the loop never sets it.
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

/**
 * Places a Fire Bar base with its fireballs.
 * args: direction (nonzero: positive rotation), name, x, y, z, fireball count,
 * rotation speed in degrees per frame (0 means 1), event, flag variable (already set: removed).
 * The fireballs are NPCs named name plus a hex index; US and later offset each one's Z by its index.
 */
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
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    mobj->position = (Vec){x, y, z};
    mobj->mainFunc = mobj_koopa_fireber_dodai_evt;
    mobj->event = script;
    mobj->extraEvent = 0;
    mobj->flags |= (MOBJ_FLAG_HIT_KOOPA_HEAD | MOBJ_FLAG_HIT_KOOPA_BODY_LV3 | MOBJ_FLAG_HIT_KOOPA_HIP | MOBJ_FLAG_HIT_KOOPA_FLAME);
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
        npc->flags |= NPC_FLAG_2D;
        npc->hazardFlags |= (KPA_ENCOUNT_STOMP | KPA_ENCOUNT_FIRE | KPA_ENCOUNT_STAR | KPA_ENCOUNT_MOBJ);
        npc->position.x = x;
        npc->position.y = 25.0f + y + 25.0f * i;
#if VERSION >= VERSION_US
        npc->position.z = 175.0f + z + i;
#else
        npc->position.z = 175.0f + z;
#endif
        animPoseSetAnim(npc->poseId, "FRB_S_1", 1);
        npc->flags |= NPC_FLAG_NO_SHADOW;
    }
    return EVT_RETURN_DONE;
}

#if VERSION <= VERSION_DEMO_JP
/** Early demos only: turn Mario to face the given angle */
static USER_FUNC(kpa_set_dir) {
    f32 dir = evtGetFloat(event, event->args[0]);
    MarioWork* mario = marioGetPtr();
    mario->directionView = dir;
    mario->moveDirection = 270.0f;
    mario->displayDirectionTarget = reviseAngle(270.0f - dir);
    mario->displayDirection = mario->displayDirectionTarget;
    return EVT_RETURN_DONE;
}
#endif
/**
 * Writes the name of the goal pole whose event is running (event->thisMapObj) to args[0],
 * or 0 outside a map object event.
 */
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

/** Writes the name of the flag attached to the running goal pole (its linkedName, "<pole>_h") to args[0], or 0. */
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

/** Writes Bowser's power level (kpaGetLevel, 0-3) to args[0]. */
static USER_FUNC(kpa_get_level) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], kpaGetLevel());
    return EVT_RETURN_DONE;
}

/** Writes the width of the player's collision box (Bowser's, growing with his level) to args[0] as an integer. */
USER_FUNC(kpa_get_width) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], (s32)marioGetPtr()->wPlayerCollisionBox.x);
    return EVT_RETURN_DONE;
}

#if VERSION <= VERSION_DEMO_JP
/** Early demos only: read Mario's position into three float variables */
static USER_FUNC(kpa_get_mario_pos) {
    EvtWord* args = event->args;
    MarioWork* mario = marioGetPtr();
    evtSetFloat(event, args[0], mario->position.x);
    evtSetFloat(event, args[1], mario->position.y);
    evtSetFloat(event, args[2], mario->position.z);
    return EVT_RETURN_DONE;
}
#endif
/** Formats the score args[0] (0-9999) without leading zeros into the floating pole-score text. */
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

/** Draw callback that prints the pole score text at half size at the stored position. */
static void kpa_disp_pole_score(CameraId camId, void* param) {
    Mtx trans;
    Mtx scale;
    PSMTXTrans(trans, psw->x, psw->y, 300.0f);
    PSMTXScale(scale, 0.5f, 0.5f, 0.5f);
    PSMTXConcat(trans, scale, trans);
    FontDrawStart();
    FontDrawColor((GXColor){255, 255, 255, 255});
    FontDrawStringMtx(trans, psw->str);
}

/**
 * Shows the goal-pole score rising from (args[0], args[1], args[2]) by args[3] per frame until Y 230;
 * never returns, so it runs in its own inline event.
 */
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
    dispEntry(CAMERA_3D, 0.0f, DISP_MODE_XLU, kpa_disp_pole_score, NULL);
    return EVT_RETURN_BLOCK;
}

/** Allocates and clears the pole-score text work from map memory; called at the start of the goal-pole sequence. */
USER_FUNC(kpa_disp_pole_init) {
    psw = _mapAlloc(sizeof(KpaPoleScoreWork));
    psw->str[0] = 0;
    psw->z = 0.0f;
    psw->y = 0.0f;
    psw->x = 0.0f;
    return EVT_RETURN_DONE;
}

/** Clears Mario's air timer so Bowser's jump onto the goal pole does not count as a long fall. */
USER_FUNC(kpa_clear_jump_data) {
    MarioWork* mario = marioGetPtr();
    mario->airTimer = 0;
    return EVT_RETURN_DONE;
}

#if VERSION <= VERSION_DEMO_JP
/** Event that runs kpa_powerup with LW(0)-LW(2); started when Bowser grabs a power-up in his side-scrolling stages. */
EVT_BEGIN(kpa_powerup_evt)
    CALL(kpa_powerup, LW(0), LW(1), LW(2))
    RETURN
EVT_END()
EVT_BEGIN(evt_fire)
    CALL(fire_func, LW(0), LW(1), LW(2), LW(3))
    RETURN
EVT_END()
#endif
/**
 * Goal pole sequence for small Bowser (levels 0-1):
 * shows the score, slides him and the flag down to the ground, then moves the camera on and hands control back.
 * Run by mobj_koopa_pole_evt with the score in LW(0).
 */
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
#if VERSION <= VERSION_DEMO_JP
    CALL(kpa_get_mario_pos, LW(0), LW(1), LW(2))
    SUBTRACT_FLOAT(LW(1), FLOAT(25.0f))
    SET_FLOAT_VALUE(LW(14), FLOAT(155.0f))
    DIVIDE_FLOAT(LW(14), LW(1))
    MULTIPLY_FLOAT(LW(14), FLOAT(1.5f))
    SET_FLOAT_VALUE(LW(13), FLOAT(180.0f))
    DIVIDE_FLOAT(LW(13), LW(1))
    MULTIPLY_FLOAT(LW(13), FLOAT(1.5f))
#else
    SET_FLOAT_VALUE(LW(14), FLOAT(2.0f))
    SET_FLOAT_VALUE(LW(13), FLOAT(2.0f))
#endif
    SET_FLOAT_VALUE(LW(7), FLOAT(50.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_mobj_get_position, LW(15), LW(3), LW(4), LW(5))
        ADD_FLOAT(LW(3), FLOAT(7.5f))
        CALL(kpa_disp_pole_score_main, LW(3), LW(7), LW(5), LW(13))
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(1), LW(2), LW(3))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_POST_SLIDE1"), LW(1), LW(2), LW(3), LW(10))
#if VERSION >= VERSION_JP
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
#else
        LOOP(0)
            CALL(kpa_get_mario_pos, LW(0), LW(1), LW(2))
            SUBTRACT_FLOAT(LW(1), FLOAT(1.5f))
            CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
            CALL(evt_mobj_get_position, LW(15), LW(3), LW(4), LW(5))
            SUBTRACT_FLOAT(LW(4), LW(14))
            CALL(evt_mobj_set_y_position, LW(15), LW(4))
        IF_INT_LESS_OR_EQUAL(LW(1), FLOAT(25.0f))
            CALL(evt_mario_set_pos, LW(0), FLOAT(25.0f), LW(2))
            CALL(evt_mobj_set_y_position, LW(15), FLOAT(25.0f))
#endif
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(10))
    WAIT_MS(200)
    CALL(evt_cam_get_pos, CAMERA_3D, LW(4), LW(5), LW(6))
    CALL(evt_cam_get_at, CAMERA_3D, LW(7), LW(8), LW(9))
    CALL(evt_cam3d_evt_set, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9), 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(kpa_get_width, LW(3))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    MULTIPLY_FLOAT(LW(3), FLOAT(1.1992188f))
    ADD_FLOAT(LW(0), LW(3))
    CALL(evt_cam3d_get_shift, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_cam3d_event_from_road, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
    CALL(evt_cam3d_evt_set, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9), 300, EASING_LINEAR)
    WAIT_MS(200)
    CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
#if VERSION <= VERSION_DEMO_JP
    CALL(kpa_set_dir, 270)
#else
    CALL(evt_mario_set_dir, 270, 0, 0)
#endif
    CALL(evt_mario_set_mov_spd, 0)
    WAIT_MS(200)
    ADD(LW(0), 20)
    CALL(evt_mario_set_pos, LW(0), 0, 10)
    WAIT_FRAMES(1)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_cam3d_evt_off, 200, EASING_SINE_IN_OUT)
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
    SET_VALUE(LSWF(5), 1) // LSWF_KPA_GOAL_WALK_STARTED (rels/kpa/include/kpa.h)
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
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
        WAIT_MS(2000)
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
        CALL(evt_snd_sfxoff, LW(10))
        CALL(kpa_chg_pose, STRING("KPA2_S_1"))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static s32 mobj_koopa_pole_evt(MapObjectEntry* mobj) {
    MarioWork* mario = marioGetPtr();
    s32 score;
    s32 next = 0;
    s32 i;
    switch (mobj->state) {
    case 0:
        if (!(mobj->flags & MOBJ_FLAG_HIT)) {
            break;
        }
        if (mario->characterId != CHARACTER_BOWSER) {
            mobj->flags &= ~MOBJ_FLAG_HIT;
            break;
        }
        switch (kpaGetLevel()) {
        case 0:
        case 1:
            if (mario->position.y < 25.0f) {
                mobj->flags &= ~MOBJ_FLAG_HIT;
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
        for (i = 0; i < npcGetWorkPtr()->total; i++) {
            NpcEntry* npc = &npcGetWorkPtr()->entries[i];
            u32 flags = npc->flags;
            if ((flags & NPC_FLAG_IN_USE) && !(npc->statusFlags & NPC_STATUS_DEAD) && (flags & NPC_FLAG_2D) && (flags & NPC_FLAG_ENEMY)) {
                npcGroupDead(npc, KPA_ENCOUNT_GOAL_POLE);
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

/**
 * Places the goal pole and its flag ("<name>_h", 180 above).
 * args: name, x, y, z, final event, event run after reaching the fortress,
 * flag variable (already set: inactive).
 */
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
    mobj->flags |= (MOBJ_FLAG_UPDATE_MTX | MOBJ_FLAG_NO_CULL);
    mobj->position = (Vec){x, y, z};
    mobj->mainFunc = mobj_koopa_pole_evt;
    mobj->event = val;
    mobj->extraEvent = val2;
    mobj->flags |= (MOBJ_FLAG_HIT_KOOPA_FRONT | MOBJ_FLAG_HIT_STAND | MOBJ_FLAG_HIT_KOOPA_HEAD | MOBJ_FLAG_HIT_KOOPA_BODY_LV3 | MOBJ_FLAG_HIT_KOOPA_HIP);
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
        hata->hitObj[0].hitObject->flags |= HIT_FLAG_OFF;
    }
    return EVT_RETURN_DONE;
}

