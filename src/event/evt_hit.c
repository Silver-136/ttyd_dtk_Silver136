#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "driver/hitdrv.h"
#include "manager/evtmgr.h"
#include "memory.h"
#include <string.h>


HitDamageReturn* hitDmgretPtr;

static void name_callback_sub(const char* name, void* evt, BOOL flag);

USER_FUNC(evt_hitobj_onoff) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 isGroup = evtGetValue(event, args[1]);
    s32 off = evtGetValue(event, args[2]);

    if (isGroup == 0) {
        if (off != 0) {
            hitObjFlagOff(name, 1);
        } else {
            hitObjFlagOn(name, 1);
        }
    } else {
        if (off != 0) {
            hitGrpFlagOff(name, 1);
        } else {
            hitGrpFlagOn(name, 1);
        }
    }
    return EVT_RETURN_DONE;
}

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

USER_FUNC(evt_hit_bind_mapobj) {
    EvtWord* args = event->args;
    const char* hitName = (const char*)evtGetValue(event, args[0]);
    const char* mapobjName = (const char*)evtGetValue(event, args[1]);
    hitBindMapObj(hitName, mapobjName);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_hit_bind_update) {
    hitBindUpdate((const char*)evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_hit_get_position) {
    EvtWord* args = event->args;
    Vec pos;

    hitObjGetPos((const char*)evtGetValue(event, args[0]), &pos);
    evtSetFloat(event, args[1], pos.x);
    evtSetFloat(event, args[2], pos.y);
    evtSetFloat(event, args[3], pos.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(L_evt_hitobj_flag_onoff) {
    EvtWord* args = event->args;
    s32 isGroup = evtGetValue(event, args[0]);
    s32 off = evtGetValue(event, args[1]);
    const char* name = (const char*)evtGetValue(event, args[2]);
    s32 flag = args[3];

    if (isGroup == 0) {
        if (off == 0) {
            hitObjFlagOff(name, flag);
        } else {
            hitObjFlagOn(name, flag);
        }
    } else {
        if (off == 0) {
            hitGrpFlagOff(name, flag);
        } else {
            hitGrpFlagOn(name, flag);
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_hitobj_attr_onoff) {
    EvtWord* args = event->args;
    s32 isGroup = evtGetValue(event, args[0]);
    s32 off = evtGetValue(event, args[1]);
    const char* name = (const char*)evtGetValue(event, args[2]);
    s32 attr = args[3];

    if (isGroup == 0) {
        if (off == 0) {
            hitObjAttrOff(name, attr);
        } else {
            hitObjAttrOn(name, attr);
        }
    } else {
        if (off == 0) {
            hitGrpAttrOff(name, attr);
        } else {
            hitGrpAttrOn(name, attr);
        }
    }
    return EVT_RETURN_DONE;
}

static void name_callback_sub(const char* name, void* evt, BOOL flag) {
    HitEntry* hit = hitNameToPtr(name);
    if (hit != NULL) {
        evtEntry(evt, 0, EVENT_START_IMMEDIATE)->lwData[0] = (EvtWord)name;
        if (hit->child != NULL) {
            name_callback_sub(*(const char**)hit->child->joint, evt, TRUE);
        }
        if (flag && hit->sibling != NULL) {
            name_callback_sub(*(const char**)hit->sibling->joint, evt, TRUE);
        }
    }
}

USER_FUNC(evt_hitobj_name_callback) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    void* evt = (void*)evtGetValue(event, args[1]);
    name_callback_sub(name, evt, FALSE);
    return EVT_RETURN_DONE;
}
