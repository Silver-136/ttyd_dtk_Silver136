// X-Naut Fortress (aji): the lights on Sir Grodus's (shuryo) head and staff (light_head, light_tue).

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_shuryolight.h"
#include "evt/evt_cmd.h"
#include "evt/evt_sub.h"

#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "manager/evtmgr_cmd.h"
#include "memory.h"

static ShuryoWork* slhp;

static USER_FUNC(evt_shuryolight_set_anim);
static USER_FUNC(evt_shuryolight_wait_anim_end);
static USER_FUNC(evt_shuryolight_delete);
static USER_FUNC(evt_shuryolight_run_tue_evt);
static USER_FUNC(evt_shuryolight_run_head_evt);
static USER_FUNC(evt_shuryolight_run_power_evt);
static USER_FUNC(evt_shuryolight_get_status);
static USER_FUNC(evt_shuryolight_get_anim_time);
static s32 shuryolightRunEvt(EvtWord* code, s32 id);
static void shuryolightSetAnim(s32 index, s32 anim);
static void shuryolight_callback(void* param, s32 group, Mtx* modeling);

static const char* green_light_anim[5];
static const char* yellow_light_anim[5];
static const char* red_light_anim[5];
static const char* orange_light_anim[5];
EvtWord shuryolight_pow_normal[44];
static EvtWord shuryolight_pow_tenmetu[31];
EvtWord shuryolight_head_normal[30];
static EvtWord shuryolight_head_tenmetu[20];
static EvtWord shuryolight_head_off[14];
EvtWord shuryolight_tue_normal[31];

/**
 * Animation names of the green lamp (the power lamp on the base of Grodus's staff): five animations L_1A to
 * L_1E, indexed by animation number.
 */
static const char* green_light_anim[] = {
    "L_1C",
    "L_1B",
    "L_1D",
    "L_1A",
    "L_1E",
};

/** Animation names of the yellow lamp (the third lamp on Grodus's head): five animations L_2A to L_2E. */
static const char* yellow_light_anim[] = {
    "L_2C",
    "L_2B",
    "L_2D",
    "L_2A",
    "L_2E",
};

/**
 * Animation names of the red lamp (the second head lamp and the lamp on the staff): five animations L_3A to
 * L_3E.
 */
static const char* red_light_anim[] = {
    "L_3C",
    "L_3B",
    "L_3D",
    "L_3A",
    "L_3E",
};

/** Animation names of the orange lamp (the first lamp on Grodus's head): five animations L_4A to L_4E. */
static const char* orange_light_anim[] = {
    "L_4C",
    "L_4B",
    "L_4D",
    "L_4A",
    "L_4E",
};

/**
 * Normal blinking of the power lamp (light 0) on Sir Grodus's staff base:
 * steady light for 3 to 6 seconds, then a short random flicker of off-on blinks at 80 ms each, forever.
 */
EVT_BEGIN(shuryolight_pow_normal)
    LOOP(0)
        CALL(evt_shuryolight_set_anim, 0, 1)
        CALL(evt_sub_random, 3000, LW(0))
        ADD(LW(0), 3000)
        WAIT_MS(LW(0))
        CALL(evt_sub_random, 100, LW(0))
        DIVIDE(LW(0), 30)
        IF_INT_GREATER(LW(0), 0)
            LOOP(LW(0))
                CALL(evt_shuryolight_set_anim, 0, 3)
                WAIT_MS(80)
                CALL(evt_shuryolight_set_anim, 0, 1)
                WAIT_MS(80)
            END_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

/**
 * Blinking pattern for the power lamp (light 0) of Grodus's staff base:
 * lit for 0.7 to 4.7 seconds at random, then off for 200 ms, forever.
 */
