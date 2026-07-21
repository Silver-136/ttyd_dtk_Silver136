#include "npc_event.h"
#include "event/evt_cmd.h"

extern char str_slave_0_802ed598[];
const char str_me_80421f30[] = "me";

s32 evtSetValue(EventEntry* event, s32 target, s32 value);
f32 evtSetFloat(EventEntry* event, s32 target, f32 value);
void* evtNpcNameToPtr(EventEntry* event, const void* name);
void* effNameToPtr(void* name);
void* camGetPtr(s32 camId);
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(mahoon_get_groupname) {
    s32* args = event->args;
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    evtSetValue(event, args[0], *(s32*)(*(s32*)((s32)npc + 0x28) + 4));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(npc_check_wall_stop) {
    s32* args = event->args;
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    if (*(u32*)((s32)npc + 0x2FC) != 0) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 _wait_blow_end(void* pEvt) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern s32 npcCheckBlow(void* npc);
    extern f32 float_18_80421fe4;
    extern f32 float_0_80421f8c;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, (void*)name);

    if (npcCheckBlow(npc) != 0) {
        *(f32*)((s32)npc + 0xF0) += float_18_80421fe4;
        return 0;
    }
    *(f32*)((s32)npc + 0xF0) = float_0_80421f8c;
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 pressCheck(int param_1) {
    extern void* hitCheckFilter(s32 flags, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz,
                                f32* outX, f32* outY, f32* outZ, f32* radius,
                                f32* outA, f32* outB, f32* outC);
    extern s32 kpaGetLevel(void);
    extern f32 float_5_80421f84;
    extern f32 float_0_80421f8c;
    extern f32 float_1_80421f88;
    extern f32 float_neg1_80421f90;
    s32* args = *(s32**)(param_1 + 0x18);
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);
    f32 a;
    f32 b;
    f32 c;
    f32 radius = float_5_80421f84;
    f32 d;
    f32 e;
    f32 f;
    void* hit;

    if (kpaGetLevel() == 0) {
        evtSetValue((void*)param_1, args[0], 0);
        return 2;
    }
    hit = hitCheckFilter(0, *(f32*)((s32)npc + 0x8C),
                         *(f32*)((s32)npc + 0x90) + float_1_80421f88,
                         *(f32*)((s32)npc + 0x94), float_0_80421f8c,
                         float_neg1_80421f90, float_0_80421f8c,
                         &a, &b, &c, &radius, &d, &e, &f);
    if (hit != NULL) {
        evtSetValue((void*)param_1, args[0], 1);
    } else {
        evtSetValue((void*)param_1, args[0], 0);
    }
    return 2;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


s32 _2d_dead_jump(EventEntry* event, s32 first) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern void* evtNpcNameToPtr(EventEntry* event, const void* name);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern f32 angleABf(f32 x0, f32 z0, f32 x1, f32 z1);
    extern double sin(double);
    extern double cos(double);
    extern f32 float_1000_80421fd0;
    extern f32 float_6p2832_80421fdc;
    extern f32 float_360_80421fe0;
    extern f32 float_0_80421f8c;
    extern f32 float_1_80421f88;
    extern f32 float_0p5_80421fd4;
    extern f32 float_neg980_80421fd8;
    s32* args = event->args;
    void* name = (void*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    f32 time = (f32)evtGetValue(event, args[4]) / float_1000_80421fd0;
    f32 speed = evtGetFloat(event, args[5]);
    f32 height = evtGetFloat(event, args[6]);
    f32 floorY = (f32)evtGetValue(event, args[7]);
    void* npc = evtNpcNameToPtr(event, name);
    s32 ret = 0;

    *(u32*)((s32)npc + 0x1D4) |= 0x10;
    *(u32*)((s32)npc + 0x1D4) |= 0x1000;

    if (first == 0) {
        f32 angle = (float_6p2832_80421fdc * *(f32*)((s32)npc + 0x1AC)) / float_360_80421fe0;
        *(f32*)((s32)npc + 0x1C0) += *(f32*)((s32)npc + 0x21C) * *(f32*)((s32)npc + 0x1B0) * (f32)sin(angle);
        *(f32*)((s32)npc + 0x1C4) += *(f32*)((s32)npc + 0x21C) * *(f32*)((s32)npc + 0x1B0) * -(f32)cos(angle);
        if (*(f32*)((s32)npc + 0x90) < floorY) {
            *(f32*)((s32)npc + 0x1B0) = float_0_80421f8c;
            *(f32*)((s32)npc + 0x1B8) = float_0_80421f8c;
            *(f32*)((s32)npc + 0x1C8) = float_1_80421f88;
            *(f32*)((s32)npc + 0x1C4) = float_1_80421f88;
            ret = 1;
        }
    } else {
        f32 diff[3];
        f32 dy;

        *(u32*)npc |= 0x20000;
        *(u32*)npc &= 0xBFFFFFEF;
        *(s32*)((s32)npc + 0x18C) = 0;
        *(s32*)((s32)npc + 0x188) = 0;
        *(s32*)((s32)npc + 0x17C) = 0;
        *(s32*)((s32)npc + 0x178) = 0;
        *(s32*)((s32)npc + 0x108) = 0;

        *(f32*)((s32)npc + 0x164) = x;
        *(f32*)((s32)npc + 0x168) = y;
        *(f32*)((s32)npc + 0x16C) = z;
        *(u32*)((s32)npc + 0x158) = *(u32*)((s32)npc + 0x8C);
        *(u32*)((s32)npc + 0x15C) = *(u32*)((s32)npc + 0x90);
        *(u32*)((s32)npc + 0x160) = *(u32*)((s32)npc + 0x94);
        PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x158), diff);
        *(f32*)((s32)npc + 0x1AC) = angleABf(float_0_80421f8c, float_0_80421f8c, diff[0], diff[2]);
        if (time <= float_0_80421f8c) {
            *(f32*)((s32)npc + 0x1B0) = speed;
        } else {
            f32 dx = x - *(f32*)((s32)npc + 0x8C);
            if (dx < float_0_80421f8c) {
                dx = -dx;
            }
            *(f32*)((s32)npc + 0x1B0) = dx / time;
        }

        *(f32*)((s32)npc + 0x1C0) = float_0_80421f8c;
        *(f32*)((s32)npc + 0x1B8) = float_0_80421f8c;
        *(f32*)((s32)npc + 0x1C4) = float_0_80421f8c;
        *(f32*)((s32)npc + 0x1C8) = float_1_80421f88;
        dy = *(f32*)((s32)npc + 0x168) - *(f32*)((s32)npc + 0x15C);
        if (height == float_0_80421f8c) {
            if (float_0_80421f8c <= dy) {
                return 1;
            }
        } else {
            f32 h = height;
            if (float_0_80421f8c < dy) {
                h = height + dy;
            }
            *(f32*)((s32)npc + 0x1B8) = float_0_80421f8c;
            if (time == float_0_80421f8c) {
                *(f32*)((s32)npc + 0x1B8) = height;
            } else {
                f32 half = float_0p5_80421fd4 * time;
                f32 accel = -(time * float_neg980_80421fd8 * *(f32*)((s32)npc + 0x1C4) * time) / time;
                f32 denom;
                *(f32*)((s32)npc + 0x1B8) = accel;
                denom = accel * half + half * float_neg980_80421fd8 * *(f32*)((s32)npc + 0x1C4) * half;
                if (float_0_80421f8c != denom) {
                    *(f32*)((s32)npc + 0x1C8) = h / denom;
                }
            }
        }
    }
    return ret;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _2d_get_dead_jump_offset(int param_1) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* marioGetPtr(void);
    extern f32 float_0_80421f8c;
    extern f32 float_neg1_80421f90;
    extern f32 float_1_80421f88;
    extern f32 float_20_80421fcc;
    s32* args = *(s32**)(param_1 + 0x18);
    s32 npcName = evtGetValue((void*)param_1, args[0]);
    void* npc = evtNpcNameToPtr((void*)param_1, (void*)npcName);
    s32 dst = args[1];
    void* mario = marioGetPtr();
    f32 dir;

    if (*(f32*)((s32)mario + 0x8C) == *(f32*)((s32)npc + 0x8C)) {
        dir = float_0_80421f8c;
    } else if (*(f32*)((s32)mario + 0x8C) > *(f32*)((s32)npc + 0x8C)) {
        dir = float_neg1_80421f90;
    } else {
        dir = float_1_80421f88;
    }
    evtSetValue((void*)param_1, dst, (s32)(float_20_80421fcc * dir));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 killer_make_name(void* pEvt) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern char* strcpy(char* dst, const char* src);
    extern s32 strlen(const char* str);
    static char name[64];
    s32* args = *(s32**)((s32)pEvt + 0x18);
    void* npc = evtNpcNameToPtr(pEvt, str_me_80421f30);
    s32 index = evtGetValue(pEvt, args[0]);
    s32 len;

    strcpy(name, (char*)((s32)npc + 8));
    len = strlen(name);
    name[len] = index + 0x31;
    name[len + 1] = 0;
    evtSetValue(pEvt, args[1], (s32)name);
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(piders_get_height) {
    s32* args = event->args;
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    evtSetFloat(event, args[0], *(f32*)((s32)npc + 0x210));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void piders_draw_yarn_sub(s32 unused, void* npc) {
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 comptype, s32 compsize, s32 frac);
    extern void GXSetNumChans(s32 nChans);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambsrc, s32 matsrc, s32 lightmask, s32 diff_fn, s32 attn_fn);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXSetNumTexGens(s32 nTexGens);
    extern void GXSetNumTevStages(s32 nStages);
    extern void GXSetTevColorOp(s32 tevstage, s32 tevop, s32 tevbias, s32 tevscale, s32 clamp, s32 tevregid);
    extern void GXSetTevColorIn(s32 tevstage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaOp(s32 tevstage, s32 tevop, s32 tevbias, s32 tevscale, s32 clamp, s32 tevregid);
    extern void GXSetTevAlphaIn(s32 tevstage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevOrder(s32 tevstage, s32 texcoord, s32 texmap, s32 color);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 nverts);
    extern u32 vec3_802ed730[];
    extern u32 vec3_802ed73c[];
    extern u32 dat_80421f5c;
    extern f32 float_30_80421fb4;
    extern f32 float_1_80421f88;
    extern f32 float_5_80421f84;
    u32 posA[3];
    u32 posB[3];
    u32 a[3];
    u32 b[3];
    u32 color;
    void* cam;

    cam = camGetPtr(4);
    posB[0] = vec3_802ed730[0];
    posB[1] = vec3_802ed730[1];
    posB[2] = vec3_802ed730[2];
    posA[0] = vec3_802ed73c[0];
    posA[1] = vec3_802ed73c[1];
    posA[2] = vec3_802ed73c[2];
    *(f32*)&posB[0] = *(f32*)((s32)npc + 0x1FC);
    *(f32*)&posB[1] = *(f32*)((s32)npc + 0x200) + float_30_80421fb4;
    *(f32*)&posB[2] = *(f32*)((s32)npc + 0x204);
    *(f32*)&posA[0] = *(f32*)((s32)npc + 0x8C);
    *(f32*)&posA[1] = *(f32*)((s32)npc + 0x90) + float_30_80421fb4;
    *(f32*)&posA[2] = *(f32*)((s32)npc + 0x94);
    b[0] = posB[0];
    b[1] = posB[1];
    b[2] = posB[2];
    a[0] = posA[0];
    a[1] = posA[1];
    a[2] = posA[2];

    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
    color = dat_80421f5c;
    GXSetChanMatColor(4, &color);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 10);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 7, 7, 5);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = *(f32*)&b[0] - float_1_80421f88;
    *(volatile f32*)0xCC008000 = *(f32*)&b[1];
    *(volatile f32*)0xCC008000 = *(f32*)&b[2] - float_5_80421f84;
    *(volatile f32*)0xCC008000 = *(f32*)&b[0] + float_1_80421f88;
    *(volatile f32*)0xCC008000 = *(f32*)&b[1];
    *(volatile f32*)0xCC008000 = *(f32*)&b[2] - float_5_80421f84;
    *(volatile f32*)0xCC008000 = *(f32*)&a[0] + float_1_80421f88;
    *(volatile f32*)0xCC008000 = *(f32*)&a[1];
    *(volatile f32*)0xCC008000 = *(f32*)&a[2] - float_5_80421f84;
    *(volatile f32*)0xCC008000 = *(f32*)&a[0] - float_1_80421f88;
    *(volatile f32*)0xCC008000 = *(f32*)&a[1];
    *(volatile f32*)0xCC008000 = *(f32*)&a[2] - float_5_80421f84;
}

