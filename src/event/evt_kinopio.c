#include "event/evt_kinopio.h"

void* wp;
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
f32 evtSetFloat(EventEntry* event, s32 target, f32 value);
void marioPaperOff(void);
void psndSetFlag(s32 flag);
void psndClearFlag(s32 flag);

USER_FUNC(paper_off) {
    marioPaperOff();
    return 2;
}

USER_FUNC(snd_off) {
    psndSetFlag(0x80);
    return 2;
}

USER_FUNC(snd_on) {
    psndClearFlag(0x80);
    return 2;
}

USER_FUNC(get_from_bed_evt) {
    void* data = *(void**)wp;
    evtSetValue(event, event->args[0], *(s32*)((s32)data + 0x94));
    return 2;
}

USER_FUNC(get_to_bed_evt) {
    void* data = *(void**)wp;
    evtSetValue(event, event->args[0], *(s32*)((s32)data + 0x90));
    return 2;
}

USER_FUNC(get_beddeg) {
    void* data = *(void**)wp;
    evtSetFloat(event, event->args[0], *(f32*)((s32)data + 0x44));
    return 2;
}

USER_FUNC(get_npcname) {
    void* data = *(void**)wp;
    evtSetValue(event, event->args[0], *(s32*)((s32)data + 0x54));
    return 2;
}

USER_FUNC(get_ryoukin) {
    void* data = *(void**)wp;
    evtSetValue(event, event->args[0], *(s32*)((s32)data + 0x10));
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(get_npctalk) {
    s32* args = event->args;
    void* data = *(void**)wp;
    evtSetFloat(event, args[0], *(f32*)((s32)data + 0x48));
    data = *(void**)wp;
    evtSetFloat(event, args[1], *(f32*)((s32)data + 0x4C));
    data = *(void**)wp;
    evtSetFloat(event, args[2], *(f32*)((s32)data + 0x50));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(get_bedside) {
    s32* args = event->args;
    void* data = *(void**)wp;
    evtSetFloat(event, args[0], *(f32*)((s32)data + 0x20));
    data = *(void**)wp;
    evtSetFloat(event, args[1], *(f32*)((s32)data + 0x24));
    data = *(void**)wp;
    evtSetFloat(event, args[2], *(f32*)((s32)data + 0x28));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(get_bed) {
    s32* args = event->args;
    void* data = *(void**)wp;
    evtSetFloat(event, args[0], *(f32*)((s32)data + 0x14));
    data = *(void**)wp;
    evtSetFloat(event, args[1], *(f32*)((s32)data + 0x18));
    data = *(void**)wp;
    evtSetFloat(event, args[2], *(f32*)((s32)data + 0x1C));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 paper_on(s32 pEvt, s32 param_2) {
    return 0;
}


s32 power_chk(int param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 offset(s32 pEvt) {
    extern f32 evtGetFloat(EventEntry* event, s32 value);
    extern void* camGetPtr(s32 id);
    extern f64 sin(f64 value);
    extern f64 cos(f64 value);
    extern f32 float_90_80421f1c;
    extern f32 float_6p2832_80421f18;
    extern f32 float_360_80421f20;
    s32* args = *(s32**)(pEvt + 0x18);
    f32 dist = evtGetFloat((void*)pEvt, args[0]);
    f32 angle = (float_6p2832_80421f18 * (float_90_80421f1c + *(f32*)((s32)camGetPtr(4) + 0x114))) / float_360_80421f20;

    evtSetFloat((void*)pEvt, args[1], dist * -(f32)sin(angle));
    evtSetFloat((void*)pEvt, args[2], dist * (f32)cos(angle));
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
s32 get_cam(void* pEvt) {
    s32* args = *(s32**)((s32)pEvt + 0x18);
    void* data = *(void**)wp;
    evtSetFloat(pEvt, args[0], *(f32*)((s32)data + 0x2C));
    data = *(void**)wp;
    evtSetFloat(pEvt, args[1], *(f32*)((s32)data + 0x30));
    data = *(void**)wp;
    evtSetFloat(pEvt, args[2], *(f32*)((s32)data + 0x34));
    data = *(void**)wp;
    evtSetFloat(pEvt, args[3], *(f32*)((s32)data + 0x38));
    data = *(void**)wp;
    evtSetFloat(pEvt, args[4], *(f32*)((s32)data + 0x3C));
    data = *(void**)wp;
    evtSetFloat(pEvt, args[5], *(f32*)((s32)data + 0x40));
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
s32 evt_kinopio_setup(void* pEvt) {
    extern s32 evtGetValue(EventEntry* event, s32 value);
    extern void* npcNameToPtr(s32 name);
    extern s32 animGroupBaseAsync(const char* name, s32 flags, s32 unused);
    extern char str_p_slit_802ed424[];
    extern s32 kinopio_house_talk_evt[];
    s32* args = *(s32**)((s32)pEvt + 0x18);
    void* data = (void*)evtGetValue(pEvt, args[0]);
    void* npc = npcNameToPtr(*(s32*)((s32)data + 0x54));

    if (animGroupBaseAsync(str_p_slit_802ed424, 0, 0) == 0) {
        return 0;
    }
    *(void**)((s32)npc + 0x128) = kinopio_house_talk_evt;
    *(void**)wp = data;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 breakfast(void) {
    return 0;
}


s32 coin_chk(void* pEvt) {
    return 0;
}


s32 msg_no(void* pEvt) {
    return 0;
}


s32 N_cameraman_on(void) {
    return 0;
}
