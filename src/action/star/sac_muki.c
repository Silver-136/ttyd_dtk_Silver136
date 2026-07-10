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
    typedef f32 Mtx[3][4];

    extern void* get_ptr(void);
    extern void* memcpy(void* dest, const void* src, u32 size);
    extern void PSMTXScale(Mtx mtx, f32 x, f32 y, f32 z);
    extern void PSMTXTransApply(Mtx src, Mtx dst, f32 x, f32 y, f32 z);
    extern void btlDispTexPlane2(Mtx mtx, s32 texId, void* color);
    extern s32 dat_8030112c[];
    extern s32 dat_8030114c[];
    extern u32 dat_8030116c[];
    extern u32 dat_80428518;
    extern f32 float_0_8042854c;

    void* work;
    Mtx mtx;
    s32 scaleOffsets[8];
    s32 transOffsetsA[8];
    u32 color[8];
    void* point;
    s32 i;
    s32 j;

    work = get_ptr();
    memcpy(scaleOffsets, dat_8030112c, sizeof(scaleOffsets));
    memcpy(transOffsetsA, dat_8030114c, sizeof(transOffsetsA));
    memcpy(color, dat_8030116c, sizeof(color));

    for (i = 0; i < 9; i++) {
        point = (void*)((s32)work + i * 0x24 + 0x78);
        if (*(s32*)point >= 2 && *(s32*)point < 8) {
            for (j = 0; j < 4; j++) {
                PSMTXScale(
                    mtx,
                    *(f32*)((s32)point + 0x14) * scaleOffsets[j * 2],
                    *(f32*)((s32)point + 0x18) * scaleOffsets[j * 2 + 1],
                    *(f32*)((s32)point + 0x1C));
                PSMTXTransApply(
                    mtx,
                    mtx,
                    *(f32*)((s32)point + 8),
                    *(f32*)((s32)point + 0xC),
                    *(f32*)((s32)point + 0x10));
                PSMTXTransApply(
                    mtx,
                    mtx,
                    *(f32*)((s32)point + 0x14) * transOffsetsA[j * 2],
                    *(f32*)((s32)point + 0x18) * transOffsetsA[j * 2 + 1],
                    float_0_8042854c);
                color[0] = dat_80428518;
                btlDispTexPlane2(mtx, 0x58, color);
            }
        }
    }

    if (*(s32*)((s32)work + 0x38) >= 2 && *(s32*)((s32)work + 0x38) < 7) {
        for (j = 0; j < 4; j++) {
            PSMTXScale(
                mtx,
                *(f32*)((s32)work + 0x68) * scaleOffsets[j * 2],
                *(f32*)((s32)work + 0x6C) * scaleOffsets[j * 2 + 1],
                *(f32*)((s32)work + 0x70));
            PSMTXTransApply(
                mtx,
                mtx,
                *(f32*)((s32)work + 0x44),
                *(f32*)((s32)work + 0x48),
                *(f32*)((s32)work + 0x4C));
            PSMTXTransApply(
                mtx,
                mtx,
                *(f32*)((s32)work + 0x68) * transOffsetsA[j * 2],
                *(f32*)((s32)work + 0x6C) * transOffsetsA[j * 2 + 1],
                float_0_8042854c);
            color[0] = *(u32*)((s32)work + 0x74);
            btlDispTexPlane2(mtx, 0x57, color);
        }
    }
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
    extern void* get_ptr(void);
    extern s32 irand(s32 max);
    extern f32 intplGetValue(s32 type, s32 current, f32 start, f32 end, s32 max);
    extern u32 vec3_803010e4[];
    extern u32 vec3_803010f0[];
    extern f32 float_0_8042854c;
    extern f32 float_180_80428598;
    extern f32 float_1080_8042859c;
    extern f32 float_255_804285a0;
    extern f32 float_900_804285a4;
    extern f32 float_9_804285a8;

    void* icon;
    void* effWork;

    icon = (void*)((s32)get_ptr() + index * 0x38 + 0x1BC);
    switch (*(s32*)icon) {
        case 0:
            *(u32*)((s32)icon + 0xC) = vec3_803010e4[0];
            *(u32*)((s32)icon + 0x10) = vec3_803010e4[1];
            *(u32*)((s32)icon + 0x14) = vec3_803010e4[2];
            *(u8*)((s32)icon + 0x34) = 0xFF;
            break;
        case 1:
            *(s32*)icon = 2;
            *(s32*)((s32)icon + 4) = 0;
            *(s32*)((s32)icon + 8) = irand(3);
            *(f32*)((s32)icon + 0x28) = float_0_8042854c;
            *(u32*)((s32)icon + 0x18) = vec3_803010f0[0];
            *(u32*)((s32)icon + 0x1C) = vec3_803010f0[1];
            *(u32*)((s32)icon + 0x20) = vec3_803010f0[2];
            /* fallthrough */
        case 2:
            *(s32*)((s32)icon + 4) += 1;
            *(f32*)((s32)icon + 0x28) = intplGetValue(4, *(s32*)((s32)icon + 4), float_180_80428598, float_1080_8042859c, 0x1E);
            *(u8*)((s32)icon + 0x34) = (s32)intplGetValue(0, *(s32*)((s32)icon + 4), float_0_8042854c, float_255_804285a0, 0x1E);
            if (*(s32*)((s32)icon + 4) >= 0x1E) {
                *(s32*)icon = 7;
            }
            break;
        case 3:
            *(s32*)icon = 4;
            *(s32*)((s32)icon + 4) = 0;
            *(f32*)((s32)icon + 0x28) = float_0_8042854c;
            /* fallthrough */
        case 4:
            *(s32*)((s32)icon + 4) += 1;
            *(f32*)((s32)icon + 0x28) = intplGetValue(1, *(s32*)((s32)icon + 4), float_0_8042854c, float_900_804285a4, 0x1E);
            *(u8*)((s32)icon + 0x34) = (s32)intplGetValue(0, *(s32*)((s32)icon + 4), float_255_804285a0, float_0_8042854c, 0x1E);
            if (*(s32*)((s32)icon + 4) >= 0x1E) {
                *(s32*)icon = 0;
            }
            break;
        case 5:
            *(s32*)icon = 6;
            /* fallthrough */
        case 6:
            *(s32*)icon = 0;
            break;
        case 7:
            *(s32*)icon = 8;
            *(s32*)((s32)icon + 4) = 0;
            *(f32*)((s32)icon + 0x28) = float_0_8042854c;
            break;
        case 8:
            *(s32*)((s32)icon + 4) += 1;
            *(u8*)((s32)icon + 0x34) = 0xFF;
            if (*(s32*)((s32)icon + 4) >= 0x5A) {
                *(s32*)icon = 3;
            }
            break;
        case 9:
            if (*(u8*)((s32)icon + 0x34) >= 8) {
                *(u8*)((s32)icon + 0x34) -= 8;
            } else {
                *(u8*)((s32)icon + 0x34) = 0;
            }
            break;
    }

    if (*(void**)((s32)icon + 0x30) != 0) {
        effWork = *(void**)((s32)*(void**)((s32)icon + 0x30) + 0xC);
        *(f32*)((s32)effWork + 8) = *(f32*)((s32)icon + 0xC);
        *(f32*)((s32)effWork + 0xC) = *(f32*)((s32)icon + 0x10);
        *(f32*)((s32)effWork + 0x10) = *(f32*)((s32)icon + 0x14);
        *(f32*)((s32)effWork + 0x18) = *(f32*)((s32)icon + 0x24);
        *(f32*)((s32)effWork + 0x1C) = *(f32*)((s32)icon + 0x28);
        *(f32*)((s32)effWork + 0x20) = *(f32*)((s32)icon + 0x2C);
        *(f32*)((s32)effWork + 0x14) = float_9_804285a8 * *(f32*)((s32)icon + 0x18);
        *(u8*)((s32)effWork + 0x5F) = *(u8*)((s32)icon + 0x34);
    }
}

