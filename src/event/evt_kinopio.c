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
