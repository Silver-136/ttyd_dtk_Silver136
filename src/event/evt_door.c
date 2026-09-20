#include "event/evt_door.h"
#include "event/evt_cmd.h"

s32 DoorFLAG;
s32 door_mapflag;

void* mapGetMapObj(void);
void mapErrorEntry(s32 type, s32 value);
void door_dark_flag_sub(s32 unk0, s32 unk1, s32 unk2, s32 unk3);
extern void* mapalloc_base_ptr;
extern const f32 float_10_80421b58;
extern const f32 float_30_80421b60;
extern const f32 float_40_80421b5c;
extern void* marioGetPtr(void);
extern void* partyGetPtr(s32 id);
extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
extern void* npcNameToPtr_NoAssert(void* name);
extern void* mobjNameToPtrNoAssert(void* name);
extern void* itemNameToPtr(void* name);
extern void iconFlagOn(void* icon, u32 flag);
extern void animPoseSetMaterialFlagOff(s32 poseId, u32 flag);
extern void iconFlagOff(void* icon, u32 flag);

/* Forward declarations for recovered evt_door event bytecode. */
extern u32 door_close_se[];
extern u32 door_dark[];
extern u32 door_dark2[];
extern u32 door_down_se[];
extern u32 door_light[];
extern u32 door_light2[];
extern u32 door_open_se[];
extern u32 door_up_se[];
extern u32 evt_dark[];
extern u32 evt_dark2[];
extern u32 evt_door_close[];
extern u32 evt_door_close_roll[];
extern u32 evt_door_entry[];
extern u32 evt_door_open[];
extern u32 evt_door_open_bom_01[];
extern u32 evt_door_open_quick[];
extern u32 evt_door_open_roll[];
extern u32 evt_door_setup[];
extern u32 evt_light[];
extern u32 evt_light2[];
extern u32 house_close_se[];
extern u32 house_in[];
extern u32 house_open_se[];
extern u32 house_out[];
extern u32 in_dooranim[];
extern u32 in_houseanim[];
extern u32 out_dooranim[];
extern u32 out_houseanim[];
extern s32 N_evt_mario_party_door_halve_hitbox();
extern s32 evt_cam3d_event_from_road();
extern s32 evt_cam3d_evt_off();
extern s32 evt_cam3d_evt_set();
extern s32 evt_cam3d_evt_set_xyz();
extern s32 evt_cam3d_evt_xyz_off();
extern s32 evt_cam3d_get_shift();
extern s32 evt_cam_letter_box_disable();
extern s32 evt_hit_get_position();
extern s32 evt_hitobj_onoff();
extern s32 evt_map_blend_off();
extern s32 evt_map_checkanim();
extern s32 evt_map_playanim();
extern s32 evt_map_set_blend();
extern s32 evt_mapobj_flag_onoff();
extern s32 evt_mapobj_get_position();
extern s32 evt_mapobj_rotate();
extern s32 evt_mario_get_pos();
extern s32 evt_mario_key_onoff();
extern s32 evt_mario_mov_pos2();
extern s32 evt_mario_party_door_move();
extern s32 evt_party_move_beside_mario();
extern s32 evt_run_case_evt();
extern s32 evt_snd_bgmoff();
extern s32 evt_snd_bgmon();
extern s32 evt_snd_env_lpf();
extern s32 evt_snd_envoff();
extern s32 evt_snd_envon();
extern s32 evt_snd_sfxon_3d();
extern s32 evt_sub_intpl_msec_get_value();
extern s32 evt_sub_intpl_msec_get_value_para();
extern s32 evt_sub_intpl_msec_init();