static EVT_BEGIN(shuryolight_pow_tenmetu)
    LOOP(0)
        CALL(evt_shuryolight_set_anim, 0, 1)
        CALL(evt_sub_random, 3000, LW(0))
        CALL(evt_sub_random, 1000, LW(1))
        ADD(LW(0), LW(1))
        ADD(LW(0), 700)
        WAIT_MS(LW(0))
        CALL(evt_shuryolight_set_anim, 0, 3)
        WAIT_MS(200)
    END_LOOP
    RETURN
EVT_END()

/**
 * Normal pattern of the three head lamps (lights 1 to 3):
 * they light one at a time from the third to the first, 400 ms apiece, pause, and repeat.
 */
EVT_BEGIN(shuryolight_head_normal)
    SET_VALUE(LW(0), 3)
    LOOP(0)
        CALL(evt_shuryolight_set_anim, LW(0), 1)
        WAIT_MS(400)
        CALL(evt_shuryolight_set_anim, LW(0), 3)
        SUBTRACT(LW(0), 1)
        IF_INT_LESS(LW(0), 1)
            SET_VALUE(LW(0), 3)
            WAIT_MS(400)
        END_IF
    END_LOOP
    RETURN
EVT_END()

/**
 * Flashing pattern for the three head lamps (lights 1 to 3):
 * plays animation 4 on all of them and waits for it to end, forever.
 */
static EVT_BEGIN(shuryolight_head_tenmetu)
    LOOP(0)
        CALL(evt_shuryolight_set_anim, 1, 4)
        CALL(evt_shuryolight_set_anim, 2, 4)
        CALL(evt_shuryolight_set_anim, 3, 4)
        CALL(evt_shuryolight_wait_anim_end, 1)
    END_LOOP
    RETURN
EVT_END()

/** Turns the three head lamps (lights 1 to 3) off by setting animation 3 on each. */
static EVT_BEGIN(shuryolight_head_off)
    CALL(evt_shuryolight_set_anim, 1, 3)
    CALL(evt_shuryolight_set_anim, 2, 3)
    CALL(evt_shuryolight_set_anim, 3, 3)
    RETURN
EVT_END()

/**
 * Normal pattern of the staff lamp (light 4): plays its animation 0, holds 1 for a second, plays animation 2,
 * holds 3 for a second, forever.
 */
EVT_BEGIN(shuryolight_tue_normal)
    LOOP(0)
        CALL(evt_shuryolight_set_anim, 4, 0)
        CALL(evt_shuryolight_wait_anim_end, 4)
        CALL(evt_shuryolight_set_anim, 4, 1)
        WAIT_MS(1000)
        CALL(evt_shuryolight_set_anim, 4, 2)
        CALL(evt_shuryolight_wait_anim_end, 4)
        CALL(evt_shuryolight_set_anim, 4, 3)
        WAIT_MS(1000)
    END_LOOP
    RETURN
EVT_END()

static void shuryolight_callback(void* param, s32 group, Mtx* modeling) {
    ShuryoLight* lights = param;
    s32 i;

    for (i = 0; i < 5; i++) {
        if (group == lights[i].group) {
            break;
        }
    }

    // Only the first light bound to this group is advanced and drawn.
    if (i != 5) {
#if VERSION >= VERSION_JP
        animPoseMain(lights[i].npc->poseId);
#endif
        animPoseDrawShape(lights[i].npc->poseId, lights[i].shape);
    }
}

/**
 * Switches lamp index (0 power, 1 to 3 head, 4 staff) to animation anim of its colour set (green, orange, red,
 * yellow, red) and remembers it as the lamp's status.
 */
static void shuryolightSetAnim(s32 index, s32 anim) {
    ShuryoLight* light = &slhp->lights[index];

    light->status = anim;
    animPoseSetAnim(light->npc->poseId, light->anims[anim], TRUE);
}

/**
 * Starts the lamp script code as a new event, first deleting the previous one with event ID id
 * if it is still running; returns the new event's ID.
 */
