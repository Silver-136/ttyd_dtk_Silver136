// Script commands for map collision objects: flags, positions and damage settings (hitdrv).

#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "driver/hitdrv.h"
#include "manager/evtmgr.h"
#include "memory.h"
#include <string.h>

static HitDamageReturn* hitDmgretPtr;

/**
 * Enables (args[2] nonzero) or disables the collision of hit object args[0],
 * or of its whole group when args[1] is nonzero, by clearing or setting hit flag 0x1.
 * Doors use it on their inside groups.
 */
USER_FUNC(evt_hitobj_onoff) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 isGroup = evtGetValue(event, args[1]);
    s32 on = evtGetValue(event, args[2]);

    if (isGroup == 0) {
        if (on != 0) {
            hitObjFlagOff(name, HIT_FLAG_OFF);
        } else {
            hitObjFlagOn(name, HIT_FLAG_OFF);
        }
    } else {
        if (on != 0) {
            hitGrpFlagOff(name, HIT_FLAG_OFF);
        } else {
            hitGrpFlagOn(name, HIT_FLAG_OFF);
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Copies a NULL-ended table of (hit group, position) pairs into map memory and attaches each to its group:
 * the spot Mario is put back on after falling into that hazard. y -1000 means the object's own position.
 */
USER_FUNC(evt_hit_damage_return_set) {
    s32 n;
    HitDamageReturn* q;
    u32 size;
    HitDamageReturn* tbl;
    HitDamageReturn* p;

    tbl = (HitDamageReturn*)evtGetValue(event, event->args[0]);
    if (isFirstCall) {
        q = tbl;
        n = 1;
        for (;;) {
            if (q->mapobjName == NULL) {
                break;
            }
            q++;
            n++;
        }
        size = n * sizeof(HitDamageReturn);
        p = (hitDmgretPtr = (HitDamageReturn*)_mapAlloc(size));
        memcpy(p, tbl, size);
        for (;;) {
            if (p->mapobjName == NULL) {
                break;
            }
            if (p->position.y == -1000.0f) {
                hitObjGetPos(p->mapobjName, &p->position);
            }
            hitGrpDamageReturnSet(p->mapobjName, p);
            p++;
        }
    }
    return EVT_RETURN_DONE;
}

/** Binds hit object args[0] to map object args[1] so the collision follows that object's movement. */
USER_FUNC(evt_hit_bind_mapobj) {
    EvtWord* args = event->args;
    const char* hitName = (const char*)evtGetValue(event, args[0]);
    const char* mapobjName = (const char*)evtGetValue(event, args[1]);
    hitBindMapObj(hitName, mapobjName);
    return EVT_RETURN_DONE;
}

/** Re-syncs hit object args[0] with the map object it is bound to. */
USER_FUNC(evt_hit_bind_update) {
    hitBindUpdate((const char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

/** Returns the position of hit object args[0] in the float variables args[1..3]. */
USER_FUNC(evt_hit_get_position) {
    EvtWord* args = event->args;
    Vec pos;

    hitObjGetPos((const char*)evtGetValue(event, args[0]), &pos);
    evtSetFloat(event, args[1], pos.x);
    evtSetFloat(event, args[2], pos.y);
    evtSetFloat(event, args[3], pos.z);
    return EVT_RETURN_DONE;
}

/**
 * Sets (args[1] nonzero) or clears hit flag mask args[3] (a literal, not a variable) on hit object args[2],
 * or on its whole group when args[0] is nonzero.
 */
USER_FUNC(evt_hitobj_flag_onoff) {
    EvtWord* args = event->args;
    s32 isGroup = evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);
    const char* name = (const char*)evtGetValue(event, args[2]);
    s32 flag = args[3];

    if (isGroup == 0) {
        if (on == 0) {
            hitObjFlagOff(name, flag);
        } else {
            hitObjFlagOn(name, flag);
        }
    } else {
        if (on == 0) {
            hitGrpFlagOff(name, flag);
        } else {
            hitGrpFlagOn(name, flag);
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Sets (args[1] nonzero) or clears attribute mask args[3] (a literal) on hit object args[2],
 * or on its whole group when args[0] is nonzero.
 */
USER_FUNC(evt_hitobj_attr_onoff) {
    EvtWord* args = event->args;
    s32 isGroup = evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);
    const char* name = (const char*)evtGetValue(event, args[2]);
    s32 attr = args[3];

    if (isGroup == 0) {
        if (on == 0) {
            hitObjAttrOff(name, attr);
        } else {
            hitObjAttrOn(name, attr);
        }
    } else {
        if (on == 0) {
            hitGrpAttrOff(name, attr);
        } else {
            hitGrpAttrOn(name, attr);
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Starts script evt once for hit object name and every object below it, passing each name in LW(0);
 * withSiblings also walks the siblings (it is FALSE only for the starting object).
 */
static void name_callback_sub(const char* name, void* evt, BOOL withSiblings) {
    HitEntry* hit = hitNameToPtr(name);
    if (hit != NULL) {
        evtEntry(evt, 0, EVENT_START_IMMEDIATE)->lwData[0] = (EvtWord)name;
        if (hit->child != NULL) {
            name_callback_sub(hit->child->joint->name, evt, TRUE);
        }
        if (withSiblings && hit->sibling != NULL) {
            name_callback_sub(hit->sibling->joint->name, evt, TRUE);
        }
    }
}

/** Runs script args[1] once for hit object args[0] and each of its descendants, with the name in LW(0). */
USER_FUNC(evt_hitobj_name_callback) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    void* evt = (void*)evtGetValue(event, args[1]);
    name_callback_sub(name, evt, FALSE);
    return EVT_RETURN_DONE;
}