/* fallback stub-fill: map=?snd_door_in addr=0x800e9708 size=0x00000028 */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
static s32 snd_door_in(void) {
    extern void psndSetFlag(s32 flag);

    psndSetFlag(0x100);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
/* CHATGPT FALLBACK MISSING STUBS: main/event/evt_door 20260624_191429 */

/* fallback stub-fill: map=?snd_door_out addr=0x800e96e0 size=0x00000028 */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
static s32 snd_door_out(void) {
    extern void psndClearFlag(s32 flag);

    psndClearFlag(0x100);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 door_entry(EventEntry* event) {
    typedef struct Vec3 { f32 x; f32 y; f32 z; } Vec3;
    extern void* _mapAlloc(void* heap, s32 size);
    extern void hitObjGetPos(void* hit, Vec3* pos);
    extern void hitObjGetNormal(void* hit, Vec3* normal);
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

s32 outside_group(EventEntry* event) {
    s32* args;
    void* info;

    args = event->args;
    info = (void*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], *(s32*)(*(s32*)info + 0x24));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 door_group(EventEntry* event) {
    s32* args;
    void* info;

    args = event->args;
    info = (void*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], *(s32*)(*(s32*)info + 0x28));
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
USER_FUNC(npc_dark) {

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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(npc_light) {

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


void door_position_sub(s32 kind, char* mapObjName, char* hit0, char* hit1, void* out0, void* out1, s32 useMarioPos) {
    typedef struct Vec { f32 x; f32 y; f32 z; } Vec;
    typedef struct RawVec { u32 x; u32 y; u32 z; } RawVec;
    extern void PSMTXInverse(void* in, void* out);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXMultVec(void* mtx, void* in, void* out);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern f64 atan(f64 x);
    extern f64 cos(f64 x);
    extern void hitObjGetPos(char* hit, void* out);
    extern void hitObjGetNormal(char* hit, void* out);
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
            *(RawVec*)out1 = *(const RawVec*)(mario + 0x8C);
        } else {
            if (useMarioPos == 0) {
                hitObjGetPos(hit1, &pos);
            } else {
                mario = marioGetPtr();
                *(RawVec*)&pos = *(const RawVec*)(mario + 0x8C);
            }
            hitObjGetNormal(hit1, &normal);
            ((Vec*)out1)->x = normal.z * inset + normal.x * side + pos.x;
            ((Vec*)out1)->z = -(normal.x * inset - (normal.z * side + pos.z));
        }

        if (hit0 == NULL) {
            mario = marioGetPtr();
            *(RawVec*)out0 = *(const RawVec*)(mario + 0x8C);
        } else {
            if (useMarioPos == 0) {
                hitObjGetPos(hit0, &pos);
            } else {
                mario = marioGetPtr();
                *(RawVec*)&pos = *(const RawVec*)(mario + 0x8C);
            }
            hitObjGetNormal(hit0, &normal);
            ((Vec*)out0)->x = normal.x * front + pos.x;
            ((Vec*)out0)->z = normal.z * front + pos.z;
        }
    } else if (kind >= 0 && kind < 2) {
        if (hit1 == NULL) {
            mario = marioGetPtr();
            *(RawVec*)out1 = *(const RawVec*)(mario + 0x8C);
        } else {
            if (useMarioPos == 0) {
                hitObjGetPos(hit1, &pos);
            } else {
                mario = marioGetPtr();
                *(RawVec*)&pos = *(const RawVec*)(mario + 0x8C);
            }
            hitObjGetNormal(hit1, &normal);
            ((Vec*)out1)->x = -(normal.z * inset - (normal.x * side + pos.x));
            ((Vec*)out1)->z = normal.x * inset + normal.z * side + pos.z;
        }

        if (hit0 == NULL) {
            mario = marioGetPtr();
            *(RawVec*)out0 = *(const RawVec*)(mario + 0x8C);
        } else {
            if (useMarioPos == 0) {
                hitObjGetPos(hit0, &pos);
            } else {
                mario = marioGetPtr();
                *(RawVec*)&pos = *(const RawVec*)(mario + 0x8C);
            }
            hitObjGetNormal(hit0, &normal);
            ((Vec*)out0)->x = normal.x * front + pos.x;
            ((Vec*)out0)->z = normal.z * front + pos.z;
        }
    } else if (kind < 4) {
        if (hit1 == NULL) {
            mario = marioGetPtr();
            *(RawVec*)out1 = *(const RawVec*)(mario + 0x8C);
        } else {
            if (useMarioPos == 0) {
                hitObjGetPos(hit1, &pos);
            } else {
                mario = marioGetPtr();
                *(RawVec*)&pos = *(const RawVec*)(mario + 0x8C);
            }
            hitObjGetNormal(hit1, &normal);
            ((Vec*)out1)->x = normal.x * side + pos.x;
            ((Vec*)out1)->z = normal.z * side + pos.z;
        }

        if (hit0 == NULL) {
            mario = marioGetPtr();
            *(RawVec*)out0 = *(const RawVec*)(mario + 0x8C);
        } else {
            if (useMarioPos == 0) {
                hitObjGetPos(hit0, &pos);
            } else {
                mario = marioGetPtr();
                *(RawVec*)&pos = *(const RawVec*)(mario + 0x8C);
            }
            hitObjGetNormal(hit0, &normal);
            ((Vec*)out0)->x = normal.x * side + pos.x;
            ((Vec*)out0)->z = normal.z * side + pos.z;
        }
    }
}
void door_dark_flag_sub(s32 flag, s32 npcNames, s32 mobjNames, s32 itemNames) {
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


s32 evt_door_param(EventEntry* event) {
    typedef struct Vec { f32 x; f32 y; f32 z; } Vec;
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
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(npc_hide_onoff) {

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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_door_data_copy) {
    typedef struct DoorCopyData {
        s32 word[18];
    } DoorCopyData;

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


s32 evt_door_end_wait(void) {
    register s32 flag = DoorFLAG;
    register s32 result = 2;
    register s32 mask;
    asm {
        neg mask, flag
        or mask, mask, flag
        srawi mask, mask, 31
        andc result, result, mask
    }
    return result;
}

const char str_SFX_DOOR_OPEN1_802e3f00[] = "SFX_DOOR_OPEN1";
const char str_SFX_DOOR_OPEN_WOOD1_802e3f10[] = "SFX_DOOR_OPEN_WOOD1";
const char str_SFX_DOOR_OPEN_WOOD2_802e3f24[] = "SFX_DOOR_OPEN_WOOD2";
const char str_SFX_DOOR_OPEN_WOOD3_802e3f38[] = "SFX_DOOR_OPEN_WOOD3";
const char str_SFX_DOOR_OPEN_IRON1_802e3f4c[] = "SFX_DOOR_OPEN_IRON1";
const char str_SFX_DOOR_OPEN_IRON2_802e3f60[] = "SFX_DOOR_OPEN_IRON2";
const char str_SFX_DOOR_OPEN_GRILLE_802e3f74[] = "SFX_DOOR_OPEN_GRILLE1";
const char str_SFX_DOOR_OPEN_SLIDE1_802e3f8c[] = "SFX_DOOR_OPEN_SLIDE1";
const char str_SFX_DOOR_OPEN_TURN1_802e3fa4[] = "SFX_DOOR_OPEN_TURN1";
const char str_SFX_DOOR_OPEN_ELEC1_802e3fb8[] = "SFX_DOOR_OPEN_ELEC1";
const char str_SFX_DOOR_OPEN_STONE1_802e3fcc[] = "SFX_DOOR_OPEN_STONE1";
const char str_SFX_DOOR_OPEN_CASTLE_802e3fe4[] = "SFX_DOOR_OPEN_CASTLE1";
const char str_SFX_DOOR_OPEN_CASTLE_802e3ffc[] = "SFX_DOOR_OPEN_CASTLE2";
const char str_SFX_DOOR_OPEN_METAL1_802e4014[] = "SFX_DOOR_OPEN_METAL1";
const char str_SFX_DOOR_SHUT1_802e402c[] = "SFX_DOOR_SHUT1";
const char str_SFX_DOOR_SHUT_WOOD1_802e403c[] = "SFX_DOOR_SHUT_WOOD1";
const char str_SFX_DOOR_SHUT_WOOD2_802e4050[] = "SFX_DOOR_SHUT_WOOD2";
const char str_SFX_DOOR_SHUT_IRON1_802e4064[] = "SFX_DOOR_SHUT_IRON1";
const char str_SFX_DOOR_SHUT_IRON2_802e4078[] = "SFX_DOOR_SHUT_IRON2";
const char str_SFX_DOOR_SHUT_GRILLE_802e408c[] = "SFX_DOOR_SHUT_GRILLE1";
const char str_SFX_DOOR_SHUT_SLIDE1_802e40a4[] = "SFX_DOOR_SHUT_SLIDE1";
const char str_SFX_DOOR_SHUT_TURN1_802e40bc[] = "SFX_DOOR_SHUT_TURN1";
const char str_SFX_DOOR_SHUT_ELEC1_802e40d0[] = "SFX_DOOR_SHUT_ELEC1";
const char str_SFX_DOOR_SHUT_STONE1_802e40e4[] = "SFX_DOOR_SHUT_STONE1";
const char str_SFX_DOOR_SHUT_CASTLE_802e40fc[] = "SFX_DOOR_SHUT_CASTLE1";
const char str_SFX_DOOR_SHUT_CASTLE_802e4114[] = "SFX_DOOR_SHUT_CASTLE2";
const char str_SFX_DOOR_SHUT_METAL1_802e412c[] = "SFX_DOOR_SHUT_METAL1";
const char str_SFX_DOOR_FALL1_802e4144[] = "SFX_DOOR_FALL1";
const char str_SFX_HOUSE_OPEN1_802e4154[] = "SFX_HOUSE_OPEN1";
const f32 vec3_802e4164[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802e4170[3] = { 0.0f, 0.0f, 0.0f };
const char str_S_mise_open_802e417c[] = "S_mise_open";
const char str_SFX_HOUSE_DOWN1_802e4188[] = "SFX_HOUSE_DOWN1";
const char str_S_mise_close_802e4198[] = "S_mise_close";
const char str_SFX_HOUSE_SHUT1_802e41a8[] = "SFX_HOUSE_SHUT1";
const f32 vec3_802e41b8[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802e41c4[3] = { 0.0f, 0.0f, 0.0f };

const f32 float_10_80421b58 = 10.0f;
const f32 float_40_80421b5c = 40.0f;
const f32 float_30_80421b60 = 30.0f;
const f32 float_50_80421b64 = 50.0f;
const f32 float_0_80421b68 = 0.0f;
const f32 float_0p5_80421b6c = 0.5f;

/*
 * Recovered evt_door event bytecode.
 * Exact US target words are preserved; relocation words remain symbolic.
 * jumptable_803316A0 is intentionally excluded as compiler-generated code data.
 */
/* .data */
u32 door_open_se[] = {
    0x0004005B, (u32)evt_door_param, 0xFE363C80, 0x00000008, 0xFE363C81, 0x0006005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000001, 0xFE363C82, 0xFE363C83, 0xFE363C84,
    0x0005005B, (u32)evt_hit_get_position, 0xFE363C83, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00010022, 0xFE363C81, 0x00010024, 0x00000000, 0x00010024, 0x00000001,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN1_802e3f00, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000000, 0x00010024, 0x00000002, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_WOOD1_802e3f10,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000, 0x00010024, 0x00000003,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_WOOD2_802e3f24, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000000, 0x00010024, 0x00000004, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_WOOD3_802e3f38,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000, 0x00010024, 0x00000005,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_IRON1_802e3f4c, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000000, 0x00010024, 0x00000006, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_IRON2_802e3f60,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000, 0x00010024, 0x00000007,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_GRILLE_802e3f74, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000000, 0x00010024, 0x00000008, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_SLIDE1_802e3f8c,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000, 0x00010024, 0x00000009,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_TURN1_802e3fa4, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000000, 0x00010024, 0x0000000A, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_ELEC1_802e3fb8,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000, 0x00010024, 0x0000000B,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_STONE1_802e3fcc, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000000, 0x00010024, 0x0000000C, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_CASTLE_802e3fe4,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000, 0x00010024, 0x0000000D,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_CASTLE_802e3ffc, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000000, 0x00010024, 0x0000000E, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_METAL1_802e4014,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000, 0x00000031, 0x00000002,
    0x00000001,
};

u32 door_close_se[] = {
    0x0004005B, (u32)evt_door_param, 0xFE363C80, 0x00000008, 0xFE363C81, 0x0006005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000001, 0xFE363C82, 0xFE363C83, 0xFE363C84,
    0x0005005B, (u32)evt_hit_get_position, 0xFE363C83, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00010022, 0xFE363C81, 0x00010024, 0x00000000, 0x00010024, 0x00000001,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT1_802e402c, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000000, 0x00010024, 0x00000002, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_WOOD1_802e403c,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000, 0x00010024, 0x00000003,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_WOOD2_802e4050, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000000, 0x00010024, 0x00000004, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_WOOD2_802e4050,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000, 0x00010024, 0x00000005,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_IRON1_802e4064, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000000, 0x00010024, 0x00000006, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_IRON2_802e4078,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000, 0x00010024, 0x00000007,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_GRILLE_802e408c, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000000, 0x00010024, 0x00000008, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_SLIDE1_802e40a4,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000, 0x00010024, 0x00000009,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_TURN1_802e40bc, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000000, 0x00010024, 0x0000000A, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_ELEC1_802e40d0,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000, 0x00010024, 0x0000000B,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_STONE1_802e40e4, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000000, 0x00010024, 0x0000000C, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_CASTLE_802e40fc,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000, 0x00010024, 0x0000000D,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_CASTLE_802e4114, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000000, 0x00010024, 0x0000000E, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_METAL1_802e412c,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000, 0x00000031, 0x00000002,
    0x00000001,
};

u32 door_down_se[] = {
    0x0004005B, (u32)evt_door_param, 0xFE363C80, 0x00000008, 0xFE363C81, 0x0006005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000001, 0xFE363C82, 0xFE363C83, 0xFE363C84,
    0x0005005B, (u32)evt_hit_get_position, 0xFE363C83, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00010022, 0xFE363C81, 0x00010024, 0x00000000, 0x0000002A, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_FALL1_802e4144, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000,
    0x00000031, 0x00000002, 0x00000001,
};

u32 house_open_se[] = {
    0x0004005B, (u32)evt_door_param, 0xFE363C80, 0x00000009, 0xFE363C81, 0x0002001E,
    0xFE363C81, 0x00000002, 0x00000002, 0x00000021, 0x0006005B, (u32)evt_door_param,
    0xFE363C80, 0x00000001, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0x0005005B,
    (u32)evt_hit_get_position, 0xFE363C83, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_HOUSE_OPEN1_802e4154, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000000,
    0x00000002, 0x00000001,
};

u32 evt_dark[] = {
    0x0005005B, (u32)evt_sub_intpl_msec_init, 0x0000000B, 0x000000FF, 0x00000000, 0x0000012C,
    0x00010005, 0x00000000, 0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x0006005B, (u32)evt_map_set_blend,
    0x00000000, 0xFE363C80, 0xFE363C80, 0xFE363C80, 0x000000FF, 0x00020018,
    0xFE363C81, 0x00000000, 0x00000007, 0x00000021, 0x00010009, 0x00000001,
    0x00000006, 0x0006005B, (u32)evt_map_set_blend, 0x00000000, 0xFE363C80, 0xFE363C80,
    0xFE363C80, 0x00000000, 0x00000002, 0x00000001,
};

u32 evt_dark2[] = {
    0x0005005B, (u32)evt_sub_intpl_msec_init, 0x0000000B, 0x000000FF, 0x00000000, 0x0000012C,
    0x00010005, 0x00000000, 0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x0006005B, (u32)evt_map_set_blend,
    0x00000001, 0xFE363C80, 0xFE363C80, 0xFE363C80, 0x000000FF, 0x00020018,
    0xFE363C81, 0x00000000, 0x00000007, 0x00000021, 0x00010009, 0x00000001,
    0x00000006, 0x0006005B, (u32)evt_map_set_blend, 0x00000001, 0xFE363C80, 0xFE363C80,
    0xFE363C80, 0x00000000, 0x00000002, 0x00000001,
};

u32 evt_light[] = {
    0x0005005B, (u32)evt_sub_intpl_msec_init, 0x0000000B, 0x00000000, 0x000000FF, 0x0000012C,
    0x00010005, 0x00000000, 0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x0006005B, (u32)evt_map_set_blend,
    0x00000000, 0xFE363C80, 0xFE363C80, 0xFE363C80, 0x000000FF, 0x00020018,
    0xFE363C81, 0x00000000, 0x00000007, 0x00000021, 0x00010009, 0x00000001,
    0x00000006, 0x0002005B, (u32)evt_map_blend_off, 0x00000000, 0x00000002, 0x00000001,
};

u32 evt_light2[] = {
    0x0005005B, (u32)evt_sub_intpl_msec_init, 0x0000000B, 0x00000000, 0x000000FF, 0x0000012C,
    0x00010005, 0x00000000, 0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x0006005B, (u32)evt_map_set_blend,
    0x00000001, 0xFE363C80, 0xFE363C80, 0xFE363C80, 0x000000FF, 0x00020018,
    0xFE363C81, 0x00000000, 0x00000007, 0x00000021, 0x00010009, 0x00000001,
    0x00000006, 0x0002005B, (u32)evt_map_blend_off, 0x00000001, 0x00000002, 0x00000001,
};

u32 in_houseanim[] = {
    0x0001000A, 0x000001F4, 0x0006005B, (u32)animation, 0xFE363C8A, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C83, 0x00020019, 0xFE363C80, 0x00000000,
    0x0000006B, 0x0002000C, 0xFE363C80, (u32)(char*)str_S_mise_open_802e417c, 0x0003005B, (u32)door_group,
    0xFE363C8A, 0xFE363C80, 0x0005005B, (u32)evt_mapobj_get_position, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_HOUSE_OPEN1_802e4154, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x00000000, 0x00010005, 0x00000004, 0x0001000A,
    0x000000A6, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_HOUSE_DOWN1_802e4188, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0x00000000, 0x00000006, 0x00000020, 0x0003005B, (u32)door_group,
    0xFE363C8A, 0xFE363C80, 0x0005005B, (u32)evt_mapobj_get_position, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_HOUSE_OPEN1_802e4154, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x00000000, 0x00000021, 0x0000006D, 0x0004005B,
    (u32)evt_map_playanim, 0xFE363C80, 0x00000000, 0x00000000, 0x0004005B, (u32)evt_map_checkanim,
    0xFE363C80, 0xFE363C84, 0xFE363C85, 0x0001000A, 0xFE363C85, 0x00000021,
    0x00000002, 0x00000001,
};

u32 out_houseanim[] = {
    0x0006005B, (u32)animation, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xFE363C83, 0x00020019, 0xFE363C82, 0x00000000, 0x0000006B, 0x0002000C,
    0xFE363C82, (u32)(char*)str_S_mise_close_802e4198, 0x0003005B, (u32)door_group, 0xFE363C8A, 0xFE363C80,
    0x0005005B, (u32)evt_mapobj_get_position, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xFE363C84,
    0x00010005, 0x00000004, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_HOUSE_DOWN1_802e4188, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x00000000, 0x0001000A, 0x000000A6, 0x00000006,
    0x00000021, 0x0000006D, 0x0004005B, (u32)evt_map_playanim, 0xFE363C82, 0x00000000,
    0x00000000, 0x0004005B, (u32)evt_map_checkanim, 0xFE363C82, 0xFE363C84, 0xFE363C85,
    0x0001000A, 0xFE363C85, 0x0000006B, 0x0003005B, (u32)door_group, 0xFE363C8A,
    0xFE363C80, 0x0005005B, (u32)evt_mapobj_get_position, 0xFE363C80, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_HOUSE_SHUT1_802e41a8, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0x00000000, 0x0000006D, 0x00000021, 0x00000002, 0x00000001,
};

u32 in_dooranim[] = {
    0x0003005B, (u32)door_group, 0xFE363C8A, 0xFE363C89, 0x0000006B, 0x0005005B,
    (u32)evt_mapobj_get_position, 0xFE363C89, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN1_802e3f00, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0x00000000,
    0x0001000A, 0x000005DC, 0x0005005B, (u32)evt_mapobj_get_position, 0xFE363C89, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT1_802e402c, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x00000000, 0x0001000A, 0x000001F4, 0x0005005B,
    (u32)evt_mapobj_get_position, 0xFE363C89, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_FALL1_802e4144, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0x00000000,
    0x0000006D, 0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000005, 0x00000000, 0x00000050,
    0x000001F4, 0x00010003, 0x00000000, 0x0003005B, (u32)evt_sub_intpl_msec_get_value_para, 0xFE363C80,
    0xFE363C81, 0x0006005B, (u32)evt_mapobj_rotate, 0x00000001, 0xFE363C89, 0x00000000,
    0xFE363C80, 0x00000000, 0x00010009, 0x00000001, 0x00020018, 0xFE363C81,
    0x00000001, 0x00010004, 0x00000000, 0x00000021, 0x0001000A, 0x000001F4,
    0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000002, 0x00000050, 0x00000000, 0x000001F4,
    0x00010003, 0x00000001, 0x0003005B, (u32)evt_sub_intpl_msec_get_value_para, 0xFE363C80, 0xFE363C81,
    0x0006005B, (u32)evt_mapobj_rotate, 0x00000001, 0xFE363C89, 0x00000000, 0xFE363C80,
    0x00000000, 0x00010009, 0x00000001, 0x00020018, 0xFE363C81, 0x00000001,
    0x00010004, 0x00000001, 0x00000021, 0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000002,
    0x00000000, 0x0000005A, 0x000001F4, 0x00010003, 0x00000002, 0x0003005B,
    (u32)evt_sub_intpl_msec_get_value_para, 0xFE363C80, 0xFE363C81, 0x0006005B, (u32)evt_mapobj_rotate, 0x00000001,
    0xFE363C89, 0xFE363C80, 0x00000000, 0x00000000, 0x00010009, 0x00000001,
    0x00020018, 0xFE363C81, 0x00000001, 0x00010004, 0x00000002, 0x00000021,
    0x00000002, 0x00000001,
};

u32 out_dooranim[] = {
    0x0003005B, (u32)door_group, 0xFE363C8A, 0xFE363C89, 0x0000006B, 0x0001000A,
    0x000001F4, 0x0005005B, (u32)evt_mapobj_get_position, 0xFE363C89, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN1_802e3f00, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0x00000000, 0x0001000A, 0x000003E8, 0x0005005B, (u32)evt_mapobj_get_position,
    0xFE363C89, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_SHUT1_802e402c, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0x00000000, 0x0000006D,
    0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000005, 0x0000005A, 0x00000000, 0x000001F4,
    0x00010003, 0x00000000, 0x0003005B, (u32)evt_sub_intpl_msec_get_value_para, 0xFE363C80, 0xFE363C81,
    0x0006005B, (u32)evt_mapobj_rotate, 0x00000001, 0xFE363C89, 0xFE363C80, 0x00000000,
    0x00000000, 0x00010009, 0x00000001, 0x00020018, 0xFE363C81, 0x00000001,
    0x00010004, 0x00000000, 0x00000021, 0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000005,
    0x00000000, 0x00000050, 0x000001F4, 0x00010003, 0x00000001, 0x0003005B,
    (u32)evt_sub_intpl_msec_get_value_para, 0xFE363C80, 0xFE363C81, 0x0006005B, (u32)evt_mapobj_rotate, 0x00000001,
    0xFE363C89, 0x00000000, 0xFE363C80, 0x00000000, 0x00010009, 0x00000001,
    0x00020018, 0xFE363C81, 0x00000001, 0x00010004, 0x00000001, 0x00000021,
    0x0001000A, 0x000001F4, 0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000005, 0x00000050,
    0x00000000, 0x000001F4, 0x00010003, 0x00000002, 0x0003005B, (u32)evt_sub_intpl_msec_get_value_para,
    0xFE363C80, 0xFE363C81, 0x0006005B, (u32)evt_mapobj_rotate, 0x00000001, 0xFE363C89,
    0x00000000, 0xFE363C80, 0x00000000, 0x00010009, 0x00000001, 0x00020018,
    0xFE363C81, 0x00000001, 0x00010004, 0x00000002, 0x00000021, 0x00000002,
    0x00000001,
};

u32 house_in[] = {
    0x00020055, 0xFE363C80, (u32)&DoorFLAG, 0x00020018, 0xFE363C80, 0x00000001,
    0x00000002, 0x00000021, 0x00020053, 0x00000001, (u32)&DoorFLAG, 0x0002005B,
    (u32)evt_snd_bgmoff, 0x00004800, 0x0002005B, (u32)evt_snd_envoff, 0x00004800, 0x0003005B,
    (u32)evt_snd_env_lpf, 0x00000000, 0x00000320, 0x0002005B, (u32)evt_mario_key_onoff, 0x00000000,
    0x0002005B, (u32)evt_cam_letter_box_disable, 0x00000001, 0x0000006B, 0x0005005B, (u32)evt_mario_get_pos,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)in_pos,
    0xFE363C8A, 0xFE363C80, 0xFE363C82, 0x00020035, 0xFE363C81, 0x00000001,
    0x0007005B, (u32)evt_cam3d_evt_set_xyz, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x00000096, 0x0000000B, 0x0000006D, 0x00020032, 0xFE363C80, 0x00000000,
    0x00010005, 0x00000000, 0x0004005B, (u32)get_noclip_map, 0xFE363C8A, 0xFE363C80,
    0xFE363C81, 0x00020018, 0xFE363C81, 0xFFFFFFFF, 0x00000007, 0x00000021,
    0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, 0xFE363C81, 0x00000002,
    0x00020035, 0xFE363C80, 0x00000001, 0x00000006, 0x0003005B, (u32)outside_group,
    0xFE363C8A, 0xFE363C80, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001,
    0xFE363C80, 0x00000800, 0x0003005B, (u32)door_group, 0xFE363C8A, 0xFE363C80,
    0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, 0xFE363C80, 0x00000800,
    0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, 0xFE363C80, 0x00001000,
    0x0004005B, (u32)inside_group, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0x0005005B,
    (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, 0xFE363C80, 0x00000001, 0x0004005B,
    (u32)evt_hitobj_onoff, 0xFE363C81, 0x00000001, 0x00000001, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000001, 0xFE363C80, 0x00000800, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000001, 0xFE363C80, 0x00001000, 0x0001005B, (u32)N_evt_mario_party_door_halve_hitbox,
    0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)out_pos, 0xFE363C8A, 0xFE363C80, 0xFE363C82, 0x0004005B,
    (u32)evt_mario_mov_pos2, 0xFE363C80, 0xFE363C82, 0xF24C9680, 0x0002005B, (u32)evt_party_move_beside_mario,
    0x00000000, 0x0002005B, (u32)evt_party_move_beside_mario, 0x00000001, 0x0002005D, (u32)in_dooranim,
    0xFE363C8F, 0x0000006B, 0x0002005B, (u32)npc_dark, 0xFE363C8A, 0x0001005E,
    (u32)in_houseanim, 0x0001005C, (u32)evt_dark2, 0x0000006D, 0x0001000A, 0x0000012C,
    0x0001005C, (u32)evt_dark, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)in_pos, 0xFE363C8A, 0xFE363C80,
    0xFE363C82, 0x0004005B, (u32)evt_mario_party_door_move, 0xFE363C80, 0xFE363C82, 0xF24C9680,
    0x0003005B, (u32)evt_cam3d_evt_xyz_off, 0x00000000, 0x0000000B, 0x0002005B, (u32)evt_mario_key_onoff,
    0x00000001, 0x0002005B, (u32)evt_cam_letter_box_disable, 0x00000000, 0x00010005, 0x00000000,
    0x0002006A, 0xFE363C8F, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000,
    0x00000007, 0x00000021, 0x00010009, 0x00000001, 0x00000006, 0x0001005B,
    (u32)snd_door_in, 0x00020053, 0x00000000, (u32)&DoorFLAG, 0x00000002, 0x00000001,
};

u32 house_out[] = {
    0x00020055, 0xFE363C80, (u32)&DoorFLAG, 0x00020018, 0xFE363C80, 0x00000001,
    0x00000002, 0x00000021, 0x00020053, 0x00000001, (u32)&DoorFLAG, 0x0003005B,
    (u32)evt_snd_bgmon, 0x000000A0, 0x00000000, 0x0003005B, (u32)evt_snd_envon, 0x000000A0,
    0x00000000, 0x0003005B, (u32)evt_snd_env_lpf, 0x00000000, 0x00000000, 0x0002005B,
    (u32)evt_mario_key_onoff, 0x00000000, 0x0002005B, (u32)evt_cam_letter_box_disable, 0x00000001, 0x0000006B,
    0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)out_pos, 0xFE363C8A, 0xFE363C80, 0xFE363C82, 0x00020035,
    0xFE363C81, 0x00000001, 0x0007005B, (u32)evt_cam3d_evt_set_xyz, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000096, 0x0000000B, 0x0000006D, 0x0000006B,
    0x0001005C, (u32)evt_light2, 0x0001005E, (u32)out_houseanim, 0x0001005C, (u32)evt_light,
    0x0000006D, 0x0002005D, (u32)out_dooranim, 0xFE363C8F, 0x0001000A, 0x000003E8,
    0x0001005B, (u32)N_evt_mario_party_door_halve_hitbox, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)out_pos, 0xFE363C8A, 0xFE363C80,
    0xFE363C82, 0x0004005B, (u32)evt_mario_party_door_move, 0xFE363C80, 0xFE363C82, 0xF24C9680,
    0x00020032, 0xFE363C80, 0x00000000, 0x00010005, 0x00000000, 0x0004005B,
    (u32)get_noclip_map, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0x00020018, 0xFE363C81,
    0xFFFFFFFF, 0x00000007, 0x00000021, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, 0xFE363C81, 0x00000002, 0x00020035, 0xFE363C80, 0x00000001,
    0x00000006, 0x0003005B, (u32)evt_cam3d_evt_xyz_off, 0x00000064, 0x0000000B, 0x0002005B,
    (u32)evt_mario_key_onoff, 0x00000001, 0x0002005B, (u32)evt_cam_letter_box_disable, 0x00000000, 0x00010005,
    0x00000000, 0x0002006A, 0xFE363C8F, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000000, 0x00000007, 0x00000021, 0x00010009, 0x00000001, 0x00000006,
    0x00020053, 0x00000000, (u32)&DoorFLAG, 0x0004005B, (u32)inside_group, 0xFE363C8A,
    0xFE363C80, 0xFE363C81, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001,
    0xFE363C80, 0x00000001, 0x0004005B, (u32)evt_hitobj_onoff, 0xFE363C81, 0x00000001,
    0x00000000, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, 0xFE363C80,
    0x00000800, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, 0xFE363C80,
    0x00001000, 0x0003005B, (u32)outside_group, 0xFE363C8A, 0xFE363C80, 0x0005005B,
    (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, 0xFE363C80, 0x00000800, 0x0003005B,
    (u32)door_group, 0xFE363C8A, 0xFE363C80, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000000, 0xFE363C80, 0x00000800, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000000, 0xFE363C80, 0x00001000, 0x0002005B, (u32)npc_light, 0xFE363C8A,
    0x0001005B, (u32)snd_door_out, 0x00000002, 0x00000001,
};

u32 evt_door_setup[] = {
    0x0003005B, (u32)door_entry, 0xFE363C80, 0xFE363C8A, 0x00020053, 0x00000000,
    (u32)&DoorFLAG, 0x0004005B, (u32)init_inside_group, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0x00020019, 0xFE363C80, 0x00000000, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, 0xFE363C80, 0x00000001, 0x00000021, 0x00020019, 0xFE363C81,
    0x00000000, 0x0004005B, (u32)evt_hitobj_onoff, 0xFE363C81, 0x00000001, 0x00000000,
    0x00000021, 0x0006005B, (u32)inout_hit, 0xFE363C8A, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xFE363C83, 0x0007005B, (u32)evt_run_case_evt, 0xFE363C80, 0x00000001,
    0xFE363C81, 0x00000000, (u32)house_in, 0x00000000, 0x0007005B, (u32)evt_run_case_evt,
    0xFE363C82, 0x00000001, 0xFE363C83, 0x00000000, (u32)house_out, 0x00000000,
    0x00000002, 0x00000001,
};

u32 door_dark[] = {
    0x0005005B, (u32)evt_sub_intpl_msec_init, 0x0000000B, 0x000000FF, 0x00000000, 0x0000012C,
    0x00010003, 0x00000000, 0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x0006005B, (u32)evt_map_set_blend,
    0x00000000, 0xFE363C80, 0xFE363C80, 0xFE363C80, 0x000000FF, 0x00010009,
    0x00000001, 0x00020018, 0xFE363C81, 0x00000001, 0x00010004, 0x00000000,
    0x00000021, 0x0006005B, (u32)evt_map_set_blend, 0x00000000, 0xFE363C80, 0xFE363C80,
    0xFE363C80, 0x00000000, 0x00000002, 0x00000001,
};

u32 door_dark2[] = {
    0x0005005B, (u32)evt_sub_intpl_msec_init, 0x0000000B, 0x000000FF, 0x00000000, 0x0000012C,
    0x00010003, 0x00000000, 0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x0006005B, (u32)evt_map_set_blend,
    0x00000001, 0xFE363C80, 0xFE363C80, 0xFE363C80, 0x000000FF, 0x00010009,
    0x00000001, 0x00020018, 0xFE363C81, 0x00000001, 0x00010004, 0x00000000,
    0x00000021, 0x0006005B, (u32)evt_map_set_blend, 0x00000001, 0xFE363C80, 0xFE363C80,
    0xFE363C80, 0x00000000, 0x00000002, 0x00000001,
};

u32 door_light[] = {
    0x0005005B, (u32)evt_sub_intpl_msec_init, 0x0000000B, 0x00000000, 0x000000FF, 0x0000012C,
    0x00010003, 0x00000000, 0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x0006005B, (u32)evt_map_set_blend,
    0x00000000, 0xFE363C80, 0xFE363C80, 0xFE363C80, 0x000000FF, 0x00010009,
    0x00000001, 0x00020018, 0xFE363C81, 0x00000001, 0x00010004, 0x00000000,
    0x00000021, 0x0002005B, (u32)evt_map_blend_off, 0x00000000, 0x00000002, 0x00000001,
};

u32 door_light2[] = {
    0x0005005B, (u32)evt_sub_intpl_msec_init, 0x0000000B, 0x00000000, 0x000000FF, 0x0000012C,
    0x00010003, 0x00000000, 0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x0006005B, (u32)evt_map_set_blend,
    0x00000001, 0xFE363C80, 0xFE363C80, 0xFE363C80, 0x000000FF, 0x00010009,
    0x00000001, 0x00020018, 0xFE363C81, 0x00000001, 0x00010004, 0x00000000,
    0x00000021, 0x0002005B, (u32)evt_map_blend_off, 0x00000001, 0x00000002, 0x00000001,
};

u32 evt_door_open[] = {
    0x0002005B, (u32)evt_cam_letter_box_disable, 0x00000001, 0x0002005B, (u32)evt_mario_key_onoff, 0x00000000,
    0x00020053, 0x00000001, (u32)&DoorFLAG, 0x0005005B, (u32)evt_door_param, 0xFE363C80,
    0x00000007, 0xFE363C81, 0xFE363C82, 0x00020019, 0xFE363C81, 0x00000000,
    0x00020032, 0xFBD3E280, 0x00000000, 0x0001005C, 0xFE363C81, 0x00000021,
    0x0005005B, (u32)evt_door_param, 0xFE363C80, 0x00000002, 0xFE363C81, 0xFE363C82,
    0x00020019, 0xFE363C81, 0x00000000, 0x0002005B, (u32)evt_door_save_mapflag, 0xFE363C81,
    0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, 0xFE363C81, 0x00000002,
    0x00000021, 0x0000006B, 0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000004,
    0xFE363C81, 0xFE363C82, 0xFE363C83, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000,
    0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00020035, 0xFE363C85, 0x00000002,
    0x0007005B, (u32)evt_cam3d_evt_set_xyz, 0xFE363C81, 0xFE363C85, 0xFE363C83, 0x00000000,
    0x00000096, 0x0000000B, 0x0000006D, 0x0005005B, (u32)evt_door_param, 0xFE363C80,
    0x00000002, 0xFE363C81, 0xFE363C82, 0x00020019, 0xFE363C81, 0x00000000,
    0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, 0xFE363C81, 0x00000800,
    0x00000021, 0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000001, 0xFE363C81,
    0xFE363C82, 0xFE363C83, 0x00020019, 0xFE363C81, 0x00000000, 0x0005005B,
    (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, 0xFE363C81, 0x00000800, 0x0005005B,
    (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, 0xFE363C81, 0x00001000, 0x00000021,
    0x0005005B, (u32)evt_door_param, 0xFE363C80, 0x00000003, 0xFE363C81, 0xFE363C82,
    0x00020019, 0xFE363C81, 0x00000000, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000000, 0xFE363C81, 0x00000001, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, 0xFE363C81, 0x00000800, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, 0xFE363C81, 0x00001000, 0x00000021, 0x00020019, 0xFE363C82,
    0x00000000, 0x0004005B, (u32)evt_hitobj_onoff, 0xFE363C82, 0x00000001, 0x00000001,
    0x00000021, 0x0004005B, (u32)evt_door_param, 0xFE363C80, 0x00000009, 0xFE363C81,
    0x0002001E, 0xFE363C81, 0x00000001, 0x0003005B, (u32)npc_hide_onoff, 0xFE363C80,
    0x00000000, 0x00000021, 0x0003005B, (u32)door_dark_flag, 0xFE363C80, 0x00000001,
    0x0002001E, 0xFE363C8F, 0x00000004, 0x0006005B, (u32)evt_door_param, 0xFE363C80,
    0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89, 0x00020019, 0xFE363C89,
    0x00000000, 0x0000006B, 0x0001000A, 0x000001F4, 0x0001005C, (u32)house_open_se,
    0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88,
    0xFE363C89, 0x00020032, 0xFE363C86, 0x00000000, 0x0005005B, (u32)evt_sub_intpl_msec_init,
    0x00000002, 0x00000000, 0x000000B4, 0x000001F4, 0x00010003, 0x00000002,
    0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x00020032, 0xFE363C85, 0xFE363C81, 0x00020032,
    0xFE363C81, 0x00000000, 0x00020032, 0xFBD3E280, 0x00000000, 0x0001005C,
    0xFE363C89, 0x00020032, 0xFE363C86, 0x00000001, 0x00010009, 0x00000001,
    0x00020018, 0xFE363C85, 0x00000001, 0x00010004, 0x00000002, 0x00000021,
    0x0000006D, 0x00000021, 0x00000021, 0x0001005B, (u32)N_evt_mario_party_door_halve_hitbox, 0x0006005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000005, 0xFE363C81, 0xFE363C82, 0xFE363C83,
    0x0004005B, (u32)evt_mario_mov_pos2, 0xFE363C81, 0xFE363C83, 0xF24C9680, 0x0002005B,
    (u32)evt_party_move_beside_mario, 0x00000000, 0x0002005B, (u32)evt_party_move_beside_mario, 0x00000001, 0x0002001F,
    0xFE363C8F, 0x00000002, 0x0001005C, (u32)door_dark, 0x00000021, 0x0001005C,
    (u32)door_open_se, 0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87,
    0xFE363C88, 0xFE363C89, 0x00020019, 0xFE363C87, 0x00000000, 0x0000006B,
    0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88,
    0xFE363C89, 0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000005, 0x00000000, 0x000000B4,
    0x000001F4, 0x00010003, 0x00000000, 0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x00020032,
    0xFE363C85, 0xFE363C81, 0x00020032, 0xFE363C81, 0x00000000, 0x00020032,
    0xFBD3E280, 0x00000000, 0x0001005C, 0xFE363C87, 0x00010009, 0x00000001,
    0x00020018, 0xFE363C85, 0x00000001, 0x00010004, 0x00000000, 0x00000021,
    0x0000006D, 0x00000021, 0x0001000A, 0x000001F4, 0x0006005B, (u32)evt_door_param,
    0xFE363C80, 0x00000004, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x0004005B,
    (u32)evt_mario_party_door_move, 0xFE363C81, 0xFE363C83, 0xF24C9680, 0x0006005B, (u32)evt_door_param,
    0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89, 0x00020019,
    0xFE363C87, 0x00000000, 0x0000006B, 0x0001005C, (u32)door_close_se, 0x0000006D,
    0x0000006B, 0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87,
    0xFE363C88, 0xFE363C89, 0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000002, 0x000000B4,
    0x00000000, 0x000001F4, 0x00010003, 0x00000001, 0x0001005B, (u32)evt_sub_intpl_msec_get_value,
    0x00020032, 0xFE363C85, 0xFE363C81, 0x00020032, 0xFE363C81, 0x00000001,
    0x00020032, 0xFBD3E280, 0x00000000, 0x0001005C, 0xFE363C87, 0x00010009,
    0x00000001, 0x00020018, 0xFE363C85, 0x00000001, 0x00010004, 0x00000001,
    0x00000021, 0x0000006D, 0x00000021, 0x0002001F, 0xFE363C8F, 0x00000004,
    0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88,
    0xFE363C89, 0x00020019, 0xFE363C89, 0x00000000, 0x0000006B, 0x0001005C,
    (u32)house_open_se, 0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87,
    0xFE363C88, 0xFE363C89, 0x00020032, 0xFE363C86, 0x00000000, 0x0005005B,
    (u32)evt_sub_intpl_msec_init, 0x00000002, 0x00000000, 0x000000B4, 0x000001F4, 0x00010003,
    0x00000002, 0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x00020032, 0xFE363C85, 0xFE363C81,
    0x00020032, 0xFE363C81, 0x00000000, 0x00020032, 0xFBD3E280, 0x00000000,
    0x0001005C, 0xFE363C89, 0x00020032, 0xFE363C86, 0x00000001, 0x00010009,
    0x00000001, 0x00020018, 0xFE363C85, 0x00000001, 0x00010004, 0x00000002,
    0x00000021, 0x0000006D, 0x00000021, 0x00000021, 0x0001000A, 0x000001F4,
    0x0002001F, 0xFE363C8F, 0x00000002, 0x0001005C, (u32)door_dark2, 0x00000021,
    0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88,
    0xFE363C89, 0x00020019, 0xFE363C88, 0x00000000, 0x0000006B, 0x0001000A,
    0x000001F4, 0x0001005C, (u32)door_down_se, 0x0000006D, 0x0000006B, 0x0006005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89,
    0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000002, 0x00000000, 0x000000B4, 0x000001F4,
    0x00010003, 0x00000003, 0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x00020032, 0xFBD3E280,
    0x00000000, 0x0001005C, 0xFE363C88, 0x00010009, 0x00000001, 0x00020018,
    0xFE363C81, 0x00000001, 0x00010004, 0x00000003, 0x00000021, 0x0000006D,
    0x00000021, 0x0001000A, 0x000001F4, 0x0003005B, (u32)evt_cam3d_evt_xyz_off, 0x00000000,
    0x0000000B, 0x00020053, 0x00000000, (u32)&DoorFLAG, 0x00010009, 0x00000002,
    0x0002005B, (u32)evt_mario_key_onoff, 0x00000001, 0x0002005B, (u32)evt_cam_letter_box_disable, 0x00000000,
    0x0001005B, (u32)snd_door_in, 0x00000002, 0x00000001,
};

u32 evt_door_open_bom_01[] = {
    0x0005005B, (u32)evt_door_param, 0xFE363C80, 0x00000007, 0xFE363C81, 0xFE363C82,
    0x00020019, 0xFE363C81, 0x00000000, 0x0001005C, 0xFE363C81, 0x00000021,
    0x0005005B, (u32)evt_door_param, 0xFE363C80, 0x00000002, 0xFE363C81, 0xFE363C82,
    0x00020019, 0xFE363C81, 0x00000000, 0x0002005B, (u32)evt_door_save_mapflag, 0xFE363C81,
    0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, 0xFE363C81, 0x00000002,
    0x00000021, 0x0005005B, (u32)evt_door_param, 0xFE363C80, 0x00000002, 0xFE363C81,
    0xFE363C82, 0x00020019, 0xFE363C81, 0x00000000, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000001, 0xFE363C81, 0x00000800, 0x00000021, 0x0006005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000001, 0xFE363C81, 0xFE363C82, 0xFE363C83,
    0x00020019, 0xFE363C81, 0x00000000, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, 0xFE363C81, 0x00000800, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, 0xFE363C81, 0x00001000, 0x00000021, 0x0005005B, (u32)evt_door_param,
    0xFE363C80, 0x00000003, 0xFE363C81, 0xFE363C82, 0x00020019, 0xFE363C81,
    0x00000000, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, 0xFE363C81,
    0x00000001, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, 0xFE363C81,
    0x00000800, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, 0xFE363C81,
    0x00001000, 0x00000021, 0x00020019, 0xFE363C82, 0x00000000, 0x0004005B,
    (u32)evt_hitobj_onoff, 0xFE363C82, 0x00000001, 0x00000001, 0x00000021, 0x0003005B,
    (u32)door_dark_flag, 0xFE363C80, 0x00000001, 0x0001005C, (u32)door_open_se, 0x0006005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89,
    0x00020019, 0xFE363C87, 0x00000000, 0x0000006B, 0x0006005B, (u32)evt_door_param,
    0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89, 0x0005005B,
    (u32)evt_sub_intpl_msec_init, 0x00000005, 0x00000000, 0x000000B4, 0x000001F4, 0x00010003,
    0x00000000, 0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x00020032, 0xFE363C85, 0xFE363C81,
    0x00020032, 0xFE363C81, 0x00000000, 0x0001005C, 0xFE363C87, 0x00010009,
    0x00000001, 0x00020018, 0xFE363C85, 0x00000001, 0x00010004, 0x00000000,
    0x00000021, 0x0000006D, 0x00000021, 0x0001000A, 0x000001F4, 0x00020032,
    0xFAA2B580, 0x00000001, 0x00010005, 0x00000000, 0x00020018, 0xFAA2B580,
    0x00000000, 0x00000007, 0x00000021, 0x00010009, 0x00000001, 0x00000006,
    0x0001005C, (u32)door_dark, 0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006,
    0xFE363C87, 0xFE363C88, 0xFE363C89, 0x00020019, 0xFE363C87, 0x00000000,
    0x0000006B, 0x0001005C, (u32)door_close_se, 0x0000006D, 0x0000006B, 0x0006005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89,
    0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000002, 0x000000B4, 0x00000000, 0x000001F4,
    0x00010003, 0x00000001, 0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x00020032, 0xFE363C85,
    0xFE363C81, 0x00020032, 0xFE363C81, 0x00000001, 0x0001005C, 0xFE363C87,
    0x00010009, 0x00000001, 0x00020018, 0xFE363C85, 0x00000001, 0x00010004,
    0x00000001, 0x00000021, 0x0000006D, 0x00000021, 0x0006005B, (u32)evt_door_param,
    0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89, 0x00020019,
    0xFE363C89, 0x00000000, 0x0000006B, 0x0001005C, (u32)house_open_se, 0x0006005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89,
    0x00020032, 0xFE363C86, 0x00000000, 0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000002,
    0x00000000, 0x000000B4, 0x000001F4, 0x00010003, 0x00000002, 0x0001005B,
    (u32)evt_sub_intpl_msec_get_value, 0x00020032, 0xFE363C85, 0xFE363C81, 0x00020032, 0xFE363C81,
    0x00000000, 0x0001005C, 0xFE363C89, 0x00020032, 0xFE363C86, 0x00000001,
    0x00010009, 0x00000001, 0x00020018, 0xFE363C85, 0x00000001, 0x00010004,
    0x00000002, 0x00000021, 0x0000006D, 0x00000021, 0x0001000A, 0x000001F4,
    0x0001005C, (u32)door_dark2, 0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006,
    0xFE363C87, 0xFE363C88, 0xFE363C89, 0x00020019, 0xFE363C88, 0x00000000,
    0x0000006B, 0x0001000A, 0x000001F4, 0x0001005C, (u32)door_down_se, 0x0000006D,
    0x0000006B, 0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87,
    0xFE363C88, 0xFE363C89, 0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000002, 0x00000000,
    0x000000B4, 0x000001F4, 0x00010003, 0x00000003, 0x0001005B, (u32)evt_sub_intpl_msec_get_value,
    0x0001005C, 0xFE363C88, 0x00010009, 0x00000001, 0x00020018, 0xFE363C81,
    0x00000001, 0x00010004, 0x00000003, 0x00000021, 0x0000006D, 0x00000021,
    0x0001000A, 0x000001F4, 0x0001005B, (u32)snd_door_in, 0x00000002, 0x00000001,
};

u32 evt_door_open_roll[] = {
    0x0002005B, (u32)evt_mario_key_onoff, 0x00000000, 0x0005005B, (u32)evt_door_param, 0xFE363C80,
    0x00000007, 0xFE363C81, 0xFE363C82, 0x00020019, 0xFE363C81, 0x00000000,
    0x0001005C, 0xFE363C81, 0x00000021, 0x0005005B, (u32)evt_door_param, 0xFE363C80,
    0x00000002, 0xFE363C81, 0xFE363C82, 0x00020019, 0xFE363C81, 0x00000000,
    0x0002005B, (u32)evt_door_save_mapflag, 0xFE363C81, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000000, 0xFE363C81, 0x00000002, 0x00000021, 0x0005005B, (u32)evt_door_param,
    0xFE363C80, 0x00000002, 0xFE363C81, 0xFE363C82, 0x00020019, 0xFE363C81,
    0x00000000, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, 0xFE363C81,
    0x00000800, 0x00000021, 0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000001,
    0xFE363C81, 0xFE363C82, 0xFE363C83, 0x00020019, 0xFE363C81, 0x00000000,
    0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, 0xFE363C81, 0x00000800,
    0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, 0xFE363C81, 0x00001000,
    0x00000021, 0x0005005B, (u32)evt_door_param, 0xFE363C80, 0x00000003, 0xFE363C81,
    0xFE363C82, 0x00020019, 0xFE363C81, 0x00000000, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000000, 0xFE363C81, 0x00000001, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000001, 0xFE363C81, 0x00000800, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000001, 0xFE363C81, 0x00001000, 0x00000021, 0x00020019,
    0xFE363C82, 0x00000000, 0x0004005B, (u32)evt_hitobj_onoff, 0xFE363C82, 0x00000001,
    0x00000001, 0x00000021, 0x0003005B, (u32)door_dark_flag, 0xFE363C80, 0x00000001,
    0x0001005C, (u32)door_dark, 0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006,
    0xFE363C87, 0xFE363C88, 0xFE363C89, 0x00020019, 0xFE363C89, 0x00000000,
    0x0000006B, 0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87,
    0xFE363C88, 0xFE363C89, 0x00020032, 0xFE363C86, 0x00000000, 0x0005005B,
    (u32)evt_sub_intpl_msec_init, 0x00000002, 0x00000000, 0x000000B4, 0x000001F4, 0x00010003,
    0x00000002, 0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x00020032, 0xFE363C85, 0xFE363C81,
    0x00020032, 0xFE363C81, 0x00000000, 0x0001005C, 0xFE363C89, 0x00020032,
    0xFE363C86, 0x00000001, 0x00010009, 0x00000001, 0x00020018, 0xFE363C85,
    0x00000001, 0x00010004, 0x00000002, 0x00000021, 0x0000006D, 0x00000021,
    0x0001000A, 0x000001F4, 0x0001005C, (u32)door_dark2, 0x0006005B, (u32)evt_door_param,
    0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89, 0x00020019,
    0xFE363C88, 0x00000000, 0x0000006B, 0x0006005B, (u32)evt_door_param, 0xFE363C80,
    0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89, 0x0005005B, (u32)evt_sub_intpl_msec_init,
    0x00000002, 0x00000000, 0x000000B4, 0x000001F4, 0x00010003, 0x00000003,
    0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x0001005C, 0xFE363C88, 0x00010009, 0x00000001,
    0x00020018, 0xFE363C81, 0x00000001, 0x00010004, 0x00000003, 0x00000021,
    0x0000006D, 0x00000021, 0x0001000A, 0x000001F4, 0x0003005B, (u32)evt_cam3d_evt_xyz_off,
    0x00000000, 0x0000000B, 0x0002005B, (u32)evt_mario_key_onoff, 0x00000001, 0x0001005B,
    (u32)snd_door_in, 0x00000002, 0x00000001,
};

u32 evt_door_close[] = {
    0x0002005B, (u32)evt_cam_letter_box_disable, 0x00000001, 0x0002005B, (u32)evt_mario_key_onoff, 0x00000000,
    0x00020053, 0x00000001, (u32)&DoorFLAG, 0x0005005B, (u32)evt_door_param, 0xFE363C80,
    0x00000007, 0xFE363C81, 0xFE363C82, 0x00020019, 0xFE363C82, 0x00000000,
    0x00020032, 0xFBD3E280, 0x00000001, 0x0001005C, 0xFE363C82, 0x00000021,
    0x0000006B, 0x0002001F, 0xFE363C8F, 0x00000010, 0x0006005B, (u32)evt_door_param,
    0xFE363C80, 0x00000005, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x00000020,
    0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x0000000B, 0xFE363C81, 0xFE363C82,
    0xFE363C83, 0x00000021, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C84,
    0xFE363C85, 0xFE363C86, 0x00020035, 0xFE363C85, 0x00000002, 0x0007005B,
    (u32)evt_cam3d_evt_set_xyz, 0xFE363C81, 0xFE363C85, 0xFE363C83, 0x00000000, 0x00000096,
    0x0000000B, 0x0000006D, 0x0002001F, 0xFE363C8F, 0x00000002, 0x0001005C,
    (u32)door_light2, 0x00000021, 0x0001005C, (u32)door_up_se, 0x0006005B, (u32)evt_door_param,
    0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89, 0x00020019,
    0xFE363C88, 0x00000000, 0x0000006B, 0x0006005B, (u32)evt_door_param, 0xFE363C80,
    0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89, 0x0005005B, (u32)evt_sub_intpl_msec_init,
    0x00000005, 0x000000B4, 0x00000000, 0x000001F4, 0x00010003, 0x00000003,
    0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x00020032, 0xFBD3E280, 0x00000001, 0x0001005C,
    0xFE363C88, 0x00010009, 0x00000001, 0x00020018, 0xFE363C81, 0x00000001,
    0x00010004, 0x00000003, 0x00000021, 0x0000006D, 0x00000021, 0x0006005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89,
    0x00020019, 0xFE363C89, 0x00000000, 0x0000006B, 0x0001000A, 0x00000190,
    0x0001005C, (u32)house_close_se, 0x0000006D, 0x0000006B, 0x0006005B, (u32)evt_door_param,
    0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89, 0x00020032,
    0xFE363C86, 0x00000000, 0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000005, 0x000000B4,
    0x00000000, 0x000001F4, 0x00010003, 0x00000002, 0x0001005B, (u32)evt_sub_intpl_msec_get_value,
    0x00020032, 0xFE363C85, 0xFE363C81, 0x00020032, 0xFE363C81, 0x00000003,
    0x00020032, 0xFBD3E280, 0x00000001, 0x0001005C, 0xFE363C89, 0x00020032,
    0xFE363C86, 0x00000001, 0x00010009, 0x00000001, 0x00020018, 0xFE363C85,
    0x00000001, 0x00010004, 0x00000002, 0x00000021, 0x0000006D, 0x00000021,
    0x0001000A, 0x000001F4, 0x0002001F, 0xFE363C8F, 0x00000002, 0x0001005C,
    (u32)door_light, 0x00000021, 0x0001005C, (u32)door_open_se, 0x0006005B, (u32)evt_door_param,
    0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89, 0x00020019,
    0xFE363C87, 0x00000000, 0x0000006B, 0x0006005B, (u32)evt_door_param, 0xFE363C80,
    0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89, 0x0005005B, (u32)evt_sub_intpl_msec_init,
    0x00000005, 0x00000000, 0x000000B4, 0x000001F4, 0x00010003, 0x00000001,
    0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x00020032, 0xFE363C85, 0xFE363C81, 0x00020032,
    0xFE363C81, 0x00000002, 0x00020032, 0xFBD3E280, 0x00000001, 0x0001005C,
    0xFE363C87, 0x00010009, 0x00000001, 0x00020018, 0xFE363C85, 0x00000001,
    0x00010004, 0x00000001, 0x00000021, 0x0000006D, 0x00000021, 0x0001000A,
    0x000001F4, 0x0002001F, 0xFE363C8F, 0x00000010, 0x0006005B, (u32)evt_door_param,
    0xFE363C80, 0x00000005, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x00000020,
    0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x0000000B, 0xFE363C81, 0xFE363C82,
    0xFE363C83, 0x00000021, 0x0002001F, 0xFE363C8F, 0x00000008, 0x0004005B,
    (u32)evt_mario_party_door_move, 0xFE363C81, 0xFE363C83, 0xF24C9680, 0x00000021, 0x0006005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89,
    0x00020019, 0xFE363C87, 0x00000000, 0x0000006B, 0x0001005C, (u32)door_close_se,
    0x0000006D, 0x0000006B, 0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006,
    0xFE363C87, 0xFE363C88, 0xFE363C89, 0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000005,
    0x000000B4, 0x00000000, 0x000001F4, 0x00010003, 0x00000000, 0x0001005B,
    (u32)evt_sub_intpl_msec_get_value, 0x00020032, 0xFE363C85, 0xFE363C81, 0x00020032, 0xFE363C81,
    0x00000003, 0x00020032, 0xFBD3E280, 0x00000001, 0x0001005C, 0xFE363C87,
    0x00010009, 0x00000001, 0x00020018, 0xFE363C85, 0x00000001, 0x00010004,
    0x00000000, 0x00000021, 0x0000006D, 0x0001000A, 0x000001F4, 0x00000021,
    0x0003005B, (u32)evt_cam3d_evt_xyz_off, 0x00000064, 0x0000000B, 0x0003005B, (u32)door_dark_flag,
    0xFE363C80, 0x00000000, 0x0004005B, (u32)evt_door_param, 0xFE363C80, 0x00000009,
    0xFE363C81, 0x0002001E, 0xFE363C81, 0x00000001, 0x0003005B, (u32)npc_hide_onoff,
    0xFE363C80, 0x00000001, 0x00000021, 0x0005005B, (u32)evt_door_param, 0xFE363C80,
    0x00000003, 0xFE363C81, 0xFE363C82, 0x00020019, 0xFE363C81, 0x00000000,
    0x0002001F, 0xFE363C8F, 0x00000001, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, 0xFE363C81, 0x00000001, 0x00000021, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000000, 0xFE363C81, 0x00000800, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000000, 0xFE363C81, 0x00001000, 0x00000021, 0x00020019,
    0xFE363C82, 0x00000000, 0x0004005B, (u32)evt_hitobj_onoff, 0xFE363C82, 0x00000001,
    0x00000000, 0x00000021, 0x0005005B, (u32)evt_door_param, 0xFE363C80, 0x00000002,
    0xFE363C81, 0xFE363C82, 0x00020019, 0xFE363C81, 0x00000000, 0x0005005B,
    (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, 0xFE363C81, 0x00000800, 0x00000021,
    0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000001, 0xFE363C81, 0xFE363C82,
    0xFE363C83, 0x00020019, 0xFE363C81, 0x00000000, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000000, 0xFE363C81, 0x00000800, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000000, 0xFE363C81, 0x00001000, 0x00000021, 0x0005005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000002, 0xFE363C81, 0xFE363C82, 0x00020019,
    0xFE363C81, 0x00000000, 0x0002005B, (u32)evt_door_load_mapflag, 0xFE363C81, 0x00000021,
    0x00020053, 0x00000000, (u32)&DoorFLAG, 0x00010009, 0x00000002, 0x0002005B,
    (u32)evt_mario_key_onoff, 0x00000001, 0x0002005B, (u32)evt_cam_letter_box_disable, 0x00000000, 0x0001005B,
    (u32)snd_door_out, 0x00000002, 0x00000001,
};

u32 evt_door_close_roll[] = {
    0x0002005B, (u32)evt_mario_key_onoff, 0x00000000, 0x0005005B, (u32)evt_door_param, 0xFE363C80,
    0x00000007, 0xFE363C81, 0xFE363C82, 0x00020019, 0xFE363C82, 0x00000000,
    0x0001005C, 0xFE363C82, 0x00000021, 0x0000006B, 0x0006005B, (u32)evt_door_param,
    0xFE363C80, 0x00000005, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00020035,
    0xFE363C85, 0x00000002, 0x0007005B, (u32)evt_cam3d_evt_set_xyz, 0xFE363C81, 0xFE363C85,
    0xFE363C83, 0x00000000, 0x00000096, 0x0000000B, 0x0000006D, 0x0001005C,
    (u32)door_light2, 0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87,
    0xFE363C88, 0xFE363C89, 0x00020019, 0xFE363C88, 0x00000000, 0x0000006B,
    0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88,
    0xFE363C89, 0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000005, 0x000000B4, 0x00000000,
    0x000001F4, 0x00010003, 0x00000003, 0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x0001005C,
    0xFE363C88, 0x00010009, 0x00000001, 0x00020018, 0xFE363C81, 0x00000001,
    0x00010004, 0x00000003, 0x00000021, 0x0000006D, 0x00000021, 0x0006005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89,
    0x00020019, 0xFE363C89, 0x00000000, 0x0000006B, 0x0006005B, (u32)evt_door_param,
    0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89, 0x00020032,
    0xFE363C86, 0x00000000, 0x0005005B, (u32)evt_sub_intpl_msec_init, 0x00000005, 0x000000B4,
    0x00000000, 0x000001F4, 0x00010003, 0x00000002, 0x0001005B, (u32)evt_sub_intpl_msec_get_value,
    0x00020032, 0xFE363C85, 0xFE363C81, 0x00020032, 0xFE363C81, 0x00000003,
    0x0001005C, 0xFE363C89, 0x00020032, 0xFE363C86, 0x00000001, 0x00010009,
    0x00000001, 0x00020018, 0xFE363C85, 0x00000001, 0x00010004, 0x00000002,
    0x00000021, 0x0000006D, 0x00000021, 0x0001000A, 0x000001F4, 0x0001005C,
    (u32)door_light, 0x0003005B, (u32)evt_cam3d_evt_xyz_off, 0x00000064, 0x0000000B, 0x0003005B,
    (u32)door_dark_flag, 0xFE363C80, 0x00000000, 0x0005005B, (u32)evt_door_param, 0xFE363C80,
    0x00000003, 0xFE363C81, 0xFE363C82, 0x00020019, 0xFE363C81, 0x00000000,
    0x0002001F, 0xFE363C8F, 0x00000001, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, 0xFE363C81, 0x00000001, 0x00000021, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000000, 0xFE363C81, 0x00000800, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000000, 0xFE363C81, 0x00001000, 0x00000021, 0x00020019,
    0xFE363C82, 0x00000000, 0x0004005B, (u32)evt_hitobj_onoff, 0xFE363C82, 0x00000001,
    0x00000000, 0x00000021, 0x0005005B, (u32)evt_door_param, 0xFE363C80, 0x00000002,
    0xFE363C81, 0xFE363C82, 0x00020019, 0xFE363C81, 0x00000000, 0x0005005B,
    (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, 0xFE363C81, 0x00000800, 0x00000021,
    0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000001, 0xFE363C81, 0xFE363C82,
    0xFE363C83, 0x00020019, 0xFE363C81, 0x00000000, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000000, 0xFE363C81, 0x00000800, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000000, 0xFE363C81, 0x00001000, 0x00000021, 0x0005005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000002, 0xFE363C81, 0xFE363C82, 0x00020019,
    0xFE363C81, 0x00000000, 0x0002005B, (u32)evt_door_load_mapflag, 0xFE363C81, 0x00000021,
    0x0002005B, (u32)evt_mario_key_onoff, 0x00000001, 0x0001005B, (u32)snd_door_out, 0x00000002,
    0x00000001,
};

u32 evt_door_open_quick[] = {
    0x0005005B, (u32)evt_door_param, 0xFE363C80, 0x00000007, 0xFE363C81, 0xFE363C82,
    0x00020019, 0xFE363C81, 0x00000000, 0x0001005C, 0xFE363C81, 0x00000021,
    0x0005005B, (u32)evt_door_param, 0xFE363C80, 0x00000002, 0xFE363C81, 0xFE363C82,
    0x00020019, 0xFE363C81, 0x00000000, 0x0002005B, (u32)evt_door_save_mapflag, 0xFE363C81,
    0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, 0xFE363C81, 0x00000002,
    0x00000021, 0x0000006B, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x0006005B,
    (u32)evt_door_param, 0xFE363C8A, 0x00000004, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C84, 0xFE363C85, 0xFE363C86,
    0x00020032, 0xFE363C81, 0xFE363C85, 0x0005005B, (u32)evt_cam3d_get_shift, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C83, 0x000B005B, (u32)evt_cam3d_event_from_road, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0xFE363C85, 0xFE363C86,
    0xFE363C87, 0xFE363C88, 0xFE363C89, 0x0009005B, (u32)evt_cam3d_evt_set, 0xFE363C84,
    0xFE363C85, 0xFE363C86, 0xFE363C87, 0xFE363C88, 0xFE363C89, 0x00000000,
    0x0000000B, 0x00010009, 0x00000001, 0x0003005B, (u32)evt_cam3d_evt_off, 0x00000000,
    0x0000000B, 0x0000006D, 0x0005005B, (u32)evt_door_param, 0xFE363C80, 0x00000002,
    0xFE363C81, 0xFE363C82, 0x00020019, 0xFE363C81, 0x00000000, 0x0005005B,
    (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, 0xFE363C81, 0x00000800, 0x00000021,
    0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000001, 0xFE363C81, 0xFE363C82,
    0xFE363C83, 0x00020019, 0xFE363C81, 0x00000000, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000001, 0xFE363C81, 0x00000800, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000001, 0xFE363C81, 0x00001000, 0x00000021, 0x0005005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000003, 0xFE363C81, 0xFE363C82, 0x00020019,
    0xFE363C81, 0x00000000, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000,
    0xFE363C81, 0x00000001, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001,
    0xFE363C81, 0x00000800, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001,
    0xFE363C81, 0x00001000, 0x00000021, 0x00020019, 0xFE363C82, 0x00000000,
    0x0004005B, (u32)evt_hitobj_onoff, 0xFE363C82, 0x00000001, 0x00000001, 0x00000021,
    0x0003005B, (u32)door_dark_flag, 0xFE363C80, 0x00000001, 0x0006005B, (u32)evt_map_set_blend,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0006005B,
    (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88, 0xFE363C89,
    0x00020019, 0xFE363C89, 0x00000000, 0x0000006B, 0x00020032, 0xFE363C80,
    0x000000B4, 0x0001005C, 0xFE363C89, 0x0000006D, 0x00000021, 0x0006005B,
    (u32)evt_map_set_blend, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000006, 0xFE363C87, 0xFE363C88,
    0xFE363C89, 0x00020019, 0xFE363C88, 0x00000000, 0x0000006B, 0x00020032,
    0xFE363C80, 0x000000B4, 0x0001005C, 0xFE363C88, 0x0000006D, 0x00000021,
    0x0001005B, (u32)snd_door_in, 0x00000002, 0x00000001,
};

u32 evt_door_entry[] = {
    0x0005005B, (u32)evt_door_param, 0xFE363C80, 0x00000003, 0xFE363C81, 0xFE363C82,
    0x00020019, 0xFE363C81, 0x00000000, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, 0xFE363C81, 0x00000001, 0x00000021, 0x00020019, 0xFE363C82,
    0x00000000, 0x0004005B, (u32)evt_hitobj_onoff, 0xFE363C82, 0x00000001, 0x00000000,
    0x00000021, 0x0006005B, (u32)evt_door_param, 0xFE363C80, 0x00000001, 0xFE363C81,
    0xFE363C82, 0xFE363C83, 0x00020019, 0xFE363C82, 0x00000000, 0x00020032,
    0xFE363C8F, 0x00000000, 0x0007005B, (u32)evt_run_case_evt, 0x00000009, 0x00000001,
    0xFE363C82, 0x00000000, (u32)evt_door_close, 0x00000000, 0x00000021, 0x00020019,
    0xFE363C83, 0x00000000, 0x00020032, 0xFE363C8F, 0x00000000, 0x0007005B,
    (u32)evt_run_case_evt, 0x00000009, 0x00000001, 0xFE363C83, 0x00000000, (u32)evt_door_open,
    0x00000000, 0x00000021, 0x0004005B, (u32)evt_door_param, 0xFE363C80, 0x00000009,
    0xFE363C81, 0x0002001E, 0xFE363C81, 0x00000001, 0x0003005B, (u32)npc_hide_onoff,
    0xFE363C80, 0x00000001, 0x00000021, 0x00000002, 0x00000001,
};

/* .sdata */
u32 door_up_se[] = {
    0x00000002, 0x00000001,
};

u32 house_close_se[] = {
    0x0001005E, (u32)house_open_se, 0x00000002, 0x00000001,
};
