#include "event/evt_hit.h"

s32 evt_hit_bind_update(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void hitBindUpdate(s32 name);
    s32* args = *(s32**)((s32)event + 0x18);

    hitBindUpdate(evtGetValue(event, args[0]));
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_hitobj_name_callback(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void name_callback_sub(s32 name, s32 callback, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 callback = evtGetValue(event, args[1]);

    name_callback_sub(name, callback, 0);
    return 2;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_hit_get_position(void* event, s32 isFirstCall) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern s32 evtGetValue(void* event, s32 value);
    extern void evtSetFloat(void* event, s32 target, f32 value);
    extern void hitObjGetPos(s32 name, Vec* pos);
    Vec pos;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);

    hitObjGetPos(name, &pos);
    evtSetFloat(event, args[1], pos.x);
    evtSetFloat(event, args[2], pos.y);
    evtSetFloat(event, args[3], pos.z);
    return 2;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


void name_callback_sub(char* name, void* callback, s32 followSibling) {
    extern void* hitNameToPtr(char* name);
    extern void* evtEntry(void* code, s32 priority, s32 flags);
    void* hit0;
    void* hit1;
    void* hit2;
    void* link;
    void* evt;
    char* name1;
    char* name2;

#define START_CALLBACK(_name) \
    do { \
        evt = evtEntry(callback, 0, 0x20); \
        *(char**)((s32)evt + 0x84) = (_name); \
    } while (0)

    hit0 = hitNameToPtr(name);
    if (hit0 == 0) {
        return;
    }
    START_CALLBACK(name);

    link = *(void**)((s32)hit0 + 0xD8);
    if (link != 0) {
        name1 = **(char***)((s32)link + 8);
        hit1 = hitNameToPtr(name1);
        if (hit1 != 0) {
            START_CALLBACK(name1);
            link = *(void**)((s32)hit1 + 0xD8);
            if (link != 0) {
                name2 = **(char***)((s32)link + 8);
                hit2 = hitNameToPtr(name2);
                if (hit2 != 0) {
                    START_CALLBACK(name2);
                    link = *(void**)((s32)hit2 + 0xD8);
                    if (link != 0) {
                        name_callback_sub(**(char***)((s32)link + 8), callback, 1);
                    }
                    link = *(void**)((s32)hit2 + 0xDC);
                    if (link != 0) {
                        name_callback_sub(**(char***)((s32)link + 8), callback, 1);
                    }
                }
            }
            link = *(void**)((s32)hit1 + 0xDC);
            if (link != 0) {
                name2 = **(char***)((s32)link + 8);
                hit2 = hitNameToPtr(name2);
                if (hit2 != 0) {
                    START_CALLBACK(name2);
                    link = *(void**)((s32)hit2 + 0xD8);
                    if (link != 0) {
                        name_callback_sub(**(char***)((s32)link + 8), callback, 1);
                    }
                    link = *(void**)((s32)hit2 + 0xDC);
                    if (link != 0) {
                        name_callback_sub(**(char***)((s32)link + 8), callback, 1);
                    }
                }
            }
        }
    }

    if (followSibling != 0) {
        link = *(void**)((s32)hit0 + 0xDC);
        if (link != 0) {
            name1 = **(char***)((s32)link + 8);
            hit1 = hitNameToPtr(name1);
            if (hit1 != 0) {
                START_CALLBACK(name1);
                link = *(void**)((s32)hit1 + 0xD8);
                if (link != 0) {
                    name2 = **(char***)((s32)link + 8);
                    hit2 = hitNameToPtr(name2);
                    if (hit2 != 0) {
                        START_CALLBACK(name2);
                        link = *(void**)((s32)hit2 + 0xD8);
                        if (link != 0) {
                            name_callback_sub(**(char***)((s32)link + 8), callback, 1);
                        }
                        link = *(void**)((s32)hit2 + 0xDC);
                        if (link != 0) {
                            name_callback_sub(**(char***)((s32)link + 8), callback, 1);
                        }
                    }
                }
                link = *(void**)((s32)hit1 + 0xDC);
                if (link != 0) {
                    name2 = **(char***)((s32)link + 8);
                    hit2 = hitNameToPtr(name2);
                    if (hit2 != 0) {
                        START_CALLBACK(name2);
                        link = *(void**)((s32)hit2 + 0xD8);
                        if (link != 0) {
                            name_callback_sub(**(char***)((s32)link + 8), callback, 1);
                        }
                        link = *(void**)((s32)hit2 + 0xDC);
                        if (link != 0) {
                            name_callback_sub(**(char***)((s32)link + 8), callback, 1);
                        }
                    }
                }
            }
        }
    }
#undef START_CALLBACK
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_hit_damage_return_set(void* event, s32 isFirstCall) {
    typedef struct HitDamageReturn {
        s32 name;
        f32 x;
        f32 y;
        f32 z;
    } HitDamageReturn;
    extern s32 evtGetValue(void* event, s32 value);
    extern void* _mapAlloc(void* heap, u32 size);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern void hitObjGetPos(s32 name, void* pos);
    extern void hitGrpDamageReturnSet(s32 name, void* entry);
    extern void* mapalloc_base_ptr;
    extern HitDamageReturn* hitDmgretPtr;
    extern f32 float_neg1000_804203d0;
    s32* args = *(s32**)((s32)event + 0x18);
    HitDamageReturn* src = (HitDamageReturn*)evtGetValue(event, args[0]);
    HitDamageReturn* entry;
    s32 count;
    u32 size;

    if (isFirstCall != 0) {
        count = 1;
        entry = src;
        while (entry->name != 0) {
            entry++;
            count++;
        }
        size = count * 0x10;
        hitDmgretPtr = _mapAlloc(mapalloc_base_ptr, size);
        entry = hitDmgretPtr;
        memcpy(entry, src, size);
        while (entry->name != 0) {
            if (entry->y == float_neg1000_804203d0) {
                hitObjGetPos(entry->name, &entry->x);
            }
            hitGrpDamageReturnSet(entry->name, entry);
            entry++;
        }
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_hitobj_onoff(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void hitObjFlagOff(s32 name, unsigned short flag);
    extern void hitObjFlagOn(s32 name, unsigned short flag);
    extern void hitGrpFlagOff(s32 name, unsigned short flag);
    extern void hitGrpFlagOn(s32 name, unsigned short flag);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
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
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 L_evt_hitobj_flag_onoff(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void hitObjFlagOff(s32 name, unsigned short flag);
    extern void hitObjFlagOn(s32 name, unsigned short flag);
    extern void hitGrpFlagOff(s32 name, unsigned short flag);
    extern void hitGrpFlagOn(s32 name, unsigned short flag);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 isGroup = evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);
    s32 name = evtGetValue(event, args[2]);
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
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_hitobj_attr_onoff(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void hitObjAttrOff(s32 name, s32 attr);
    extern void hitObjAttrOn(s32 name, s32 attr);
    extern void hitGrpAttrOff(s32 name, s32 attr);
    extern void hitGrpAttrOn(s32 name, s32 attr);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 isGroup = evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);
    s32 name = evtGetValue(event, args[2]);
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
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_hit_bind_mapobj(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void hitBindMapObj(s32 name, s32 mapObjName);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 mapObjName = evtGetValue(event, args[1]);

    hitBindMapObj(name, mapObjName);
    return 2;
}

