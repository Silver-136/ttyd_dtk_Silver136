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


u8 piders_draw_yarn_sub(s32 param_1, int param_2) {
    return 0;
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
int unk_800f1ac8() {
    return 0;
}
