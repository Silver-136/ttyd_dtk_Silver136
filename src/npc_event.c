#include "npc_event.h"
#include "event/evt_cmd.h"

extern char str_slave_0_802ed598[];
const char str_me_80421f30[] = "me";

s32 evtSetValue(EventEntry* event, s32 target, s32 value);
f32 evtSetFloat(EventEntry* event, s32 target, f32 value);
void* evtNpcNameToPtr(EventEntry* event, const void* name);
void* effNameToPtr(void* name);
void* camGetPtr(s32 camId);

USER_FUNC(kamec_fire_magic_delete) {
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    void* eff = effNameToPtr((void*)((s32)npc + 8));
    if (eff != 0) {
        *(s32*)(*(s32*)((s32)eff + 0xC) + 0x30) = 1;
    }
    return 2;
}

USER_FUNC(set_bottle_flag_init) {
    void* npc = evtNpcNameToPtr(event, str_slave_0_802ed598);
    *(u32*)npc |= 0x28020020;
    *(u16*)((s32)npc + 0x31E) |= 0x1F;
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
USER_FUNC(piders_get_height) {
    s32* args = event->args;
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    evtSetFloat(event, args[0], *(f32*)((s32)npc + 0x210));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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


u8 _2d_dead_jump(void) {
    return 0;
}


u8 chain_main(void) {
    return 0;
}


u8 zakoEntryDokan(void) {
    return 0;
}


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

u8 zakoEntryFall(void) {
    return 0;
}


u8 cliffCheck(void) {
    return 0;
}


s32 kamec_fire_magic(int param_1) {
    return 0;
}


s32 upperCheck(int param_1) {
    return 0;
}


s32 pressCheck(int param_1) {
    return 0;
}


s32 dokanCheck(int param_1) {
    return 0;
}


s32 gesso_ground_check(int param_1) {
    return 0;
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
    return 0;
}


s32 _wait_blow_end(void* pEvt) {
    return 0;
}


s32 limitSpd(int param_1) {
    return 0;
}


s32 piders_draw_yawn(int param_1) {
    return 0;
}


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