s32 piders_draw_yawn(int param_1) {
    extern s32 dispEntry(s32 camera, s32 order, void* callback, void* data, f32 priority);
    extern f32 float_0_80421f8c;
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);

    dispEntry(4, 1, piders_draw_yarn_sub, npc, float_0_80421f8c);
    return 2;
}

s32 kamec_fire_magic(int param_1) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* effMagic1N64Entry(s32 arg0, s32 time, f32 x, f32 y, f32 z, f32 a, f32 b, f32 c);
    extern void effSetName(void* eff, void* name);
    s32* args = *(s32**)(param_1 + 0x18);
    s32 x = evtGetValue((void*)param_1, args[0]);
    s32 y = evtGetValue((void*)param_1, args[1]);
    s32 z = evtGetValue((void*)param_1, args[2]);
    s32 a = evtGetValue((void*)param_1, args[3]);
    s32 b = evtGetValue((void*)param_1, args[4]);
    s32 c = evtGetValue((void*)param_1, args[5]);
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);
    void* eff = effMagic1N64Entry(0, 60, (f32)x, (f32)y, (f32)z, (f32)a, (f32)b, (f32)c);

    effSetName(eff, (void*)((s32)npc + 8));
    evtSetValue((void*)param_1, args[6], (s32)eff);
    return 2;
}

