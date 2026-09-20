#include "event/evt_movefloor.h"
#include "event/evt_cmd.h"

extern const f32 float_500_8042658c;
extern const f32 float_0_80426584;
extern const f32 float_neg1_80426588;
extern const f32 float_3_80426590;
extern const f32 float_5p625_80426594;
extern const f32 float_360_80426598;
extern const f32 float_1000_8042659c;
extern const f32 float_0p5_804265a0;
extern const f32 float_1_804265a4;
static f32 sintbl[450];

extern const char str_yuu_80426580[];
extern const char str_sjis_00000000[];
extern const char str_sjis_00000018[];
extern const char str_SFX_STG1_TIK_FLOOR_M_802fc2e8[];
extern void* mapalloc_base_ptr;
extern void* gp;

void* evtGetPtrID(s32 id);
void psndSFXOff(s32 id);
void _mapFree(void* heap, void* ptr);
void mapObjGetPos(s32 id, f32* pos);
void* mapGetMapObj(s32 id);
void mapErrorEntry(s32 type, s32 id);
s32 psndGetFlag(void);
s32 psndSFXOn_3D(s32 sfxId, f32* pos);

extern s32 evt_sub_get_areaname();
extern s32 evt_bero_exec_wait();
extern s32 evt_bero_get_now_number();
extern s32 evt_bero_get_info_nextarea();
extern s32 evt_hit_bind_mapobj();
extern s32 evt_mapobj_trans();
extern s32 evt_mapobj_rotate();
extern s32 evt_hit_bind_update();
extern s32 evt_mapobj_scale();
extern s32 evt_mario_get_name_hitobj_head();
extern s32 evt_mario_get_party();
extern s32 evt_party_get_name_hitobj_head();
extern s32 evt_mario_get_exparty();
extern s32 evt_mario_get_name_hitobj_ride();
extern s32 evt_mario_set_hosei_xyz();
extern s32 evt_party_get_name_hitobj_ride();
extern s32 evt_party_set_hosei_xyz();
extern s32 evt_snd_sfxoff();
extern s32 evt_mapobj_get_position();
extern s32 evt_snd_sfxon_3d();

