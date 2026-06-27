#include "action/star/sac_deka.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void effDelete(void* effect);
void smartFree(void* ptr);
void mapGrpFlagOff(const char* name, s32 flag);
extern const char str_stg_b_yuka_80301050[];

void disp_3D(void) {
    ;
}

void disp_3D_alpha(void) {
    ;
}

void* get_ptr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1F4C);
}

USER_FUNC(get_result) {
    void* work = get_ptr();
    evtSetValue(event, event->args[0], *(s32*)((s32)work + 0x44));
    return 2;
}

USER_FUNC(star_stone_attack) {
    void* work = get_ptr();
    if (isFirstCall != 0) {
        *(s32*)((s32)work + 0x3F0) = 0xA;
    }
    if (*(s32*)((s32)work + 0x3F0) == 0xD) {
        return 2;
    }
    return 0;
}

USER_FUNC(star_stone_appear) {
    void* wp;

    wp = get_ptr();
    *(s32*)((s32)wp + 0x3F0) = 1;
    return 2;
}

USER_FUNC(wait_game_end) {
    s32 value = *(s32*)((s32)get_ptr() + 0x30);
    return value == 5 ? 2 : 0;
}

USER_FUNC(start_game) {
    void* wp;

    wp = get_ptr();
    *(s32*)((s32)wp + 0x30) = 1;

    return 2;
}

USER_FUNC(wait_star_stone_take_on) {
    s32 value = *(s32*)((s32)get_ptr() + 0x3F0);
    return value == 4 ? 2 : 0;
}

s32 weaponGetPower_Deka(void) {
    return *(s32*)((s32)get_ptr() + 0x44) + 1;
}

USER_FUNC(end_deka) {
    void* work = get_ptr();

    effDelete(*(void**)(*(s32*)(*(s32*)((s32)work + 0x44C) + 0xC) + 0x34));
    effDelete(*(void**)((s32)work + 0x44C));
    return 2;
}

USER_FUNC(yuka_end) {
    void* work = get_ptr();

    smartFree(*(void**)work);
    **(s32**)work = 0;
    smartFree(*(void**)((s32)work + 0x2C));
    **(s32**)((s32)work + 0x2C) = 0;
    mapGrpFlagOff(str_stg_b_yuka_80301050, 0x400001);
    return 2;
}

USER_FUNC(get_ride_pos) {
    extern s32 evtGetValue(EventEntry* event, s32 target);
    extern void evtSetFloat(EventEntry* event, s32 target, f32 value);
    extern f32 float_0p5_804283ec;
    extern f32 float_0p25_804283f0;
    extern f32 float_64_804283f4;
    extern f32 float_5_804283f8;
    extern f32 float_4_804283fc;

    void* work;
    s32* args;
    f32 value;

    work = get_ptr();
    args = event->args;
    if (evtGetValue(event, args[0]) == 0) {
        value = *(f32*)((s32)work + 0x3FC) + (float_0p5_804283ec * (float_0p25_804283f0 * (*(f32*)((s32)work + 0x42C) * float_64_804283f4)));
    } else {
        value = *(f32*)((s32)work + 0x3FC) - (float_0p5_804283ec * (float_0p25_804283f0 * (*(f32*)((s32)work + 0x42C) * float_64_804283f4)));
    }
    evtSetFloat(event, args[1], value);
    args += 2;
    value = *(f32*)((s32)work + 0x400) + (float_0p25_804283f0 * (float_0p25_804283f0 * (*(f32*)((s32)work + 0x430) * float_64_804283f4)));
    evtSetFloat(event, args[0], value);
    value = float_5_804283f8 + *(f32*)((s32)work + 0x404) + (float_4_804283fc * *(f32*)((s32)work + 0x434));
    evtSetFloat(event, args[1], value);
    return 2;
}

