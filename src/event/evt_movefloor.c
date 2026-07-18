#include "event/evt_movefloor.h"
#include "event/evt_cmd.h"

extern f32 float_500_8042658c;
extern f32 float_0_80426584;
extern f32 float_neg1_80426588;
extern f32 float_3_80426590;
extern f32 float_5p625_80426594;
extern f32 float_360_80426598;
extern f32 sintbl[];
extern void* mapalloc_base_ptr;
extern void* gp;

void* evtGetPtrID(s32 id);
void evtDeleteID(s32 id);
void psndSFXOff(s32 id);
void _mapFree(void* heap, void* ptr);
void mapObjGetPos(s32 id, f32* pos);
void* mapGetMapObj(s32 id);
void mapErrorEntry(s32 type, s32 id);
s32 psndGetFlag(void);
s32 psndSFXOn_3D(s32 sfxId, f32* pos);

USER_FUNC(moving_floor_halt_se) {
    void* floor = (void*)evtGetValue(event, event->args[0]);
    *(s32*)((s32)floor + 0x78) = 1;
    return 2;
}

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


s32 evt_moving_floor_alloc(void* event) {
    extern void* _mapAlloc(void*, s32);
    extern void* mapalloc_base_ptr;
    extern void* memset(void*, s32, s32);
    extern s32 evtSetValue(void*, s32, s32);
    extern f32 sintbl[];
    extern void* gp;
    u8* evt = event;
    s32* args = *(s32**)(evt + 0x18);
    s32* lw = (s32*)(evt + 0x9C);
    u32* floor = _mapAlloc(mapalloc_base_ptr, 0x88);
    s32 type;

    memset(floor, 0, 0x88);
    floor[8] = lw[6];
    floor[1] = lw[12];
    type = floor[8];
    if (type == 0 || type == 5) {
        floor[0] &= ~1;
        floor[10] = 0;
        *(f32*)&floor[25] = 1.0f;
        *(f32*)&floor[11] = (f32)lw[2];
        *(f32*)&floor[14] = (f32)lw[3];
        floor[26] = lw[4];
        *(f32*)&floor[17] = *(f32*)&floor[11];
        *(f32*)&floor[32] = (f32)lw[11];
    } else if (type == 1) {
        floor[10] = 1;
        *(f32*)&floor[25] = 1.0f;
        *(f32*)&floor[12] = (f32)lw[2];
        *(f32*)&floor[15] = (f32)lw[3];
        floor[27] = lw[4];
        *(f32*)&floor[18] = *(f32*)&floor[12];
        *(f32*)&floor[32] = (f32)lw[11];
    } else if (type == 2) {
        floor[10] = 2;
        *(f32*)&floor[25] = 1.0f;
        *(f32*)&floor[13] = (f32)lw[2];
        *(f32*)&floor[16] = (f32)lw[3];
        floor[28] = lw[4];
        *(f32*)&floor[19] = *(f32*)&floor[13];
        *(f32*)&floor[32] = (f32)lw[11];
    } else if (type == 3 || type == 4) {
        floor[10] = type == 3 ? 0 : 2;
        *(f32*)&floor[25] = 1.0f;
        *(f32*)&floor[11] = (f32)lw[2];
        *(f32*)&floor[14] = (f32)lw[3];
        floor[26] = lw[4];
        *(f32*)&floor[17] = 0.5f * (*(f32*)&floor[11] + *(f32*)&floor[14]);
        *(f32*)&floor[13] = (f32)lw[8];
        *(f32*)&floor[16] = (f32)lw[9];
        floor[28] = lw[10];
        *(f32*)&floor[19] = 0.5f * (*(f32*)&floor[13] + *(f32*)&floor[16]);
        *(f32*)&floor[32] = (f32)lw[11];
    }
    floor[29] = lw[5];
    floor[20] = floor[17];
    floor[21] = floor[18];
    floor[22] = floor[19];
    floor[24] = floor[23];
    if (floor[1] == 1) {
        s32 phase = (*(s32*)((u8*)*(void**)gp + 0x1C) + *(s32*)(evt + 0x15C) * 0x27) & 0x3F;
        *(f32*)&floor[23] = 3.0f * sintbl[(s32)(5.625f * (f32)(u32)phase)];
    } else {
        *(f32*)&floor[23] = 0.0f;
    }
    evtSetValue(event, args[0], (s32)floor);
    return 2;
}

