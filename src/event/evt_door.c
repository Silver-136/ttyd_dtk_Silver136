#include "event/evt_door.h"
#include "event/evt_cmd.h"

s32 DoorFLAG;
s32 door_mapflag;

s32 evtGetValue(EventEntry* event, s32 value);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void* mapGetMapObj(void);
void mapErrorEntry(s32 type, s32 value);
void door_dark_flag_sub(s32 unk0, s32 unk1, s32 unk2, s32 unk3);

s32 evt_door_end_wait(void) {
    return DoorFLAG ? 0 : 2;
}

USER_FUNC(evt_door_load_mapflag) {
    s32 value = evtGetValue(event, event->args[0]);
    s32* mapObj = mapGetMapObj();
    if (mapObj == 0) {
        mapErrorEntry(0, value);
        return 2;
    }
    *mapObj = door_mapflag;
    return 2;
}

USER_FUNC(evt_door_save_mapflag) {
    s32 value = evtGetValue(event, event->args[0]);
    s32* mapObj = mapGetMapObj();
    if (mapObj == 0) {
        mapErrorEntry(0, value);
        return 2;
    }
    door_mapflag = *mapObj;
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(door_dark_flag) {
    s32* args = event->args;
    void* info = (void*)evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    door_dark_flag_sub(value, *(s32*)((s32)info + 0x34), *(s32*)((s32)info + 0x38), *(s32*)((s32)info + 0x3C));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(init_inside_group) {
    s32* args = event->args;
    void* info = (void*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], *(s32*)(*(s32*)info + 0x34));
    evtSetValue(event, args[2], *(s32*)(*(s32*)info + 0x38));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_door_set_param) {
    s32* args = event->args;
    void* info = (void*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    s32 value = evtGetValue(event, args[2]);

    switch (flag) {
        case 0:
            *(s32*)((s32)info + 0x34) = value;
            break;
        default:
            break;
    }

    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void door_position_sub(s32 kind, char* mapObjName, char* hit0, char* hit1, void* out0, void* out1, s32 useMarioPos) {
    typedef struct Vec { f32 x; f32 y; f32 z; } Vec;
    extern void PSMTXInverse(void* in, void* out);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXMultVec(void* mtx, void* in, void* out);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern f64 atan(f64 x);
    extern f64 cos(f64 x);
    extern void hitObjGetPos(char* hit, void* out);
    extern void hitObjGetNormal(char* hit, void* out);
    extern void* marioGetPtr(void);
    extern const f32 float_10_80421b58;
    extern const f32 float_30_80421b60;
    extern const f32 float_40_80421b5c;
    extern const f32 float_50_80421b64;
    extern const f32 float_0_80421b68;
    extern const f32 float_0p5_80421b6c;

    void* mapObj;
    u8 inv[0x30];
    u8 mtx[0x30];
    Vec min;
    Vec max;
    Vec diff;
    Vec pos;
    Vec normal;
    u8* mario;
    f32 inset;
    f32 side;
    f32 front;

    mapObj = ((void*(*)(char*))mapGetMapObj)(mapObjName);
    inset = float_10_80421b58;
    side = float_40_80421b5c;
    front = float_30_80421b60;
    if (useMarioPos != 0) {
        side = float_50_80421b64;
        front = float_50_80421b64;
    }

    if (mapObj != NULL) {
        PSMTXInverse((void*)((s32)mapObj + 0xAC), inv);
        PSMTXConcat((void*)((s32)mapObj + 0x1C), inv, mtx);
        PSMTXMultVec(mtx, (void*)(*(s32*)((s32)mapObj + 8) + 0x3C), &min);
        PSMTXMultVec(mtx, (void*)(*(s32*)((s32)mapObj + 8) + 0x48), &max);
        PSVECSubtract(&max, &min, &diff);
        if (diff.x == float_0_80421b68) {
            inset = diff.z * float_0p5_80421b6c;
        } else {
            inset = (diff.x / (f32)cos((f32)atan(diff.z / diff.x))) * float_0p5_80421b6c;
        }
    }

    if (kind == 2) {
        if (hit1 == NULL) {
            mario = marioGetPtr();
            ((Vec*)out1)->x = *(f32*)(mario + 0x8C);
            ((Vec*)out1)->y = *(f32*)(mario + 0x90);
            ((Vec*)out1)->z = *(f32*)(mario + 0x94);
        } else {
            if (useMarioPos == 0) {
                hitObjGetPos(hit1, &pos);
            } else {
                mario = marioGetPtr();
                pos.x = *(f32*)(mario + 0x8C);
                pos.y = *(f32*)(mario + 0x90);
                pos.z = *(f32*)(mario + 0x94);
            }
            hitObjGetNormal(hit1, &normal);
            ((Vec*)out1)->x = normal.z * inset + normal.x * side + pos.x;
            ((Vec*)out1)->z = -(normal.x * inset - (normal.z * side + pos.z));
        }

        if (hit0 == NULL) {
            mario = marioGetPtr();
            ((Vec*)out0)->x = *(f32*)(mario + 0x8C);
            ((Vec*)out0)->y = *(f32*)(mario + 0x90);
            ((Vec*)out0)->z = *(f32*)(mario + 0x94);
        } else {
            if (useMarioPos == 0) {
                hitObjGetPos(hit0, &pos);
            } else {
                mario = marioGetPtr();
                pos.x = *(f32*)(mario + 0x8C);
                pos.y = *(f32*)(mario + 0x90);
                pos.z = *(f32*)(mario + 0x94);
            }
            hitObjGetNormal(hit0, &normal);
            ((Vec*)out0)->x = normal.x * front + pos.x;
            ((Vec*)out0)->z = normal.z * front + pos.z;
        }
    } else if (kind >= 0 && kind < 2) {
        if (hit1 == NULL) {
            mario = marioGetPtr();
            ((Vec*)out1)->x = *(f32*)(mario + 0x8C);
            ((Vec*)out1)->y = *(f32*)(mario + 0x90);
            ((Vec*)out1)->z = *(f32*)(mario + 0x94);
        } else {
            if (useMarioPos == 0) {
                hitObjGetPos(hit1, &pos);
            } else {
                mario = marioGetPtr();
                pos.x = *(f32*)(mario + 0x8C);
                pos.y = *(f32*)(mario + 0x90);
                pos.z = *(f32*)(mario + 0x94);
            }
            hitObjGetNormal(hit1, &normal);
            ((Vec*)out1)->x = -(normal.z * inset - (normal.x * side + pos.x));
            ((Vec*)out1)->z = normal.x * inset + normal.z * side + pos.z;
        }

        if (hit0 == NULL) {
            mario = marioGetPtr();
            ((Vec*)out0)->x = *(f32*)(mario + 0x8C);
            ((Vec*)out0)->y = *(f32*)(mario + 0x90);
            ((Vec*)out0)->z = *(f32*)(mario + 0x94);
        } else {
            if (useMarioPos == 0) {
                hitObjGetPos(hit0, &pos);
            } else {
                mario = marioGetPtr();
                pos.x = *(f32*)(mario + 0x8C);
                pos.y = *(f32*)(mario + 0x90);
                pos.z = *(f32*)(mario + 0x94);
            }
            hitObjGetNormal(hit0, &normal);
            ((Vec*)out0)->x = normal.x * front + pos.x;
            ((Vec*)out0)->z = normal.z * front + pos.z;
        }
    } else if (kind < 4) {
        if (hit1 == NULL) {
            mario = marioGetPtr();
            ((Vec*)out1)->x = *(f32*)(mario + 0x8C);
            ((Vec*)out1)->y = *(f32*)(mario + 0x90);
            ((Vec*)out1)->z = *(f32*)(mario + 0x94);
        } else {
            if (useMarioPos == 0) {
                hitObjGetPos(hit1, &pos);
            } else {
                mario = marioGetPtr();
                pos.x = *(f32*)(mario + 0x8C);
                pos.y = *(f32*)(mario + 0x90);
                pos.z = *(f32*)(mario + 0x94);
            }
            hitObjGetNormal(hit1, &normal);
            ((Vec*)out1)->x = normal.x * side + pos.x;
            ((Vec*)out1)->z = normal.z * side + pos.z;
        }

        if (hit0 == NULL) {
            mario = marioGetPtr();
            ((Vec*)out0)->x = *(f32*)(mario + 0x8C);
            ((Vec*)out0)->y = *(f32*)(mario + 0x90);
            ((Vec*)out0)->z = *(f32*)(mario + 0x94);
        } else {
            if (useMarioPos == 0) {
                hitObjGetPos(hit0, &pos);
            } else {
                mario = marioGetPtr();
                pos.x = *(f32*)(mario + 0x8C);
                pos.y = *(f32*)(mario + 0x90);
                pos.z = *(f32*)(mario + 0x94);
            }
            hitObjGetNormal(hit0, &normal);
            ((Vec*)out0)->x = normal.x * side + pos.x;
            ((Vec*)out0)->z = normal.z * side + pos.z;
        }
    }
}

s32 evt_door_param(EventEntry* event) {
    typedef struct Vec { f32 x; f32 y; f32 z; } Vec;
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    extern void door_position_sub(s32 kind, char* mapObjName, char* hit0, char* hit1, Vec* out0, Vec* out1, s32 useMarioPos);
    extern Vec vec3_802e41b8;
    extern Vec vec3_802e41c4;

    s32* args;
    s32* info;
    s32 kind;
    Vec out0;
    Vec out1;

    args = event->args;
    info = (s32*)evtGetValue(event, args[0]);
    kind = evtGetValue(event, args[1]);
    out0 = vec3_802e41b8;
    out1 = vec3_802e41c4;

    switch (kind) {
        case 0:
            evtSetValue(event, args[2], info[1]);
            break;
        case 1:
            evtSetValue(event, args[2], info[3]);
            evtSetValue(event, args[3], info[4]);
            evtSetValue(event, args[4], info[5]);
            break;
        case 2:
            evtSetValue(event, args[2], info[6]);
            evtSetValue(event, args[3], info[7]);
            break;
        case 3:
            evtSetValue(event, args[2], info[8]);
            evtSetValue(event, args[3], info[9]);
            break;
        case 4:
            door_position_sub(info[1], (char*)info[3], (char*)info[4], (char*)info[5], &out0, &out1, 0);
            evtSetValue(event, args[2], (s32)out0.x);
            evtSetValue(event, args[3], (s32)out0.y);
            evtSetValue(event, args[4], (s32)out0.z);
            break;
        case 5:
            door_position_sub(info[1], (char*)info[3], (char*)info[4], (char*)info[5], &out0, &out1, 0);
            evtSetValue(event, args[2], (s32)out1.x);
            evtSetValue(event, args[3], (s32)out1.y);
            evtSetValue(event, args[4], (s32)out1.z);
            break;
        case 6:
            evtSetValue(event, args[2], info[10]);
            evtSetValue(event, args[3], info[11]);
            evtSetValue(event, args[4], info[12]);
            break;
        case 7:
            evtSetValue(event, args[2], info[16]);
            evtSetValue(event, args[3], info[17]);
            break;
        case 8:
            evtSetValue(event, args[2], info[2]);
            break;
        case 9:
            evtSetValue(event, args[2], info[0]);
            break;
        case 10:
            door_position_sub(info[1], (char*)info[3], (char*)info[4], (char*)info[5], &out0, &out1, 1);
            evtSetValue(event, args[2], (s32)out0.x);
            evtSetValue(event, args[3], (s32)out0.y);
            evtSetValue(event, args[4], (s32)out0.z);
            break;
        case 11:
            door_position_sub(info[1], (char*)info[3], (char*)info[4], (char*)info[5], &out0, &out1, 1);
            evtSetValue(event, args[2], (s32)out1.x);
            evtSetValue(event, args[3], (s32)out1.y);
            evtSetValue(event, args[4], (s32)out1.z);
            break;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 door_entry(EventEntry* event) {
    typedef struct Vec3 { f32 x; f32 y; f32 z; } Vec3;
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    extern void* mapalloc_base_ptr;
    extern void* _mapAlloc(void* heap, s32 size);
    extern void hitObjGetPos(void* hit, Vec3* pos);
    extern void hitObjGetNormal(void* hit, Vec3* normal);
    extern const f32 float_10_80421b58;
    extern const f32 float_30_80421b60;
    extern const f32 float_40_80421b5c;
    extern Vec3 vec3_802e4164;
    extern Vec3 vec3_802e4170;
    s32* args;
    void* data;
    void* entry;
    Vec3 normal;
    Vec3 pos;

    args = event->args;
    data = (void*)evtGetValue(event, args[0]);
    pos = vec3_802e4164;
    normal = vec3_802e4170;
    entry = _mapAlloc(mapalloc_base_ptr, 0x1C);
    *(void**)entry = data;
    switch (*(s32*)data) {
        case 0:
        case 1:
            hitObjGetPos(*(void**)((s32)data + 8), &pos);
            hitObjGetNormal(*(void**)((s32)data + 8), &normal);
            *(f32*)((s32)entry + 0x10) = (float_40_80421b5c * normal.x + pos.x) - float_10_80421b58 * normal.z;
            *(f32*)((s32)entry + 0x18) = float_40_80421b5c * normal.z + pos.z + float_10_80421b58 * normal.x;
            hitObjGetPos(*(void**)((s32)data + 0x10), &pos);
            hitObjGetNormal(*(void**)((s32)data + 0x10), &normal);
            *(f32*)((s32)entry + 4) = float_30_80421b60 * normal.x + pos.x;
            *(f32*)((s32)entry + 0xC) = float_30_80421b60 * normal.z + pos.z;
            break;
        case 2:
            hitObjGetPos(*(void**)((s32)data + 8), &pos);
            hitObjGetNormal(*(void**)((s32)data + 8), &normal);
            *(f32*)((s32)entry + 0x10) = float_40_80421b5c * normal.x + pos.x + float_10_80421b58 * normal.z;
            *(f32*)((s32)entry + 0x18) = (float_40_80421b5c * normal.z + pos.z) - float_10_80421b58 * normal.x;
            hitObjGetPos(*(void**)((s32)data + 0x10), &pos);
            hitObjGetNormal(*(void**)((s32)data + 0x10), &normal);
            *(f32*)((s32)entry + 4) = float_30_80421b60 * normal.x + pos.x;
            *(f32*)((s32)entry + 0xC) = float_30_80421b60 * normal.z + pos.z;
            break;
        case 3:
            hitObjGetPos(*(void**)((s32)data + 8), &pos);
            hitObjGetNormal(*(void**)((s32)data + 8), &normal);
            *(f32*)((s32)entry + 0x10) = float_40_80421b5c * normal.x + pos.x;
            *(f32*)((s32)entry + 0x18) = float_40_80421b5c * normal.z + pos.z;
            hitObjGetPos(*(void**)((s32)data + 0x10), &pos);
            hitObjGetNormal(*(void**)((s32)data + 0x10), &normal);
            *(f32*)((s32)entry + 4) = float_40_80421b5c * normal.x + pos.x;
            *(f32*)((s32)entry + 0xC) = float_40_80421b5c * normal.z + pos.z;
            break;
    }
    evtSetValue(event, args[1], (s32)entry);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void door_dark_flag_sub(s32 flag, s32 npcNames, s32 mobjNames, s32 itemNames) {
    extern void* marioGetPtr(void);
    extern void* partyGetPtr(s32 id);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern void animPoseSetMaterialFlagOff(s32 poseId, u32 flag);
    extern void* npcNameToPtr_NoAssert(void* name);
    extern void* mobjNameToPtrNoAssert(void* name);
    extern void* itemNameToPtr(void* name);
    extern void iconFlagOn(void* icon, u32 flag);
    extern void iconFlagOff(void* icon, u32 flag);
    void* mario;
    void* party0;
    void* party1;
    void** names;
    void* name;

    mario = marioGetPtr();
    party0 = partyGetPtr(0);
    party1 = partyGetPtr(1);
    if (flag != 0) {
        if (mario != 0) {
            animPoseSetMaterialFlagOn(*(s32*)((s32)mario + 0x22C), 0x1800);
            animPoseSetMaterialFlagOn(*(s32*)((s32)mario + 0x230), 0x1800);
            animPoseSetMaterialFlagOn(*(s32*)((s32)mario + 0x234), 0x1800);
        }
        if (party0 != 0) {
            animPoseSetMaterialFlagOn(*(s32*)((s32)party0 + 0xC), 0x1800);
        }
        if (party1 != 0) {
            animPoseSetMaterialFlagOn(*(s32*)((s32)party1 + 0xC), 0x1800);
        }
        if ((u32)npcNames != 0) {
            names = (void**)npcNames;
            while (*names != 0) {
                name = npcNameToPtr_NoAssert(*names);
                if (name != 0) {
                    animPoseSetMaterialFlagOn(*(s32*)((s32)name + 0x104), 0x1800);
                }
                names++;
            }
        }
        if ((u32)mobjNames != 0) {
            names = (void**)mobjNames;
            while (*names != 0) {
                name = mobjNameToPtrNoAssert(*names);
                if (name != 0) {
                    animPoseSetMaterialFlagOn(*(s32*)((s32)name + 0x70), 0x1800);
                }
                names++;
            }
        }
        if ((u32)itemNames != 0) {
            names = (void**)itemNames;
            while (*names != 0) {
                name = itemNameToPtr(*names);
                if (name != 0) {
                    iconFlagOn((void*)((s32)name + 0xC), 0x40);
                }
                names++;
            }
        }
    } else {
        if (mario != 0) {
            animPoseSetMaterialFlagOff(*(s32*)((s32)mario + 0x22C), 0x1800);
            animPoseSetMaterialFlagOff(*(s32*)((s32)mario + 0x230), 0x1800);
            animPoseSetMaterialFlagOff(*(s32*)((s32)mario + 0x234), 0x1800);
        }
        if (party0 != 0) {
            animPoseSetMaterialFlagOff(*(s32*)((s32)party0 + 0xC), 0x1800);
        }
        if (party1 != 0) {
            animPoseSetMaterialFlagOff(*(s32*)((s32)party1 + 0xC), 0x1800);
        }
        if ((u32)npcNames != 0) {
            names = (void**)npcNames;
            while (*names != 0) {
                name = npcNameToPtr_NoAssert(*names);
                if (name != 0) {
                    animPoseSetMaterialFlagOff(*(s32*)((s32)name + 0x104), 0x1800);
                }
                names++;
            }
        }
        if ((u32)mobjNames != 0) {
            names = (void**)mobjNames;
            while (*names != 0) {
                name = mobjNameToPtrNoAssert(*names);
                if (name != 0) {
                    animPoseSetMaterialFlagOff(*(s32*)((s32)name + 0x70), 0x1800);
                }
                names++;
            }
        }
        if ((u32)itemNames != 0) {
            names = (void**)itemNames;
            while (*names != 0) {
                name = itemNameToPtr(*names);
                if (name != 0) {
                    iconFlagOff((void*)((s32)name + 0xC), 0x40);
                }
                names++;
            }
        }
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(npc_light) {
    extern void* marioGetPtr(void);
    extern void* partyGetPtr(s32 id);
    extern void animPoseSetMaterialFlagOff(s32 poseId, u32 flag);
    extern void* npcNameToPtr_NoAssert(void* name);
    extern void* mobjNameToPtrNoAssert(void* name);
    extern void* itemNameToPtr(void* name);
    extern void iconFlagOff(void* icon, u32 flag);

    void* info = (void*)evtGetValue(event, event->args[0]);
    void* mario = marioGetPtr();
    void* party0 = partyGetPtr(0);
    void* party1 = partyGetPtr(1);
    void** names;
    void* name;

    if (mario != 0) {
        animPoseSetMaterialFlagOff(*(s32*)((s32)marioGetPtr() + 0x22C), 0x1800);
        animPoseSetMaterialFlagOff(*(s32*)((s32)marioGetPtr() + 0x230), 0x1800);
        animPoseSetMaterialFlagOff(*(s32*)((s32)marioGetPtr() + 0x234), 0x1800);
    }
    if (party0 != 0) {
        animPoseSetMaterialFlagOff(*(s32*)((s32)party0 + 0xC), 0x1800);
    }
    if (party1 != 0) {
        animPoseSetMaterialFlagOff(*(s32*)((s32)party1 + 0xC), 0x1800);
    }

    names = *(void***)(*(s32*)info + 0x3C);
    if (names != 0) {
        while ((name = *names) != 0) {
            void* npc = npcNameToPtr_NoAssert(name);

            if (npc != 0) {
                animPoseSetMaterialFlagOff(*(s32*)((s32)npc + 0x104), 0x1800);
            }
            names++;
        }
    }

    names = *(void***)(*(s32*)info + 0x44);
    if (names != 0) {
        while ((name = *names) != 0) {
            void* mobj = mobjNameToPtrNoAssert(name);

            if (mobj != 0) {
                animPoseSetMaterialFlagOff(*(s32*)((s32)mobj + 0x70), 0x1800);
            }
            names++;
        }
    }

    names = *(void***)(*(s32*)info + 0x48);
    if (names != 0) {
        while ((name = *names) != 0) {
            void* item = itemNameToPtr(name);

            if (item != 0) {
                iconFlagOff((void*)((s32)item + 0xC), 0x40);
            }
            names++;
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(npc_dark) {
    extern void* marioGetPtr(void);
    extern void* partyGetPtr(s32 id);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern void* npcNameToPtr_NoAssert(void* name);
    extern void* mobjNameToPtrNoAssert(void* name);
    extern void* itemNameToPtr(void* name);
    extern void iconFlagOn(void* icon, u32 flag);

    void* info = (void*)evtGetValue(event, event->args[0]);
    void* mario = marioGetPtr();
    void* party0 = partyGetPtr(0);
    void* party1 = partyGetPtr(1);
    void** names;
    void* name;

    if (mario != 0) {
        animPoseSetMaterialFlagOn(*(s32*)((s32)marioGetPtr() + 0x22C), 0x1800);
        animPoseSetMaterialFlagOn(*(s32*)((s32)marioGetPtr() + 0x230), 0x1800);
        animPoseSetMaterialFlagOn(*(s32*)((s32)marioGetPtr() + 0x234), 0x1800);
    }
    if (party0 != 0) {
        animPoseSetMaterialFlagOn(*(s32*)((s32)party0 + 0xC), 0x1800);
    }
    if (party1 != 0) {
        animPoseSetMaterialFlagOn(*(s32*)((s32)party1 + 0xC), 0x1800);
    }

    names = *(void***)(*(s32*)info + 0x3C);
    if (names != 0) {
        while ((name = *names) != 0) {
            void* npc = npcNameToPtr_NoAssert(name);

            if (npc != 0) {
                animPoseSetMaterialFlagOn(*(s32*)((s32)npc + 0x104), 0x1800);
            }
            names++;
        }
    }

    names = *(void***)(*(s32*)info + 0x44);
    if (names != 0) {
        while ((name = *names) != 0) {
            void* mobj = mobjNameToPtrNoAssert(name);

            if (mobj != 0) {
                animPoseSetMaterialFlagOn(*(s32*)((s32)mobj + 0x70), 0x1800);
            }
            names++;
        }
    }

    names = *(void***)(*(s32*)info + 0x48);
    if (names != 0) {
        while ((name = *names) != 0) {
            void* item = itemNameToPtr(name);

            if (item != 0) {
                iconFlagOn((void*)((s32)item + 0xC), 0x40);
            }
            names++;
        }
    }

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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
USER_FUNC(npc_hide_onoff) {
    extern void* npcNameToPtr_NoAssert(void* name);

    EventEntry* evt = event;
    s32* args = evt->args;
    void* info = (void*)evtGetValue(evt, args[0]);
    s32 on = evtGetValue(evt, args[1]);
    void** names = *(void***)((s32)info + 0x34);
    void* name;

    while ((name = *names) != 0) {
        void* npc = npcNameToPtr_NoAssert(name);

        if (npc != 0) {
            if (on != 0) {
                *(u32*)npc |= 0x800;
            } else {
                *(u32*)npc &= ~0x800;
            }
        }
        names++;
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
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(get_noclip_map) {
    s32* args = event->args;
    void* info = (void*)evtGetValue(event, args[0]);
    s32 index = evtGetValue(event, args[1]);
    void** table = *(void***)(*(s32*)info + 0x40);

    if (table != 0 && table[index] != 0) {
        evtSetValue(event, args[2], (s32)table[index]);
        return 2;
    }

    evtSetValue(event, args[2], -1);
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(animation) {
    s32* args = event->args;
    void* info = (void*)evtGetValue(event, args[0]);

    evtSetValue(event, args[1], *(s32*)(*(s32*)info + 0x14));
    evtSetValue(event, args[2], *(s32*)(*(s32*)info + 0x18));
    evtSetValue(event, args[3], *(s32*)(*(s32*)info + 0x1C));
    evtSetValue(event, args[4], *(s32*)(*(s32*)info + 0x20));
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
USER_FUNC(inout_hit) {
    s32* args = event->args;
    void* info = (void*)evtGetValue(event, args[0]);

    evtSetValue(event, args[1], *(s32*)(*(s32*)info + 4));
    evtSetValue(event, args[2], *(s32*)(*(s32*)info + 8));
    evtSetValue(event, args[3], *(s32*)(*(s32*)info + 0xC));
    evtSetValue(event, args[4], *(s32*)(*(s32*)info + 0x10));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_door_data_copy) {
    typedef struct DoorCopyData {
        s32 word[18];
    } DoorCopyData;

    extern void* mapalloc_base_ptr;
    extern void* _mapAlloc(void* heap, u32 size);

    s32* args = event->args;
    DoorCopyData* src = (DoorCopyData*)evtGetValue(event, args[0]);
    DoorCopyData* dst = (DoorCopyData*)_mapAlloc(mapalloc_base_ptr, 0x48);

    *dst = *src;
    evtSetValue(event, args[1], (s32)dst);
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
USER_FUNC(in_pos) {
    s32* args = event->args;
    void* info = (void*)evtGetValue(event, args[0]);

    evtSetValue(event, args[1], (s32)*(f32*)((s32)info + 4));
    evtSetValue(event, args[2], (s32)*(f32*)((s32)info + 0xC));
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
USER_FUNC(out_pos) {
    s32* args = event->args;
    void* info = (void*)evtGetValue(event, args[0]);

    evtSetValue(event, args[1], (s32)*(f32*)((s32)info + 0x10));
    evtSetValue(event, args[2], (s32)*(f32*)((s32)info + 0x18));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 inside_group(EventEntry* event) {
    s32* args;
    void* info;

    args = event->args;
    info = (void*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], *(s32*)(*(s32*)info + 0x2C));
    evtSetValue(event, args[2], *(s32*)(*(s32*)info + 0x30));
    return 2;
}

s32 door_group(EventEntry* event) {
    s32* args;
    void* info;

    args = event->args;
    info = (void*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], *(s32*)(*(s32*)info + 0x28));
    return 2;
}

s32 outside_group(EventEntry* event) {
    s32* args;
    void* info;

    args = event->args;
    info = (void*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], *(s32*)(*(s32*)info + 0x24));
    return 2;
}

/* CHATGPT FALLBACK MISSING STUBS: main/event/evt_door 20260624_191429 */

/* fallback stub-fill: map=?snd_door_out addr=0x800e96e0 size=0x00000028 */
static s32 snd_door_out(void) {
    extern void psndClearFlag(s32 flag);

    psndClearFlag(0x100);
    return 2;
}

/* fallback stub-fill: map=?snd_door_in addr=0x800e9708 size=0x00000028 */
static s32 snd_door_in(void) {
    extern void psndSetFlag(s32 flag);

    psndSetFlag(0x100);
    return 2;
}