void main_enemy(void) {
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
    extern void main_enemy_sub(void* unit, s32* count);

    void* battleWork;
    void* work;
    void* unit;
    s32 count;
    s32 i;

    battleWork = _battleWorkPointer;
    work = get_ptr();
    count = 0;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != NULL && *(s8*)((s32)unit + 0xC) == 1) {
            main_enemy_sub(unit, &count);
        }
    }
    while (count < 10) {
        *(s32*)((s32)work + 0x450 + count * 4) = -1;
        count++;
    }
}

void main_timing(s32 index) {
    extern u32 vec3_80300ed4[];

    void* work;
    void* timing;

    work = get_ptr();
    timing = (void*)((s32)work + index * 0x38 + 0x70);
    switch (*(s32*)timing) {
        case 0:
            *(s32*)((s32)timing + 8) = 0;
            break;
        case 2:
            *(u8*)((s32)timing + 0x34) = 0xFF;
            *(u32*)((s32)timing + 0x10) = vec3_80300ed4[0];
            *(u32*)((s32)timing + 0x14) = vec3_80300ed4[1];
            *(u32*)((s32)timing + 0x18) = vec3_80300ed4[2];
            break;
        case 3:
            *(s32*)timing = 4;
            *(u8*)((s32)timing + 0x34) = 0xFF;
            *(s32*)((s32)timing + 4) = 0;
            break;
    }
}

USER_FUNC(main_deka) {
    extern void* memset(void* dest, s32 value, u32 size);
    extern s32 evtGetValue(EventEntry* event, s32 target);
    extern void yuka_init(void);
    extern void main_base(void);
    extern void main_gauge(void);
    extern void main_star(void);
    extern void yuka_main(void);
    extern void dispEntry(s32 cameraId, s32 order, void* callback, f32 priority, void* param);
    extern void disp_2D(void);
    extern void yuka_disp(void);
    extern f32 float_901_80428490;
    extern f32 float_0_80428424;

    void* work;
    s32* args;
    s32 i;

    work = get_ptr();
    args = event->args;
    if (isFirstCall != 0) {
        memset(work, 0, 0x580);
        *(s32*)((s32)work + 0x48) = evtGetValue(event, args[0]);
        yuka_init();
    }
    if (*(void**)*(void**)work == NULL) {
        return 2;
    }
    if (*(void**)*(void**)((s32)work + 0x2C) == NULL) {
        return 2;
    }
    main_base();
    main_gauge();
    for (i = 0; i < 8; i++) {
        main_timing(i);
    }
    main_star();
    main_enemy();
    yuka_main();
    dispEntry(8, 1, disp_2D, float_901_80428490, NULL);
    dispEntry(4, 1, disp_3D, float_0_80428424, NULL);
    dispEntry(4, 2, disp_3D_alpha, float_0_80428424, NULL);
    dispEntry(4, 0, yuka_disp, float_0_80428424, NULL);
    return 0;
}


/* CHATGPT STUB FILL: main/action/star/sac_deka 20260624_184929 */

/* stub-fill: create_timing | missing_definition | ghidra_signature */
u8 create_timing(void) {
    return 0;
}

/* stub-fill: yuka_disp | prototype_only | source_prototype */
void yuka_disp(void) {
    return;
}

/* stub-fill: yuka_capture | missing_definition | ghidra_signature */
u8 yuka_capture(int param_1) {
    return 0;
}

/* stub-fill: disp_2D | prototype_only | source_prototype */
void disp_2D(void) {
    return;
}

/* stub-fill: main_enemy_sub | prototype_only | source_prototype */
void main_enemy_sub(void* unit, s32* count) {
    return;
}

/* stub-fill: main_star | prototype_only | source_prototype */
void main_star(void) {
    return;
}

/* stub-fill: main_gauge | prototype_only | source_prototype */
void main_gauge(void) {
    return;
}

/* stub-fill: main_base | prototype_only | source_prototype */
void main_base(void) {
    return;
}

/* stub-fill: yuka_main | prototype_only | source_prototype */
void yuka_main(void) {
    return;
}

/* stub-fill: yuka_init | prototype_only | source_prototype */
void yuka_init(void) {
    return;
}
