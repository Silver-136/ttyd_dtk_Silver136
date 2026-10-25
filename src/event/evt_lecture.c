#include "evt/evt_lecture.h"
#include "driver/camdrv.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mariost.h"
#include "party.h"

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

// s32 level
USER_FUNC(lect_set_systemlevel) {
    marioStSystemLevel(evtGetValue(event, *event->args));
    return EVT_RETURN_DONE;
}

USER_FUNC(lect_test_systemlevel) {
    marioStGetSystemLevel();
    return EVT_RETURN_DONE;
}

// save the camera, Mario, partner and follower positions
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

// s32 msec
USER_FUNC(lect_cam_load) {
    s32 msec;
    CameraEntry* camera;
    MarioWork* mario;
    PartyEntry* party;
    PartyEntry* extra;

    msec = evtGetValue(event, *event->args);
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
    camera->stateA = 3;
    camera->curveA = 11;

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

// void* evt
USER_FUNC(lect_juyoitem_rel_set) {
    lect_work = (void*)evtGetValue(event, *event->args);
    return EVT_RETURN_DONE;
}

// void* evt (out)
USER_FUNC(lect_juyoitem_rel_get) {
    evtSetValue(event, *event->args, (EvtWord)lect_work);
    return EVT_RETURN_DONE;
}

// clang-format off
EVT_BEGIN(all_juyoitem_lecture)
    CALL(lect_juyoitem_rel_get, LW(0))
    CALL_CHILD(LW(0))
    RETURN
    END
EVT_END()
// clang-format on
