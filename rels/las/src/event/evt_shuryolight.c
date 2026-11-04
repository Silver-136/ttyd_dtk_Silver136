// Palace of Shadow (las, the "last" dungeon): the coloured lamps on Sir Grodus's (shuryo) field model and his staff, animated in the las_28 and las_29 scenes.

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
USER_FUNC(evt_shuryolight_delete);
static USER_FUNC(evt_shuryolight_run_tue_evt);
USER_FUNC(evt_shuryolight_run_head_evt);
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
EvtWord shuryolight_pow_tenmetu[31];
EvtWord shuryolight_head_normal[30];
EvtWord shuryolight_head_tenmetu[20];
EvtWord shuryolight_head_off[14];
EvtWord shuryolight_tue_normal[31];

/** Animation names of Grodus's power lamp (green light): its five lit and dim states. */
static const char* green_light_anim[] = {
    "L_1C",
    "L_1B",
    "L_1D",
    "L_1A",
    "L_1E",
};

/** Animation names of Grodus's third head lamp (yellow light): its five lit and dim states. */
static const char* yellow_light_anim[] = {
    "L_2C",
    "L_2B",
    "L_2D",
    "L_2A",
    "L_2E",
};

/** Animation names of Grodus's second head lamp and staff lamp (red light): its five lit and dim states. */
static const char* red_light_anim[] = {
    "L_3C",
    "L_3B",
    "L_3D",
    "L_3A",
    "L_3E",
};

/** Animation names of Grodus's first head lamp (orange light): its five lit and dim states. */
static const char* orange_light_anim[] = {
    "L_4C",
    "L_4B",
    "L_4D",
    "L_4A",
    "L_4E",
};

/**
 * Event script: idle flicker of Grodus's power lamp (light 0):
 * lit steady for 3 to 6 seconds, then flickers a few times between the lit and dim animation.
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
 * Event script: the power lamp in its "blinking" state (started together with shuryolight_head_off in las_28
 * and las_29):
 * lit for 0.7 to 4.7 seconds, then dim for 200 ms, repeated forever.
 */
EVT_BEGIN(shuryolight_pow_tenmetu)
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
 * Event script: idle pattern of the three head lamps:
 * lights 3, 2 and 1 light up in turn, 400 ms each, with a longer pause between rounds.
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
 * Event script: makes the three head lamps blink together in animation 4 (the fast blink),
 * waiting for the first animation to end each round.
 */
EVT_BEGIN(shuryolight_head_tenmetu)
    LOOP(0)
        CALL(evt_shuryolight_set_anim, 1, 4)
        CALL(evt_shuryolight_set_anim, 2, 4)
        CALL(evt_shuryolight_set_anim, 3, 4)
        CALL(evt_shuryolight_wait_anim_end, 1)
    END_LOOP
    RETURN
EVT_END()

/** Event script: switches the three head lamps off (animation 3). */
EVT_BEGIN(shuryolight_head_off)
    CALL(evt_shuryolight_set_anim, 1, 3)
    CALL(evt_shuryolight_set_anim, 2, 3)
    CALL(evt_shuryolight_set_anim, 3, 3)
    RETURN
EVT_END()

/**
 * Event script: loops the staff light (light 4) through its four animations:
 * start, hold for a second, end, hold for a second.
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

/**
 * Display callback for Grodus's lamps: when the pose group being drawn is one of the lamp locators,
 * draws that lamp's shape (on newer versions after advancing its animation).
 */
static void shuryolight_callback(void* param, s32 group, Mtx* modeling) {
    ShuryoLight* lights = param;
    s32 i;

    for (i = 0; i < 5; i++) {
        if (group == lights[i].group) {
            break;
        }
    }
    if (i != 5) {
#if VERSION >= VERSION_JP
        animPoseMain(lights[i].npc->poseId);
#endif
        animPoseDrawShape(lights[i].npc->poseId, lights[i].shape);
    }
}