static s32 shuryolightRunEvt(EvtWord* code, s32 id) {
    if (evtCheckID(id)) {
        evtDeleteID(id);
    }
    return evtEntryType(code, 0, 0, 0)->eventId;
}

/**
 * Script function: sets up the lamps on Sir Grodus's NPC (args[0] is his name).
 * It creates five hidden helper NPCs from the c_shuryo model (light_power, light_head_1 to 3, light_tue_1),
 * ties each to a locator of Grodus's skeleton (locator6, 9, 8, 7, 5) and its coloured animation set,
 * sets their initial animations, and installs shuryolight_callback to draw them.
 */
USER_FUNC(evt_shuryolight_init) {
    NpcEntry* base = npcNameToPtr((const char*)evtGetValue(event, event->args[0]));
    ShuryoLight* lights;
    NpcEntry* npc;

    npcEntry("light_power", "c_shuryo");
    npcEntry("light_head_1", "c_shuryo");
    npcEntry("light_head_2", "c_shuryo");
    npcEntry("light_head_3", "c_shuryo");
    npcEntry("light_tue_1", "c_shuryo");
    slhp = _mapAlloc(sizeof(ShuryoWork));
    slhp->powerEvt = 0;
    slhp->headEvt = 0;
    slhp->tueEvt = 0;
    lights = slhp->lights;
    npc = npcNameToPtr("light_power");
    npc->flags |= (NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW);
    lights[0].npc = npc;
    lights[0].anims = (const char**)green_light_anim;
    lights[0].group = animPoseGetGroupIdx(base->poseId, "locator6");
    lights[0].shape = animPoseGetShapeIdx(npc->poseId, "pPlaneShape34");
    shuryolightSetAnim(0, 1);
    npc = npcNameToPtr("light_head_1");
    npc->flags |= (NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW);
    lights[1].npc = npc;
    lights[1].anims = (const char**)orange_light_anim;
    lights[1].group = animPoseGetGroupIdx(base->poseId, "locator9");
    lights[1].shape = animPoseGetShapeIdx(npc->poseId, "pPlaneShape37");
    shuryolightSetAnim(1, 3);
    npc = npcNameToPtr("light_head_2");
    npc->flags |= (NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW);
    lights[2].npc = npc;
    lights[2].anims = (const char**)red_light_anim;
    lights[2].group = animPoseGetGroupIdx(base->poseId, "locator8");
    lights[2].shape = animPoseGetShapeIdx(npc->poseId, "pPlaneShape36");
    shuryolightSetAnim(2, 3);
    npc = npcNameToPtr("light_head_3");
    npc->flags |= (NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW);
    lights[3].npc = npc;
    lights[3].anims = (const char**)yellow_light_anim;
    lights[3].group = animPoseGetGroupIdx(base->poseId, "locator7");
    lights[3].shape = animPoseGetShapeIdx(npc->poseId, "pPlaneShape35");
    shuryolightSetAnim(3, 3);
    npc = npcNameToPtr("light_tue_1");
    npc->flags |= (NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW);
    lights[4].npc = npc;
    lights[4].anims = (const char**)red_light_anim;
    lights[4].group = animPoseGetGroupIdx(base->poseId, "locator5");
    lights[4].shape = animPoseGetShapeIdx(npc->poseId, "pPlaneShape36");
    shuryolightSetAnim(4, 1);
    animPoseSetDispCallBack(base->poseId, shuryolight_callback, lights);
    return EVT_RETURN_DONE;
}

/** Script function: sets lamp args[0] to animation args[1] (shuryolightSetAnim). */
static USER_FUNC(evt_shuryolight_set_anim) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    shuryolightSetAnim(index, evtGetValue(event, args[1]));
    return EVT_RETURN_DONE;
}

