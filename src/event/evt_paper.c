// Script commands that load and release paper-pose animations (animPaperPose).

#include "evt/evt_paper.h"
#include "manager/evtmgr_cmd.h"
#include "driver/animdrv.h"
#include "mario/mariost.h"

/** const char* animName */
// const char* animName
USER_FUNC(evt_paper_entry) {
    const char* name;
    s32 group;

    name = (const char*)evtGetValue(event, *event->args);
    group = gp->inBattle ? 1 : 0;
    if (animGroupBaseAsync(name, group, NULL) == 0) {
        return EVT_RETURN_BLOCK;
    }
    animPaperPoseEntry(name, group);
    return EVT_RETURN_DONE;
}

/** const char* animName */
// const char* animName
USER_FUNC(evt_paper_delete) {
    const char* name;

    name = (const char*)evtGetValue(event, *event->args);
    animPaperPoseRelease(animPaperPoseGetId(name, gp->inBattle != 0));
    return EVT_RETURN_DONE;
}
