#include "action/star/sac_genki.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void statusWinOpen(void);
void* BattleGetPartyPtr(void* battleWork);

void disp_2D(void) {
}

void disp_3D_alpha(void) {
}

void* get_ptr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1F4C);
}

USER_FUNC(recover_cmd_disable) {
    void* work = _battleWorkPointer;
    void* party;
    *(s32*)((s32)work + 0x19050) = 0;
    *(s32*)((s32)work + 0x19054) = 0;
    *(s32*)((s32)work + 0x19058) = 0;
    party = BattleGetPartyPtr(work);
    if (party != 0) {
        *(u16*)((s32)party + 0x13C) = 0;
        *(u16*)((s32)party + 0x13E) = 0;
    }
    return 2;
}

USER_FUNC(status_on) {
    statusWinOpen();
    return 2;
}

USER_FUNC(star_stone_appear) {
    void* work = get_ptr();
    *(s32*)((s32)work + 0x510) = 1;
    *(s32*)((s32)work + 0x518) = 1;
    return 2;
}

USER_FUNC(start_game) {
    *(s32*)((s32)get_ptr() + 4) = 1;
    return 2;
}

USER_FUNC(wait_game_end) {
    s32 value = *(s32*)((s32)get_ptr() + 4);
    return value > 3 ? 2 : 0;
}

s32 object_get_num(void) {
    void* work = get_ptr();
    s32 i;
    s32 offset = 0;
    s32 count = 0;
    for (i = 0; i < 10; i++) {
        if (*(s32*)((s32)work + 0x358 + offset) == 2) {
            count++;
        }
        offset += 0x2C;
    }
    return count;
}

void object_entry(void) {
    void* work = get_ptr();
    s32 i;
    s32 offset = 0;
    for (i = 0; i < 10; i++) {
        if (*(s32*)((s32)work + 0x358 + offset) == 0) {
            *(s32*)((s32)work + 0x358 + offset) = 1;
            break;
        }
        offset += 0x2C;
    }
}

USER_FUNC(get_score) {
    s32* args = event->args;
    void* work = get_ptr();
    evtSetValue(event, args[0], *(s32*)((s32)work + 0x10));
    evtSetValue(event, args[1], *(s32*)((s32)work + 0x14));
    evtSetValue(event, args[2], *(s32*)((s32)work + 0x18));
    return 2;
}