/* stub-fill: main_point | prototype_only | source_prototype */
void main_point(s32 index) {
    extern void* get_ptr(void);
    extern s32 irand(s32 max);
    extern f32 intplGetValue(s32 type, s32 current, f32 start, f32 end, s32 max);
    extern char str_btl_wn_sac_mukimuki__80301078[];
    extern f32 float_0_8042854c;
    extern f32 float_0p5_80428568;

    void* work;
    void* point;
    void* icon;
    char* ro;
    s32 row;
    s32 col;
    s32 i;
    s32 timer;
    f32 value;

    ro = str_btl_wn_sac_mukimuki__80301078;
    work = get_ptr();
    point = (void*)((s32)work + index * 0x24 + 0x78);

    switch (*(s32*)point) {
        case 1:
            *(s32*)point = 2;
            *(s32*)((s32)point + 4) = irand(0x1E) + 0x1E;
            row = index / 3;
            col = index - row * 3;
            *(f32*)((s32)point + 8) = (f32)(col * 0x30 - 0x30);
            *(f32*)((s32)point + 0xC) = (f32)(0x7D - row * 0x30);
            *(u32*)((s32)point + 0x10) = *(u32*)(ro + 0x50);
            *(u32*)((s32)point + 0x14) = *(u32*)(ro + 0x54);
            *(u32*)((s32)point + 0x18) = *(u32*)(ro + 0x58);
            *(u32*)((s32)point + 0x1C) = *(u32*)(ro + 0x5C);
            *(void**)((s32)point + 0x20) = 0;
            /* fallthrough */
        case 2:
            timer = *(s32*)((s32)point + 4) - 1;
            *(s32*)((s32)point + 4) = timer;
            if (timer <= 0x1E) {
                value = intplGetValue(0, 0x1E - timer, float_0_8042854c, float_0p5_80428568, 0x1E);
                *(f32*)((s32)point + 0x18) = value;
                *(f32*)((s32)point + 0x14) = value;
            }
            if (*(s32*)((s32)point + 4) <= 0) {
                *(s32*)point = 3;
                *(s32*)((s32)point + 4) = 0;
            }
            break;
        case 3:
            *(s32*)point = 4;
            *(s32*)((s32)point + 4) = 0;
            *(u32*)((s32)point + 0x14) = *(u32*)(ro + 0x60);
            *(u32*)((s32)point + 0x18) = *(u32*)(ro + 0x64);
            *(u32*)((s32)point + 0x1C) = *(u32*)(ro + 0x68);
            break;
        case 4:
            icon = *(void**)((s32)point + 0x20);
            if (icon != 0) {
                if (*(s32*)icon == 0) {
                    *(void**)((s32)point + 0x20) = 0;
                }
            } else if (irand(0x41) == 0) {
                for (i = 0; i < 9; i++) {
                    icon = (void*)((s32)work + i * 0x38 + 0x1BC);
                    if (*(s32*)icon == 0) {
                        *(s32*)icon = 1;
                        *(u32*)((s32)icon + 0xC) = *(u32*)((s32)point + 8);
                        *(u32*)((s32)icon + 0x10) = *(u32*)((s32)point + 0xC);
                        *(u32*)((s32)icon + 0x14) = *(u32*)((s32)point + 0x10);
                        *(f32*)((s32)icon + 0x14) = float_0_8042854c;
                        *(void**)((s32)point + 0x20) = icon;
                        break;
                    }
                }
            }
            break;
        case 6:
            *(s32*)point = 7;
            *(s32*)((s32)point + 4) = irand(0x1E) + 0x1E;
            *(void**)((s32)point + 0x20) = 0;
            /* fallthrough */
        case 7:
            timer = *(s32*)((s32)point + 4) - 1;
            *(s32*)((s32)point + 4) = timer;
            if (timer <= 0x1E) {
                value = intplGetValue(0, 0x1E - timer, float_0p5_80428568, float_0_8042854c, 0x1E);
                *(f32*)((s32)point + 0x18) = value;
                *(f32*)((s32)point + 0x14) = value;
            }
            if (*(s32*)((s32)point + 4) <= 0) {
                *(s32*)point = 8;
                *(s32*)((s32)point + 4) = 0;
            }
            break;
    }
}

/* stub-fill: main_cursor | prototype_only | source_prototype */
void main_cursor(void) {
    return;
}

/* stub-fill: main_base | prototype_only | source_prototype */
void main_base(void) {
    return;
}