USER_FUNC(kamec_fire_magic_delete) {
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    void* eff = effNameToPtr((void*)((s32)npc + 8));
    if (eff != 0) {
        *(s32*)(*(s32*)((s32)eff + 0xC) + 0x30) = 1;
    }
    return 2;
}

s32 chain_main(EventEntry* event) {
    extern f32 float_0p25_80421fc0;
    extern f32 float_10_80421fc4;
    extern f32 float_300_80421fc8;
    extern f32 float_1_80421f88;
    extern double sqrt(double);
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    void* tail = *(void**)((s32)npc + 0x33C);
    f32 dx = *(f32*)((s32)npc + 0x8C) - *(f32*)((s32)tail + 0x8C);
    f32 dy = *(f32*)((s32)npc + 0x90) - *(f32*)((s32)tail + 0x90);
    f32 dz = *(f32*)((s32)npc + 0x94) - *(f32*)((s32)tail + 0x94);
    f32 len = (f32)sqrt(dx * dx + dy * dy + dz * dz);
    f32 step = len * float_0p25_80421fc0;
    f32 fall;
    s32 i;
    void* base;

    if (step < float_10_80421fc4) {
        step = float_10_80421fc4;
    }
    fall = float_300_80421fc8 / (step * step);

    {
        void* seg = *(void**)((s32)npc + 0x330);
        f32 x = *(f32*)((s32)seg + 0x8C);
        f32 y = *(f32*)((s32)seg + 0x90) - fall;
        f32 z = *(f32*)((s32)seg + 0x94);
        f32 vx = *(f32*)((s32)npc + 0x8C) - x;
        f32 vy = *(f32*)((s32)npc + 0x90) - y;
        f32 vz = *(f32*)((s32)npc + 0x94) - z;
        f32 dist = (f32)sqrt(vx * vx + vy * vy + vz * vz);
        f32 scale = float_1_80421f88 - step / dist;
        y += vy * scale;
        if (y < *(f32*)((s32)npc + 0x200)) {
            y = *(f32*)((s32)npc + 0x200);
        }
        *(f32*)((s32)seg + 0x8C) = x + vx * scale;
        *(f32*)((s32)seg + 0x90) = y;
        *(f32*)((s32)seg + 0x94) = z + vz * scale;
    }

    base = npc;
    for (i = 0; i < 2; i++, base = (void*)((s32)base + 4)) {
        void* prev = *(void**)((s32)base + 0x330);
        void* seg = *(void**)((s32)base + 0x334);
        f32 x = *(f32*)((s32)seg + 0x8C);
        f32 y = *(f32*)((s32)seg + 0x90) - fall;
        f32 z = *(f32*)((s32)seg + 0x94);
        f32 vx = *(f32*)((s32)prev + 0x8C) - x;
        f32 vy = *(f32*)((s32)prev + 0x90) - y;
        f32 vz = *(f32*)((s32)prev + 0x94) - z;
        f32 dist = (f32)sqrt(vx * vx + vy * vy + vz * vz);
        f32 scale = float_1_80421f88 - step / dist;
        y += vy * scale;
        if (y < *(f32*)((s32)npc + 0x200)) {
            y = *(f32*)((s32)npc + 0x200);
        }
        *(f32*)((s32)seg + 0x8C) = x + vx * scale;
        *(f32*)((s32)seg + 0x90) = y;
        *(f32*)((s32)seg + 0x94) = z + vz * scale;
    }

    base = (void*)((s32)npc + 0xC);
    for (i = 3; i > 0; i--, base = (void*)((s32)base - 4)) {
        void* seg = *(void**)((s32)base + 0x32C);
        void* prev = *(void**)((s32)base + 0x330);
        f32 x = *(f32*)((s32)seg + 0x8C);
        f32 y = *(f32*)((s32)seg + 0x90) - fall;
        f32 z = *(f32*)((s32)seg + 0x94);
        f32 vx = *(f32*)((s32)prev + 0x8C) - x;
        f32 vy = *(f32*)((s32)prev + 0x90) - y;
        f32 vz = *(f32*)((s32)prev + 0x94) - z;
        f32 dist = (f32)sqrt(vx * vx + vy * vy + vz * vz);
        f32 scale = float_1_80421f88 - step / dist;
        y += vy * scale;
        if (y < *(f32*)((s32)npc + 0x200)) {
            y = *(f32*)((s32)npc + 0x200);
        }
        *(f32*)((s32)seg + 0x8C) = x + vx * scale;
        *(f32*)((s32)seg + 0x90) = y;
        *(f32*)((s32)seg + 0x94) = z + vz * scale;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(camGetPos) {
    s32* args = event->args;
    void* cam = camGetPtr(4);
    evtSetFloat(event, args[0], *(f32*)((s32)cam + 0xC));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 cliffCheck(int param_1) {
    extern void* hitCheckFilter(s32 flags, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz,
                                f32* outX, f32* outY, f32* outZ, f32* radius,
                                f32* outA, f32* outB, f32* outC);
    extern f32 npcTransRytoFaceDir(void* npc);
    extern f32 float_90_80421fbc;
    extern f32 float_5_80421f84;
    extern f32 float_0_80421f8c;
    extern f32 float_1_80421f88;
    extern f32 float_neg1_80421f90;
    s32* args = *(s32**)(param_1 + 0x18);
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);
    f32 dir;
    f32 a;
    f32 b;
    f32 c;
    f32 radius = float_5_80421f84;
    f32 d;
    f32 e;
    f32 f;
    void* down;
    void* ahead;
    s32 result;

    if (npcTransRytoFaceDir(npc) == float_90_80421fbc) {
        dir = float_1_80421f88;
    } else {
        dir = float_neg1_80421f90;
    }
    down = hitCheckFilter(0, *(f32*)((s32)npc + 0x8C),
                          *(f32*)((s32)npc + 0x90) + float_1_80421f88,
                          *(f32*)((s32)npc + 0x94), float_0_80421f8c,
                          float_neg1_80421f90, float_0_80421f8c,
                          &a, &b, &c, &radius, &d, &e, &f);
    ahead = hitCheckFilter(0, *(f32*)((s32)npc + 0x8C),
                           *(f32*)((s32)npc + 0x90) + float_1_80421f88,
                           *(f32*)((s32)npc + 0x94), dir, float_neg1_80421f90,
                           float_0_80421f8c, &a, &b, &c, &radius, &d, &e, &f);
    if (down != NULL && ahead != NULL) {
        result = 0;
    } else if (down != NULL && ahead == NULL) {
        result = 1;
    } else {
        result = 2;
    }
    evtSetValue((void*)param_1, args[0], result);
    return 2;
}

s32 upperCheck(int param_1) {
    extern void* hitCheckFilter(s32 flags, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz,
                                f32* outX, f32* outY, f32* outZ, f32* radius,
                                f32* outA, f32* outB, f32* outC);
    extern f32 float_5_80421f84;
    extern f32 float_0_80421f8c;
    extern f32 float_15_80421fb0;
    extern f32 float_30_80421fb4;
    extern f32 float_2_80421fb8;
    s32* args = *(s32**)(param_1 + 0x18);
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);
    f32 a;
    f32 b;
    f32 c;
    f32 radius = float_5_80421f84;
    f32 d;
    f32 e;
    f32 f;
    void* hit1;
    void* hit2;

    hit1 = hitCheckFilter(0, *(f32*)((s32)npc + 0x8C) - float_15_80421fb0,
                          *(f32*)((s32)npc + 0x90) + float_30_80421fb4,
                          *(f32*)((s32)npc + 0x94), float_0_80421f8c,
                          float_2_80421fb8, float_0_80421f8c,
                          &a, &b, &c, &radius, &d, &e, &f);
    hit2 = hitCheckFilter(0, *(f32*)((s32)npc + 0x8C) + float_15_80421fb0,
                          *(f32*)((s32)npc + 0x90) + float_30_80421fb4,
                          *(f32*)((s32)npc + 0x94), float_0_80421f8c,
                          float_2_80421fb8, float_0_80421f8c,
                          &a, &b, &c, &radius, &d, &e, &f);
    evtSetValue((void*)param_1, args[0], hit1 != NULL || hit2 != NULL);
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(getSpdWalk) {
    s32* args = event->args;
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    evtSetFloat(event, args[0], *(f32*)((s32)npc + 0x210));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(getSpdRun) {
    s32* args = event->args;
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    evtSetFloat(event, args[0], *(f32*)((s32)npc + 0x21C));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 limitSpd(int param_1) {
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    s32* args = *(s32**)(param_1 + 0x18);
    f32 speed = evtGetFloat((void*)param_1, args[0]);
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);

    *(f32*)((s32)npc + 0x1D0) = speed;
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 zakoEntryDokan(EventEntry* event) {
    extern s32 strcmp(const char* a, const char* b);
    extern char* strcpy(char* dst, const char* src);
    extern void* npcNameToPtr(const char* name);
    extern void evtDeleteID(s32 id);
    extern void* evtEntry(void* script, s32 order, u32 flags);
    extern char str_kemuri_test_802ed438[];
    extern u32 vec3_802ed9a8[];
    extern f32 float_neg75_80421fa0;
    extern f32 float_35_80421fac;
    void* owner;
    void* npc;
    void* child;
    char mobjName[32];
    char names[56];
    char* name;
    s32 i;

    owner = evtNpcNameToPtr(event, str_me_80421f30);
    if (strcmp((char*)owner + 8, str_kemuri_test_802ed438 + 0x690) == 0) {
        strcpy(names, str_kemuri_test_802ed438 + 0x6C8);
        strcpy(names + 0x10, str_kemuri_test_802ed438 + 0x6D8);
        strcpy(names + 0x20, str_kemuri_test_802ed438 + 0x6E8);
        strcpy(mobjName, str_kemuri_test_802ed438 + 0x6F8);
    }
    if (strcmp((char*)owner + 8, str_kemuri_test_802ed438 + 0x6A8) == 0) {
        strcpy(names, str_kemuri_test_802ed438 + 0x704);
        strcpy(names + 0x10, str_kemuri_test_802ed438 + 0x714);
        strcpy(names + 0x20, str_kemuri_test_802ed438 + 0x724);
        strcpy(mobjName, str_kemuri_test_802ed438 + 0x734);
    }

    name = names;
    for (i = 0; i < 3; i++, name += 0x10) {
        npc = npcNameToPtr(name);
        if (*(f32*)((s32)npc + 0x90) <= float_neg75_80421fa0) {
            *(u32*)npc |= 0x20410;
            *(u32*)npc &= ~0x20;
            *(u32*)((s32)npc + 0x1D4) |= 0x1002;
            *(u32*)((s32)npc + 0x1D4) &= 0xEFAFFFFB;
            *(u32*)((s32)npc + 0x8C) = *(u32*)((s32)owner + 0x8C);
            *(f32*)((s32)npc + 0x90) = *(f32*)((s32)owner + 0x90) - float_35_80421fac;
            *(u32*)((s32)npc + 0x94) = *(u32*)((s32)owner + 0x94);
            *(u32*)((s32)npc + 0xEC) = vec3_802ed9a8[0];
            *(u32*)((s32)npc + 0xF0) = vec3_802ed9a8[1];
            *(u32*)((s32)npc + 0xF4) = vec3_802ed9a8[2];
            *(s32*)((s32)npc + 0x304) = 1;
            if (*(void**)((s32)npc + 0x124) == NULL) {
                return 2;
            }
            if (*(s32*)((s32)npc + 0x11C) != 0) {
                evtDeleteID(*(s32*)((s32)npc + 0x11C));
            }
            child = evtEntry(*(void**)((s32)npc + 0x124), 0, 0x20);
            *(s8*)((s32)child + 0x10) = 1;
            *(void**)((s32)child + 0x170) = npc;
            *(s32*)((s32)npc + 0x11C) = *(s32*)((s32)child + 0x15C);
            return 2;
        }
    }
    return 2;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

s32 dokanCheck(int param_1) {
    extern s32 strcmp(const char* a, const char* b);
    extern char* strcpy(char* dst, const char* src);
    extern void* mobjNameToPtrNoAssert(char* name);
    extern char str_kemuri_test_802ed438[];
    extern char str_mobj9_80421fa4[];
    char name[32];
    s32* args = *(s32**)(param_1 + 0x18);
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);
    void* mobj;

    if (strcmp((char*)((s32)npc + 8), str_kemuri_test_802ed438 + 0x690) == 0) {
        strcpy(name, str_mobj9_80421fa4);
    }
    if (strcmp((char*)((s32)npc + 8), str_kemuri_test_802ed438 + 0x6A8) == 0) {
        strcpy(name, str_kemuri_test_802ed438 + 0x6C0);
    }
    mobj = mobjNameToPtrNoAssert(name);
    evtSetValue((void*)param_1, args[0], mobj != NULL);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 zakoEntryFall(EventEntry* event) {
    extern s32 strcmp(const char* a, const char* b);
    extern char* strcpy(char* dst, const char* src);
    extern void* npcNameToPtr_NoAssert(const char* name);
    extern void evtDeleteID(s32 id);
    extern void* evtEntry(void* script, s32 order, u32 flags);
    extern char str_kemuri_test_802ed438[];
    extern u32 vec3_802ed9c0[];
    extern f32 float_neg75_80421fa0;
    void* owner;
    void* npc;
    void* child;
    char names[56];
    char* name;
    s32 i;

    owner = evtNpcNameToPtr(event, str_me_80421f30);
    if (strcmp((char*)owner + 8, str_kemuri_test_802ed438 + 0x5EC) == 0) {
        strcpy(names, str_kemuri_test_802ed438 + 0x600);
        strcpy(names + 0x10, "");
        strcpy(names + 0x20, "");
    }
    if (strcmp((char*)owner + 8, str_kemuri_test_802ed438 + 0x610) == 0) {
        strcpy(names, str_kemuri_test_802ed438 + 0x600);
        strcpy(names + 0x10, str_kemuri_test_802ed438 + 0x628);
        strcpy(names + 0x20, str_kemuri_test_802ed438 + 0x638);
    }
    if (strcmp((char*)owner + 8, str_kemuri_test_802ed438 + 0x648) == 0) {
        strcpy(names, str_kemuri_test_802ed438 + 0x660);
        strcpy(names + 0x10, str_kemuri_test_802ed438 + 0x670);
        strcpy(names + 0x20, str_kemuri_test_802ed438 + 0x680);
    }

    name = names;
    for (i = 0; i < 3; i++, name += 0x10) {
        npc = npcNameToPtr_NoAssert(name);
        if (npc != NULL && *(f32*)((s32)npc + 0x90) <= float_neg75_80421fa0) {
            *(u32*)npc |= 0x20410;
            *(u32*)npc &= ~0x20;
            *(u32*)((s32)npc + 0x1D4) |= 0x1002;
            *(u32*)((s32)npc + 0x1D4) &= 0xEF8FFFFB;
            *(u32*)((s32)npc + 0x8C) = *(u32*)((s32)owner + 0x8C);
            *(u32*)((s32)npc + 0x90) = *(u32*)((s32)owner + 0x90);
            *(u32*)((s32)npc + 0x94) = *(u32*)((s32)owner + 0x94);
            *(u32*)((s32)npc + 0xEC) = vec3_802ed9c0[0];
            *(u32*)((s32)npc + 0xF0) = vec3_802ed9c0[1];
            *(u32*)((s32)npc + 0xF4) = vec3_802ed9c0[2];
            *(s32*)((s32)npc + 0x304) = 2;
            if (*(void**)((s32)npc + 0x124) == NULL) {
                return 2;
            }
            if (*(s32*)((s32)npc + 0x11C) != 0) {
                evtDeleteID(*(s32*)((s32)npc + 0x11C));
            }
            child = evtEntry(*(void**)((s32)npc + 0x124), 0, 0x20);
            *(s8*)((s32)child + 0x10) = 1;
            *(void**)((s32)child + 0x170) = npc;
            *(s32*)((s32)npc + 0x11C) = *(s32*)((s32)child + 0x15C);
            return 2;
        }
    }
    return 2;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


/* CHATGPT FALLBACK MISSING STUBS: main/npc_event 20260624_191429 */

/* fallback stub-fill: map=unk_800f1ac8 addr=0x800f1ac8 size=0x00000184 */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 unk_800f1ac8(EventEntry* event, s32 first) {
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* effFireEntry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 duration);
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void effDelete(void* eff);
    extern f32 float_1_80421f88;
    extern f32 float_0_80421f8c;
    extern f32 float_255_80421f94;
    s32* args;
    void* npc;
    f32 x;
    f32 y;
    f32 z;
    s32 frames;
    void* eff;
    f32 value;
    f32 zero;
    f32 alpha;

    npc = evtNpcNameToPtr(event, str_me_80421f30);
    args = event->args;
    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    z = evtGetFloat(event, args[2]);
    frames = evtGetValue(event, args[3]);
    if (first != 0) {
        *(void**)((s32)npc + 0x304) = effFireEntry(x, y, z, float_1_80421f88, 5, 1000);
        *(s32*)((s32)npc + 0x308) = frames;
    }
    frames = *(s32*)((s32)npc + 0x308) - 1;
    eff = *(void**)((s32)npc + 0x304);
    *(s32*)((s32)npc + 0x308) = frames;
    if (*(s32*)((s32)npc + 0x308) <= 30) {
        value = intplGetValue(1, 30 - *(s32*)((s32)npc + 0x308), 30, float_1_80421f88, float_0_80421f8c);
        zero = float_0_80421f8c;
        *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x78) = value;
        *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x7C) = value;
        alpha = float_255_80421f94;
        frames = *(s32*)((s32)npc + 0x308);
        value = intplGetValue(0, 30 - frames, 30, alpha, zero);
        *(u8*)((s32)*(void**)((s32)eff + 0xC) + 0x10) = (u8)(s32)value;
    }
    if (*(s32*)((s32)npc + 0x308) <= 0) {
        effDelete(eff);
        return 2;
    }
    return 0;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(set_bottle_flag_init) {
    void* npc = evtNpcNameToPtr(event, str_slave_0_802ed598);
    *(u32*)npc |= 0x28020020;
    *(u16*)((s32)npc + 0x31E) |= 0x1F;
    return 2;
}

s32 gesso_ground_check(int param_1) {
    extern void* hitCheckFilter(s32 flags, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz,
                                f32* outX, f32* outY, f32* outZ, f32* radius,
                                f32* outA, f32* outB, f32* outC);
    extern f32 float_5_80421f84;
    extern f32 float_0_80421f8c;
    extern f32 float_1_80421f88;
    extern f32 float_neg1_80421f90;
    s32* args = *(s32**)(param_1 + 0x18);
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);
    f32 a;
    f32 b;
    f32 c;
    f32 radius = float_5_80421f84;
    f32 d;
    f32 e;
    f32 f;
    void* hit = hitCheckFilter(0, *(f32*)((s32)npc + 0x8C),
                               *(f32*)((s32)npc + 0x90) + float_1_80421f88,
                               *(f32*)((s32)npc + 0x94), float_0_80421f8c,
                               float_neg1_80421f90, float_0_80421f8c,
                               &a, &b, &c, &radius, &d, &e, &f);

    evtSetValue((void*)param_1, args[0], hit != NULL);
    return 2;
}
