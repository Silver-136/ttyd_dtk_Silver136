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
    extern void* get_ptr(void);
    extern void PSMTXRotRad(f32 matrix[3][4], s32 axis, f32 radians);
    extern void PSMTXScaleApply(f32 src[3][4], f32 dst[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXTransApply(f32 src[3][4], f32 dst[3][4], f32 x, f32 y, f32 z);
    extern f64 cos(f64 angle);
    extern f64 sin(f64 angle);
    extern void btlDispTexPlane2(f32 matrix[3][4], s32 texture, u32* color);
    extern void btlDispTexPlane3(f32 matrix[3][4], s32 texture,
                                 u32* color0, u32* color1,
                                 u32* color2, u32* color3);

    u8* work = get_ptr();
    s32 i;

    for (i = 0; i < 9; i++) {
        s32* icon = (s32*)(work + 0x1BC + i * 0x38);

        if (icon[0] > 0 && icon[0] < 10 && icon[2] >= 0 && icon[2] < 3) {
            f32 matrix[3][4];
            f32 angle;
            f32 xOffset;
            f32 zOffset;
            f32 scaleX = (f32)icon[6];
            f32 scaleY = (f32)icon[7];
            u8 alpha = *(u8*)((u8*)icon + 0x34);

            PSMTXRotRad(matrix, 'y', 0.017453292f * (f32)icon[10]);
            if (icon[2] < 2) {
                scaleX *= 2.0f;
                scaleY *= 2.0f;
            }
            PSMTXScaleApply(matrix, matrix, scaleX, scaleY, (f32)icon[8]);
            PSMTXTransApply(matrix, matrix, (f32)icon[3], (f32)icon[4], (f32)icon[5]);

            angle = (2.0f * (f32)icon[10] * 3.141592f) / 360.0f;
            zOffset = 15.0f * (f32)cos((f64)angle);
            xOffset = 15.0f * (f32)sin((f64)angle);
            PSMTXTransApply(matrix, matrix, xOffset, 0.0f, zOffset);

            if (icon[2] == 0) {
                u32 bright = 0xE94A3F00 | alpha;
                u32 dark = 0xFEE63E00 | alpha;
                btlDispTexPlane3(matrix, 0x5B,
                                 &bright, &bright, &dark, &dark);
            } else if (icon[2] == 1) {
                u32 bright = 0x3F3FE900 | alpha;
                u32 dark = 0x74C9FF00 | alpha;
                btlDispTexPlane3(matrix, 0x5B,
                                 &bright, &bright, &dark, &dark);
            } else {
                u32 color = 0xFFFFFF00 | alpha;
                btlDispTexPlane2(matrix, 0x47, &color);
            }
        }
    }
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
    typedef f32 Mtx[3][4];
    extern void* get_ptr(void);
    extern void btlGetScreenPoint(f32* in, f32* out);
    extern void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void btlDispTexPlane2(Mtx m, s32 tex, u32* color);
    extern void btlDispGXInit2DRasta(void);
    extern void btlDispGXQuads2DRasta(f64, f64, f64, f64, s32, s32, s32, s32);
    extern void iconDispGxCol(Mtx m, u16 flags, u16 icon, u32* color);
    extern void iconNumberDispGx(f32 x, f32 y, f32 z, s32 value, s32 flags, u32* color);
    u8* work = get_ptr();
    f32 world[3], screen[3];
    Mtx trans, scale, model;
    u32 white = 0xFFFFFFFF;
    s32 state = *(s32*)(work + 4);
    s32 pass;
    if (state < 2 || state > 7) return;
    world[0] = *(f32*)(work + 0x2C);
    world[1] = *(f32*)(work + 0x30);
    world[2] = *(f32*)(work + 0x34);
    btlGetScreenPoint(world, screen);
    for (pass = 0; pass < 2; pass++) {
        f32 y = screen[1] - pass * 30.0f;
        f32 amount = *(f32*)(work + (pass ? 0x20 : 0x18));
        s32 flash = *(s32*)(work + (pass ? 0x28 : 0x24));
        u32 fill = pass ? 0x40C0FFFF : 0xFF8040FF;
        PSMTXTrans(trans, screen[0] - 56.0f, y, screen[2]);
        btlDispTexPlane2(trans, 0x59, &white);
        PSMTXScale(scale, 6.0f, 1.0f, 1.0f);
        PSMTXTrans(trans, screen[0], y, screen[2]);
        PSMTXConcat(trans, scale, model);
        btlDispTexPlane2(model, 0x5A, &white);
        PSMTXScale(scale, -1.0f, 1.0f, 1.0f);
        PSMTXTrans(trans, screen[0] + 56.0f, y, screen[2]);
        PSMTXConcat(trans, scale, model);
        btlDispTexPlane2(model, 0x59, &white);
        btlDispGXInit2DRasta();
        btlDispGXQuads2DRasta(screen[0] - 48.0f, y + 5.0f,
            screen[0] - 48.0f + amount * 9.6f, y - 5.0f,
            (fill >> 24) & 0xFF, (fill >> 16) & 0xFF, (fill >> 8) & 0xFF, 0xFF);
        if ((flash % 12) < 6) {
            PSMTXTrans(trans, screen[0] - 70.0f, y, screen[2]);
            btlDispTexPlane2(trans, 0x5B, &white);
            iconDispGxCol(trans, 0x10, 0x1F5, &white);
            iconNumberDispGx(screen[0] + 64.0f, y, screen[2], (s32)amount, 0, &white);
        }
    }
}

/* stub-fill: main_star | prototype_only | source_prototype */
void main_star(void) {
    extern void* _battleWorkPointer;
    extern void* get_ptr(void);
    extern void* BattleGetMarioPtr(void* battleWork);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern void* effStarStoneEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
    extern f64 intplGetValue(f64 start, f64 end, s32 type, s32 current, s32 max);
    extern u32 vec3_803010fc[];
    extern u32 vec3_80301108[];

    u8* work = get_ptr();
    void* mario = BattleGetMarioPtr(_battleWorkPointer);
    s32 state = *(s32*)(work + 0x3B4);

    if (state != 2) {
        if (state > 1) {
            if (state < 4) {
                *(s32*)(work + 0x3B8) += 1;
                *(f32*)(work + 0x3C0) =
                    (f32)intplGetValue(*(f32*)(work + 0x3CC),
                                       *(f32*)(work + 0x3D8), 1,
                                       *(s32*)(work + 0x3B8), 0x3C);
                if (*(s32*)(work + 0x3B8) > 0x3B) {
                    *(s32*)(work + 0x3B4) = 5;
                    *(s32*)(work + 0x3B8) = 0;
                }
            }
            goto update_effect;
        }
        if (state <= 0) {
            goto update_effect;
        }

        *(s32*)(work + 0x3B4) = 2;
        *(s32*)(work + 0x3B8) = 0;
        *(void**)(work + 0x404) = effStarStoneEntry(2, 0.0f, -1000.0f, 0.0f, 1.0f);
        BtlUnit_GetPos(mario,
                       (f32*)(work + 0x3BC),
                       (f32*)(work + 0x3C0),
                       (f32*)(work + 0x3C4));
        *(f32*)(work + 0x3C0) +=
            *(f32*)((s32)mario + 0x114) * (f32)*(s16*)((s32)mario + 0xCE) + 37.0f;
        *(u32*)(work + 0x3C8) = *(u32*)(work + 0x3BC);
        *(u32*)(work + 0x3CC) = *(u32*)(work + 0x3C0);
        *(u32*)(work + 0x3D0) = *(u32*)(work + 0x3C4);
        *(u32*)(work + 0x3D4) = *(u32*)(work + 0x3BC);
        *(u32*)(work + 0x3D8) = *(u32*)(work + 0x3C0);
        *(u32*)(work + 0x3DC) = *(u32*)(work + 0x3C4);
        *(f32*)(work + 0x3D8) += 50.0f;
        *(f32*)(work + 0x3DC) -= 1.0f;
        *(u32*)(work + 0x3EC) = vec3_803010fc[0];
        *(u32*)(work + 0x3F0) = vec3_803010fc[1];
        *(u32*)(work + 0x3F4) = vec3_803010fc[2];
        *(u32*)(work + 0x3F8) = vec3_80301108[0];
        *(u32*)(work + 0x3FC) = vec3_80301108[1];
        *(u32*)(work + 0x400) = vec3_80301108[2];
    }

    *(s32*)(work + 0x3B8) += 1;
    if (*(s32*)(work + 0x3B8) < 0x65) {
        f32 scale = (f32)intplGetValue(0.0, 2.0, 0,
                                      *(s32*)(work + 0x3B8), 100);

        *(f32*)(work + 0x3BC) =
            (f32)intplGetValue(*(f32*)(work + 0x3C8),
                               *(f32*)(work + 0x3D4), 0,
                               *(s32*)(work + 0x3B8), 100);
        *(f32*)(work + 0x3C0) =
            (f32)intplGetValue(*(f32*)(work + 0x3CC),
                               *(f32*)(work + 0x3D8), 0,
                               *(s32*)(work + 0x3B8), 100);
        *(f32*)(work + 0x3C4) =
            (f32)intplGetValue(*(f32*)(work + 0x3D0),
                               *(f32*)(work + 0x3DC), 0,
                               *(s32*)(work + 0x3B8), 100);
        *(f32*)(work + 0x3EC) = scale;
        *(f32*)(work + 0x3F0) = scale;
        *(f32*)(work + 0x3F4) = scale;
    } else {
        *(u32*)(work + 0x3BC) = *(u32*)(work + 0x3D4);
        *(u32*)(work + 0x3C0) = *(u32*)(work + 0x3D8);
        *(u32*)(work + 0x3C4) = *(u32*)(work + 0x3DC);
        *(f32*)(work + 0x3EC) = 2.0f;
        *(f32*)(work + 0x3F0) = 2.0f;
        *(f32*)(work + 0x3F4) = 2.0f;
    }
    *(f32*)(work + 0x3FC) =
        (f32)intplGetValue(0.0, 2160.0, 4,
                           *(s32*)(work + 0x3B8), 0x78);
    if (*(s32*)(work + 0x3B8) > 0x77) {
        *(s32*)(work + 0x3B4) = 3;
        *(s32*)(work + 0x3B8) = 0;
        *(u32*)(work + 0x3C8) = *(u32*)(work + 0x3BC);
        *(u32*)(work + 0x3CC) = *(u32*)(work + 0x3C0);
        *(u32*)(work + 0x3D0) = *(u32*)(work + 0x3C4);
        *(f32*)(work + 0x3D8) = 300.0f;
    }

update_effect:
    if (*(void**)(work + 0x404) != NULL) {
        u8* effectWork = *(u8**)((s32)*(void**)(work + 0x404) + 0xC);
        *(u32*)(effectWork + 8) = *(u32*)(work + 0x3BC);
        *(u32*)(effectWork + 0xC) = *(u32*)(work + 0x3C0);
        *(u32*)(effectWork + 0x10) = *(u32*)(work + 0x3C4);
        *(f32*)(effectWork + 0x14) = 1.5f * *(f32*)(work + 0x3EC);
        *(u32*)(effectWork + 0x18) = *(u32*)(work + 0x3F8);
        *(u32*)(effectWork + 0x1C) = *(u32*)(work + 0x3FC);
        *(u32*)(effectWork + 0x20) = *(u32*)(work + 0x400);
    }
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
    extern void* _battleWorkPointer;
    extern void* get_ptr(void);
    extern void* BattleGetMarioPtr(void* battleWork);
    extern u16 keyGetButtonTrg(s32 pad);
    extern u32 keyGetDirTrg(s32 pad);
    extern f64 intplGetValue(f64 start, f64 end, s32 type, s32 current, s32 max);
    extern void* effHitEntry(f32 x, f32 y, f32 z, s32 type, s32 count, s32 arg);
    u8* work = get_ptr();
    void* mario = BattleGetMarioPtr(_battleWorkPointer);
    s32 state = *(s32*)(work + 0x38);
    s32 index = *(s32*)(work + 0x40);
    u32 dir;
    (void)mario;
    if (state == 1 && *(s32*)(work + 0x108) >= 5) {
        *(s32*)(work + 0x38) = 2;
        *(s32*)(work + 0x3C) = 0;
        *(f32*)(work + 0x44) = *(f32*)(work + 0x110);
        *(f32*)(work + 0x48) = *(f32*)(work + 0x114);
        *(f32*)(work + 0x4C) = *(f32*)(work + 0x118);
        state = 2;
    }
    if (state == 2) {
        if ((keyGetButtonTrg(0) & 0x100) != 0) {
            u8* point = *(u8**)(work + 0x98) + index * 0x24;
            *(s32*)(work + 0x38) = 5;
            *(s32*)point = 5;
            effHitEntry(*(f32*)(work + 0x44), *(f32*)(work + 0x48), *(f32*)(work + 0x4C), 0, 5, 0);
            if (*(s32*)(point + 8) == 0) {
                *(f32*)(work + 0x14) += 0.2f;
                if (*(f32*)(work + 0x14) > 9.0f) *(f32*)(work + 0x14) = 9.0f;
                *(s32*)(work + 0x24) = 30;
            } else if (*(s32*)(point + 8) == 1) {
                *(f32*)(work + 0x1C) += 0.2f;
                if (*(f32*)(work + 0x1C) > 9.0f) *(f32*)(work + 0x1C) = 9.0f;
                *(s32*)(work + 0x28) = 30;
            }
            return;
        }
        dir = keyGetDirTrg(0);
        if ((dir & 0x8000) && index % 3 != 2) index++;
        else if ((dir & 0x4000) && index % 3 != 0) index--;
        else if ((dir & 0x1000) && index / 3 != 0) index -= 3;
        else if ((dir & 0x2000) && index / 3 != 2) index += 3;
        if (index != *(s32*)(work + 0x40)) {
            *(f32*)(work + 0x50) = *(f32*)(work + 0x44);
            *(f32*)(work + 0x54) = *(f32*)(work + 0x48);
            *(s32*)(work + 0x40) = index;
            *(s32*)(work + 0x38) = 3;
            *(s32*)(work + 0x3C) = 0;
        }
    } else if (state == 3) {
        *(s32*)(work + 0x38) = 4;
        *(s32*)(work + 0x3C) = 0;
    } else if (state == 4) {
        u8* point = *(u8**)(work + 0x98) + index * 0x24;
        s32 timer = ++*(s32*)(work + 0x3C);
        *(f32*)(work + 0x44) = (f32)intplGetValue(*(f32*)(work + 0x50), *(f32*)(point + 0x18), 0, timer, 5);
        *(f32*)(work + 0x48) = (f32)intplGetValue(*(f32*)(work + 0x54), *(f32*)(point + 0x1C), 0, timer, 5);
        if (timer >= 5) *(s32*)(work + 0x38) = 2;
    } else if (state == 5) {
        *(s32*)(work + 0x38) = 6;
    } else if (state == 6) {
        *(s32*)(work + 0x38) = 2;
    }
}

/* stub-fill: main_base | prototype_only | source_prototype */
void main_base(void) {
    extern void* _battleWorkPointer;
    extern void* get_ptr(void);
    extern void* BattleGetMarioPtr(void* battleWork);
    extern s32 BtlUnit_GetBodyPartsId(void* unit);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partId);
    extern void BtlUnit_SetAnim(void* part, const char* name);
    extern f64 intplGetValue(f64 start, f64 end, s32 type, s32 current, s32 max);
    extern void* evtEntry(const void* code, s32 priority, u32 flags);
    extern const char str_M_X_1_804285d0[];
    extern const char str_M_S_1_804285d8[];
    extern const s32 nice_event_pow[];

    u8* work = get_ptr();
    void* mario = BattleGetMarioPtr(_battleWorkPointer);
    void* part = BtlUnit_GetPartsPtr(mario, BtlUnit_GetBodyPartsId(mario));
    s32 i;

    if (*(s32*)(work + 0x24) > 0) {
        *(s32*)(work + 0x24) -= 1;
    }
    if (*(s32*)(work + 0x28) > 0) {
        *(s32*)(work + 0x28) -= 1;
    }

    switch (*(s32*)(work + 4)) {
        case 1:
            *(s32*)(work + 4) = 2;
            *(f32*)(work + 0x14) = 0.0f;
            *(f32*)(work + 0x18) = 0.0f;
            *(f32*)(work + 0x1C) = 0.0f;
            *(f32*)(work + 0x20) = 0.0f;
            *(f32*)(work + 0x2C) = -400.0f;
            *(f32*)(work + 0x30) = 90.0f;
            *(s32*)(work + 0xC) = 899;
            for (i = 0; i < 9; i++) {
                *(s32*)(work + 0x78 + i * 0x24) = 1;
            }
            /* fallthrough */
        case 2: {
            s32 complete = 0;
            for (i = 0; i < 9; i++) {
                if (*(s32*)(work + 0x78 + i * 0x24) == 4) {
                    complete++;
                } else {
                    break;
                }
            }
            if (complete == 9) {
                *(s32*)(work + 4) = 3;
                *(s32*)(work + 8) = 0;
            }
            break;
        }
        case 3:
            *(s32*)(work + 8) += 1;
            *(f32*)(work + 0x2C) =
                (f32)intplGetValue(-400.0, -120.0, 4,
                                   *(s32*)(work + 8), 0x1E);
            if (*(s32*)(work + 8) > 0x1D) {
                *(s32*)(work + 4) = 4;
                *(s32*)(work + 8) = 0;
                *(s32*)(work + 0x38) = 1;
                for (i = 0; i < 9; i++) {
                    *(s32*)(work + 0x78 + i * 0x24) = 5;
                }
            }
            break;
        case 4:
            if ((*(u32*)work & 1) != 0) {
                *(s32*)(work + 0x34) += 1;
                if (*(s32*)(work + 0x34) == 0x1E) {
                    BtlUnit_SetAnim(part, str_M_X_1_804285d0);
                }
            }
            *(s32*)(work + 0xC) -= 1;
            if (*(s32*)(work + 0xC) < 1) {
                s32 result;

                *(s32*)(work + 4) = 5;
                *(s32*)(work + 8) = 0;
                *(s32*)(work + 0x38) = 7;
                BtlUnit_SetAnim(part, str_M_S_1_804285d8);
                for (i = 0; i < 9; i++) {
                    *(s32*)(work + 0x78 + i * 0x24) = 6;
                    *(s32*)(work + 0x1BC + i * 0x38) = 9;
                }

                result = (s32)(*(f32*)(work + 0x14) + *(f32*)(work + 0x1C));
                if (result > 0) {
                    void* event;

                    if (result < 5) {
                        result += 1;
                    } else {
                        result = 6;
                    }
                    event = evtEntry(nice_event_pow, 0, 0x20);
                    *(s32*)((s32)event + 0x9C) = result;
                }
            }
            break;
        case 5: {
            s32 complete = 0;
            for (i = 0; i < 9; i++) {
                if (*(s32*)(work + 0x78 + i * 0x24) == 8) {
                    complete++;
                } else {
                    break;
                }
            }
            if (complete == 9) {
                *(s32*)(work + 4) = 6;
                *(s32*)(work + 8) = 0;
            }
            break;
        }
        case 6:
            *(s32*)(work + 8) += 1;
            if (*(s32*)(work + 8) > 0x1D) {
                *(s32*)(work + 4) = 7;
                *(s32*)(work + 8) = 0;
            }
            break;
    }

    {
        f32 delta = *(f32*)(work + 0x14) - *(f32*)(work + 0x18);
        if (delta != 0.0f) {
            if (delta <= 0.0f) {
                f32 step = 0.1f;
                if (0.1f * delta <= -0.1f) {
                    step = -0.1f * delta;
                }
                *(f32*)(work + 0x18) -= step;
                if (*(f32*)(work + 0x14) - *(f32*)(work + 0x18) > 0.0f) {
                    *(f32*)(work + 0x18) = *(f32*)(work + 0x14);
                }
            } else {
                f32 step = 0.1f * delta;
                if (step < 0.1f) {
                    step = 0.1f;
                }
                *(f32*)(work + 0x18) += step;
                if (*(f32*)(work + 0x14) - *(f32*)(work + 0x18) < 0.0f) {
                    *(f32*)(work + 0x18) = *(f32*)(work + 0x14);
                }
            }
        }
    }
    {
        f32 delta = *(f32*)(work + 0x1C) - *(f32*)(work + 0x20);
        if (delta != 0.0f) {
            if (delta <= 0.0f) {
                f32 step = 0.1f;
                if (0.1f * delta <= -0.1f) {
                    step = -0.1f * delta;
                }
                *(f32*)(work + 0x20) -= step;
                if (*(f32*)(work + 0x1C) - *(f32*)(work + 0x20) > 0.0f) {
                    *(f32*)(work + 0x20) = *(f32*)(work + 0x1C);
                }
            } else {
                f32 step = 0.1f * delta;
                if (step < 0.1f) {
                    step = 0.1f;
                }
                *(f32*)(work + 0x20) += step;
                if (*(f32*)(work + 0x1C) - *(f32*)(work + 0x20) < 0.0f) {
                    *(f32*)(work + 0x20) = *(f32*)(work + 0x1C);
                }
            }
        }
    }
}