/** Script function: blocks until the animation of lamp args[0] has played through once. */
static USER_FUNC(evt_shuryolight_wait_anim_end) {
    s32 index = evtGetValue(event, event->args[0]);
    ShuryoLight* light = &slhp->lights[index];

    if (animPoseGetLoopTimes(light->npc->poseId) < 1.0f) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/** Script function: returns in args[1] the length in milliseconds of the current animation loop of lamp args[0]. */
static USER_FUNC(evt_shuryolight_get_anim_time) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    ShuryoLight* light = &slhp->lights[index];
    AnimData* data = animPoseGetAnimDataPtr(light->npc->poseId);

    // Convert the loop span from 60 Hz frames to integer milliseconds.
    evtSetValue(event, args[1], 1000.0f * (data->loopInfo->end - data->loopInfo->start) / 60.0f);
    return EVT_RETURN_DONE;
}

/** Script function: returns in args[1] the current animation (status) number of lamp args[0]. */
static USER_FUNC(evt_shuryolight_get_status) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    ShuryoLight* light = &slhp->lights[index];

    evtSetValue(event, args[1], light->status);
    return EVT_RETURN_DONE;
}

/**
 * Script function: starts the lamp scripts for the power lamp (args[0]),
 * the head lamps (args[1]) and the staff lamp (args[2]), each replacing the one running before;
 * a NULL pointer leaves that lamp alone. Run from the map init scripts of aji_10 and aji_17.
 */
USER_FUNC(evt_shuryolight_run_evt) {
    EvtWord* args = event->args;
    EvtWord* power = (EvtWord*)evtGetValue(event, args[0]);
    EvtWord* head = (EvtWord*)evtGetValue(event, args[1]);
    EvtWord* tue = (EvtWord*)evtGetValue(event, args[2]);

    // A null script leaves that channel and its existing event unchanged.
    if (power != NULL) {
        slhp->powerEvt = shuryolightRunEvt(power, slhp->powerEvt);
    }
    if (head != NULL) {
        slhp->headEvt = shuryolightRunEvt(head, slhp->headEvt);
    }
    if (tue != NULL) {
        slhp->tueEvt = shuryolightRunEvt(tue, slhp->tueEvt);
    }
    return EVT_RETURN_DONE;
}

/** Script function: starts the event script in args[0] as the power lamp's script, replacing the previous one. */
static USER_FUNC(evt_shuryolight_run_power_evt) {
    EvtWord* code = (EvtWord*)evtGetValue(event, event->args[0]);

    slhp->powerEvt = shuryolightRunEvt(code, slhp->powerEvt);
    return EVT_RETURN_DONE;
}

/** Script function: starts the event script in args[0] as the head lamps' script, replacing the previous one. */
static USER_FUNC(evt_shuryolight_run_head_evt) {
    EvtWord* code = (EvtWord*)evtGetValue(event, event->args[0]);

    slhp->headEvt = shuryolightRunEvt(code, slhp->headEvt);
    return EVT_RETURN_DONE;
}

/** Script function: starts the event script in args[0] as the staff lamp's script, replacing the previous one. */
static USER_FUNC(evt_shuryolight_run_tue_evt) {
    EvtWord* code = (EvtWord*)evtGetValue(event, event->args[0]);

    slhp->tueEvt = shuryolightRunEvt(code, slhp->tueEvt);
    return EVT_RETURN_DONE;
}

/** Script function: stops the running power, head and staff lamp scripts. */
static USER_FUNC(evt_shuryolight_delete) {
    s32 i;

    if (slhp->powerEvt != 0) {
        evtDeleteID(slhp->powerEvt);
        slhp->powerEvt = 0;
    }
    if (slhp->headEvt != 0) {
        evtDeleteID(slhp->headEvt);
        slhp->headEvt = 0;
    }
    if (slhp->tueEvt != 0) {
        evtDeleteID(slhp->tueEvt);
        slhp->tueEvt = 0;
    }
    for (i = 0; i < 5; i++) {
        npcDeleteGroup(slhp->lights[i].npc);
    }
    _mapFree(slhp);
    slhp = NULL;
    return EVT_RETURN_DONE;
}