s32 evt_moving_floor_alloc(void* event) {
    extern void* _mapAlloc(void*, s32);
    extern void* memset(void*, s32, s32);
    extern s32 evtSetValue(void*, s32, s32);
    u8* evt = event;
    s32* args = *(s32**)(evt + 0x18);
    s32* lw = (s32*)(evt + 0x9C);
    u32* floor = _mapAlloc(mapalloc_base_ptr, 0x88);

    memset(floor, 0, 0x88);
    floor[8] = lw[6];
    floor[1] = lw[12];
    switch (floor[8]) {
        case 0:
            floor[0] &= ~1;
            floor[10] = 0;
            *(f32*)&floor[25] = float_1_804265a4;
            *(f32*)&floor[11] = (f32)lw[2];
            *(f32*)&floor[14] = (f32)lw[3];
            floor[26] = lw[4];
            *(f32*)&floor[17] = *(f32*)&floor[11];
            *(f32*)&floor[32] = (f32)lw[11];
            break;
        case 1:
            floor[10] = 1;
            *(f32*)&floor[25] = float_1_804265a4;
            *(f32*)&floor[12] = (f32)lw[2];
            *(f32*)&floor[15] = (f32)lw[3];
            floor[27] = lw[4];
            *(f32*)&floor[18] = *(f32*)&floor[12];
            *(f32*)&floor[32] = (f32)lw[11];
            break;
        case 2:
            floor[10] = 2;
            *(f32*)&floor[25] = float_1_804265a4;
            *(f32*)&floor[13] = (f32)lw[2];
            *(f32*)&floor[16] = (f32)lw[3];
            floor[28] = lw[4];
            *(f32*)&floor[19] = *(f32*)&floor[13];
            *(f32*)&floor[32] = (f32)lw[11];
            break;
        case 3:
            floor[10] = 0;
            *(f32*)&floor[25] = float_1_804265a4;
            *(f32*)&floor[11] = (f32)lw[2];
            *(f32*)&floor[14] = (f32)lw[3];
            floor[26] = lw[4];
            *(f32*)&floor[17] = float_0p5_804265a0 * (*(f32*)&floor[11] + *(f32*)&floor[14]);
            *(f32*)&floor[13] = (f32)lw[8];
            *(f32*)&floor[16] = (f32)lw[9];
            floor[28] = lw[10];
            *(f32*)&floor[19] = float_0p5_804265a0 * (*(f32*)&floor[13] + *(f32*)&floor[16]);
            *(f32*)&floor[32] = (f32)lw[11];
            break;
        case 4:
            floor[10] = 2;
            *(f32*)&floor[25] = float_1_804265a4;
            *(f32*)&floor[11] = (f32)lw[2];
            *(f32*)&floor[14] = (f32)lw[3];
            floor[26] = lw[4];
            *(f32*)&floor[17] = *(f32*)&floor[11];
            *(f32*)&floor[13] = (f32)lw[8];
            *(f32*)&floor[16] = (f32)lw[9];
            floor[28] = lw[10];
            *(f32*)&floor[19] = *(f32*)&floor[13];
            *(f32*)&floor[32] = (f32)lw[11];
            break;
        case 5:
            floor[0] &= ~1;
            floor[10] = 0;
            *(f32*)&floor[25] = float_1_804265a4;
            *(f32*)&floor[11] = (f32)lw[2];
            *(f32*)&floor[14] = (f32)lw[3];
            floor[26] = lw[4];
            *(f32*)&floor[17] = *(f32*)&floor[11];
            *(f32*)&floor[32] = (f32)lw[11];
            break;
    }
    floor[29] = lw[5];
    floor[20] = floor[17];
    floor[21] = floor[18];
    floor[22] = floor[19];
    floor[24] = floor[23];
    if (floor[1] == 1) {
        s32 phase = (*(s32*)((u8*)*(void**)gp + 0x1C) + *(s32*)(evt + 0x15C) * 0x27) & 0x3F;
        *(f32*)&floor[23] = float_3_80426590 * sintbl[(s32)(float_5p625_80426594 * (f32)(u32)phase)];
    } else {
        *(f32*)&floor[23] = float_0_80426584;
    }
    evtSetValue(event, args[0], (s32)floor);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_moving_floor_free) {
    s32* args = event->args;
    void* floor = (void*)evtGetValue(event, args[0]);

    if (floor == NULL) {
        return 2;
    }
    _mapFree(mapalloc_base_ptr, floor);
    evtSetValue(event, args[0], 0);
    return 2;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


u8 evt_moving_floor_init(s32 event) {
    extern s32 evtGetValue(void*, s32);
    u8* evt = (u8*)event;
    s32* args = *(s32**)(evt + 0x18);
    u32* floor = (u32*)evtGetValue(evt, args[0]);
    s32 direction;
    f32 fraction;

    fraction = float_0p5_804265a0;
    switch ((s32)floor[8]) {
        case 0:
            direction = floor[9];
            fraction = float_0_80426584;
            floor[9]++;
            if ((s32)floor[9] >= 2) {
                floor[9] = 0;
            }
            break;
        case 1:
            direction = floor[9];
            fraction = float_0_80426584;
            floor[9]++;
            if ((s32)floor[9] >= 2) {
                floor[9] = 0;
            }
            break;
        case 2:
            direction = floor[9];
            fraction = float_0_80426584;
            floor[9]++;
            if ((s32)floor[9] >= 2) {
                floor[9] = 0;
            }
            break;
        case 3:
            switch (floor[9]) {
                case 0:
                    direction = 1; fraction = float_0p5_804265a0; floor[0] &= ~1; floor[10] = 0; *(f32*)&floor[25] = float_1_804265a4;
                    break;
                case 1:
                    direction = 0; fraction = float_0_80426584; floor[0] |= 1; floor[10] = 0; *(f32*)&floor[25] = float_0p5_804265a0;
                    break;
                case 2:
                    direction = 1; fraction = float_0p5_804265a0; floor[0] &= ~1; floor[10] = 2; *(f32*)&floor[25] = float_1_804265a4;
                    break;
                case 3:
                    direction = 0; fraction = float_0_80426584; floor[0] |= 1; floor[10] = 2; *(f32*)&floor[25] = float_0p5_804265a0;
                    break;
                case 4:
                    direction = 0; fraction = float_0p5_804265a0; floor[0] &= ~1; floor[10] = 0; *(f32*)&floor[25] = float_1_804265a4;
                    break;
                case 5:
                    direction = 1; fraction = float_0_80426584; floor[0] |= 1; floor[10] = 0; *(f32*)&floor[25] = float_0p5_804265a0;
                    break;
                case 6:
                    direction = 0; fraction = float_0p5_804265a0; floor[0] &= ~1; floor[10] = 2; *(f32*)&floor[25] = float_1_804265a4;
                    break;
                case 7:
                    direction = 1; fraction = float_0_80426584; floor[0] |= 1; floor[10] = 2; *(f32*)&floor[25] = float_0p5_804265a0;
                    break;
            }
            floor[9]++;
            if ((s32)floor[9] >= 8) {
                floor[9] = 0;
            }
            break;
        case 4:
            switch (floor[9]) {
                case 0:
                    floor[10] = 2; direction = 0; fraction = float_0_80426584; *(f32*)&floor[25] = float_1_804265a4;
                    break;
                case 1:
                    floor[10] = 0; direction = 0; fraction = float_0_80426584; *(f32*)&floor[25] = float_1_804265a4;
                    break;
                case 2:
                    floor[10] = 2; direction = 1; fraction = float_0_80426584; *(f32*)&floor[25] = float_1_804265a4;
                    break;
                case 3:
                    floor[10] = 0; direction = 1; fraction = float_0_80426584; *(f32*)&floor[25] = float_1_804265a4;
                    break;
            }
            floor[9]++;
            if ((s32)floor[9] >= 4) {
                floor[9] = 0;
            }
            break;
        case 5:
            direction = floor[9];
            fraction = float_1_804265a4;
            break;
    }

    if (direction != 0) {
        *(f32*)&floor[2] = (*(f32*)&floor[floor[10] + 11] - *(f32*)&floor[floor[10] + 14]) /
                           (f32)(s32)floor[floor[10] + 26];
    } else {
        *(f32*)&floor[2] = (*(f32*)&floor[floor[10] + 14] - *(f32*)&floor[floor[10] + 11]) /
                           (f32)(s32)floor[floor[10] + 26];
    }
    if (direction != 0) {
        *(f32*)&floor[3] = *(f32*)&floor[floor[10] + 14] * (float_1_804265a4 - fraction) +
                           *(f32*)&floor[floor[10] + 11] * fraction;
    } else {
        *(f32*)&floor[3] = *(f32*)&floor[floor[10] + 11] * (float_1_804265a4 - fraction) +
                           *(f32*)&floor[floor[10] + 14] * fraction;
    }
    *(f32*)&floor[4] = (f32)(s32)floor[floor[10] + 26] * fraction;
    return 2;
}

s32 evt_moving_floor_main(void* event) {
    s32* args = *(s32**)((s32)event + 0x18);
    void* floor = (void*)evtGetValue(event, args[0]);
    u32 ticks;
    u32 elapsed;
    f32 frame;
    s32 phase;
    s32 index;

    ticks = (*(u32*)0x800000F8 >> 2) / 1000;
    elapsed = ((*(u32*)((s32)event + 4) - *(u32*)((s32)floor + 0x1C)) << 3) / ticks;
    frame = (f32)(s32)elapsed;
    *(f32*)((s32)floor + 0xC) += (*(f32*)((s32)floor + 8) * frame) / float_1000_8042659c;
    *(f32*)((s32)floor + 0x10) += frame / float_1000_8042659c;
    *(f32*)((s32)floor + 0x50) = *(f32*)((s32)floor + 0x44);
    *(f32*)((s32)floor + 0x54) = *(f32*)((s32)floor + 0x48);
    *(f32*)((s32)floor + 0x58) = *(f32*)((s32)floor + 0x4C);
    *(f32*)((s32)floor + 0x60) = *(f32*)((s32)floor + 0x5C);
    if (*(s32*)((s32)floor + 4) == 1) {
        phase = (*(s32*)((s32)*(void**)gp + 0x1C) + *(s32*)((s32)event + 0x15C) * 0x27) & 0x3F;
        index = (s32)(float_5p625_80426594 * (f32)(u32)phase);
        *(f32*)((s32)floor + 0x5C) = float_3_80426590 * sintbl[index];
    } else {
        *(f32*)((s32)floor + 0x5C) = float_0_80426584;
    }
    *(f32*)((s32)floor + 0x44 + *(s32*)((s32)floor + 0x28) * 4) = *(f32*)((s32)floor + 0xC);
    evtSetFloat(event, args[1], *(f32*)((s32)floor + 0x44));
    evtSetFloat(event, args[2], *(f32*)((s32)floor + 0x48) + *(f32*)((s32)floor + 0x5C));
    evtSetFloat(event, args[3], *(f32*)((s32)floor + 0x4C));
    return 2;
}

USER_FUNC(evt_moving_floor_main2) {
    volatile s32* floor = (volatile s32*)evtGetValue(event, event->args[0]);
    s32 value0 = *(volatile s32*)event;
    s32 value4 = *(volatile s32*)((s32)event + 4);

    floor[7] = value4;
    floor[6] = value0;
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(moving_floor_get_end) {
    s32* args = event->args;
    void* floor = (void*)evtGetValue(event, args[0]);
    s32 index = *(s32*)((s32)floor + 0x28);
    s32 now = (s32)*(f32*)((s32)floor + 0x10);
    s32 end = (s32)(*(s32*)((s32)floor + 0x68 + index * 4) * *(f32*)((s32)floor + 0x64));

    if (now >= end) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(moving_floor_get_pause) {
    s32* args = event->args;
    void* floor = (void*)evtGetValue(event, args[0]);

    if (*(s32*)floor & 2) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(moving_floor_get_kind) {
    s32* args = event->args;
    void* floor = (void*)evtGetValue(event, args[0]);

    evtSetValue(event, args[1], *(s32*)((s32)floor + 4));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(moving_floor_get_mode) {
    s32* args = event->args;
    void* floor = (void*)evtGetValue(event, args[0]);

    evtSetValue(event, args[1], *(s32*)((s32)floor + 0x20));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(moving_floor_update_turn) {
    s32* args = event->args;
    void* floor = (void*)evtGetValue(event, args[0]);
    f32 zero;
    f32 delta;
    f32 angle;

    zero = float_0_80426584;
    angle = *(f32*)((s32)floor + 0x84);
    delta = *(f32*)((s32)floor + 0x80);
    angle += delta;
    *(f32*)((s32)floor + 0x84) = angle;
    angle = *(f32*)((s32)floor + 0x84);
    if (angle < zero) {
        *(f32*)((s32)floor + 0x84) = angle + float_360_80426598;
    }
    angle = *(f32*)((s32)floor + 0x84);
    if (angle >= float_360_80426598) {
        *(f32*)((s32)floor + 0x84) = angle - float_360_80426598;
    }
    evtSetFloat(event, args[1], *(f32*)((s32)floor + 0x84));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(moving_floor_getf_pos) {
    s32* args = event->args;
    void* floor = (void*)evtGetValue(event, args[0]);
    s32 kind;
    s32 time;
    s32 phase;
    s32 index;

    *(f32*)((s32)floor + 0x60) = *(f32*)((s32)floor + 0x5C);
    kind = *(s32*)((s32)floor + 4);
    if (kind == 1) {
        time = *(s32*)((s32)event + 0x15C);
        phase = (*(s32*)((s32)*(void**)gp + 0x1C) + time * 0x27) & 0x3F;
        index = (s32)(float_5p625_80426594 * (f32)(u32)phase);
        *(f32*)((s32)floor + 0x5C) = float_3_80426590 * sintbl[index];
    } else {
        *(f32*)((s32)floor + 0x5C) = float_0_80426584;
    }

    evtSetFloat(event, args[1], *(f32*)((s32)floor + 0x44));
    evtSetFloat(event, args[2], *(f32*)((s32)floor + 0x48) + *(f32*)((s32)floor + 0x5C));
    evtSetFloat(event, args[3], *(f32*)((s32)floor + 0x4C));
    return 2;
}

#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(moving_floor_getf_now_bpoint) {
    s32* args = event->args;
    void* floor = (void*)evtGetValue(event, args[0]);

    evtSetFloat(event, args[1], *(f32*)((s32)floor + 0x44) - *(f32*)((s32)floor + 0x50));
    evtSetFloat(event, args[2],
                (*(f32*)((s32)floor + 0x48) + *(f32*)((s32)floor + 0x5C)) -
                    (*(f32*)((s32)floor + 0x54) + *(f32*)((s32)floor + 0x60)));
    evtSetFloat(event, args[3], *(f32*)((s32)floor + 0x4C) - *(f32*)((s32)floor + 0x58));
    return 2;
}

#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(moving_floor_get_stmsec) {
    s32* args = event->args;
    void* floor = (void*)evtGetValue(event, args[0]);

    if (*(s32*)floor & 1) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], (*(s32*)((s32)floor + 0x74) * 60) / 1000);
    }
    return 2;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(moving_floor_getf_xz_scale) {
    s32* args = event->args;
    void* floor = (void*)evtGetValue(event, args[0]);
    s32 value = *(f32*)((s32)floor + 0x48) + *(f32*)((s32)floor + 0x5C);
    f32 scale = evtGetFloat(event, value);

    evtSetFloat(event, args[1], (float_500_8042658c - scale) / float_500_8042658c);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(moving_floor_reverse) {
    void* floor = (void*)evtGetValue(event, event->args[0]);
    f32 speed = *(f32*)((s32)floor + 8);
    f32 distance;
    s32 index;

    if (speed >= float_0_80426584) {
        return 2;
    }
    *(f32*)((s32)floor + 8) = speed * float_neg1_80426588;
    index = *(s32*)((s32)floor + 0x28);
    distance = *(s32*)((s32)floor + 0x68 + index * 4);
    distance *= *(f32*)((s32)floor + 0x64);
    *(f32*)((s32)floor + 0x10) = distance - *(f32*)((s32)floor + 0x10);
    *(s32*)((s32)floor + 0x24) ^= 1;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(moving_floor_set_se) {
    s32* args = event->args;
    void* floor = (void*)evtGetValue(event, args[0]);
    s32 objId = evtGetValue(event, args[1]);
    f32 pos[3];
    void* obj;
    s32 flags;

    if (*(u32*)((s32)floor + 0x78) != 0) {
        return 2;
    }
    if (*(u32*)((s32)floor + 0x7C) == 0) {
        mapObjGetPos(objId, pos);
        obj = mapGetMapObj(objId);
        if (obj == NULL) {
            mapErrorEntry(0, objId);
            return 2;
        }
        flags = 0;
        if ((psndGetFlag() & 0x100) && !(*(s32*)obj & 0x1800)) {
            flags = 0x10000;
        }
        *(s32*)((s32)floor + 0x7C) = psndSFXOn_3D(flags | 0x424, pos);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
USER_FUNC(moving_floor_stop_se) {
    void* floor = (void*)evtGetValue(event, event->args[0]);

    if (*(u32*)((s32)floor + 0x78) == 0) {
        u32 handle = *(u32*)((s32)floor + 0x7C);
        if (handle != 0) {
            psndSFXOff(handle);
            *(s32*)((s32)floor + 0x7C) = 0;
        }
    }
    return 2;
}

USER_FUNC(moving_floor_halt_se) {
    void* floor = (void*)evtGetValue(event, event->args[0]);
    *(s32*)((s32)floor + 0x78) = 1;
    return 2;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_moving_floor_delete) {
    s32 id = evtGetValue(event, event->args[0]);
    void* evt = evtGetPtrID(id);
    void* floor = *(void**)((s32)evt + 0xC0);

    if (*(u32*)((s32)floor + 0x78) == 0) {
        u32 handle = *(u32*)((s32)floor + 0x7C);
        if (handle != 0) {
            psndSFXOff(handle);
            *(s32*)((s32)floor + 0x7C) = 0;
        }
    }
    if (floor != NULL) {
        _mapFree(mapalloc_base_ptr, floor);
        *(void**)((s32)evt + 0xC0) = NULL;
    }
    if (evtCheckID(id)) {
        evtDeleteID(id);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


static f32 sintbl[450] = {
    0.000000f, 0.017452f, 0.034899f, 0.052336f, 0.069756f, 0.087156f, 0.104528f, 0.121869f,
    0.139173f, 0.156434f, 0.173648f, 0.190809f, 0.207912f, 0.224951f, 0.241922f, 0.258819f,
    0.275637f, 0.292372f, 0.309017f, 0.325568f, 0.342020f, 0.358368f, 0.374607f, 0.390731f,
    0.406737f, 0.422618f, 0.438371f, 0.453990f, 0.469471f, 0.484810f, 0.500000f, 0.515038f,
    0.529919f, 0.544639f, 0.559193f, 0.573576f, 0.587785f, 0.601815f, 0.615661f, 0.629320f,
    0.642788f, 0.656059f, 0.669131f, 0.681998f, 0.694658f, 0.707107f, 0.719340f, 0.731354f,
    0.743145f, 0.754709f, 0.766044f, 0.777146f, 0.788011f, 0.798635f, 0.809017f, 0.819152f,
    0.829037f, 0.838670f, 0.848048f, 0.857167f, 0.866025f, 0.874620f, 0.882947f, 0.891006f,
    0.898794f, 0.906308f, 0.913545f, 0.920505f, 0.927184f, 0.933580f, 0.939693f, 0.945518f,
    0.951056f, 0.956305f, 0.961262f, 0.965926f, 0.970296f, 0.974370f, 0.978148f, 0.981627f,
    0.984808f, 0.987688f, 0.990268f, 0.992546f, 0.994522f, 0.996195f, 0.997564f, 0.998630f,
    0.999391f, 0.999848f, 1.000000f, 0.999848f, 0.999391f, 0.998630f, 0.997564f, 0.996195f,
    0.994522f, 0.992546f, 0.990268f, 0.987688f, 0.984808f, 0.981627f, 0.978148f, 0.974370f,
    0.970296f, 0.965926f, 0.961262f, 0.956305f, 0.951057f, 0.945519f, 0.939693f, 0.933581f,
    0.927184f, 0.920505f, 0.913546f, 0.906308f, 0.898794f, 0.891007f, 0.882948f, 0.874620f,
    0.866026f, 0.857168f, 0.848048f, 0.838671f, 0.829038f, 0.819152f, 0.809017f, 0.798636f,
    0.788011f, 0.777146f, 0.766045f, 0.754710f, 0.743145f, 0.731354f, 0.719340f, 0.707107f,
    0.694659f, 0.681999f, 0.669131f, 0.656059f, 0.642788f, 0.629321f, 0.615662f, 0.601815f,
    0.587786f, 0.573577f, 0.559193f, 0.544639f, 0.529920f, 0.515039f, 0.500000f, 0.484810f,
    0.469472f, 0.453991f, 0.438372f, 0.422619f, 0.406737f, 0.390732f, 0.374607f, 0.358369f,
    0.342021f, 0.325569f, 0.309017f, 0.292372f, 0.275638f, 0.258820f, 0.241923f, 0.224952f,
    0.207912f, 0.190809f, 0.173649f, 0.156435f, 0.139174f, 0.121870f, 0.104529f, 0.087156f,
    0.069757f, 0.052337f, 0.034900f, 0.017453f, 0.000001f, -0.017452f, -0.034899f, -0.052335f,
    -0.069756f, -0.087155f, -0.104528f, -0.121869f, -0.139173f, -0.156434f, -0.173648f, -0.190808f,
    -0.207911f, -0.224950f, -0.241921f, -0.258819f, -0.275637f, -0.292371f, -0.309016f, -0.325568f,
    -0.342019f, -0.358367f, -0.374606f, -0.390730f, -0.406736f, -0.422618f, -0.438371f, -0.453990f,
    -0.469471f, -0.484809f, -0.499999f, -0.515037f, -0.529919f, -0.544639f, -0.559192f, -0.573576f,
    -0.587785f, -0.601814f, -0.615661f, -0.629320f, -0.642787f, -0.656058f, -0.669130f, -0.681998f,
    -0.694658f, -0.707106f, -0.719339f, -0.731353f, -0.743144f, -0.754709f, -0.766044f, -0.777145f,
    -0.788010f, -0.798635f, -0.809016f, -0.819152f, -0.829037f, -0.838670f, -0.848048f, -0.857167f,
    -0.866025f, -0.874619f, -0.882947f, -0.891006f, -0.898794f, -0.906307f, -0.913545f, -0.920504f,
    -0.927183f, -0.933580f, -0.939692f, -0.945518f, -0.951056f, -0.956304f, -0.961261f, -0.965926f,
    -0.970295f, -0.974370f, -0.978147f, -0.981627f, -0.984808f, -0.987688f, -0.990268f, -0.992546f,
    -0.994522f, -0.996195f, -0.997564f, -0.998629f, -0.999391f, -0.999848f, -1.000000f, -0.999848f,
    -0.999391f, -0.998630f, -0.997564f, -0.996195f, -0.994522f, -0.992546f, -0.990268f, -0.987688f,
    -0.984808f, -0.981627f, -0.978148f, -0.974370f, -0.970296f, -0.965926f, -0.961262f, -0.956305f,
    -0.951057f, -0.945519f, -0.939693f, -0.933581f, -0.927184f, -0.920505f, -0.913546f, -0.906308f,
    -0.898795f, -0.891007f, -0.882948f, -0.874620f, -0.866026f, -0.857168f, -0.848049f, -0.838671f,
    -0.829038f, -0.819153f, -0.809018f, -0.798636f, -0.788011f, -0.777147f, -0.766045f, -0.754710f,
    -0.743146f, -0.731354f, -0.719341f, -0.707108f, -0.694659f, -0.681999f, -0.669132f, -0.656060f,
    -0.642788f, -0.629321f, -0.615662f, -0.601816f, -0.587786f, -0.573577f, -0.559194f, -0.544640f,
    -0.529920f, -0.515039f, -0.500001f, -0.484811f, -0.469473f, -0.453991f, -0.438372f, -0.422619f,
    -0.406737f, -0.390732f, -0.374608f, -0.358369f, -0.342022f, -0.325569f, -0.309018f, -0.292373f,
    -0.275638f, -0.258820f, -0.241923f, -0.224952f, -0.207913f, -0.190810f, -0.173649f, -0.156435f,
    -0.139175f, -0.121871f, -0.104530f, -0.087157f, -0.069758f, -0.052337f, -0.034901f, -0.017454f,
    -0.000001f, 0.017451f, 0.034899f, 0.052335f, 0.069755f, 0.087154f, 0.104527f, 0.121868f,
    0.139172f, 0.156433f, 0.173647f, 0.190808f, 0.207910f, 0.224950f, 0.241921f, 0.258818f,
    0.275636f, 0.292371f, 0.309015f, 0.325567f, 0.342019f, 0.358367f, 0.374605f, 0.390730f,
    0.406736f, 0.422617f, 0.438370f, 0.453990f, 0.469471f, 0.484809f, 0.499999f, 0.515037f,
    0.529918f, 0.544638f, 0.559191f, 0.573575f, 0.587784f, 0.601814f, 0.615661f, 0.629319f,
    0.642787f, 0.656058f, 0.669130f, 0.681997f, 0.694657f, 0.707105f, 0.719339f, 0.731353f,
    0.743144f, 0.754709f, 0.766044f, 0.777145f, 0.788010f, 0.798635f, 0.809016f, 0.819151f,
    0.829037f, 0.838670f, 0.848047f, 0.857166f, 0.866025f, 0.874619f, 0.882947f, 0.891006f,
    0.898793f, 0.906307f, 0.913545f, 0.920504f, 0.927183f, 0.933580f, 0.939692f, 0.945518f,
    0.951056f, 0.956304f, 0.961261f, 0.965925f, 0.970295f, 0.974370f, 0.978147f, 0.981627f,
    0.984808f, 0.987688f, 0.990268f, 0.992546f, 0.994522f, 0.996195f, 0.997564f, 0.998629f,
    0.999391f, 0.999848f,
};

u32 moving_floor_bero_check[42] = {
    0x0002005B, (u32)evt_sub_get_areaname, 0xFE363C80, 0x0002000C,
    0xFE363C80, (u32)(char*)str_yuu_80426580, 0x00000002, 0x00000021,
    0x00010005, 0x00000000, 0x0002005B, (u32)evt_bero_exec_wait,
    0x00010000, 0x00010071, (u32)(char*)str_sjis_00000000, 0x0002005B,
    (u32)evt_bero_exec_wait, 0x00000001, 0x0002005B, (u32)evt_bero_get_now_number,
    0xFE363C8A, 0x0004005B, (u32)evt_bero_get_info_nextarea, 0xFE363C8A,
    0xFE363C83, 0xFE363C84, 0x00020019, 0xFE363C83,
    0x00000000, 0x00000007, 0x00000021, 0x00000006,
    0x00010071, (u32)(char*)str_sjis_00000018, 0x0002005B, (u32)moving_floor_stop_se,
    0xFE363C89, 0x0002005B, (u32)moving_floor_halt_se, 0xFE363C89,
    0x00000002, 0x00000001,
};

u32 moving_floor[500] = {
    0x0002005B, (u32)evt_moving_floor_alloc, 0xFE363C89, 0x0003005B,
    (u32)evt_hit_bind_mapobj, 0xFE363C81, 0xFE363C80, 0x0001005C,
    (u32)moving_floor_bero_check, 0x00010005, 0x00000000, 0x0002005B,
    (u32)evt_moving_floor_init, 0xFE363C89, 0x0002005B, (u32)evt_moving_floor_main2,
    0xFE363C89, 0x0003005B, (u32)moving_floor_get_mode, 0xFE363C89,
    0xFE363C88, 0x00020018, 0xFE363C88, 0x00000005,
    0x00010004, 0x00000000, 0x00000021, 0x00010005,
    0x00000000, 0x00010005, 0x00000000, 0x0003005B,
    (u32)moving_floor_get_pause, 0xFE363C89, 0xFE363C88, 0x00020018,
    0xFE363C88, 0x00000000, 0x00000007, 0x00000021,
    0x00010009, 0x00000001, 0x0005005B, (u32)moving_floor_getf_pos,
    0xFE363C89, 0xFE363C8C, 0xFE363C8D, 0xFE363C8E,
    0x0002005B, (u32)evt_moving_floor_main2, 0xFE363C89, 0x0006005B,
    (u32)evt_mapobj_trans, 0x00000001, 0xFE363C80, 0xFE363C8C,
    0xFE363C8D, 0xFE363C8E, 0x0003005B, (u32)moving_floor_update_turn,
    0xFE363C89, 0xFE363C88, 0x0006005B, (u32)evt_mapobj_rotate,
    0x00000001, 0xFE363C80, 0xF24A7A80, 0xFE363C88,
    0xF24A7A80, 0x0002005B, (u32)evt_hit_bind_update, 0xFE363C81,
    0x0002000D, 0xFE363C87, 0x00000000, 0x0006005B,
    (u32)evt_mapobj_trans, 0x00000001, 0xFE363C87, 0xFE363C8C,
    0x00000000, 0xFE363C8E, 0x0006005B, (u32)evt_mapobj_rotate,
    0x00000001, 0xFE363C87, 0xF24A7A80, 0xFE363C88,
    0xF24A7A80, 0x0003005B, (u32)moving_floor_getf_xz_scale, 0xFE363C89,
    0xFE363C88, 0x0006005B, (u32)evt_mapobj_scale, 0x00000001,
    0xFE363C87, 0xFE363C88, 0x00000001, 0xFE363C88,
    0x00000021, 0x00000006, 0x00010009, 0x00000001,
    0x0005005B, (u32)evt_moving_floor_main, 0xFE363C89, 0xFE363C8C,
    0xFE363C8D, 0xFE363C8E, 0x0002005B, (u32)evt_moving_floor_main2,
    0xFE363C89, 0x0006005B, (u32)evt_mapobj_trans, 0x00000001,
    0xFE363C80, 0xFE363C8C, 0xFE363C8D, 0xFE363C8E,
    0x0003005B, (u32)moving_floor_update_turn, 0xFE363C89, 0xFE363C88,
    0x0006005B, (u32)evt_mapobj_rotate, 0x00000001, 0xFE363C80,
    0xF24A7A80, 0xFE363C88, 0xF24A7A80, 0x0002005B,
    (u32)evt_hit_bind_update, 0xFE363C81, 0x0002000D, 0xFE363C87,
    0x00000000, 0x0006005B, (u32)evt_mapobj_trans, 0x00000001,
    0xFE363C87, 0xFE363C8C, 0x00000000, 0xFE363C8E,
    0x0006005B, (u32)evt_mapobj_rotate, 0x00000001, 0xFE363C87,
    0xF24A7A80, 0xFE363C88, 0xF24A7A80, 0x0003005B,
    (u32)moving_floor_getf_xz_scale, 0xFE363C89, 0xFE363C88, 0x0006005B,
    (u32)evt_mapobj_scale, 0x00000001, 0xFE363C87, 0xFE363C88,
    0x00000001, 0xFE363C88, 0x00000021, 0x0002005B,
    (u32)evt_mario_get_name_hitobj_head, 0xFE363C82, 0x0002005B, (u32)evt_mario_get_party,
    0xFE363C8A, 0x00020032, 0xFE363C83, 0x00000000,
    0x00020019, 0xFE363C8A, 0x00000000, 0x0003005B,
    (u32)evt_party_get_name_hitobj_head, 0x00000000, 0xFE363C83, 0x00000021,
    0x0002005B, (u32)evt_mario_get_exparty, 0xFE363C8A, 0x00020032,
    0xFE363C84, 0x00000000, 0x00020019, 0xFE363C8A,
    0x00000000, 0x0003005B, (u32)evt_party_get_name_hitobj_head, 0x00000001,
    0xFE363C84, 0x00000021, 0x00020032, 0xFBD3E280,
    0x00000000, 0x00020018, 0xFE363C86, 0x00000001,
    0x0002000C, 0xFE363C82, 0xFE363C81, 0x00020032,
    0xFBD3E280, 0x00000001, 0x00000021, 0x0002000C,
    0xFE363C83, 0xFE363C81, 0x00020032, 0xFBD3E280,
    0x00000001, 0x00000021, 0x0002000C, 0xFE363C84,
    0xFE363C81, 0x00020032, 0xFBD3E280, 0x00000001,
    0x00000021, 0x00000021, 0x00020018, 0xFBD3E280,
    0x00000001, 0x0002005B, (u32)moving_floor_reverse, 0xFE363C89,
    0x00000021, 0x0003005B, (u32)moving_floor_get_kind, 0xFE363C89,
    0xFE363C88, 0x00020018, 0xFE363C88, 0x00000000,
    0x0005005B, (u32)moving_floor_getf_now_bpoint, 0xFE363C89, 0xFE363C8C,
    0xFE363C8D, 0xFE363C8E, 0x0002005B, (u32)evt_mario_get_name_hitobj_ride,
    0xFE363C88, 0x0002000C, 0xFE363C88, 0xFE363C81,
    0x0004005B, (u32)evt_mario_set_hosei_xyz, 0xFE363C8C, 0xFE363C8D,
    0xFE363C8E, 0x00000021, 0x0003005B, (u32)evt_party_get_name_hitobj_ride,
    0x00000000, 0xFE363C88, 0x0002000C, 0xFE363C88,
    0xFE363C81, 0x0005005B, (u32)evt_party_set_hosei_xyz, 0x00000000,
    0xFE363C8C, 0xFE363C8D, 0xFE363C8E, 0x00000021,
    0x0003005B, (u32)evt_party_get_name_hitobj_ride, 0x00000001, 0xFE363C88,
    0x0002000C, 0xFE363C88, 0xFE363C81, 0x0005005B,
    (u32)evt_party_set_hosei_xyz, 0x00000001, 0xFE363C8C, 0xFE363C8D,
    0xFE363C8E, 0x00000021, 0x00000021, 0x0003005B,
    (u32)moving_floor_get_end, 0xFE363C89, 0xFE363C88, 0x00020019,
    0xFE363C88, 0x00000000, 0x00000007, 0x00000021,
    0x0003005B, (u32)moving_floor_get_kind, 0xFE363C89, 0xFE363C88,
    0x00020018, 0xFE363C88, 0x00000000, 0x00020032,
    0xFBD3E280, 0x00000001, 0x00020012, 0xFE363C8C,
    0xF24A7A80, 0x00020012, 0xFE363C8D, 0xF24A7A80,
    0x00020012, 0xFE363C8E, 0xF24A7A80, 0x00020032,
    0xFBD3E280, 0x00000000, 0x00000021, 0x00000021,
    0x00000021, 0x00020018, 0xFBD3E280, 0x00000000,
    0x0002005B, (u32)moving_floor_stop_se, 0xFE363C89, 0x00000020,
    0x0003005B, (u32)moving_floor_set_se, 0xFE363C89, 0xFE363C80,
    0x00000021, 0x00000021, 0x00000006, 0x0002005B,
    (u32)moving_floor_stop_se, 0xFE363C89, 0x00010003, 0x00000000,
    0x0003005B, (u32)moving_floor_get_stmsec, 0xFE363C89, 0xFE363C8A,
    0x00010005, 0xFE363C8A, 0x0003005B, (u32)moving_floor_get_mode,
    0xFE363C89, 0xFE363C88, 0x00020018, 0xFE363C88,
    0x00000005, 0x00020032, 0xFE363C8A, 0x00000000,
    0x00000020, 0x00020018, 0xFE363C8A, 0x00000000,
    0x00000007, 0x00000021, 0x00000021, 0x00010005,
    0x00000000, 0x0003005B, (u32)moving_floor_get_pause, 0xFE363C89,
    0xFE363C88, 0x00020018, 0xFE363C88, 0x00000000,
    0x00000007, 0x00000021, 0x00010009, 0x00000001,
    0x0005005B, (u32)moving_floor_getf_pos, 0xFE363C89, 0xFE363C8C,
    0xFE363C8D, 0xFE363C8E, 0x0002005B, (u32)evt_moving_floor_main2,
    0xFE363C89, 0x0006005B, (u32)evt_mapobj_trans, 0x00000001,
    0xFE363C80, 0xFE363C8C, 0xFE363C8D, 0xFE363C8E,
    0x0003005B, (u32)moving_floor_update_turn, 0xFE363C89, 0xFE363C88,
    0x0006005B, (u32)evt_mapobj_rotate, 0x00000001, 0xFE363C80,
    0xF24A7A80, 0xFE363C88, 0xF24A7A80, 0x0002005B,
    (u32)evt_hit_bind_update, 0xFE363C81, 0x0002000D, 0xFE363C87,
    0x00000000, 0x0006005B, (u32)evt_mapobj_trans, 0x00000001,
    0xFE363C87, 0xFE363C8C, 0x00000000, 0xFE363C8E,
    0x0006005B, (u32)evt_mapobj_rotate, 0x00000001, 0xFE363C87,
    0xF24A7A80, 0xFE363C88, 0xF24A7A80, 0x0003005B,
    (u32)moving_floor_getf_xz_scale, 0xFE363C89, 0xFE363C88, 0x0006005B,
    (u32)evt_mapobj_scale, 0x00000001, 0xFE363C87, 0xFE363C88,
    0x00000001, 0xFE363C88, 0x00000021, 0x00000006,
    0x00010009, 0x00000001, 0x0005005B, (u32)moving_floor_getf_pos,
    0xFE363C89, 0xFE363C8C, 0xFE363C8D, 0xFE363C8E,
    0x0002005B, (u32)evt_moving_floor_main2, 0xFE363C89, 0x0006005B,
    (u32)evt_mapobj_trans, 0x00000001, 0xFE363C80, 0xFE363C8C,
    0xFE363C8D, 0xFE363C8E, 0x0003005B, (u32)moving_floor_update_turn,
    0xFE363C89, 0xFE363C88, 0x0006005B, (u32)evt_mapobj_rotate,
    0x00000001, 0xFE363C80, 0xF24A7A80, 0xFE363C88,
    0xF24A7A80, 0x0002005B, (u32)evt_hit_bind_update, 0xFE363C81,
    0x0002000D, 0xFE363C87, 0x00000000, 0x0006005B,
    (u32)evt_mapobj_trans, 0x00000001, 0xFE363C87, 0xFE363C8C,
    0x00000000, 0xFE363C8E, 0x0006005B, (u32)evt_mapobj_rotate,
    0x00000001, 0xFE363C87, 0xF24A7A80, 0xFE363C88,
    0xF24A7A80, 0x0003005B, (u32)moving_floor_getf_xz_scale, 0xFE363C89,
    0xFE363C88, 0x0006005B, (u32)evt_mapobj_scale, 0x00000001,
    0xFE363C87, 0xFE363C88, 0x00000001, 0xFE363C88,
    0x00000021, 0x00000006, 0x00000006, 0x0002005B,
    (u32)evt_moving_floor_free, 0xFE363C89, 0x00000002, 0x00000001,
};

u32 ridedown_floor[273] = {
    0x0003005B, (u32)evt_hit_bind_mapobj, 0xFE363C81, 0xFE363C80,
    0x00020032, 0xFE363C8A, 0xFE363C82, 0x00020036,
    0xFE363C8A, 0xFE363C83, 0x00020059, 0xFE363C86,
    0xFE363C8E, 0x00020059, 0xFE363C86, 0xFE363C8F,
    0x00020032, 0xFE363C89, 0x00000000, 0x00010005,
    0x00000000, 0x00020018, 0xFE363C85, 0x00000000,
    0x00020059, 0xFE363C87, 0xFE363C8C, 0x0002004D,
    0xFE363C8C, 0x0000FFFE, 0x00020057, 0xFE363C87,
    0xFE363C8C, 0x00000020, 0x00020059, 0xFE363C87,
    0xFE363C8C, 0x0002004D, 0xFE363C8C, 0x0000FFFD,
    0x00020057, 0xFE363C87, 0xFE363C8C, 0x00000021,
    0x0002005B, (u32)evt_mario_get_name_hitobj_ride, 0xFE363C8B, 0x0002000C,
    0xFE363C8B, 0xFE363C81, 0x00020018, 0xFE363C85,
    0x00000000, 0x00020059, 0xFE363C86, 0xFE363C8B,
    0x00020036, 0xFE363C8B, 0xFE363C84, 0x0002001C,
    0xFE363C8B, 0x00000000, 0x00020032, 0xFE363C8B,
    0x00000000, 0x00000021, 0x00020057, 0xFE363C86,
    0xFE363C8B, 0x00020059, 0xFE363C87, 0xFE363C8C,
    0x0002004F, 0xFE363C8C, 0x00000001, 0x00020057,
    0xFE363C87, 0xFE363C8C, 0x00000020, 0x00020059,
    0xFE363C86, 0xFE363C8B, 0x00020035, 0xFE363C8B,
    0xFE363C84, 0x0002001D, 0xFE363C8B, 0x00002710,
    0x00020032, 0xFE363C8B, 0x00002710, 0x00000021,
    0x00020057, 0xFE363C86, 0xFE363C8B, 0x00020059,
    0xFE363C87, 0xFE363C8C, 0x0002004F, 0xFE363C8C,
    0x00000002, 0x00020057, 0xFE363C87, 0xFE363C8C,
    0x00000021, 0x00000021, 0x00020059, 0xFE363C87,
    0xFE363C8C, 0x00020018, 0xFE363C8C, 0x00000000,
    0x00020018, 0xFE363C85, 0x00000000, 0x00020059,
    0xFE363C86, 0xFE363C8B, 0x0002001D, 0xFE363C8B,
    0xFE363C8F, 0x00020036, 0xFE363C8B, 0xFE363C84,
    0x0002001C, 0xFE363C8B, 0xFE363C8F, 0x00020032,
    0xFE363C8B, 0xFE363C8F, 0x00000021, 0x00000020,
    0x00020035, 0xFE363C8B, 0xFE363C84, 0x0002001D,
    0xFE363C8B, 0xFE363C8F, 0x00020032, 0xFE363C8B,
    0xFE363C8F, 0x00000021, 0x00000021, 0x00020057,
    0xFE363C86, 0xFE363C8B, 0x00000021, 0x00000021,
    0x00020059, 0xFE363C86, 0xFE363C8B, 0x00020037,
    0xFE363C8B, 0xFE363C8A, 0x0002003C, 0xFE363C8B,
    0xF24A7A8A, 0x0002003C, 0xFE363C8B, 0xF24A7A8A,
    0x00020018, 0xFE363C85, 0x00000000, 0x00020034,
    0xFE363C8C, 0xFE363C83, 0x0002003A, 0xFE363C8C,
    0xFE363C8B, 0x00000020, 0x00020034, 0xFE363C8C,
    0xFE363C82, 0x0002003B, 0xFE363C8C, 0xFE363C8B,
    0x00000021, 0x0006005B, (u32)evt_mapobj_trans, 0x00000001,
    0xFE363C80, 0x00000000, 0xFE363C8C, 0x00000000,
    0x0002005B, (u32)evt_hit_bind_update, 0xFE363C81, 0x00020034,
    0xFE363C8D, 0xFE363C8C, 0x0002003B, 0xFE363C8D,
    0xFE363C8E, 0x00020034, 0xFE363C8E, 0xFE363C8C,
    0x0002005B, (u32)evt_mario_get_name_hitobj_ride, 0xFE363C8B, 0x0002000C,
    0xFE363C8B, 0xFE363C81, 0x0004005B, (u32)evt_mario_set_hosei_xyz,
    0x00000000, 0xFE363C8D, 0x00000000, 0x00000021,
    0x0003005B, (u32)evt_party_get_name_hitobj_ride, 0x00000000, 0xFE363C8B,
    0x0002000C, 0xFE363C8B, 0xFE363C81, 0x0005005B,
    (u32)evt_party_set_hosei_xyz, 0x00000000, 0x00000000, 0xFE363C8D,
    0x00000000, 0x00000021, 0x0003005B, (u32)evt_party_get_name_hitobj_ride,
    0x00000001, 0xFE363C8B, 0x0002000C, 0xFE363C8B,
    0xFE363C81, 0x0005005B, (u32)evt_party_set_hosei_xyz, 0x00000001,
    0x00000000, 0xFE363C8D, 0x00000000, 0x00000021,
    0x00020012, 0xFE363C8D, 0xF24A7A80, 0x00020019,
    0xFE363C89, 0x00000000, 0x0002005B, (u32)evt_snd_sfxoff,
    0xFE363C89, 0x00020032, 0xFE363C89, 0x00000000,
    0x00000021, 0x00000020, 0x00020018, 0xFE363C89,
    0x00000000, 0x0005005B, (u32)evt_mapobj_get_position, 0xFE363C80,
    0xFE363C8B, 0xFE363C8C, 0xFE363C8D, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_STG1_TIK_FLOOR_M_802fc2e8, 0xFE363C8B, 0xFE363C8C,
    0xFE363C8D, 0xFE363C89, 0x00000021, 0x00000021,
    0x00010009, 0x00000001, 0x00000006, 0x00000002,
    0x00000001,
};

const char str_sjis_00000000[21] = {
    (char)0x8E, (char)0xA9, (char)0x93, (char)0xAE, (char)0x8D, (char)0xED, (char)0x8F, (char)0x9C, (char)0x83, 0x60,
    (char)0x83, 0x46, (char)0x83, 0x62, (char)0x83, 0x4E, (char)0x91, (char)0xD2, (char)0x82, (char)0xBF,
    0,
};

const char str_sjis_00000018[13] = {
    (char)0x8E, (char)0xA9, (char)0x93, (char)0xAE, (char)0x8D, (char)0xED, (char)0x8F, (char)0x9C, (char)0x8A, (char)0xAE,
    (char)0x97, (char)0xB9, 0,
};

const char str_SFX_STG1_TIK_FLOOR_M_802fc2e8[25] = "SFX_STG1_TIK_FLOOR_MOVE1";

const char str_yuu_80426580[4] = "yuu";
const f32 float_0_80426584 = 0.0f;
const f32 float_neg1_80426588 = -1.0f;
const f32 float_500_8042658c = 500.0f;
const f32 float_3_80426590 = 3.0f;
const f32 float_5p625_80426594 = 5.625f;
const f32 float_360_80426598 = 360.0f;
const f32 float_1000_8042659c = 1000.0f;
const f32 float_0p5_804265a0 = 0.5f;
const f32 float_1_804265a4 = 1.0f;