/** Switches lamp index to the given animation number and starts it looping on that lamp's pose. */
static void shuryolightSetAnim(s32 index, s32 anim) {
    ShuryoLight* light = &slhp->lights[index];

    light->status = anim;
    animPoseSetAnim(light->npc->poseId, light->anims[anim], TRUE);
}

/**
 * Starts the event code as a new event, first deleting the previous lamp event with the given id
 * if it is still running; returns the new event id.
 */
static s32 shuryolightRunEvt(EvtWord* code, s32 id) {
    if (evtCheckID(id)) {
        evtDeleteID(id);
    }
    return evtEntryType(code, 0, 0, 0)->eventId;
}

/**
 * Script helper: creates Grodus's five lamp NPCs (power lamp, three head lamps, staff lamp,
 * all with model c_shuryo) and attaches them to his body pose.
 * args[0] is the name of Grodus's own NPC; each lamp is bound to one of his locators (locator5 to locator9)
 * and draws a plane shape through the display callback. Allocates the work area slhp.
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

/** Script helper: sets the animation of lamp args[0] to args[1]. */
static USER_FUNC(evt_shuryolight_set_anim) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    shuryolightSetAnim(index, evtGetValue(event, args[1]));
    return EVT_RETURN_DONE;
}

/** Script helper: blocks until the animation of lamp args[0] has played through once. */
static USER_FUNC(evt_shuryolight_wait_anim_end) {
    s32 index = evtGetValue(event, event->args[0]);
    ShuryoLight* light = &slhp->lights[index];

    if (animPoseGetLoopTimes(light->npc->poseId) < 1.0f) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/** Script helper: returns in args[1] the length in milliseconds of one loop of lamp args[0]'s current animation. */
static USER_FUNC(evt_shuryolight_get_anim_time) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    ShuryoLight* light = &slhp->lights[index];
    AnimData* data = animPoseGetAnimDataPtr(light->npc->poseId);

    evtSetValue(event, args[1], 1000.0f * (data->loopInfo->end - data->loopInfo->start) / 60.0f);
    return EVT_RETURN_DONE;
}

/** Script helper: returns in args[1] the current animation number of lamp args[0]. */
static USER_FUNC(evt_shuryolight_get_status) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    ShuryoLight* light = &slhp->lights[index];

    evtSetValue(event, args[1], light->status);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: starts the power lamp, head lamp and staff lamp events given in args[0],
 * args[1] and args[2] (a null pointer leaves that lamp alone), replacing any events started before.
 */
USER_FUNC(evt_shuryolight_run_evt) {
    EvtWord* args = event->args;
    s32* power = (s32*)evtGetValue(event, args[0]);
    s32* head = (s32*)evtGetValue(event, args[1]);
    s32* tue = (s32*)evtGetValue(event, args[2]);

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

/** Script helper: replaces the power lamp event with the script in args[0]. */
static USER_FUNC(evt_shuryolight_run_power_evt) {
    EvtWord* code = (EvtWord*)evtGetValue(event, event->args[0]);

    slhp->powerEvt = shuryolightRunEvt(code, slhp->powerEvt);
    return EVT_RETURN_DONE;
}

/** Script helper: replaces the head lamps event with the script in args[0]. */
USER_FUNC(evt_shuryolight_run_head_evt) {
    EvtWord* code = (EvtWord*)evtGetValue(event, event->args[0]);

    slhp->headEvt = shuryolightRunEvt(code, slhp->headEvt);
    return EVT_RETURN_DONE;
}

/** Script helper: replaces the staff lamp event with the script in args[0]. */
static USER_FUNC(evt_shuryolight_run_tue_evt) {
    EvtWord* code = (EvtWord*)evtGetValue(event, event->args[0]);

    slhp->tueEvt = shuryolightRunEvt(code, slhp->tueEvt);
    return EVT_RETURN_DONE;
}

/** Script helper: stops all lamp events, deletes the five lamp NPCs and frees the work area. */
USER_FUNC(evt_shuryolight_delete) {
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
