#include "action/star/sac_suki.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
s32 evtGetValue(EventEntry* event, s32 value);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void* mapGetMapObj(s32 name);
void* BattleAudienceBaseGetPtr(void);

void disp_3D(void) {
}

void disp_3D_alpha(void) {
}

void* get_ptr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1F4C);
}

USER_FUNC(get_img_name) {
    s32 index;
    s32* args;
    void* base;
    s32 offset;

    args = event->args;
    index = (s8)evtGetValue(event, args[0]);

    base = get_ptr();
    offset = (index << 4) + 0x70;

    evtSetValue(event, args[1], (s32)base + offset);
    return 2;
}

USER_FUNC(exist_map) {
    s32* args = event->args;
    if (mapGetMapObj(evtGetValue(event, args[0])) != 0) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return 2;
}

USER_FUNC(flash_update) {
    u8 value = evtGetValue(event, event->args[0]);
    *(u8*)((s32)get_ptr() + 0x20) = value;
    return 2;
}

USER_FUNC(aud_set_draw) {
    void* base = BattleAudienceBaseGetPtr();
    s32 value = evtGetValue(event, event->args[0]);
    if (value == 0) {
        *(u32*)base |= 0x20000;
    } else {
        *(u32*)base &= ~0x20000;
    }
    return 2;
}

USER_FUNC(wait_star_stone_attack_end) {
    s32 value = *(s32*)((s32)get_ptr() + 0x22C);
    return value > 8 ? 2 : 0;
}

USER_FUNC(wait_star_stone_up_end) {
    s32 value = *(s32*)((s32)get_ptr() + 0x22C);
    return value > 4 ? 2 : 0;
}

USER_FUNC(star_stone_appear) {
    *(s32*)((s32)get_ptr() + 0x22C) = 1;
    return 2;
}

USER_FUNC(start_game) {
    *(s32*)get_ptr() = 1;
    return 2;
}
