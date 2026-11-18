// Script commands for the tutorial (lecture) scenes: system level checks and camera save/load.

#include "evt/evt_lecture.h"
#include "driver/camdrv.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mariost.h"
#include "pmario_sound.h"
#include "party.h"
#include "system.h"

static Vec stPt;
static Vec stAt;
static Vec stMpos;
static Vec stPpos;
static Vec stXpos;
static f32 stMdir1;
static f32 stMdir2;
static f32 stMdir3;
static f32 stMdir4;
static f32 stPdir1;
static f32 stPdir2;
static f32 stPdir3;
static f32 stPdir4;
static f32 stXdir1;
static f32 stXdir2;
static f32 stXdir3;
static f32 stXdir4;
static void* lect_work;

/** s32 level */
USER_FUNC(lect_set_systemlevel) {
    marioStSystemLevel(evtGetValue(event, *event->args));
    return EVT_RETURN_DONE;
}

/** Calls marioStGetSystemLevel and drops the result; nothing is returned to the script. */
USER_FUNC(lect_test_systemlevel) {
    marioStGetSystemLevel();
    return EVT_RETURN_DONE;
}

/** save the camera, Mario, partner and follower positions */
USER_FUNC(lect_cam_save) {
    CameraEntry* camera;
    MarioWork* mario;
    PartyEntry* party;
    PartyEntry* extra;

    camera = camGetPtr(CAMERA_3D);
    mario = marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    extra = partyGetPtr(marioGetExtraPartyId());

    stPt = camera->cameraPos;
    stAt = camera->cameraAt;
    stMpos = mario->position;
    stMdir1 = mario->displayDirection;
    stMdir2 = mario->displayDirectionTarget;
    stMdir3 = mario->directionView;
    stMdir4 = mario->moveDirection;
    if (party) {
        stPpos = party->position;
        stPdir1 = party->displayDirection;
        stPdir2 = party->displayDirectionTarget;
        stPdir3 = party->targetDir;
        stPdir4 = party->moveDir;
    }
    if (extra) {
        stXpos = extra->position;
        stXdir1 = extra->displayDirection;
        stXdir2 = extra->displayDirectionTarget;
        stXdir3 = extra->targetDir;
        stXdir4 = extra->moveDir;
    }
    return EVT_RETURN_DONE;
}

/** s32 msec */
USER_FUNC(lect_cam_load) {
    EvtWord* args = event->args;
    s32 msec;
    CameraEntry* camera;
    MarioWork* mario;
    PartyEntry* party;
    PartyEntry* extra;

    msec = evtGetValue(event, *args++);
    camera = camGetPtr(CAMERA_3D);
    mario = marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    extra = partyGetPtr(marioGetExtraPartyId());

    camera->presetPos = camera->cameraPos;
    camera->presetAt = camera->cameraAt;
    camera->storedPos = stPt;
    camera->storedAt = stAt;
    camera->startTimeA = gp->renderTime;
    camera->durationA = OSMillisecondsToTicks(msec);
    camera->stateA = CAM_MOVE_TO_STORED;
    camera->curveA = EASING_SINE_IN_OUT;

    mario->position = stMpos;
    mario->displayDirection = stMdir1;
    mario->displayDirectionTarget = stMdir2;
    mario->directionView = stMdir3;
    mario->moveDirection = stMdir4;
    if (party) {
        party->position = stPpos;
        party->displayDirection = stPdir1;
        party->displayDirectionTarget = stPdir2;
        party->targetDir = stPdir3;
        party->moveDir = stPdir4;
    }
    if (extra) {
        extra->position = stXpos;
        extra->displayDirection = stXdir1;
        extra->displayDirectionTarget = stXdir2;
        extra->targetDir = stXdir3;
        extra->moveDir = stXdir4;
    }
    return EVT_RETURN_DONE;
}

/** Blocks until the music started on channel 1 has begun playing. */
// Original name and size (0x38) from the JP demo map (UNUSED there: nothing in this file calls it), listed between
// lect_cam_load and lect_juyoitem_rel_set: evt_sub.c's static bgm_start_wait (0x38), the same name and size.
static USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

/** void* evt */
USER_FUNC(lect_juyoitem_rel_set) {
    lect_work = (void*)evtGetValue(event, *event->args);
    return EVT_RETURN_DONE;
}

/** void* evt (out) */
USER_FUNC(lect_juyoitem_rel_get) {
    evtSetValue(event, *event->args, (EvtWord)lect_work);
    return EVT_RETURN_DONE;
}

// clang-format off
/**
 * Runs the key-item lecture script that the current map REL stored with lect_juyoitem_rel_set
 * (all_juyoitem_lecture_rel).
 */
EVT_BEGIN(all_juyoitem_lecture)
    CALL(lect_juyoitem_rel_get, LW(0))
    CALL_CHILD(LW(0))
    RETURN
EVT_END()

// clang-format on
