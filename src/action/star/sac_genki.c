#include "action/star/sac_genki.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void statusWinOpen(void);
void* BattleGetPartyPtr(void* battleWork);
void main_star0(s32 index);
void main_star1(void);

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

USER_FUNC(star_appear) {
    void* work = get_ptr();
    s32 offset = 0;
    s32 zero = offset;
    s32 one = 1;
    s32 i;

    for (i = 0; i < 7; i++) {
        void* entry = (void*)((s32)work + offset + 0x510);

        offset += 0x64;
        *(s32*)entry = one;
        *(s32*)((s32)entry + 8) = zero;
    }
    return 2;
}

USER_FUNC(start_game) {
    void* wp;

    wp = get_ptr();
    *(s32*)((s32)wp + 0x4) = 1;

    return 2;
}

USER_FUNC(wait_game_end) {
    s32 value = *(s32*)((s32)get_ptr() + 4);
    return value == 3 ? 2 : 0;
}

s32 object_get_num(void) {
    void* work;
    s32 i;
    s32 offset;
    s32 count;
    s32 entryOffset;

    work = get_ptr();
    count = 0;
    offset = 0;

    for (i = 0; i < 10; i++) {
        entryOffset = offset;
        entryOffset += 0x358;

        if (*(s32*)((char*)work + entryOffset) == 2) {
            count++;
        }

        offset += 0x2C;
    }

    return count;
}

void object_entry(void) {
    void* work;
    s32 i;
    s32 offset;
    s32 entryOffset;

    work = get_ptr();
    offset = 0;

    for (i = 0; i < 10; i++) {
        entryOffset = offset + 0x358;

        if (*(s32*)((s32)work + entryOffset) == 0) {
            *(s32*)((s32)work + entryOffset) = 1;
            break;
        }

        offset += 0x2C;
    }
}

USER_FUNC(get_score) {
    void* work;
    s32* args;
    s32 value;

    work = get_ptr();
    args = event->args;

    value = *(s32*)((s32)work + 0x10);
    evtSetValue(event, args[0], value);

    value = *(s32*)((s32)work + 0x14);
    evtSetValue(event, args[1], value);

    value = *(s32*)((s32)work + 0x18);
    evtSetValue(event, args[2], value);

    return 2;
}

void main_star(void) {
    void* work = get_ptr();
    s32 i;

    if (*(s32*)((s32)work + 0xC) == 0) {
        for (i = 0; i < 7; i++) {
            main_star0(i);
        }
    } else {
        main_star1();
    }
}
