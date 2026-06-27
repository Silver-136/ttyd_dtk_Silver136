#include "action/star/sac_muki.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
void effDelete(void* effect);

void* get_ptr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1F4C);
}

USER_FUNC(star_stone_appear) {
    void* wp;

    wp = get_ptr();
    *(s32*)((s32)wp + 0x3B4) = 1;
    return 2;
}

USER_FUNC(start_game) {
    void* wp;

    wp = get_ptr();
    *(s32*)((s32)wp + 0x4) = 1;

    return 2;
}

USER_FUNC(end_muki) {
    s32 offset;
    void* work;
    s32 i;

    work = get_ptr();
    effDelete(*(void**)(*(s32*)(*(s32*)((s32)work + 0x404) + 0xC) + 0x34));
    effDelete(*(void**)((s32)work + 0x404));
    i = 0;
    offset = 0;
    for (; i < 9; i++, offset += 0x38) {
        effDelete(*(void**)((s32)work + offset + 0x1EC));
    }
    return 2;
}

USER_FUNC(wait_game_end) {
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);

    void* work;
    s32* args;

    work = get_ptr();
    args = event->args;
    if (*(s32*)((s32)work + 4) == 7) {
        evtSetValue(event, args[0], (s32)*(f32*)((s32)work + 0x14));
        evtSetValue(event, args[1], (s32)*(f32*)((s32)work + 0x1C));
        return 2;
    }
    return 0;
}

USER_FUNC(set_weapon) {
    extern s32 evtGetValue(EventEntry* event, s32 target);
    extern s32 evtSetValue(EventEntry* event, s32 target, s32 value);
    extern void* memcpy(void* dest, const void* src, u32 size);
    extern u8 weapon[];

    void* work;
    s32* args;
    s32 type;
    s32 value;

    args = event->args;
    type = evtGetValue(event, args[0]);
    value = evtGetValue(event, args[1]);
    work = get_ptr();
    memcpy((void*)((s32)work + 0x408), weapon, 0xC0);
    switch (type) {
        case 0:
            *(u8*)((s32)work + 0x49E) = 0x64;
            *(u8*)((s32)work + 0x49F) = 3;
            *(u8*)((s32)work + 0x4A0) = value;
            break;
        case 1:
            *(u8*)((s32)work + 0x4A1) = 0x64;
            *(u8*)((s32)work + 0x4A2) = 3;
            *(u8*)((s32)work + 0x4A3) = value;
            break;
    }
    evtSetValue(event, args[2], (s32)work + 0x408);
    return 2;
}

USER_FUNC(main_muki) {
    extern void* memset(void* dest, s32 value, u32 size);
    extern s32 evtGetValue(EventEntry* event, s32 target);
    extern void* effStarStoneEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
    extern void main_base(void);
    extern void main_cursor(void);
    extern void main_point(s32 index);
    extern void main_icon(s32 index);
    extern void main_star(void);
    extern void dispEntry(s32 cameraId, s32 order, void* callback, f32 priority, void* param);
    extern void disp_2D(void);
    extern void disp_3D(void);
    extern void disp_3D_alpha(void);
    extern f32 float_0_8042854c;
    extern f32 float_1_8042855c;
    extern f32 float_neg1000_80428580;
    extern f32 float_901_804285e8;

    s32* args;
    void* work;
    s32 i;
    s32 offset;

    work = get_ptr();
    args = event->args;
    if (isFirstCall != 0) {
        memset(work, 0, 0x4C8);
        *(s32*)((s32)work + 0x10) = evtGetValue(event, args[0]);
        i = 0;
        offset = 0;
        while (i < 9) {
            *(void**)((s32)work + offset + 0x1EC) = effStarStoneEntry(9, float_0_8042854c, float_neg1000_80428580, float_0_8042854c, float_1_8042855c);
            i++;
            offset += 0x38;
        }
    }
    main_base();
    main_cursor();
    for (i = 0; i < 9; i++) {
        main_point(i);
    }
    for (i = 0; i < 9; i++) {
        main_icon(i);
    }
    main_star();
    dispEntry(8, 1, disp_2D, float_901_804285e8, NULL);
    dispEntry(4, 1, disp_3D, float_0_8042854c, NULL);
    dispEntry(4, 2, disp_3D_alpha, float_0_8042854c, NULL);
    return 0;
}


/* CHATGPT STUB FILL: main/action/star/sac_muki 20260624_184929 */

/* stub-fill: disp_3D_alpha | prototype_only | source_prototype */
void disp_3D_alpha(void) {
    return;
}

/* stub-fill: disp_3D | prototype_only | source_prototype */
void disp_3D(void) {
    return;
}

/* stub-fill: disp_2D | prototype_only | source_prototype */
void disp_2D(void) {
    return;
}

/* stub-fill: main_star | prototype_only | source_prototype */
void main_star(void) {
    return;
}

/* stub-fill: main_icon | prototype_only | source_prototype */
void main_icon(s32 index) {
    return;
}

/* stub-fill: main_point | prototype_only | source_prototype */
void main_point(s32 index) {
    return;
}

/* stub-fill: main_cursor | prototype_only | source_prototype */
void main_cursor(void) {
    return;
}

/* stub-fill: main_base | prototype_only | source_prototype */
void main_base(void) {
    return;
}
