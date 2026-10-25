#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "manager/evtmgr_cmd.h"
#include "memory.h"

void animPoseDrawShape(s32 poseId, s32 shapeIdx);
void animPoseSetDispCallBack(s32 poseId, void (*callback)(void* param, s32 group), void* param);
typedef struct ShuryoLight { NpcEntry* npc; const char** anims; s32 status; s32 group; s32 shape; } ShuryoLight;
typedef struct ShuryoWork { s32 powerEvt; s32 headEvt; s32 tueEvt; ShuryoLight lights[5]; } ShuryoWork;
static ShuryoWork* slhp;

static USER_FUNC(evt_shuryolight_set_anim);
static USER_FUNC(evt_shuryolight_wait_anim_end);
USER_FUNC(evt_sub_random);
static USER_FUNC(evt_shuryolight_delete);
static USER_FUNC(evt_shuryolight_run_tue_evt);
static USER_FUNC(evt_shuryolight_run_head_evt);
static USER_FUNC(evt_shuryolight_run_power_evt);
USER_FUNC(evt_shuryolight_run_evt);
static USER_FUNC(evt_shuryolight_get_status);
static USER_FUNC(evt_shuryolight_get_anim_time);
USER_FUNC(evt_shuryolight_init);
static s32 shuryolightRunEvt(EvtWord* code, s32 id);
static void shuryolightSetAnim(s32 index, s32 anim);
static void shuryolight_callback(void* param, s32 group);

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

static const char* green_light_anim[] = {
    "L_1C",
    "L_1B",
    "L_1D",
    "L_1A",
    "L_1E",
};

static const char* yellow_light_anim[] = {
    "L_2C",
    "L_2B",
    "L_2D",
    "L_2A",
    "L_2E",
};

static const char* red_light_anim[] = {
    "L_3C",
    "L_3B",
    "L_3D",
    "L_3A",
    "L_3E",
};

static const char* orange_light_anim[] = {
    "L_4C",
    "L_4B",
    "L_4D",
    "L_4A",
    "L_4E",
};

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

static EVT_BEGIN(shuryolight_head_tenmetu)
    LOOP(0)
        CALL(evt_shuryolight_set_anim, 1, 4)
        CALL(evt_shuryolight_set_anim, 2, 4)
        CALL(evt_shuryolight_set_anim, 3, 4)
        CALL(evt_shuryolight_wait_anim_end, 1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(shuryolight_head_off)
    CALL(evt_shuryolight_set_anim, 1, 3)
    CALL(evt_shuryolight_set_anim, 2, 3)
    CALL(evt_shuryolight_set_anim, 3, 3)
    RETURN
EVT_END()

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

static void shuryolight_callback(void* param, s32 group) {
    ShuryoLight* lights = param;
    s32 i;

    for (i = 0; i < 5; i++) {
        if (group == lights[i].group) {
            break;
        }
    }
    if (i != 5) {
        animPoseMain(lights[i].npc->poseId);
        animPoseDrawShape(lights[i].npc->poseId, lights[i].shape);
    }
}
static void shuryolightSetAnim(s32 index, s32 anim) {
    ShuryoLight* light = &slhp->lights[index];

    light->status = anim;
    animPoseSetAnim(light->npc->poseId, light->anims[anim], TRUE);
}
static s32 shuryolightRunEvt(EvtWord* code, s32 id) {
    if (evtCheckID(id)) {
        evtDeleteID(id);
    }
    return evtEntryType(code, 0, 0, 0)->eventId;
}
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
    npc->flags |= 0xA0;
    lights[0].npc = npc;
    lights[0].anims = (const char**)green_light_anim;
    lights[0].group = animPoseGetGroupIdx(base->poseId, "locator6");
    lights[0].shape = animPoseGetShapeIdx(npc->poseId, "pPlaneShape34");
    shuryolightSetAnim(0, 1);
    npc = npcNameToPtr("light_head_1");
    npc->flags |= 0xA0;
    lights[1].npc = npc;
    lights[1].anims = (const char**)orange_light_anim;
    lights[1].group = animPoseGetGroupIdx(base->poseId, "locator9");
    lights[1].shape = animPoseGetShapeIdx(npc->poseId, "pPlaneShape37");
    shuryolightSetAnim(1, 3);
    npc = npcNameToPtr("light_head_2");
    npc->flags |= 0xA0;
    lights[2].npc = npc;
    lights[2].anims = (const char**)red_light_anim;
    lights[2].group = animPoseGetGroupIdx(base->poseId, "locator8");
    lights[2].shape = animPoseGetShapeIdx(npc->poseId, "pPlaneShape36");
    shuryolightSetAnim(2, 3);
    npc = npcNameToPtr("light_head_3");
    npc->flags |= 0xA0;
    lights[3].npc = npc;
    lights[3].anims = (const char**)yellow_light_anim;
    lights[3].group = animPoseGetGroupIdx(base->poseId, "locator7");
    lights[3].shape = animPoseGetShapeIdx(npc->poseId, "pPlaneShape35");
    shuryolightSetAnim(3, 3);
    npc = npcNameToPtr("light_tue_1");
    npc->flags |= 0xA0;
    lights[4].npc = npc;
    lights[4].anims = (const char**)red_light_anim;
    lights[4].group = animPoseGetGroupIdx(base->poseId, "locator5");
    lights[4].shape = animPoseGetShapeIdx(npc->poseId, "pPlaneShape36");
    shuryolightSetAnim(4, 1);
    animPoseSetDispCallBack(base->poseId, shuryolight_callback, lights);
    return EVT_RETURN_DONE;
}
static USER_FUNC(evt_shuryolight_set_anim) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    shuryolightSetAnim(index, evtGetValue(event, args[1]));
    return EVT_RETURN_DONE;
}
static USER_FUNC(evt_shuryolight_wait_anim_end) {
    s32 index = evtGetValue(event, event->args[0]);
    ShuryoLight* light = &slhp->lights[index];

    if (animPoseGetLoopTimes(light->npc->poseId) < 1.0f) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}
static USER_FUNC(evt_shuryolight_get_anim_time) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    ShuryoLight* light = &slhp->lights[index];
    AnimData* data = animPoseGetAnimDataPtr(light->npc->poseId);

    evtSetValue(event, args[1], 1000.0f * (data->loopInfo[2] - data->loopInfo[1]) / 60.0f);
    return EVT_RETURN_DONE;
}
static USER_FUNC(evt_shuryolight_get_status) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    ShuryoLight* light = &slhp->lights[index];

    evtSetValue(event, args[1], light->status);
    return EVT_RETURN_DONE;
}
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
static USER_FUNC(evt_shuryolight_run_power_evt) {
    EvtWord* code = (EvtWord*)evtGetValue(event, event->args[0]);

    slhp->powerEvt = shuryolightRunEvt(code, slhp->powerEvt);
    return EVT_RETURN_DONE;
}
static USER_FUNC(evt_shuryolight_run_head_evt) {
    EvtWord* code = (EvtWord*)evtGetValue(event, event->args[0]);

    slhp->headEvt = shuryolightRunEvt(code, slhp->headEvt);
    return EVT_RETURN_DONE;
}
static USER_FUNC(evt_shuryolight_run_tue_evt) {
    EvtWord* code = (EvtWord*)evtGetValue(event, event->args[0]);

    slhp->tueEvt = shuryolightRunEvt(code, slhp->tueEvt);
    return EVT_RETURN_DONE;
}
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