u8 evt_moving_floor_init(s32 event) {
    extern s32 evtGetValue(void*, s32);
    u8* evt = (u8*)event;
    s32* args = *(s32**)(evt + 0x18);
    u32* floor = (u32*)evtGetValue(evt, args[0]);
    s32 direction = 0;
    f32 fraction = 0.0f;
    s32 kind = floor[8];

    if (kind == 3) {
        switch (floor[9]) {
            case 0:
                direction = 1; fraction = 0.5f; floor[0] &= ~1; floor[10] = 0; floor[25] = 1.0f;
                break;
            case 1:
                direction = 0; fraction = 0.0f; floor[0] |= 1; floor[10] = 0; floor[25] = 0.5f;
                break;
            case 2:
                direction = 1; fraction = 0.5f; floor[0] &= ~1; floor[10] = 2; floor[25] = 1.0f;
                break;
            case 3:
                direction = 0; fraction = 0.0f; floor[0] |= 1; floor[10] = 2; floor[25] = 0.5f;
                break;
            case 4:
                direction = 0; fraction = 0.5f; floor[0] &= ~1; floor[10] = 0; floor[25] = 1.0f;
                break;
            case 5:
                direction = 1; fraction = 0.0f; floor[0] |= 1; floor[10] = 0; floor[25] = 0.5f;
                break;
            case 6:
                direction = 0; fraction = 0.5f; floor[0] &= ~1; floor[10] = 2; floor[25] = 1.0f;
                break;
            case 7:
                direction = 1; fraction = 0.0f; floor[0] |= 1; floor[10] = 2; floor[25] = 0.5f;
                break;
        }
        if (++floor[9] > 7) {
            floor[9] = 0;
        }
    } else if (kind < 3) {
        direction = floor[9];
        fraction = 0.0f;
        if (++floor[9] > 1) {
            floor[9] = 0;
        }
    } else if (kind == 5) {
        direction = floor[9];
        fraction = 1.0f;
    } else if (kind == 4) {
        fraction = 0.0f;
        switch (floor[9]) {
            case 0:
                floor[10] = 2; direction = 0; floor[25] = 1.0f;
                break;
            case 1:
                floor[10] = 0; direction = 0; floor[25] = 1.0f;
                break;
            case 2:
                floor[10] = 2; direction = 1; floor[25] = 1.0f;
                break;
            case 3:
                floor[10] = 0; direction = 1; floor[25] = 1.0f;
                break;
        }
        if (++floor[9] > 3) {
            floor[9] = 0;
        }
    }

    if (direction == 0) {
        *(f32*)&floor[2] = (*(f32*)&floor[floor[10] + 14] - *(f32*)&floor[floor[10] + 11]) /
                           (f32)(s32)floor[floor[10] + 26];
        *(f32*)&floor[3] = *(f32*)&floor[floor[10] + 11] * (1.0f - fraction) +
                           *(f32*)&floor[floor[10] + 14] * fraction;
    } else {
        *(f32*)&floor[2] = (*(f32*)&floor[floor[10] + 11] - *(f32*)&floor[floor[10] + 14]) /
                           (f32)(s32)floor[floor[10] + 26];
        *(f32*)&floor[3] = *(f32*)&floor[floor[10] + 14] * (1.0f - fraction) +
                           *(f32*)&floor[floor[10] + 11] * fraction;
    }
    *(f32*)&floor[4] = (f32)(s32)floor[floor[10] + 26] * fraction;
    return 2;
}

s32 evt_moving_floor_main(void* event) {
    extern f32 float_0_80426584;
    extern f32 float_3_80426590;
    extern f32 float_5p625_80426594;
    extern f32 float_1000_8042659c;
    extern f32 sintbl[];
    extern void* gp;
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

