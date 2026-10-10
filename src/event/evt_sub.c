#include "event/evt_sub.h"
#include "event/evt_sub_trig.h"
#include "countdown.h"
#include "effect/eff_stageclear.h"

s32 stop;
extern void* mapalloc_base_ptr;
extern void _mapFree(void* heap, void* ptr);
extern f64 cos(f64);
extern u32 DAT_800000f8;
extern f64 sin(f64);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_intpl_init(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 a = evtGetValue(event, args[0]);
    s32 b = evtGetValue(event, args[1]);
    s32 c = evtGetValue(event, args[2]);
    s32 d = evtGetValue(event, args[3]);
    *(s32*)((s32)event + 0xC8) = a;
    *(s32*)((s32)event + 0xCC) = b;
    *(s32*)((s32)event + 0xD0) = c;
    *(s32*)((s32)event + 0xD4) = 0;
    *(s32*)((s32)event + 0xD8) = d;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_intpl_get_value(void* event) {
    extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 time, s32 duration);
    s32 start = *(s32*)((s32)event + 0xCC);
    s32 end = *(s32*)((s32)event + 0xD0);
    *(s32*)((s32)event + 0x9C) = (s32)intplGetValue(
        *(s32*)((s32)event + 0xC8),
        (f32)start,
        (f32)end,
        *(s32*)((s32)event + 0xD4),
        *(s32*)((s32)event + 0xD8));
    if (*(s32*)((s32)event + 0xD4) >= *(s32*)((s32)event + 0xD8)) {
        *(s32*)((s32)event + 0xA0) = 0;
    } else {
        *(s32*)((s32)event + 0xA0) = 1;
    }
    *(s32*)((s32)event + 0xD4) = *(s32*)((s32)event + 0xD4) + 1;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_intpl_get_value_para(void* event) {
    extern void evtSetFloat(void* event, s32 id, f32 value);
    extern void evtSetValue(void* event, s32 id, s32 value);
    extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 time, s32 duration);
    s32 start = *(s32*)((s32)event + 0xCC);
    s32 end = *(s32*)((s32)event + 0xD0);
    s32* args = *(s32**)((s32)event + 0x18);
    f32 value = intplGetValue(
        *(s32*)((s32)event + 0xC8),
        (f32)start,
        (f32)end,
        *(s32*)((s32)event + 0xD4),
        *(s32*)((s32)event + 0xD8));
    evtSetFloat(event, args[0], value);
    if (*(s32*)((s32)event + 0xD4) >= *(s32*)((s32)event + 0xD8)) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], 1);
    }
    *(s32*)((s32)event + 0xD4) = *(s32*)((s32)event + 0xD4) + 1;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_intpl_init_float(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern f32 evtGetFloat(void* event, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 type = evtGetValue(event, args[0]);
    f32 start = evtGetFloat(event, args[1]);
    f32 end = evtGetFloat(event, args[2]);
    s32 duration = evtGetValue(event, args[3]);
    *(s32*)((s32)event + 0xC8) = type;
    *(s32*)((s32)event + 0xCC) = (s32)(start * 1024.0f) + 600000;
    *(s32*)((s32)event + 0xD0) = (s32)(end * 1024.0f) + 600000;
    *(s32*)((s32)event + 0xD4) = 0;
    *(s32*)((s32)event + 0xD8) = duration;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_intpl_get_float(void* event) {
    extern f32 evtGetFloat(void* event, s32 value);
    extern void evtSetFloat(void* event, s32 id, f32 value);
    extern void evtSetValue(void* event, s32 id, s32 value);
    extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 time, s32 duration);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 out = args[0];
    s32 flag = args[1];
    f32 start = evtGetFloat(event, 0xFE363C8C);
    f32 end = evtGetFloat(event, 0xFE363C8D);
    f32 value = intplGetValue(*(s32*)((s32)event + 0xC8), start, end, *(s32*)((s32)event + 0xD4), *(s32*)((s32)event + 0xD8));
    if (out != -0xEE6B280) {
        evtSetFloat(event, out, value);
    }
    if (flag != -0xEE6B280) {
        if (*(s32*)((s32)event + 0xD4) >= *(s32*)((s32)event + 0xD8)) {
            evtSetValue(event, flag, 0);
        } else {
            evtSetValue(event, flag, 1);
        }
    }
    *(s32*)((s32)event + 0xD4) = *(s32*)((s32)event + 0xD4) + 1;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_intpl_msec_init(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 a = evtGetValue(event, args[0]);
    s32 b = evtGetValue(event, args[1]);
    s32 c = evtGetValue(event, args[2]);
    s32 d = evtGetValue(event, args[3]);
    s32 t1;
    s32 t0;
    *(s32*)((s32)event + 0xC8) = a;
    *(s32*)((s32)event + 0xCC) = b;
    *(s32*)((s32)event + 0xD0) = c;
    *(s32*)((s32)event + 0xD8) = d;
    t0 = *(s32*)event;
    t1 = *(s32*)((s32)event + 4);
    *(s32*)((s32)event + 0x19C) = t1;
    *(s32*)((s32)event + 0x198) = t0;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_intpl_msec_get_value(void* event) {
    extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 time, s32 duration);
    u32 bus = *(u32*)0x800000F8 >> 2;
    u32 ticks = __mulhwu(0x10624DD3, bus) >> 6;
    u64 elapsed = *(s32*)((s32)event + 4) - *(s32*)((s32)event + 0x19C);
    u32 msec = elapsed / ticks;
    if ((s32)msec < *(s32*)((s32)event + 0xD8)) {
        s32 start = *(s32*)((s32)event + 0xCC);
        s32 end = *(s32*)((s32)event + 0xD0);
        *(s32*)((s32)event + 0x9C) = (s32)intplGetValue(*(s32*)((s32)event + 0xC8), (f32)start, (f32)end, msec, *(s32*)((s32)event + 0xD8));
        *(s32*)((s32)event + 0xA0) = 1;
    } else {
        *(s32*)((s32)event + 0x9C) = *(s32*)((s32)event + 0xD0);
        *(s32*)((s32)event + 0xA0) = 0;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_intpl_msec_get_value_para(void* event) {
    extern f32 intplGetValue(s32, f32, f32, s32, s32);
    extern void evtSetFloat(void*, s32, f32);
    extern void evtSetValue(void*, s32, s32);
    s32* args;
    u32 bus;
    u32 ticks;
    u64 elapsed;
    u32 msec;
    s32 start;
    s32 end;

    bus = *(u32*)0x800000F8 >> 2;
    ticks = __mulhwu(0x10624DD3, bus) >> 6;
    args = *(s32**)((s32)event + 0x18);
    elapsed = *(s32*)((s32)event + 4) - *(s32*)((s32)event + 0x19C);
    msec = elapsed / ticks;
    if ((s32)msec < *(s32*)((s32)event + 0xD8)) {
        start = *(s32*)((s32)event + 0xCC);
        end = *(s32*)((s32)event + 0xD0);
        evtSetFloat(event, args[0], intplGetValue(*(s32*)((s32)event + 0xC8), (f32)start, (f32)end, msec, *(s32*)((s32)event + 0xD8)));
        evtSetValue(event, args[1], 1);
    } else {
        evtSetFloat(event, args[0], (f32)*(s32*)((s32)event + 0xD0));
        evtSetValue(event, args[1], 0);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_sub_spline_init(void* event) {
    extern s32 evtGetValue(void*, s32);
    extern void* _mapAlloc(void*, u32);
    extern void spline_maketable(s32 count, s32 unk4, void* table4, void* tableC);
    s32* args;
    s32 v0;
    s32 v1;
    s32 v2;
    s32 count;
    s32 v4;
    s32 v5;
    s32* work;

    args = *(s32**)((s32)event + 0x18);
    v0 = evtGetValue(event, args[0]);
    v1 = evtGetValue(event, args[1]);
    v2 = evtGetValue(event, args[2]);
    count = evtGetValue(event, args[3]);
    v4 = evtGetValue(event, args[4]);
    v5 = evtGetValue(event, args[5]);
    work = _mapAlloc(mapalloc_base_ptr, 0x30);
    *(s32**)((s32)event + 0xD8) = work;
    work[0] = count;
    work[1] = (s32)_mapAlloc(mapalloc_base_ptr, count * 4);
    work[2] = v2;
    work[3] = (s32)_mapAlloc(mapalloc_base_ptr, count * 0xC);
    spline_maketable(count, work[2], (void*)work[1], (void*)work[3]);
    work[10] = v0;
    work[6] = v1;
    work[4] = 0;
    work[5] = v4;
    work[7] = v5;
    work[8] = *(s32*)event;
    work[9] = *(s32*)((s32)event + 4);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_spline_get_value(EventEntry* event) {
    extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 time, s32 duration);
    extern void spline_getvalue(f32 time, f32* out, s32 count, f32* points, s32 unk4, s32 unkC);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    extern f32 float_0_8041fe38;
    extern f32 float_1_8041fe58;
    extern f32 float_1024_8041fe84;
    s32* args;
    s32* work;
    f32 out[3];
    f32 elapsed;
    f32 value;

    work = *(s32**)((s32)event + 0xD8);
    args = event->args;

    if (work[10] == 0) {
        value = intplGetValue(work[6], float_0_8041fe38, float_1_8041fe58, work[4], work[5]);
    } else {
        u32 bus = *(u32*)0x800000F8 >> 2;
        u32 ticks = ((u64)bus * 0x10624DD3U) >> 38;
        elapsed = (f32)((*(u64*)event - *(u64*)&work[8]) / ticks);
        value = intplGetValue(work[6], float_0_8041fe38, float_1_8041fe58, (s32)elapsed, work[7]);
    }

    spline_getvalue(value, out, work[0], (f32*)work[2], work[1], work[3]);
    evtSetValue(event, args[0], (s32)(float_1024_8041fe84 * out[0]) - 230000000);
    evtSetValue(event, args[1], (s32)(float_1024_8041fe84 * out[1]) - 230000000);
    evtSetValue(event, args[2], (s32)(float_1024_8041fe84 * out[2]) - 230000000);

    work[4]++;
    if (work[10] == 0) {
        if (work[4] <= work[5]) {
            evtSetValue(event, args[3], 1);
            return 2;
        }
    } else {
        if (elapsed <= (f32)work[7]) {
            evtSetValue(event, args[3], 1);
            return 2;
        }
    }

    _mapFree(mapalloc_base_ptr, (void*)work[1]);
    _mapFree(mapalloc_base_ptr, (void*)work[3]);
    _mapFree(mapalloc_base_ptr, work);
    evtSetValue(event, args[3], 0);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_sub_spline_get_value_manual(void* event) {
    extern s32 evtGetValue(void*, s32);
    extern void spline_getvalue(f32, f32*, s32, f32*, s32, s32);
    extern void evtSetValue(void*, s32, s32);
    extern f32 float_1024_8041fe84;
    s32* args;
    s32* work;
    f32 out[3];
    f32 denom;
    f32 t;

    args = *(s32**)((s32)event + 0x18);
    work = *(s32**)((s32)event + 0xD8);
    t = (f32)evtGetValue(event, args[0]);
    if (work[10] != 0) {
        denom = (f32)work[7];
    } else {
        denom = (f32)work[5];
    }
    spline_getvalue(t / denom, out, work[0], (f32*)work[2], work[1], work[3]);
    evtSetValue(event, args[1], (s32)(float_1024_8041fe84 * out[0]) - 230000000);
    evtSetValue(event, args[2], (s32)(float_1024_8041fe84 * out[1]) - 230000000);
    evtSetValue(event, args[3], (s32)(float_1024_8041fe84 * out[2]) - 230000000);
    return 2;
}


s32 evt_sub_spline_free(int param_1) {
    void* spline = *(void**)(param_1 + 0xD8);

    _mapFree(mapalloc_base_ptr, *(void**)((s32)spline + 4));
    _mapFree(mapalloc_base_ptr, *(void**)((s32)spline + 0xC));
    _mapFree(mapalloc_base_ptr, spline);
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_get_sincos(void* event) {
    extern f32 evtGetFloat(void* event, s32 value);
    extern void evtSetValue(void* event, s32 id, s32 value);
    extern const f32 float_3p1416_8041fe7c;
    extern const f32 float_180_8041fe80;
    extern const f32 float_1024_8041fe84;
    s32* args = *(s32**)((s32)event + 0x18);
    f32 rad = (float_3p1416_8041fe7c * evtGetFloat(event, args[0])) / float_180_8041fe80;
    evtSetValue(event, args[1], (s32)((f32)sin(rad) * float_1024_8041fe84) - 230000000);
    evtSetValue(event, args[2], (s32)((f32)cos(rad) * float_1024_8041fe84) - 230000000);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_rumble_onoff(void* pEvt) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void padRumbleOn(s32 type);
    extern void padRumbleOff(s32 type);
    extern void padRumbleHardOff(s32 type);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 mode = evtGetValue(pEvt, args[0]);
    s32 type = evtGetValue(pEvt, args[1]);

    switch (mode) {
        case 0:
            padRumbleOn(type);
            break;
        case 1:
            padRumbleOff(type);
            break;
        case 2:
            padRumbleHardOff(type);
            break;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_random(void* param_1) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 evtSetValue(void* event, s32 index, s32 value);
    extern s32 rand(void);
    s32* args;
    s32 max;
    s32 value;

    args = *(s32**)((s32)param_1 + 0x18);
    max = evtGetValue(param_1, args[0]);
    value = rand() % (max + 1);
    evtSetValue(param_1, args[1], value);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_get_stopwatch(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* evtGetPtrID(s32 id);
    extern void evtSetValue(void* event, s32 id, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s64* watch = evtGetPtrID(evtGetValue(event, args[0]));
    u32 bus = *(u32*)0x800000F8 >> 2;
    u32 ticks = ((u64)bus * 0x10624DD3U) >> 38;
    s32 value = (s32)(*watch / ticks);
    if (value > 600000) {
        value = 600000;
    }
    evtSetValue(event, args[1], value);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_area_check(EventEntry* event) {
    extern s32 evtGetValue(EventEntry* event, s32 index);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    s32* args;
    s32 type;
    f32 x;
    f32 z;
    f32 radius;
    f32 targetX;
    f32 targetZ;
    s32 result;

    args = event->args;
    type = evtGetValue(event, args[0]);
    x = (f32)evtGetValue(event, args[1]);
    z = (f32)evtGetValue(event, args[2]);
    radius = (f32)evtGetValue(event, args[3]);
    targetX = (f32)evtGetValue(event, args[4]);
    targetZ = (f32)evtGetValue(event, args[5]);

    switch (type) {
        case 0:
            if (distABf(x, z, targetX, targetZ) <= radius) {
                result = 1;
            } else {
                result = 0;
            }
            break;
        case 1:
            result = 1;
            if ((x - radius) > targetX) {
                result = 0;
            }
            if ((x + radius) < targetX) {
                result = 0;
            }
            if ((z - radius) > targetZ) {
                result = 0;
            }
            if ((z + radius) < targetZ) {
                result = 0;
            }
            break;
        default:
            result = 0;
            break;
    }

    evtSetValue(event, args[6], result);
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_sub_get_dist(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void evtSetFloat(void* event, s32 id, f32 value);
    extern f32 distABf(f32 ax, f32 ay, f32 bx, f32 by);
    s32* args = *(s32**)((s32)event + 0x18);
    f32 ax = (f32)evtGetValue(event, args[0]);
    f32 ay = (f32)evtGetValue(event, args[1]);
    f32 bx = (f32)evtGetValue(event, args[2]);
    f32 by = (f32)evtGetValue(event, args[3]);
    evtSetFloat(event, args[4], distABf(ax, ay, bx, by));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_get_dir(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void evtSetFloat(void* event, s32 id, f32 value);
    extern f32 angleABf(f32 ax, f32 ay, f32 bx, f32 by);
    s32* args = *(s32**)((s32)event + 0x18);
    f32 ax = (f32)evtGetValue(event, args[0]);
    f32 ay = (f32)evtGetValue(event, args[1]);
    f32 bx = (f32)evtGetValue(event, args[2]);
    f32 by = (f32)evtGetValue(event, args[3]);
    evtSetFloat(event, args[4], angleABf(ax, ay, bx, by));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(evt_sub_get_system_flag) {
    extern void* gp;
    evtSetValue(event, event->args[0], *(s32*)gp);
    return 2;
}
USER_FUNC(evt_sub_system_flag_onoff) {
    extern void* gp;
    s32* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    u32 flag = args[1];
    if (on) {
        *(u32*)gp |= flag;
    } else {
        *(u32*)gp &= ~flag;
    }
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_key_get_dir) {
    extern s32 keyGetDir(s32);
    s32* args = event->args;
    evtSetValue(event, args[1], keyGetDir(evtGetValue(event, args[0])));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_key_get_button) {
    extern s32 keyGetButton(s32);
    s32* args = event->args;
    evtSetValue(event, args[1], keyGetButton(evtGetValue(event, args[0])));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_key_get_buttontrg) {
    extern s32 keyGetButtonTrg(s32);
    s32* args = event->args;
    evtSetValue(event, args[1], keyGetButtonTrg(evtGetValue(event, args[0])));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(evt_sub_get_mapname) {
    extern void* gp;
    evtSetValue(event, event->args[0], (s32)gp + 0x12C);
    return 2;
}

USER_FUNC(evt_sub_get_areaname) {
    extern void* gp;
    evtSetValue(event, event->args[0], (s32)gp + 0x13C);
    return 2;
}

USER_FUNC(evt_sub_status_gauge_force_open) { extern void statusWinForceOpen(void); statusWinForceOpen(); return 2; }

USER_FUNC(evt_sub_status_gauge_force_close) { extern void statusWinForceClose(void); statusWinForceClose(); return 2; }

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_sub_status_gauge_check_update) {
    extern s32 statusWinCheckUpdate(void);
    s32* args = event->args;
    if (statusWinCheckUpdate()) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(N_evt_sub_status_gauge_force_update) { extern void statusWinForceUpdate(void); statusWinForceUpdate(); return 2; }
s32 evt_sub_save_playtime(void* event) {
    extern void* gp;
    extern void swByteSet(s32 id, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 base = args[0];
    u32 time[2];
    time[0] = *(u32*)((s32)gp + 0x20);
    time[1] = *(u32*)((s32)gp + 0x24);
    swByteSet(base + 0xA220000 - 0x180, ((u8*)time)[0]);
    swByteSet(base + 0xA220000 - 0x17F, ((u8*)time)[1]);
    swByteSet(base + 0xA220000 - 0x17E, ((u8*)time)[2]);
    swByteSet(base + 0xA220000 - 0x17D, ((u8*)time)[3]);
    swByteSet(base + 0xA220000 - 0x17C, ((u8*)time)[4]);
    swByteSet(base + 0xA220000 - 0x17B, ((u8*)time)[5]);
    swByteSet(base + 0xA220000 - 0x17A, ((u8*)time)[6]);
    swByteSet(base + 0xA220000 - 0x179, ((u8*)time)[7]);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_sub_load_progresstime(void* event) {
    extern void* gp;
    extern s32 swByteGet(s32 id);
    extern void evtSetValue(void* event, s32 id, s32 value);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 base = args[0];
    u8 time[8];
    u32 bus;
    u32 ticks;
    s64 value;
    time[0] = swByteGet(base + 0xA220000 - 0x180);
    time[1] = swByteGet(base + 0xA220000 - 0x17F);
    time[2] = swByteGet(base + 0xA220000 - 0x17E);
    time[3] = swByteGet(base + 0xA220000 - 0x17D);
    time[4] = swByteGet(base + 0xA220000 - 0x17C);
    time[5] = swByteGet(base + 0xA220000 - 0x17B);
    time[6] = swByteGet(base + 0xA220000 - 0x17A);
    time[7] = swByteGet(base + 0xA220000 - 0x179);
    bus = *(u32*)0x800000F8 >> 2;
    ticks = __mulhwu(0x10624DD3, bus) >> 6;
    value = (*(s64*)((s32)gp + 0x20) - *(s64*)time) / ticks;
    if (value > 86400000) {
        value = 86400000;
    }
    evtSetValue(event, args[1], value);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(evt_sub_get_language) {
    extern void* gp;
    evtSetValue(event, event->args[0], *(s32*)((s32)gp + 0x16C));
    return 2;
}
s32 evt_sub_animgroup_async(int param_1) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 animGroupBaseAsync(s32 name, s32 mode, s32 flags);
    extern s32 gp;
    s32 name = evtGetValue((void*)param_1, **(s32**)(param_1 + 0x18));
    s32 mode = *(s32*)(gp + 0x14);
    mode = (u32)(-mode | mode) >> 31;

    switch (animGroupBaseAsync(name, mode, 0)) {
        case 0:
            return 0;
        default:
            return 2;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_sub_countdown_start) {
    s32* args = event->args;
    s32 time = evtGetValue(event, args[0]);
    s32 flags = evtGetValue(event, args[1]);
    countDownStart(time, flags);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_sub_countdown_get_status) {
    s32* args = event->args;
    evtSetValue(event, args[0], countDownGetStatus());
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(evt_sub_countdown_end) { countDownEnd(); return 2; }
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_sub_countdown_flag_onoff) {
    s32* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    if (on) {
        countDownFlagOn(flag);
    } else {
        countDownFlagOff(flag);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(evt_sub_countdown_set_restart_time) {
    countDownSetRestartTime(evtGetValue(event, event->args[0]));
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

void coingetDisp(void* work) {
    extern void* gp;
    extern char* msgSearch(char* name);
    extern s32 sprintf(char* dst, const char* format, ...);
    extern char* strcpy(char* dst, const char* src);
    extern void unk_800d0a4c(char* text, s32 value);
    extern u32 FontGetMessageWidthLine(char* text, u16* lines);
    extern void FontDrawStart(void);
    extern void FontDrawColor(void* color);
    extern u32 FontGetMessageWidth(char* text);
    extern void FontDrawMessage(s32 x, s32 y, char* text);
    extern char str_msg_window_coin_get_802c1888[];
    extern u32 dat_8041fe10;
    u8* data = work;
    s32* params = *(s32**)(data + 0x2C);
    u32 color;
    u16 lines[2];
    char text[0x408];
    s32 width;

    if (params[1] == 0) return;
    if (*(u32*)((u8*)gp + 0x16C) == 0) {
        sprintf(text, msgSearch(str_msg_window_coin_get_802c1888), params[0]);
    } else {
        strcpy(text, msgSearch(str_msg_window_coin_get_802c1888));
        unk_800d0a4c(text, params[0]);
    }
    width = FontGetMessageWidthLine(text, lines) & 0xFFFF;
    if (width + 20 > *(s32*)(data + 0x20)) {
        *(s32*)(data + 0x20) = width + 20;
        width = -*(s32*)(data + 0x20);
        *(s32*)(data + 0x18) = (width + ((u32)width >> 31)) >> 1;
    }
    *(u32*)(data + 0x24) = *(s32*)(*(u8**)(data + 0x28) + 0x18) + lines[0] * 24;
    FontDrawStart();
    color = dat_8041fe10;
    FontDrawColor(&color);
    width = FontGetMessageWidth(text) & 0xFFFF;
    width = -width;
    FontDrawMessage((width + ((u32)width >> 31)) >> 1,
                    *(s32*)(data + 0x1C) - 8, text);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_sub_get_coin(EventEntry* event, s32 isFirstCall) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* marioGetPtr(void);
    extern void* camGetPtr(s32 id);
    extern f32 reviseAngle(f32 angle);
    extern void* effItemGetEntry(s32 item);
    extern void* _mapAlloc(void* heap, u32 size);
    extern void iconEntry(char* name, s32 icon);
    extern void iconSetPos(f32 x, f32 y, f32 z, char* name);
    extern void iconFlagOn(char* name, u32 flags);
    extern void iconFlagOff(char* name, u32 flags);
    extern void iconDelete(char* name);
    extern void marioChgGetItemMotion(void);
    extern void marioChgStayMotion(void);
    extern s32 winMgrEntry(void* desc);
    extern void winMgrOpen(s32 id);
    extern void winMgrSetParam(s32 id, void* param);
    extern void winMgrCloseAutoDelete(s32 id);
    extern s32 winMgrAction(s32 id);
    extern void psndBGMOff(s32 id);
    extern void psndBGMOn(s32 id, char* name);
    extern s32 keyGetButtonTrg(s32 pad);
    extern void pouchAddCoin(s16 coins);
    extern void effSoftDelete(void* eff);
    extern void* gp;
    extern char str_iraiCoin_802c1868[];
    extern char str_BGM_FF_GET_ITEM1_802c1874[];
    extern char coinget_window_desc[];
    extern f32 float_52_8041fe78;
    extern f32 float_10_8041fe68;
    extern f32 float_100_8041fe44;
    extern f32 float_5_8041fe40;
    extern f32 float_6p2832_8041fe48;
    extern f32 float_360_8041fe4c;
    extern f32 float_3p1416_8041fe5c;
    extern f32 float_0p00761_8041fe50;
    extern f32 float_0p16605_8041fe54;
    extern f32 float_1_8041fe58;
    extern f32 float_1p5708_8041fe60;
    extern f32 float_4p7124_8041fe64;
    extern f32 float_0p03705_8041fe6c;
    extern f32 float_0p4967_8041fe70;
    extern f32 float_62_8041fe74;
    typedef struct LocalVec {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec;
    s32 coins;
    void* mario;
    void* cam;
    LocalVec positions[2];
    f32 angle;
    f32 sx;
    f32 cz;
    void* eff;
    s32* param;
    s32 state;
    s32 elapsed;
    s32 win;

    coins = evtGetValue(event, event->args[0]);
    mario = marioGetPtr();
    positions[0] = positions[1] = *(LocalVec*)((s32)mario + 0x8C);
    cam = camGetPtr(4);
    angle = reviseAngle((float_100_8041fe44 * *(f32*)((s32)cam + 0x114) + float_5_8041fe40) / float_100_8041fe44);
    angle = float_6p2832_8041fe48 * angle / float_360_8041fe4c;
    sx = evtSubSin(angle) * float_10_8041fe68;
    cz = evtSubCos(angle) * float_10_8041fe68;
    positions[0].x += sx;
    positions[0].y += float_62_8041fe74;
    positions[0].z -= cz;
    positions[1].x += sx;
    positions[1].y += float_52_8041fe78;
    positions[1].z -= cz;
    if (isFirstCall != 0) {
        *(s32*)((s32)event + 0x78) = 0;
        *(void**)((s32)event + 0x7C) = effItemGetEntry(7);
        *(s32*)((s32)event + 0x80) = 0;
        *(s32*)((s32)event + 0x84) = 0;
        *(void**)((s32)event + 0x88) = _mapAlloc(mapalloc_base_ptr, 8);
    }
    state = *(s32*)((s32)event + 0x78);
    eff = *(void**)((s32)event + 0x7C);
    param = *(s32**)((s32)event + 0x88);
    switch (state) {
    case 0:
        iconEntry(str_iraiCoin_802c1868, 0x193);
        iconSetPos(positions[1].x, positions[1].y, positions[1].z, str_iraiCoin_802c1868);
        iconFlagOn(str_iraiCoin_802c1868, 0x102);
        *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x38) = 5;
        marioChgGetItemMotion();
        win = winMgrEntry(coinget_window_desc);
        *(s32*)((s32)event + 0x84) = win;
        winMgrOpen(win);
        winMgrSetParam(win, param);
        param[0] = coins;
        param[1] = 0;
        psndBGMOff(0x3800);
        psndBGMOn(0x201, str_BGM_FF_GET_ITEM1_802c1874);
        *(s32*)((s32)event + 0x80) = 200;
        *(u64*)((s32)event + 0x198) = *(u64*)((s32)gp + 0x38);
        *(s32*)((s32)event + 0x78) = 1;
        return 0;
    case 1:
        elapsed = (*(u32*)((s32)gp + 0x3C) - *(u32*)((s32)event + 0x19C)) / ((*(u32*)0x800000F8 >> 2) / 1000);
        if (*(s32*)((s32)event + 0x80) < elapsed) {
            iconFlagOff(str_iraiCoin_802c1868, 2);
            param[1] = 1;
            *(s32*)((s32)event + 0x80) = 500;
            *(u64*)((s32)event + 0x198) = *(u64*)((s32)gp + 0x38);
            *(s32*)((s32)event + 0x78) = 2;
        }
        return 0;
    case 2:
        elapsed = (*(u32*)((s32)gp + 0x3C) - *(u32*)((s32)event + 0x19C)) / ((*(u32*)0x800000F8 >> 2) / 1000);
        if (*(s32*)((s32)event + 0x80) < elapsed) {
            *(s32*)((s32)event + 0x78) = 10;
        }
        return 0;
    case 10:
        if ((keyGetButtonTrg(0) & 0x100) != 0) {
            *(s32*)((s32)event + 0x78) = 100;
        }
        return 0;
    case 100:
        pouchAddCoin((s16)coins);
        effSoftDelete(eff);
        iconDelete(str_iraiCoin_802c1868);
        marioChgStayMotion();
        psndBGMOn(0xA0, 0);
        psndBGMOff(0x201);
        winMgrCloseAutoDelete(*(s32*)((s32)event + 0x84));
        param[1] = 0;
        *(s32*)((s32)event + 0x78) = 101;
        return 2;
    case 101:
        if (winMgrAction(*(s32*)((s32)event + 0x84)) == 0) {
            _mapFree(mapalloc_base_ptr, param);
            return 2;
        }
        return 0;
    default:
        return 2;
    }
}

#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma optimize_for_size off
s32 stone_ry(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern void* itemNameToPtr(s32 name);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 item = evtGetValue(event, args[0]);
    s32 angle = evtGetValue(event, args[1]);
    void* b;
    void* a = *(void**)((s32)itemNameToPtr(item) + 0x1C);
    u32 flags;

    b = *(void**)((s32)a + 0xC);
    flags = *(u16*)((s32)b + 4);
    flags &= ~4U;
    *(u16*)((s32)b + 4) = (u16)flags;
    b = *(void**)((s32)a + 0xC);
    *(f32*)((s32)b + 0x1C) = (f32)(angle % 360);
    return 2;
}
#pragma optimize_for_size on
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 stone_bg(void* event) {
    extern s32 evtGetValue(void*, s32);
    extern void* camGetPtr(s32);
    extern void* effNameToPtr(const char*);
    extern void* itemNameToPtr(s32);
    extern void* npcNameToPtr_NoAssert(const char*);
    extern void marioSetCamId(s32);
    extern const char str_sub_bg_802c162c[];
    extern const char str_sjis_00000140[];
    s32* args;
    s32 item;
    void* ptr;

    args = *(s32**)((s32)event + 0x18);
    item = evtGetValue(event, args[0]);
    *(s32*)((s32)camGetPtr(8) + 0x1E8) = 5;
    *(s32*)((s32)*(void**)((s32)effNameToPtr(str_sub_bg_802c162c) + 0xC) + 0x38) = 5;
    *(s32*)((s32)*(void**)((s32)*(void**)((s32)itemNameToPtr(item) + 0x1C) + 0xC) + 0x38) = 5;
    ptr = npcNameToPtr_NoAssert(str_sjis_00000140);
    if (ptr != 0 && evtGetValue(event, 0xF5DE0180) < 0xD3) {
        *(s32*)((s32)npcNameToPtr_NoAssert(str_sjis_00000140) + 0x1F4) = 5;
    } else {
        marioSetCamId(5);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(stone_bg_open) {
    extern void* effNameToPtr(const char* name);
    extern const char str_sub_bg_802c162c[];
    void* eff = effNameToPtr(str_sub_bg_802c162c);
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x20) = 1;
    return 2;
}

USER_FUNC(stageclear_wait) {
    extern void* effNameToPtr(const char* name);
    extern const char str_sub_clear_802c16ec[];
    s32 done = effStageClearEndChk(effNameToPtr(str_sub_clear_802c16ec));
    done = (-done | done) >> 31;
    return 2 & done;
}

USER_FUNC(set_stop) {
    stop = evtGetValue(event, event->args[0]);
    return 2;
}

USER_FUNC(get_stop) {
    evtSetValue(event, event->args[0], stop);
    return 2;
}

USER_FUNC(bgm_start_wait) {
    extern s32 psndBGMStartCheck(s32);
    s32 done = psndBGMStartCheck(1);
    done = (-done | done) >> 31;
    return 2 & done;
}


/* CHATGPT FALLBACK MISSING STUBS: main/event/evt_sub 20260624_191429 */

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
/* fallback stub-fill: map=unk_80053f10 addr=0x80053f10 size=0x000000c8 */
s32 unk_80053f10(void* event) {
    extern f32 evtGetFloat(void*, s32);
    extern void evtSetFloat(void*, s32, f32);
    extern void* gp;
    extern f32 float_60_8041fe3c;
    s32* args;
    s32 xArg;
    s32 yArg;
    f32 x;
    f32 y;
    f32 scale;

    args = *(s32**)((s32)event + 0x18);
    xArg = args[0];
    x = evtGetFloat(event, xArg);
    yArg = args[1];
    y = evtGetFloat(event, yArg);
    scale = float_60_8041fe3c / (f32)*(s32*)((s32)gp + 4);
    x *= scale;
    y *= scale;
    evtSetFloat(event, xArg, x);
    evtSetFloat(event, yArg, y);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_sub_check_intersect(EventEntry* event) {
    extern f32 evtGetFloat(EventEntry* event, s32 index);
    extern s32 evtSetValue(EventEntry* event, s32 index, s32 value);
    extern f32 float_0_8041fe38;
    s32* args;
    f32 startX;
    f32 startZ;
    f32 endX;
    f32 endZ;
    f32 targetX0;
    f32 targetZ0;
    f32 targetX1;
    f32 targetZ1;
    f32 a;
    f32 b;

    args = event->args;
    startX = evtGetFloat(event, args[0]);
    startZ = evtGetFloat(event, args[1]);
    endX = evtGetFloat(event, args[2]);
    endZ = evtGetFloat(event, args[3]);
    targetX0 = evtGetFloat(event, args[4]);
    targetZ0 = evtGetFloat(event, args[5]);
    targetX1 = evtGetFloat(event, args[6]);
    targetZ1 = evtGetFloat(event, args[7]);

    if (startX >= endX) {
        if (((startX < targetX0) && (startX < targetX1)) || ((targetX0 < endX) && (targetX1 < endX))) {
            evtSetValue(event, args[8], 0);
            return 2;
        }
    } else if (((endX < targetX0) && (endX < targetX1)) || ((targetX0 < startX) && (targetX1 < startX))) {
        evtSetValue(event, args[8], 0);
        return 2;
    }

    if (startZ >= endZ) {
        if (((startZ < targetZ0) && (startZ < targetZ1)) || ((targetZ0 < endZ) && (targetZ1 < endZ))) {
            evtSetValue(event, args[8], 0);
            return 2;
        }
    } else if (((endZ < targetZ0) && (endZ < targetZ1)) || ((targetZ0 < startZ) && (targetZ1 < startZ))) {
        evtSetValue(event, args[8], 0);
        return 2;
    }

    a = ((startZ - endZ) * (startX - targetX0)) + ((startX - endX) * (targetZ0 - startZ));
    b = ((startZ - endZ) * (startX - targetX1)) + ((startX - endX) * (targetZ1 - startZ));
    if (a * b > float_0_8041fe38) {
        evtSetValue(event, args[8], 0);
        return 2;
    }

    a = ((targetZ0 - targetZ1) * (targetX0 - startX)) + ((targetX0 - targetX1) * (startZ - targetZ0));
    b = ((targetZ0 - targetZ1) * (targetX0 - endX)) + ((targetX0 - targetX1) * (endZ - targetZ0));
    if (a * b <= float_0_8041fe38) {
        evtSetValue(event, args[8], 1);
    } else {
        evtSetValue(event, args[8], 0);
    }
    return 2;
}

s32 irai_mail_check(EventEntry* event) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void evtSetValue(EventEntry* event, s32 dst, s32 value);
    extern s32 swGet(u32 flag);
    extern u32 swByteGet(s32 index);
    extern void swSet(u32 flag);
    extern void pouchReceiveMail(u32 mail);
    extern void* gp;
    s32* args;
    s32 enabled;
    s32 value;
    u8 bytes[8];
    u64 saved;
    u64 now;
    s32 i;

    args = event->args;
    enabled = evtGetValue(event, args[0]);
    value = 0;
#define CHECK_MAIL(storyFlag, receivedFlag, byteBase, mailId) \
    if (swGet(storyFlag) != 0 && swGet(receivedFlag) == 0) { \
        for (i = 0; i < 8; i++) { \
            bytes[i] = (u8)swByteGet((byteBase) + i); \
        } \
        saved = *(u64*)bytes; \
        now = *(u64*)((s32)gp + 0x20); \
        if ((s64)(now - saved) / (*(u32*)0x800000F8 >> 2) > 7200) { \
            value = 1; \
            if (enabled != 0) { \
                swSet(receivedFlag); \
                pouchReceiveMail(mailId); \
            } \
        } \
    }
    CHECK_MAIL(0x1505, 0x17, 0x58D, 0x25)
    CHECK_MAIL(0x150A, 0x18, 0x595, 0x26)
    CHECK_MAIL(0x150B, 0x19, 0x59D, 0x27)
    CHECK_MAIL(0x150D, 0x1A, 0x5A5, 0x28)
    CHECK_MAIL(0x1512, 0x1B, 0x5AD, 0x29)
    CHECK_MAIL(0x1518, 0x1C, 0x5B5, 0x2A)
#undef CHECK_MAIL
    evtSetValue(event, args[1], value);
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(mail_evt_gor_04_keyoff_check) {
    extern s32 marioCtrlOffChk(void);
    extern s32 marioKeyOffChk(void);
    s32* args = event->args;
    s32 ctrl = marioCtrlOffChk();
    s32 key = marioKeyOffChk();
    evtSetValue(event, args[0], key + ctrl);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

const u32 dat_8041fe10 = 0xFF;
const char str_M_N_3_8041fe14[] = "M_N_3";
const char str_M_P_2_8041fe1c[] = "M_P_2";
const char str_S_8041fe24[] = "S";
const char str_next_8041fe28[] = "next";
const char str_M_D_2_8041fe30[] = "M_D_2";
const f32 float_0_8041fe38 = 0.0f;
const f32 float_60_8041fe3c = 60.0f;
const f32 float_5_8041fe40 = 5.0f;
const f32 float_100_8041fe44 = 100.0f;
const f32 float_6p2832_8041fe48 = 6.2831855f;
const f32 float_360_8041fe4c = 360.0f;
const f32 float_0p00761_8041fe50 = 0.00761f;
const f32 float_0p16605_8041fe54 = 0.16605f;
const f32 float_1_8041fe58 = 1.0f;
const f32 float_3p1416_8041fe5c = 3.1415927f;
const f32 float_1p5708_8041fe60 = 1.5707964f;
const f32 float_4p7124_8041fe64 = 4.712389f;
const f32 float_10_8041fe68 = 10.0f;
const f32 float_0p03705_8041fe6c = 0.03705f;
const f32 float_0p4967_8041fe70 = 0.4967f;
const f32 float_62_8041fe74 = 62.0f;
const f32 float_52_8041fe78 = 52.0f;
const f32 float_3p1416_8041fe7c = 3.141592f;
const f32 float_180_8041fe80 = 180.0f;
const f32 float_1024_8041fe84 = 1024.0f;

s32 evt_sub_stopwatch[] = {
    0x00010005,
    0,
    0x00010009,
    1,
    6,
    2,
    1,
};

u32 kusa_data[] = {
    0xF24A7A80,
    0xF24A7302,
    0xF24A6E82,
    0xF24A719F,
    0xF24A7766,
    0xF24A7A80,
    0xF24A7766,
    0xF24A719F,
    0xF24A6E82,
    0xF24A70BB,
    0xF24A759F,
    0xF24A7A80,
};

s32 coinget_window_desc[] = {
    0,
    0,
    8,
    -175,
    120,
    350,
    42,
    0xE1D79BFF,
    0,
    (s32)coingetDisp,
};

/* Recovered evt_sub safe-event dependencies. */
extern s32 evt_bero_mapchange();
extern s32 evt_cam3d_evt_set_npc_rel();
extern s32 evt_cam3d_evt_set_rel();
extern s32 evt_eff();
extern s32 evt_eff_delete();
extern s32 evt_eff_softdelete();
extern s32 evt_fade_end_wait();
extern s32 evt_fade_entry();
extern s32 evt_fade_reset();
extern s32 evt_fade_set_mapchange_type();
extern s32 evt_item_delete();
extern s32 evt_item_get_position();
extern s32 evt_item_set_position();
extern s32 evt_mapobj_flag_onoff();
extern s32 evt_mario_adjust_dir();
extern s32 evt_mario_init_camid();
extern s32 evt_mario_set_pose();
extern s32 evt_npc_get_position();
extern s32 evt_npc_set_anim();
extern s32 evt_npc_set_camid();
extern s32 evt_party_init_camid();
extern s32 evt_party_stop();
extern s32 evt_pouch_add_item();
extern s32 evt_pouch_get_starstone();
extern s32 evt_snd_bgmoff_f();
extern s32 evt_snd_envoff();
extern s32 evt_snd_envoff_f();
extern s32 evt_snd_sfx_all_stop();
extern s32 evt_snd_sfxon();
extern const char str_BGM_EVT_STAGE_CLEAR1_802c16b4[];
extern const char str_BGM_FF_GET_STARSTONE_802c15bc[];
extern const char str_EM_I_1_802c1608[];
extern const char str_SFX_EVT_STARSTONE_WH_802c1594[];
extern const char str_SFX_SE2_EVT_STARSTON_802c15d4[];
extern const char str_SFX_VOICE_MARIO_HAPP_802c15f0[];
extern const char str_SFX_VOICE_MARIO_HAPP_802c1704[];
extern const char str_S_aji_14_802c16e0[];
extern const char str_S_jin04_802c16d8[];
extern const char str_aji_10_802c1798[];
extern const char str_gon_11_S_802c16cc[];
extern const char str_itemget_802c1634[];
extern const char str_muj_12_802c17a8[];
extern const char str_pik_01_802c17b8[];
extern const char str_pik_03_802c17b0[];
extern const char str_radiation_n64_802c161c[];
extern const char str_stageclear_802c16f8[];
extern const char str_sub_bg_802c162c[];
extern const char str_sub_clear_802c16ec[];
extern const char str_stg1_gon_33_18_802c163c[];
extern const char str_stg1_gon_34_802c171c[];
extern const char str_stg2_mri_e25_03_802c164c[];
extern const char str_stg2_mri_e25_04_802c1728[];
extern const char str_stg3_tou_475_01_802c1738[];
extern const char str_stg3_tou_475_802c165c[];
extern const char str_stg4_jin_10_02_802c16a4[];
extern const char str_stg4_jin_10_03_802c1788[];
extern const char str_stg4_jin_39_01_802c1748[];
extern const char str_stg4_jin_39_802c166c[];
extern const char str_stg5_dou_29_01_802c1678[];
extern const char str_stg5_dou_29_02_802c1758[];
extern const char str_stg6_pik_20_01_802c1768[];
extern const char str_stg6_pik_20_802c1688[];
extern const char str_stg7_aji_42_02_802c1694[];
extern const char str_stg7_aji_42_03_802c1778[];
extern const char str_sub_hikari_802c1610[];
extern const char str_usu_00_802c17a0[];
extern const char str_sjis_00000140[];
extern u32 unk_evt_803bac3c[];
extern s32 evt_eff64();
extern s32 evt_mapobj_get_position();
extern s32 evt_mapobj_rotate();
extern const char str_SFX_HIT_TREE1_802c1548[];
extern const char str_SFX_HIT_TREE1_LEAVES_802c1558[];
extern const char str_fall_leaf_n64_802c1570[];
extern u32 kusa_evt[];
extern u32 evt_sub_kusa_access_entry[];
extern u32 evt_sub_mail_vibration[];
extern u32 mail_evt[];
extern u32 mail_evt_nok_01[];
extern u32 mail_evt_gor_03[];
extern u32 mail_evt_gor_02[];
extern u32 mail_evt_gor_01[];
extern u32 mail_evt_gor_04[];
extern u32 mail_evt_rsh_03_a[];
extern u32 mail_evt_hom_00[];
extern u32 mail_evt_gor_02_2[];
extern u32 mail_evt_bom_01[];
extern u32 mail_evt_tik_00[];
extern u32 mail_evt_win_01[];
extern u32 mail_evt_tou_01[];
extern u32 mail_evt_usu_01[];
extern u32 mail_evt_gor_01_2[];
extern u32 mail_evt_rsh_03_a_2[];
extern u32 mail_evt_pik_00[];
extern u32 evt_sub_mail_event[];
extern s32 evt_cam_letter_box_disable();
extern s32 evt_mapobj_trans();
extern s32 evt_mario_balloon_bikkuri();
extern s32 evt_mario_balloon_off();
extern s32 evt_mario_get_mode();
extern s32 evt_mario_get_pos();
extern s32 evt_mario_key_onoff();
extern s32 evt_mario_normalize();
extern s32 evt_mario_wait_rideon();
extern s32 evt_msg_print();
extern s32 evt_party_yoshi_ride();
extern s32 evt_pouch_receive_mail();
extern s32 evt_run_case_evt();
extern s32 evt_snd_bgmoff();
extern s32 evt_snd_bgmon();
extern s32 evt_snd_sfxon_3d();
extern s32 evt_win_one_message();
extern const char str_BGM_FF_MAIL_RECEPTIO_802c17c0[];
extern const char str_SFX_GRASS_SHAKE1_802c1580[];
extern const char str_bom_01_802c1830[];
extern const char str_gor_01_802c1800[];
extern const char str_gor_02_802c17f8[];
extern const char str_gor_03_802c17f0[];
extern const char str_gor_04_802c1808[];
extern const char str_hom_00_802c1828[];
extern const char str_mail_first_hit_802c17d8[];
extern const char str_nok_01_802c17e8[];
extern const char str_pik_00_802c1858[];
extern const char str_rsh_02_a_802c1810[];
extern const char str_rsh_03_a_802c181c[];
extern const char str_tik_00_802c1838[];
extern const char str_tou_01_802c1848[];
extern const char str_usu_01_802c1850[];
extern const char str_win_01_802c1840[];

/* Recovered target evt_sub tree data/event family. */
u32 tree_data[75] = {
    0xF24A7A80, 0xF24A7A80, 0xF24A7A80, 0xF24A785D, 0xF24A8049, 0xF24A80FE,
    0xF24A75D1, 0xF24A8733, 0xF24A88C4, 0xF24A7415, 0xF24A8BDB, 0xF24A8DFC,
    0xF24A745E, 0xF24A8ADD, 0xF24A8CD2, 0xF24A77C2, 0xF24A817C, 0xF24A8212,
    0xF24A7D5D, 0xF24A723B, 0xF24A7092, 0xF24A836A, 0xF24A6193, 0xF24A5D8B,
    0xF24A8825, 0xF24A5401, 0xF24A4E36, 0xF24A89CA, 0xF24A4E01, 0xF24A47CD,
    0xF24A86E8, 0xF24A528C, 0xF24A4DEA, 0xF24A80AD, 0xF24A5EA6, 0xF24A5D12,
    0xF24A7943, 0xF24A6DD7, 0xF24A6FDE, 0xF24A72CF, 0xF24A7BA7, 0xF24A80E4,
    0xF24A6F7C, 0xF24A839E, 0xF24A8ABE, 0xF24A704B, 0xF24A8429, 0xF24A8B8B,
    0xF24A73CB, 0xF24A8042, 0xF24A86E5, 0xF24A787B, 0xF24A7A48, 0xF24A7F9D,
    0xF24A7CD6, 0xF24A7497, 0xF24A7888, 0xF24A7F5C, 0xF24A718F, 0xF24A7476,
    0xF24A7FB7, 0xF24A719F, 0xF24A73DB, 0xF24A7EE8, 0xF24A7331, 0xF24A74D6,
    0xF24A7D70, 0xF24A75A1, 0xF24A76BA, 0xF24A7BCD, 0xF24A7848, 0xF24A78D7,
    0xF24A7A80, 0xF24A7A80, 0xF24A7A80
};

u32 tree_evt[251] = {
    0x00010063, 0x00000000, 0x00020032, 0xFBD3E280, 0x00000000, 0x00020019,
    0xFE363C83, 0x00000000, 0x00020032, 0xFBD3E280, 0x00000001, 0x00000021,
    0x00020019, 0xFE363C84, 0x00000000, 0x00020032, 0xFBD3E280, 0x00000001,
    0x00000021, 0x00020018, 0xFBD3E280, 0x00000001, 0x0005005B, (u32)evt_mapobj_get_position,
    0xFE363C82, 0xFE363C8D, 0xFE363C8E, 0xFE363C8F, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_HIT_TREE1_802c1548, 0xFE363C8D, 0xFE363C8E, 0xFE363C8F, 0x00000000, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_HIT_TREE1_LEAVES_802c1558, 0xFE363C8D, 0xFE363C8E, 0xFE363C8F, 0x00000000,
    0x0005005B, (u32)evt_mapobj_get_position, 0xFE363C82, 0xFE363C8D, 0xFE363C8E, 0xFE363C8F,
    0x0005005B, (u32)evt_mapobj_get_position, 0xFE363C83, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00020032, 0xFE363C8E, 0xFE363C8B, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00020032, 0xFE363C8F, 0xFE363C8C,
    0x00020036, 0xFE363C8E, 0x00000019, 0x00010022, 0xFE363C80, 0x00010024,
    0x00000000, 0x000F005B, (u32)evt_eff64, 0x00000000, (u32)(char*)str_fall_leaf_n64_802c1570, 0x00000000,
    0xFE363C8D, 0xFE363C8E, 0xFE363C8F, 0x0000003C, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010024,
    0x00000001, 0x000F005B, (u32)evt_eff64, 0x00000000, (u32)(char*)str_fall_leaf_n64_802c1570, 0x00000003,
    0xFE363C8D, 0xFE363C8E, 0xFE363C8F, 0x0000003C, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010024,
    0x00000002, 0x000F005B, (u32)evt_eff64, 0x00000000, (u32)(char*)str_fall_leaf_n64_802c1570, 0x00000004,
    0xFE363C8D, 0xFE363C8E, 0xFE363C8F, 0x0000003C, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010024,
    0x00000003, 0x000F005B, (u32)evt_eff64, 0x00000000, (u32)(char*)str_fall_leaf_n64_802c1570, 0x00000004,
    0xFE363C8D, 0xFE363C8E, 0xFE363C8F, 0x0000003C, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010024,
    0x00000004, 0x000F005B, (u32)evt_eff64, 0x00000000, (u32)(char*)str_fall_leaf_n64_802c1570, 0x00000003,
    0xFE363C8D, 0xFE363C8E, 0xFE363C8F, 0x0000003C, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010024,
    0x00000005, 0x000F005B, (u32)evt_eff64, 0x00000000, (u32)(char*)str_fall_leaf_n64_802c1570, 0x00000002,
    0xFE363C8D, 0xFE363C8E, 0xFE363C8F, 0x0000003C, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010024,
    0x00000006, 0x000F005B, (u32)evt_eff64, 0x00000000, (u32)(char*)str_fall_leaf_n64_802c1570, 0x00000002,
    0xFE363C8D, 0xFE363C8E, 0xFE363C8F, 0x0000003C, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000031,
    0x00000021, 0x00020019, 0xFE363C85, 0x00000000, 0x0001005C, 0xFE363C85,
    0x00000021, 0x0001003E, (u32)tree_data, 0x00010005, 0x00000019, 0x00030041,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00020019, 0xFE363C82, 0x00000000,
    0x0006005B, (u32)evt_mapobj_rotate, 0x00000001, 0xFE363C82, 0xF24A7A80, 0xF24A7A80,
    0xFE363C8A, 0x00000021, 0x00020019, 0xFE363C83, 0x00000000, 0x0006005B,
    (u32)evt_mapobj_trans, 0x00000001, 0xFE363C83, 0xFE363C8B, 0xF24A7A80, 0xF24A7A80,
    0x00000021, 0x00020019, 0xFE363C84, 0x00000000, 0x0006005B, (u32)evt_mapobj_trans,
    0x00000001, 0xFE363C84, 0xFE363C8C, 0xF24A7A80, 0xF24A7A80, 0x00000021,
    0x00010009, 0x00000001, 0x00000006, 0x00000002, 0x00000001
};

u32 evt_sub_tree_access_entry[28] = {
    0x0001003E, 0xFE363C80, 0x00010005, 0x00000000, 0x00030041, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00030041, 0xFE363C83, 0xFE363C84, 0xFE363C85,
    0x00020018, 0xFE363C80, 0x00000007, 0x00000007, 0x00000021, 0x0007005B,
    (u32)evt_run_case_evt, 0x0000000D, 0x00000001, 0xFE363C81, 0x00000000, (u32)tree_evt,
    0x00000000, 0x00000006, 0x00000002, 0x00000001
};

/* Recovered target evt_sub safe event/event-like arrays, in target order. */
u32 kusa_evt[43] = {
    0x00010063, 0x00000000, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C8A,
    0xFE363C8B, 0xFE363C8C, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_GRASS_SHAKE1_802c1580, 0xFE363C8A,
    0xFE363C8B, 0xFE363C8C, 0x00000000, 0x00020019, 0xFE363C83, 0x00000000,
    0x0001005C, 0xFE363C83, 0x00000021, 0x0001003E, (u32)kusa_data, 0x00010005,
    0x0000000C, 0x0001003F, 0xFE363C8A, 0x00020019, 0xFE363C82, 0x00000000,
    0x0006005B, (u32)evt_mapobj_trans, 0x00000001, 0xFE363C82, 0xFE363C8A, 0xF24A7A80,
    0xF24A7A80, 0x00000021, 0x00010009, 0x00000001, 0x00000006, 0x00000002,
    0x00000001
};

u32 evt_sub_kusa_access_entry[26] = {
    0x0001003E, 0xFE363C80, 0x00010005, 0x00000000, 0x00030041, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0001003F, 0xFE363C83, 0x00020018, 0xFE363C80,
    0x00000004, 0x00000007, 0x00000021, 0x0007005B, (u32)evt_run_case_evt, 0x00000009,
    0x00000001, 0xFE363C81, 0x00000000, (u32)kusa_evt, 0x00000000, 0x00000006,
    0x00000002, 0x00000001
};

/* Recovered target evt_sub Star Stone event; cross-object Shift-JIS dependency uses the reviewed C-safe linker identity. */
u32 evt_sub_starstone[1084] = {
    0x0002005B, (u32)evt_mario_key_onoff, 0x00000000, 0x0002005B, (u32)evt_party_stop, 0x00000000,
    0x00010022, 0xFE363C80, 0x00010024, 0x00000001, 0x0002005B, (u32)evt_pouch_get_starstone,
    0x00000001, 0x0003005B, (u32)evt_pouch_add_item, 0x00000072, 0xFE363C82, 0x00010024,
    0x00000002, 0x0002005B, (u32)evt_pouch_get_starstone, 0x00000002, 0x0003005B, (u32)evt_pouch_add_item,
    0x00000073, 0xFE363C82, 0x00010024, 0x00000003, 0x0002005B, (u32)evt_pouch_get_starstone,
    0x00000003, 0x0003005B, (u32)evt_pouch_add_item, 0x00000074, 0xFE363C82, 0x00010024,
    0x00000004, 0x0002005B, (u32)evt_pouch_get_starstone, 0x00000004, 0x0003005B, (u32)evt_pouch_add_item,
    0x00000075, 0xFE363C82, 0x00010024, 0x00000005, 0x0002005B, (u32)evt_pouch_get_starstone,
    0x00000005, 0x0003005B, (u32)evt_pouch_add_item, 0x00000076, 0xFE363C82, 0x00010024,
    0x00000006, 0x0002005B, (u32)evt_pouch_get_starstone, 0x00000006, 0x0003005B, (u32)evt_pouch_add_item,
    0x00000077, 0xFE363C82, 0x00010024, 0x00000007, 0x0002005B, (u32)evt_pouch_get_starstone,
    0x00000007, 0x0003005B, (u32)evt_pouch_add_item, 0x00000078, 0xFE363C82, 0x00010024,
    0xFFFFFFFF, 0x00000031, 0x00020018, 0xFE363C80, 0x00000001, 0x00020032,
    0xFBD3E281, 0x00000001, 0x00000021, 0x00020018, 0xFE363C80, 0x00000002,
    0x00020032, 0xFBD3E282, 0x00000001, 0x00000021, 0x00020018, 0xFE363C80,
    0x00000003, 0x00020032, 0xFBD3E283, 0x00000001, 0x00000021, 0x00020018,
    0xFE363C80, 0x00000004, 0x00020032, 0xFBD3E284, 0x00000001, 0x00000021,
    0x00020018, 0xFE363C80, 0x00000005, 0x00020032, 0xFBD3E285, 0x00000001,
    0x00000021, 0x00020018, 0xFE363C80, 0x00000006, 0x00020032, 0xFBD3E286,
    0x00000001, 0x00000021, 0x00020018, 0xFE363C80, 0x00000007, 0x00020032,
    0xFBD3E287, 0x00000001, 0x00000021, 0x00020018, 0xFE363C80, 0xFFFFFFFF,
    0x00020032, 0xFBD3E288, 0x00000001, 0x00000021, 0x0002005B, (u32)set_stop,
    0x00000000, 0x0000006B, 0x0001000A, 0x000025E4, 0x0002005B, (u32)set_stop,
    0x00000001, 0x0000006D, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_EVT_STARSTONE_WH_802c1594, 0x00000000,
    0x0006005B, (u32)evt_fade_entry, 0x0000000A, 0x0000012C, 0x000000FF, 0x000000FF,
    0x000000FF, 0x0001005B, (u32)evt_fade_end_wait, 0x00020018, 0xFBD3E288, 0x00000001,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_sjis_00000140, 0xFE363C82, 0xFE363C83, 0xFE363C84,
    0x00000020, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0x00000021, 0x0004005B, (u32)evt_sub_get_sincos, 0x00000000, 0xFE363C85,
    0xFE363C86, 0x0002003C, 0xFE363C85, 0xF24B1A80, 0x0002003C, 0xFE363C86,
    0xF24B1A80, 0x0002003A, 0xFE363C85, 0xFE363C82, 0x0002003A, 0xFE363C86,
    0xFE363C84, 0x0005005B, (u32)evt_item_set_position, 0xFE363C81, 0xFE363C85, 0xFE363C83,
    0xFE363C86, 0x0001005B, (u32)evt_mario_adjust_dir, 0x0001000A, 0x000001F4, 0x00020018,
    0xFBD3E288, 0x00000000, 0x0002005B, (u32)evt_mario_set_pose, (u32)(char*)str_M_N_3_8041fe14, 0x00000021,
    0x0002005B, (u32)evt_snd_bgmoff, 0x00000400, 0x0003005B, (u32)evt_snd_bgmon, 0x00000001,
    (u32)(char*)str_BGM_FF_GET_STARSTONE_802c15bc, 0x0001005B, (u32)bgm_start_wait, 0x0006005B, (u32)evt_fade_entry, 0x00000009,
    0x0000012C, 0x000000FF, 0x000000FF, 0x000000FF, 0x00020018, 0xFBD3E288,
    0x00000001, 0x000A005B, (u32)evt_cam3d_evt_set_npc_rel, (u32)(char*)str_sjis_00000140, 0x00000000, 0x00000032,
    0x0000018D, 0x00000000, 0x00000046, 0x00000019, 0x000007D0, 0x0000000B,
    0x00000020, 0x0009005B, (u32)evt_cam3d_evt_set_rel, 0x00000000, 0x00000032, 0x0000018D,
    0x00000000, 0x00000046, 0x00000019, 0x000007D0, 0x0000000B, 0x00000021,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_SE2_EVT_STARSTON_802c15d4, 0xFE363C85, 0xFE363C83, 0xFE363C86,
    0x00000000, 0x00020032, 0xFE363C87, 0xFE363C81, 0x00020034, 0xFE363C88,
    0xF24B0680, 0x00020034, 0xFE363C89, 0xFE363C83, 0x0005005B, (u32)evt_sub_intpl_msec_init,
    0x00000004, 0x00000000, 0x00000E10, 0x00000B11, 0x00010005, 0x00000000,
    0x0001005B, (u32)evt_sub_intpl_msec_get_value, 0x00020018, 0xFBD3E288, 0x00000001, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_sjis_00000140, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0x00000020,
    0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C82, 0xFE363C83, 0xFE363C84,
    0x00000021, 0x0004005B, (u32)evt_sub_get_sincos, 0xFE363C80, 0xFE363C85, 0xFE363C86,
    0x0002003C, 0xFE363C85, 0xFE363C88, 0x0002003C, 0xFE363C86, 0xFE363C88,
    0x0002003A, 0xFE363C85, 0xFE363C82, 0x0002003A, 0xFE363C86, 0xFE363C84,
    0x0005005B, (u32)evt_item_set_position, 0xFE363C87, 0xFE363C85, 0xFE363C89, 0xFE363C86,
    0x0003005B, (u32)stone_ry, 0xFE363C87, 0xFE363C80, 0x00020034, 0xFE363C82,
    0xF24A7B52, 0x00020034, 0xFE363C83, 0xF24A7BA1, 0x0003005B, (u32)unk_80053f10,
    0xFE363C82, 0xFE363C83, 0x0002003B, 0xFE363C88, 0xFE363C82, 0x0002003A,
    0xFE363C89, 0xFE363C83, 0x00010009, 0x00000001, 0x00020018, 0xFE363C81,
    0x00000000, 0x00000007, 0x00000021, 0x00000006, 0x00020018, 0xFBD3E288,
    0x00000001, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_sjis_00000140, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0x00000020, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x00000021, 0x00020035, 0xFE363C83, 0x00000030,
    0x0005005B, (u32)evt_item_set_position, 0xFE363C87, 0xFE363C82, 0xFE363C83, 0xFE363C84,
    0x00020019, 0xFBD3E288, 0x00000001, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_VOICE_MARIO_HAPP_802c15f0,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000021, 0x00020018,
    0xFBD3E288, 0x00000001, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_sjis_00000140, (u32)(char*)str_EM_I_1_802c1608,
    0x00000020, 0x0002005B, (u32)evt_mario_set_pose, (u32)(char*)str_M_P_2_8041fe1c, 0x00000021, 0x0005005B,
    (u32)evt_item_get_position, 0xFE363C87, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C81, 0x00000010, 0x000F005B, (u32)evt_eff64, (u32)(char*)str_sub_hikari_802c1610, (u32)(char*)str_radiation_n64_802c161c,
    0x0000000C, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00020035, 0xFE363C82, 0xFFFFFFF6, 0x000F005B, (u32)evt_eff, (u32)(char*)str_sub_bg_802c162c,
    (u32)(char*)str_itemget_802c1634, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0002005B, (u32)stone_bg, 0xFE363C87, 0x00020018, 0xFBD3E281,
    0x00000001, 0x0005005B, (u32)evt_msg_print, 0x00000000, (u32)(char*)str_stg1_gon_33_18_802c163c, 0x00000000,
    0x00000000, 0x00000021, 0x00020018, 0xFBD3E282, 0x00000001, 0x0005005B,
    (u32)evt_msg_print, 0x00000000, (u32)(char*)str_stg2_mri_e25_03_802c164c, 0x00000000, 0x00000000, 0x00000021,
    0x00020018, 0xFBD3E283, 0x00000001, 0x0005005B, (u32)evt_msg_print, 0x00000000,
    (u32)(char*)str_stg3_tou_475_802c165c, 0x00000000, 0x00000000, 0x00000021, 0x00020018, 0xFBD3E284,
    0x00000001, 0x0005005B, (u32)evt_msg_print, 0x00000000, (u32)(char*)str_stg4_jin_39_802c166c, 0x00000000,
    0x00000000, 0x00000021, 0x00020018, 0xFBD3E285, 0x00000001, 0x0005005B,
    (u32)evt_msg_print, 0x00000000, (u32)(char*)str_stg5_dou_29_01_802c1678, 0x00000000, 0x00000000, 0x00000021,
    0x00020018, 0xFBD3E286, 0x00000001, 0x0005005B, (u32)evt_msg_print, 0x00000000,
    (u32)(char*)str_stg6_pik_20_802c1688, 0x00000000, 0x00000000, 0x00000021, 0x00020018, 0xFBD3E287,
    0x00000001, 0x0005005B, (u32)evt_msg_print, 0x00000000, (u32)(char*)str_stg7_aji_42_02_802c1694, 0x00000000,
    0x00000000, 0x00000021, 0x00020018, 0xFBD3E288, 0x00000001, 0x0005005B,
    (u32)evt_msg_print, 0x00000000, (u32)(char*)str_stg4_jin_10_02_802c16a4, 0x00000000, 0x00000000, 0x00000021,
    0x00010005, 0x00000000, 0x0002005B, (u32)get_stop, 0xFE363C80, 0x00020018,
    0xFE363C80, 0x00000001, 0x00000007, 0x00000021, 0x00010009, 0x00000001,
    0x00000006, 0x0002005B, (u32)evt_snd_bgmoff, 0x00000201, 0x0003005B, (u32)evt_snd_bgmon,
    0x00000120, 0x00000000, 0x0003005B, (u32)evt_snd_bgmon, 0x00000200, (u32)(char*)str_BGM_EVT_STAGE_CLEAR1_802c16b4,
    0x0002005B, (u32)evt_snd_envoff, 0x00000200, 0x0002005B, (u32)evt_eff_softdelete, (u32)(char*)str_sub_hikari_802c1610,
    0x0002005B, (u32)stone_bg_open, 0xFE363C87, 0x0000006B, 0x00020018, 0xFBD3E281,
    0x00000001, 0x00020032, 0xFE363C80, (u32)(char*)str_gon_11_S_802c16cc, 0x00000021, 0x00020018,
    0xFBD3E282, 0x00000001, 0x00020032, 0xFE363C80, (u32)(char*)str_S_8041fe24, 0x00000021,
    0x00020018, 0xFBD3E283, 0x00000001, 0x00020032, 0xFE363C80, (u32)(char*)str_S_8041fe24,
    0x00000021, 0x00020018, 0xFBD3E284, 0x00000001, 0x00020032, 0xFE363C80,
    (u32)(char*)str_S_jin04_802c16d8, 0x00000021, 0x00020018, 0xFBD3E285, 0x00000001, 0x00020032,
    0xFE363C80, (u32)(char*)str_S_8041fe24, 0x00000021, 0x00020018, 0xFBD3E286, 0x00000001,
    0x00020032, 0xFE363C80, (u32)(char*)str_S_8041fe24, 0x00000021, 0x00020018, 0xFBD3E287,
    0x00000001, 0x00020032, 0xFE363C80, (u32)(char*)str_S_aji_14_802c16e0, 0x00000021, 0x00020018,
    0xFBD3E288, 0x00000001, 0x00020032, 0xFE363C80, (u32)(char*)str_S_jin04_802c16d8, 0x00000021,
    0x00010009, 0x0000000A, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001,
    0xFE363C80, 0x00000001, 0x0000006D, 0x0006005B, (u32)evt_fade_entry, 0x00000022,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0001005B, (u32)evt_fade_end_wait,
    0x0001000A, 0x000003E8, 0x000F005B, (u32)evt_eff, (u32)(char*)str_sub_clear_802c16ec, (u32)(char*)str_stageclear_802c16f8,
    0x0000000A, 0x00000000, 0x00000064, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0001005B, (u32)stageclear_wait, 0x00020019, 0xFBD3E288, 0x00000001, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_VOICE_MARIO_HAPP_802c1704, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x00000021, 0x0001000A, 0x000007D0, 0x0006005B, (u32)evt_fade_entry, 0x0000001E,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0001005B, (u32)evt_fade_end_wait,
    0x0002005B, (u32)evt_item_delete, 0xFE363C87, 0x0001000A, 0x000003E8, 0x00020018,
    0xFBD3E281, 0x00000001, 0x0005005B, (u32)evt_msg_print, 0x00000000, (u32)(char*)str_stg1_gon_34_802c171c,
    0x00000000, 0x00000000, 0x00000021, 0x00020018, 0xFBD3E282, 0x00000001,
    0x0005005B, (u32)evt_msg_print, 0x00000000, (u32)(char*)str_stg2_mri_e25_04_802c1728, 0x00000000, 0x00000000,
    0x00000021, 0x00020018, 0xFBD3E283, 0x00000001, 0x0005005B, (u32)evt_msg_print,
    0x00000000, (u32)(char*)str_stg3_tou_475_01_802c1738, 0x00000000, 0x00000000, 0x00000021, 0x00020018,
    0xFBD3E284, 0x00000001, 0x0005005B, (u32)evt_msg_print, 0x00000000, (u32)(char*)str_stg4_jin_39_01_802c1748,
    0x00000000, 0x00000000, 0x00000021, 0x00020018, 0xFBD3E285, 0x00000001,
    0x0005005B, (u32)evt_msg_print, 0x00000000, (u32)(char*)str_stg5_dou_29_02_802c1758, 0x00000000, 0x00000000,
    0x00000021, 0x00020018, 0xFBD3E286, 0x00000001, 0x0005005B, (u32)evt_msg_print,
    0x00000000, (u32)(char*)str_stg6_pik_20_01_802c1768, 0x00000000, 0x00000000, 0x00000021, 0x00020018,
    0xFBD3E287, 0x00000001, 0x0005005B, (u32)evt_msg_print, 0x00000000, (u32)(char*)str_stg7_aji_42_03_802c1778,
    0x00000000, 0x00000000, 0x00000021, 0x00020018, 0xFBD3E288, 0x00000001,
    0x0005005B, (u32)evt_msg_print, 0x00000000, (u32)(char*)str_stg4_jin_10_03_802c1788, 0x00000000, 0x00000000,
    0x00000021, 0x0001000A, 0x000003E8, 0x00020018, 0xFBD3E281, 0x00000001,
    0x00020032, 0xFE363C80, 0x00000001, 0x00000021, 0x00020018, 0xFBD3E282,
    0x00000001, 0x00020032, 0xFE363C80, 0x00000004, 0x00000021, 0x00020018,
    0xFBD3E283, 0x00000001, 0x00020032, 0xFE363C80, 0x00000007, 0x00000021,
    0x00020018, 0xFBD3E284, 0x00000001, 0x00020032, 0xFE363C80, 0x0000000A,
    0x00000021, 0x00020018, 0xFBD3E285, 0x00000001, 0x00020032, 0xFE363C80,
    0x0000000D, 0x00000021, 0x00020018, 0xFBD3E286, 0x00000001, 0x00020032,
    0xFE363C80, 0x00000010, 0x00000021, 0x00020018, 0xFBD3E287, 0x00000001,
    0x00020032, 0xFE363C80, 0x00000013, 0x00000021, 0x00020018, 0xFBD3E288,
    0x00000001, 0x00020032, 0xFE363C80, 0x00000016, 0x00000021, 0x00020032,
    0xFBD3E28A, 0x00000000, 0x0001005E, (u32)unk_evt_803bac3c, 0x0001005B, (u32)evt_mario_init_camid,
    0x0002005B, (u32)evt_party_init_camid, 0x00000000, 0x0002005B, (u32)evt_party_init_camid, 0x00000001,
    0x00020018, 0xFBD3E281, 0x00000001, 0x0003005B, (u32)evt_snd_bgmoff_f, 0x00000200,
    0x000007D0, 0x0003005B, (u32)evt_snd_envoff_f, 0x00000200, 0x000007D0, 0x0001005B,
    (u32)evt_snd_sfx_all_stop, 0x0006005B, (u32)evt_fade_entry, 0x00000011, 0x000003E8, 0x00000000,
    0x00000000, 0x00000000, 0x0001005B, (u32)evt_fade_end_wait, 0x0001000A, 0x000007D0,
    0x0006005B, (u32)evt_fade_set_mapchange_type, 0x00000001, 0x00000037, 0x00000000, 0x00000036,
    0x00000000, 0x0003005B, (u32)evt_bero_mapchange, (u32)(char*)str_aji_10_802c1798, 0x00000000, 0x00000021,
    0x00020018, 0xFBD3E282, 0x00000001, 0x0003005B, (u32)evt_snd_bgmoff_f, 0x00000200,
    0x000007D0, 0x0003005B, (u32)evt_snd_envoff_f, 0x00000200, 0x000007D0, 0x0001005B,
    (u32)evt_snd_sfx_all_stop, 0x0006005B, (u32)evt_fade_entry, 0x00000011, 0x000003E8, 0x00000000,
    0x00000000, 0x00000000, 0x0001005B, (u32)evt_fade_end_wait, 0x0001000A, 0x000007D0,
    0x0006005B, (u32)evt_fade_set_mapchange_type, 0x00000001, 0x00000037, 0x00000000, 0x00000036,
    0x00000000, 0x0003005B, (u32)evt_bero_mapchange, (u32)(char*)str_aji_10_802c1798, 0x00000000, 0x00000021,
    0x00020018, 0xFBD3E283, 0x00000001, 0x0003005B, (u32)evt_snd_bgmoff_f, 0x00000200,
    0x000007D0, 0x0003005B, (u32)evt_snd_envoff_f, 0x00000200, 0x000007D0, 0x0001005B,
    (u32)evt_snd_sfx_all_stop, 0x0006005B, (u32)evt_fade_entry, 0x00000011, 0x000003E8, 0x00000000,
    0x00000000, 0x00000000, 0x0001005B, (u32)evt_fade_end_wait, 0x0001000A, 0x000007D0,
    0x0006005B, (u32)evt_fade_set_mapchange_type, 0x00000001, 0x00000037, 0x00000000, 0x00000036,
    0x00000000, 0x0003005B, (u32)evt_bero_mapchange, (u32)(char*)str_aji_10_802c1798, 0x00000000, 0x00000021,
    0x00020018, 0xFBD3E284, 0x00000001, 0x0006005B, (u32)evt_fade_set_mapchange_type, 0x00000001,
    0x00000011, 0x00000320, 0x00000010, 0x00000320, 0x0003005B, (u32)evt_bero_mapchange,
    (u32)(char*)str_usu_00_802c17a0, 0x00000000, 0x00000021, 0x00020018, 0xFBD3E285, 0x00000001,
    0x0006005B, (u32)evt_fade_set_mapchange_type, 0x00000001, 0x00000011, 0x00000320, 0x00000010,
    0x00000320, 0x0003005B, (u32)evt_bero_mapchange, (u32)(char*)str_muj_12_802c17a8, 0x00000000, 0x00000021,
    0x00020018, 0xFBD3E286, 0x00000001, 0x0006005B, (u32)evt_fade_set_mapchange_type, 0x00000000,
    0x00000011, 0x00000320, 0x00000010, 0x00000320, 0x0003005B, (u32)evt_bero_mapchange,
    (u32)(char*)str_pik_03_802c17b0, (u32)(char*)str_next_8041fe28, 0x00000021, 0x00020018, 0xFBD3E287, 0x00000001,
    0x0003005B, (u32)evt_snd_bgmoff_f, 0x00000200, 0x000007D0, 0x0003005B, (u32)evt_snd_envoff_f,
    0x00000200, 0x000007D0, 0x0001005B, (u32)evt_snd_sfx_all_stop, 0x0006005B, (u32)evt_fade_entry,
    0x00000011, 0x000003E8, 0x00000000, 0x00000000, 0x00000000, 0x0001005B,
    (u32)evt_fade_end_wait, 0x0001000A, 0x000007D0, 0x0006005B, (u32)evt_fade_set_mapchange_type, 0x00000001,
    0x00000037, 0x00000000, 0x00000036, 0x00000000, 0x0003005B, (u32)evt_bero_mapchange,
    (u32)(char*)str_pik_01_802c17b8, 0x00000000, 0x00000021, 0x00020018, 0xFBD3E288, 0x00000001,
    0x0003005B, (u32)evt_snd_bgmoff_f, 0x00000200, 0x000007D0, 0x0003005B, (u32)evt_snd_envoff_f,
    0x00000200, 0x000007D0, 0x0001005B, (u32)evt_snd_sfx_all_stop, 0x0006005B, (u32)evt_fade_entry,
    0x00000011, 0x000003E8, 0x00000000, 0x00000000, 0x00000000, 0x0001005B,
    (u32)evt_fade_end_wait, 0x0001000A, 0x000007D0, 0x0002005B, (u32)evt_eff_delete, (u32)(char*)str_sub_bg_802c162c,
    0x0002005B, (u32)evt_eff_delete, (u32)(char*)str_sub_clear_802c16ec, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000000, (u32)(char*)str_S_jin04_802c16d8, 0x00000001, 0x0002005B, (u32)evt_fade_reset, 0x00000001,
    0x0003005B, (u32)evt_npc_set_camid, (u32)(char*)str_sjis_00000140, 0x00000004, 0x0002005B, (u32)evt_mario_set_pose,
    (u32)(char*)str_M_D_2_8041fe30, 0x0006005B, (u32)evt_fade_entry, 0x00000010, 0x000003E8, 0x00000000,
    0x00000000, 0x00000000, 0x0001005B, (u32)evt_fade_end_wait, 0x00000021, 0x0002005B,
    (u32)evt_mario_key_onoff, 0x00000001, 0x00000002, 0x00000001
};

u32 evt_sub_mail_vibration[26] = {
    0x00010005, 0x0000000A, 0x00010005, 0x0000000A, 0x0003005B, (u32)evt_sub_rumble_onoff,
    0x00000000, 0x00000000, 0x0001000A, 0x00000032, 0x0003005B, (u32)evt_sub_rumble_onoff,
    0x00000001, 0x00000000, 0x0001000A, 0x00000032, 0x00000006, 0x0003005B,
    (u32)evt_sub_rumble_onoff, 0x00000002, 0x00000000, 0x0001000A, 0x000007D0, 0x00000006,
    0x00000002, 0x00000001
};

u32 mail_evt[80] = {
    0x0002005B, (u32)evt_cam_letter_box_disable, 0x00000001, 0x0002005B, (u32)evt_mario_key_onoff, 0x00000000,
    0x0001005B, (u32)evt_mario_normalize, 0x0003005B, (u32)evt_party_yoshi_ride, 0x00000000, 0x00000000,
    0x0001005B, (u32)evt_mario_wait_rideon, 0x0001005B, (u32)evt_mario_balloon_bikkuri, 0x0002005B, (u32)evt_snd_bgmoff,
    0x00000400, 0x0003005B, (u32)evt_snd_bgmon, 0x00000001, (u32)(char*)str_BGM_FF_MAIL_RECEPTIO_802c17c0, 0x0002005D,
    (u32)evt_sub_mail_vibration, 0xFE363C8A, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_win_one_message,
    0x00000000, 0x0001005F, 0xFE363C8A, 0x00010009, 0x00000001, 0x0003005B,
    (u32)evt_sub_rumble_onoff, 0x00000001, 0x00000000, 0x0001005B, (u32)evt_mario_balloon_off, 0x0002005B,
    (u32)evt_snd_bgmoff, 0x00000201, 0x0003005B, (u32)evt_snd_bgmon, 0x00000120, 0x00000000,
    0x00020018, 0xF8405BA5, 0x00000000, 0x00020032, 0xF8405BA5, 0x00000001,
    0x0005005B, (u32)evt_msg_print, 0x00000000, (u32)(char*)str_mail_first_hit_802c17d8, 0x00000000, 0x00000000,
    0x00000020, 0x00020018, 0xF8405BA6, 0x00000000, 0x0005005B, (u32)evt_msg_print,
    0x00000000, (u32)(char*)str_mail_first_hit_802c17d8, 0x00000000, 0x00000000, 0x00000021, 0x00000021,
    0x0002005B, (u32)evt_mario_key_onoff, 0x00000001, 0x0002005B, (u32)evt_cam_letter_box_disable, 0x00000000,
    0x00000002, 0x00000001
};

u32 mail_evt_nok_01[64] = {
    0x00020018, 0xF8405B87, 0x00000001, 0x00000002, 0x00000021, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010005,
    0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032, 0xFE363C8B,
    0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B, (u32)evt_mario_get_pos,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)evt_sub_check_intersect,
    0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0x000000BE, 0xFFFFF830,
    0x000000BE, 0x000007D0, 0xFE363C85, 0x00020018, 0xF5DE0180, 0x0000001B,
    0x00020018, 0xFE363C85, 0x00000001, 0x00000007, 0x00000021, 0x00000021,
    0x00010009, 0x00000001, 0x00000006, 0x00020032, 0xF8405B87, 0x00000001,
    0x0002005B, (u32)evt_pouch_receive_mail, 0x00000010, 0x0002005B, (u32)evt_pouch_receive_mail, 0x0000001A,
    0x0001005E, (u32)mail_evt, 0x00000002, 0x00000001
};

u32 mail_evt_gor_03[87] = {
    0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00010005, 0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032,
    0xFE363C8B, 0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B,
    (u32)evt_sub_check_intersect, 0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0xFFFFFF9C,
    0xFFFFF830, 0xFFFFFF9C, 0x000007D0, 0xFE363C85, 0x00020018, 0xFE363C85,
    0x00000001, 0x00020032, 0xFBD3E280, 0x00000000, 0x00020018, 0xF5DE0180,
    0x0000007D, 0x00020018, 0xF8405B88, 0x00000000, 0x00020032, 0xF8405B88,
    0x00000001, 0x0002005B, (u32)evt_pouch_receive_mail, 0x00000011, 0x00020032, 0xFBD3E280,
    0x00000001, 0x00000021, 0x00000021, 0x0003005B, (u32)irai_mail_check, 0x00000000,
    0xFE363C85, 0x00020018, 0xFE363C85, 0x00000001, 0x0003005B, (u32)irai_mail_check,
    0x00000001, 0xFE363C85, 0x00020032, 0xFBD3E280, 0x00000001, 0x00000021,
    0x00020018, 0xFBD3E280, 0x00000001, 0x0001005E, (u32)mail_evt, 0x00020032,
    0xFBD3E280, 0x00000000, 0x00000021, 0x00000021, 0x00010009, 0x00000001,
    0x00000006, 0x00000002, 0x00000001
};

u32 mail_evt_gor_02[87] = {
    0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00010005, 0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032,
    0xFE363C8B, 0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B,
    (u32)evt_sub_check_intersect, 0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0xFFFFFEE8,
    0xFFFFFFCE, 0xFFFFFF1A, 0x000000DC, 0xFE363C85, 0x00020018, 0xFE363C85,
    0x00000001, 0x00020032, 0xFBD3E280, 0x00000000, 0x00020018, 0xF5DE0180,
    0x000000AF, 0x00020018, 0xF8405B89, 0x00000000, 0x00020032, 0xF8405B89,
    0x00000001, 0x0002005B, (u32)evt_pouch_receive_mail, 0x00000012, 0x00020032, 0xFBD3E280,
    0x00000001, 0x00000021, 0x00000021, 0x0003005B, (u32)irai_mail_check, 0x00000000,
    0xFE363C85, 0x00020018, 0xFE363C85, 0x00000001, 0x0003005B, (u32)irai_mail_check,
    0x00000001, 0xFE363C85, 0x00020032, 0xFBD3E280, 0x00000001, 0x00000021,
    0x00020018, 0xFBD3E280, 0x00000001, 0x0001005E, (u32)mail_evt, 0x00020032,
    0xFBD3E280, 0x00000000, 0x00000021, 0x00000021, 0x00010009, 0x00000001,
    0x00000006, 0x00000002, 0x00000001
};

u32 mail_evt_gor_01[61] = {
    0x00020018, 0xF8405B8A, 0x00000001, 0x00000002, 0x00000021, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010005,
    0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032, 0xFE363C8B,
    0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B, (u32)evt_mario_get_pos,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)evt_sub_check_intersect,
    0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0x0000015E, 0xFFFFF830,
    0x0000015E, 0x000007D0, 0xFE363C85, 0x00020018, 0xF5DE0180, 0x000000E4,
    0x00020018, 0xFE363C85, 0x00000001, 0x00000007, 0x00000021, 0x00000021,
    0x00010009, 0x00000001, 0x00000006, 0x00020032, 0xF8405B8A, 0x00000001,
    0x0002005B, (u32)evt_pouch_receive_mail, 0x00000013, 0x0001005E, (u32)mail_evt, 0x00000002,
    0x00000001
};

u32 mail_evt_gor_04[97] = {
    0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00010005, 0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032,
    0xFE363C8B, 0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B,
    (u32)evt_sub_check_intersect, 0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0x0000001E,
    0xFFFFF830, 0x0000001E, 0x000007D0, 0xFE363C85, 0x0002005B, (u32)mail_evt_gor_04_keyoff_check,
    0xFE363C83, 0x0002001B, 0xFE363C83, 0x00000000, 0x00020032, 0xFE363C85,
    0x00000000, 0x00000021, 0x00020018, 0xFE363C85, 0x00000001, 0x00020032,
    0xFBD3E280, 0x00000000, 0x00020018, 0xF5DE0180, 0x00000118, 0x00020018,
    0xF8405B8B, 0x00000000, 0x00020032, 0xF8405B8B, 0x00000001, 0x0002005B,
    (u32)evt_pouch_receive_mail, 0x00000014, 0x00020032, 0xFBD3E280, 0x00000001, 0x00000021,
    0x00000021, 0x0003005B, (u32)irai_mail_check, 0x00000000, 0xFE363C85, 0x00020018,
    0xFE363C85, 0x00000001, 0x0003005B, (u32)irai_mail_check, 0x00000001, 0xFE363C85,
    0x00020032, 0xFBD3E280, 0x00000001, 0x00000021, 0x00020018, 0xFBD3E280,
    0x00000001, 0x0001005E, (u32)mail_evt, 0x00020032, 0xFBD3E280, 0x00000000,
    0x00000021, 0x00000021, 0x00010009, 0x00000001, 0x00000006, 0x00000002,
    0x00000001
};

u32 mail_evt_rsh_03_a[61] = {
    0x00020018, 0xF8405B8C, 0x00000001, 0x00000002, 0x00000021, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010005,
    0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032, 0xFE363C8B,
    0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B, (u32)evt_mario_get_pos,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)evt_sub_check_intersect,
    0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0xFFFFFF9C, 0xFFFFF830,
    0xFFFFFF9C, 0x000007D0, 0xFE363C85, 0x0002001D, 0xF5DE0180, 0x00000148,
    0x00020018, 0xFE363C85, 0x00000001, 0x00000007, 0x00000021, 0x00000021,
    0x00010009, 0x00000001, 0x00000006, 0x00020032, 0xF8405B8C, 0x00000001,
    0x0002005B, (u32)evt_pouch_receive_mail, 0x00000015, 0x0001005E, (u32)mail_evt, 0x00000002,
    0x00000001
};

u32 mail_evt_hom_00[61] = {
    0x00020018, 0xF8405B8D, 0x00000001, 0x00000002, 0x00000021, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010005,
    0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032, 0xFE363C8B,
    0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B, (u32)evt_mario_get_pos,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)evt_sub_check_intersect,
    0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0xFFFFFDA8, 0xFFFFF830,
    0xFFFFFDA8, 0x000007D0, 0xFE363C85, 0x00020018, 0xF5DE0180, 0x00000145,
    0x00020018, 0xFE363C85, 0x00000001, 0x00000007, 0x00000021, 0x00000021,
    0x00010009, 0x00000001, 0x00000006, 0x00020032, 0xF8405B8D, 0x00000001,
    0x0002005B, (u32)evt_pouch_receive_mail, 0x00000016, 0x0001005E, (u32)mail_evt, 0x00000002,
    0x00000001
};

u32 mail_evt_gor_02_2[61] = {
    0x00020018, 0xF8405B8E, 0x00000001, 0x00000002, 0x00000021, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010005,
    0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032, 0xFE363C8B,
    0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B, (u32)evt_mario_get_pos,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)evt_sub_check_intersect,
    0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0xFFFFFC18, 0xFFFFEF7E,
    0x000003E8, 0x00001CCE, 0xFE363C85, 0x00020018, 0xF5DE0180, 0x0000015C,
    0x00020018, 0xFE363C85, 0x00000001, 0x00000007, 0x00000021, 0x00000021,
    0x00010009, 0x00000001, 0x00000006, 0x00020032, 0xF8405B8E, 0x00000001,
    0x0002005B, (u32)evt_pouch_receive_mail, 0x00000017, 0x0001005E, (u32)mail_evt, 0x00000002,
    0x00000001
};

u32 mail_evt_bom_01[65] = {
    0x00020018, 0xF8405B8F, 0x00000001, 0x00000002, 0x00000021, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010005,
    0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032, 0xFE363C8B,
    0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B, (u32)evt_mario_get_pos,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)evt_sub_check_intersect,
    0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0xFFFFFF1A, 0xFFFFF830,
    0xFFFFFF1A, 0x000007D0, 0xFE363C85, 0x00020018, 0xF5DE0180, 0x00000162,
    0x00020018, 0xF8406AA2, 0x00000001, 0x00020018, 0xFE363C85, 0x00000001,
    0x00000007, 0x00000021, 0x00000021, 0x00000021, 0x00010009, 0x00000001,
    0x00000006, 0x00020032, 0xF8405B8F, 0x00000001, 0x0002005B, (u32)evt_pouch_receive_mail,
    0x00000018, 0x0001005E, (u32)mail_evt, 0x00000002, 0x00000001
};

u32 mail_evt_tik_00[80] = {
    0x00020018, 0xF8405B90, 0x00000001, 0x00000002, 0x00000021, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010005,
    0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032, 0xFE363C8B,
    0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B, (u32)evt_mario_get_pos,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)evt_sub_check_intersect,
    0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0x000001D6, 0xFFFFF830,
    0x000001D6, 0x000007D0, 0xFE363C85, 0x000A005B, (u32)evt_sub_check_intersect, 0xFE363C80,
    0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0xFFFFFE0C, 0xFFFFF830, 0xFFFFFE0C,
    0x000007D0, 0xFE363C86, 0x00020018, 0xF5DE0180, 0x0000017A, 0x00020018,
    0xFE363C85, 0x00000001, 0x00000007, 0x00000021, 0x00020018, 0xFE363C86,
    0x00000001, 0x00000007, 0x00000021, 0x00000021, 0x00010009, 0x00000001,
    0x00000006, 0x00020032, 0xF8405B90, 0x00000001, 0x0002005B, (u32)evt_pouch_receive_mail,
    0x00000019, 0x0002005B, (u32)evt_pouch_receive_mail, 0x00000021, 0x0001005E, (u32)mail_evt,
    0x00000002, 0x00000001
};

u32 mail_evt_win_01[61] = {
    0x00020018, 0xF8405B91, 0x00000001, 0x00000002, 0x00000021, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010005,
    0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032, 0xFE363C8B,
    0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B, (u32)evt_mario_get_pos,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)evt_sub_check_intersect,
    0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0x00000186, 0xFFFFF830,
    0x00000186, 0x000007D0, 0xFE363C85, 0x00020018, 0xF5DE0180, 0x00000057,
    0x00020018, 0xFE363C85, 0x00000001, 0x00000007, 0x00000021, 0x00000021,
    0x00010009, 0x00000001, 0x00000006, 0x00020032, 0xF8405B91, 0x00000001,
    0x0002005B, (u32)evt_pouch_receive_mail, 0x0000001B, 0x0001005E, (u32)mail_evt, 0x00000002,
    0x00000001
};

u32 mail_evt_tou_01[67] = {
    0x00020018, 0xF8405B92, 0x00000001, 0x00000002, 0x00000021, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010005,
    0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032, 0xFE363C8B,
    0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B, (u32)evt_mario_get_pos,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)evt_sub_check_intersect,
    0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0xFFFFFFCE, 0xFFFFF830,
    0xFFFFFFCE, 0x000007D0, 0xFE363C85, 0x00020018, 0xF5DE0180, 0x00000089,
    0x00020018, 0xFE363C85, 0x00000001, 0x00000007, 0x00000021, 0x00000021,
    0x00010009, 0x00000001, 0x00000006, 0x00020032, 0xF8405B92, 0x00000001,
    0x0002005B, (u32)evt_pouch_receive_mail, 0x0000001C, 0x00020032, 0xF8405C7A, 0x00000001,
    0x0002005B, (u32)evt_sub_save_playtime, 0xF5DE0185, 0x0001005E, (u32)mail_evt, 0x00000002,
    0x00000001
};

u32 mail_evt_usu_01[61] = {
    0x00020018, 0xF8405B93, 0x00000001, 0x00000002, 0x00000021, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010005,
    0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032, 0xFE363C8B,
    0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B, (u32)evt_mario_get_pos,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)evt_sub_check_intersect,
    0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0x0000006E, 0xFFFFF830,
    0x0000006E, 0x000007D0, 0xFE363C85, 0x00020018, 0xF5DE0180, 0x000000BB,
    0x00020018, 0xFE363C85, 0x00000001, 0x00000007, 0x00000021, 0x00000021,
    0x00010009, 0x00000001, 0x00000006, 0x00020032, 0xF8405B93, 0x00000001,
    0x0002005B, (u32)evt_pouch_receive_mail, 0x0000001D, 0x0001005E, (u32)mail_evt, 0x00000002,
    0x00000001
};

u32 mail_evt_gor_01_2[93] = {
    0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00010005, 0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032,
    0xFE363C8B, 0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B,
    (u32)evt_sub_check_intersect, 0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0xFFFFF830,
    0x000000C8, 0x000007D0, 0x000000C8, 0xFE363C85, 0x00020018, 0xFE363C85,
    0x00000001, 0x00020032, 0xFBD3E280, 0x00000000, 0x00020018, 0xF5DE0180,
    0x0000010E, 0x00020018, 0xF8405B94, 0x00000000, 0x00020032, 0xF8405B94,
    0x00000001, 0x0002005B, (u32)evt_pouch_receive_mail, 0x0000001E, 0x00020032, 0xF8405C7B,
    0x00000001, 0x0002005B, (u32)evt_sub_save_playtime, 0xF5DE018E, 0x00020032, 0xFBD3E280,
    0x00000001, 0x00000021, 0x00000021, 0x0003005B, (u32)irai_mail_check, 0x00000000,
    0xFE363C85, 0x00020018, 0xFE363C85, 0x00000001, 0x0003005B, (u32)irai_mail_check,
    0x00000001, 0xFE363C85, 0x00020032, 0xFBD3E280, 0x00000001, 0x00000021,
    0x00020018, 0xFBD3E280, 0x00000001, 0x0001005E, (u32)mail_evt, 0x00020032,
    0xFBD3E280, 0x00000000, 0x00000021, 0x00000021, 0x00010009, 0x00000001,
    0x00000006, 0x00000002, 0x00000001
};

u32 mail_evt_rsh_03_a_2[61] = {
    0x00020018, 0xF8405B95, 0x00000001, 0x00000002, 0x00000021, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010005,
    0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032, 0xFE363C8B,
    0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B, (u32)evt_mario_get_pos,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)evt_sub_check_intersect,
    0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0x00000190, 0xFFFFF830,
    0x00000190, 0x000007D0, 0xFE363C85, 0x00020018, 0xF5DE0180, 0x0000012F,
    0x00020018, 0xFE363C85, 0x00000001, 0x00000007, 0x00000021, 0x00000021,
    0x00010009, 0x00000001, 0x00000006, 0x00020032, 0xF8405B95, 0x00000001,
    0x0002005B, (u32)evt_pouch_receive_mail, 0x0000001F, 0x0001005E, (u32)mail_evt, 0x00000002,
    0x00000001
};

u32 mail_evt_pik_00[71] = {
    0x00020018, 0xF8405B96, 0x00000001, 0x00000002, 0x00000021, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010005,
    0x00000000, 0x00020032, 0xFE363C8A, 0xFE363C80, 0x00020032, 0xFE363C8B,
    0xFE363C81, 0x00020032, 0xFE363C8C, 0xFE363C82, 0x0005005B, (u32)evt_mario_get_pos,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000A005B, (u32)evt_sub_check_intersect,
    0xFE363C80, 0xFE363C82, 0xFE363C8A, 0xFE363C8C, 0xFFFFFFC4, 0xFFFFF830,
    0xFFFFFFC4, 0x000007D0, 0xFE363C85, 0x00020018, 0xF5DE0180, 0x00000162,
    0x00020018, 0xF8406AA2, 0x00000001, 0x00020018, 0xFE363C85, 0x00000001,
    0x00000007, 0x00000021, 0x00000021, 0x00000021, 0x00010009, 0x00000001,
    0x00000006, 0x00020032, 0xF8405B96, 0x00000001, 0x0002005B, (u32)evt_pouch_receive_mail,
    0x00000020, 0x00020032, 0xF8405C7C, 0x00000001, 0x0002005B, (u32)evt_sub_save_playtime,
    0xF5DE0197, 0x0001005E, (u32)mail_evt, 0x00000002, 0x00000001
};

u32 evt_sub_mail_event[123] = {
    0x0002005B, (u32)evt_sub_get_system_flag, 0xFE363C80, 0x0002001E, 0xFE363C80, 0x00001000,
    0x00000002, 0x00000021, 0x0002005B, (u32)evt_mario_get_mode, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x00000002, 0x00000021, 0x0002005B, (u32)evt_sub_get_mapname,
    0xFE363C80, 0x0002000C, 0xFE363C80, (u32)(char*)str_nok_01_802c17e8, 0x0001005E, (u32)mail_evt_nok_01,
    0x00000002, 0x00000021, 0x0002000C, 0xFE363C80, (u32)(char*)str_gor_03_802c17f0, 0x0001005E,
    (u32)mail_evt_gor_03, 0x00000002, 0x00000021, 0x0002000C, 0xFE363C80, (u32)(char*)str_gor_02_802c17f8,
    0x0001005C, (u32)mail_evt_gor_02, 0x0001005E, (u32)mail_evt_gor_02_2, 0x00000002, 0x00000021,
    0x0002000C, 0xFE363C80, (u32)(char*)str_gor_01_802c1800, 0x0001005C, (u32)mail_evt_gor_01_2, 0x0001005E,
    (u32)mail_evt_gor_01, 0x00000002, 0x00000021, 0x0002000C, 0xFE363C80, (u32)(char*)str_gor_04_802c1808,
    0x0001005E, (u32)mail_evt_gor_04, 0x00000002, 0x00000021, 0x0002000C, 0xFE363C80,
    (u32)(char*)str_rsh_02_a_802c1810, 0x0001005E, (u32)mail_evt_rsh_03_a, 0x00000002, 0x00000021, 0x0002000C,
    0xFE363C80, (u32)(char*)str_rsh_03_a_802c181c, 0x0001005E, (u32)mail_evt_rsh_03_a_2, 0x00000002, 0x00000021,
    0x0002000C, 0xFE363C80, (u32)(char*)str_hom_00_802c1828, 0x0001005E, (u32)mail_evt_hom_00, 0x00000002,
    0x00000021, 0x0002000C, 0xFE363C80, (u32)(char*)str_bom_01_802c1830, 0x0001005E, (u32)mail_evt_bom_01,
    0x00000002, 0x00000021, 0x0002000C, 0xFE363C80, (u32)(char*)str_tik_00_802c1838, 0x0001005E,
    (u32)mail_evt_tik_00, 0x00000002, 0x00000021, 0x0002000C, 0xFE363C80, (u32)(char*)str_win_01_802c1840,
    0x0001005E, (u32)mail_evt_win_01, 0x00000002, 0x00000021, 0x0002000C, 0xFE363C80,
    (u32)(char*)str_tou_01_802c1848, 0x0001005E, (u32)mail_evt_tou_01, 0x00000002, 0x00000021, 0x0002000C,
    0xFE363C80, (u32)(char*)str_usu_01_802c1850, 0x0001005E, (u32)mail_evt_usu_01, 0x00000002, 0x00000021,
    0x0002000C, 0xFE363C80, (u32)(char*)str_pik_00_802c1858, 0x0001005E, (u32)mail_evt_pik_00, 0x00000002,
    0x00000021, 0x00000002, 0x00000001
};
