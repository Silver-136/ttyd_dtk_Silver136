#include "action/star/sac_deka.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
void effDelete(void* effect);
void smartFree(void* ptr);
void mapGrpFlagOff(const char* name, s32 flag);
extern const char str_stg_b_yuka_80301050[];
extern f32 float_0_80428424;
extern f32 float_0p5_804283ec;
extern f32 float_32767_80428404;
extern void* memcpy(void* dst, const void* src, u32 size);
void* get_ptr(void);
void main_timing(s32 index);
void main_enemy(void);
void disp_3D(void);
void disp_3D_alpha(void);
void yuka_init(void);
void main_base(void);
void main_gauge(void);
void main_star(void);
void yuka_main(void);
void disp_2D(void);
void yuka_capture(s32 cameraId);
void create_timing(void);
void main_enemy_sub(void* unit, s32* count);

USER_FUNC(main_deka) {
    extern void* memset(void* dest, s32 value, u32 size);
    extern void dispEntry(s32 cameraId, s32 order, void* callback, f32 priority, void* param);
    extern void yuka_disp(void);
    extern f32 float_901_80428490;

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

/* stub-fill: yuka_init | prototype_only | source_prototype */
void yuka_init(void) {
    typedef struct Vec { f32 x, y, z; } Vec;
    typedef f32 Mtx[3][4];
    extern void* mapGetMapObj(const char* name);
    extern void mapGrpFlagOn(const char* name, u32 flags);
    extern void PSMTXMultVec(Mtx matrix, Vec* source, Vec* destination);
    extern u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, u8 mipmap, u8 maxLevel);
    extern void* smartAlloc(u32 size, s32 type);
    extern void dispEntry(s32 camera, s32 renderMode, void* callback, void* param, f32 order);
    extern f32 float_0p0078125_80428444;
    extern f32 vec3_80300e98[];
    u8* data = (u8*)vec3_80300e98;
    u8* work = (u8*)get_ptr();
    u8* map = (u8*)mapGetMapObj((const char*)(data + 0x1B8));
    u8* memory;
    s32 width, height, row, column;
    u32 textureSize;
    mapGrpFlagOn((const char*)(data + 0x1B8), 0x400001);
    PSMTXMultVec((f32(*)[4])(map + 0x1C), (Vec*)(*(u8**)(map + 8) + 0x3C), (Vec*)(work + 4));
    PSMTXMultVec((f32(*)[4])(map + 0x1C), (Vec*)(*(u8**)(map + 8) + 0x48), (Vec*)(work + 0x10));
    width = (s32)(*(f32*)(work + 0x10) - *(f32*)(work + 4));
    *(s32*)(work + 0x24) = width;
    *(s32*)(work + 0x1C) = width;
    height = (s32)(*(f32*)(work + 0x18) - *(f32*)(work + 0xC));
    *(s32*)(work + 0x28) = height;
    *(s32*)(work + 0x20) = height;
    *(u32*)(work + 0x24) += *(u32*)(work + 0x24) & 1;
    *(u32*)(work + 0x28) += *(u32*)(work + 0x28) & 1;
    if (*(s32*)(work + 0x24) > 0x260) *(s32*)(work + 0x24) = 0x260;
    if (*(s32*)(work + 0x28) > 0x1E0) *(s32*)(work + 0x28) = 0x1E0;
    textureSize = GXGetTexBufferSize((u16)*(u32*)(work + 0x24), (u16)*(u32*)(work + 0x28), 4, 0, 0);
    *(void**)work = smartAlloc(textureSize, 2);
    *(void**)(work + 0x2C) = smartAlloc(0x3870, 2);
    memory = **(u8***)(work + 0x2C);
    for (row = 0; row < 2; row++) {
        for (column = 0; column < 0x81; column++) {
            f32* vertex = (f32*)(memory + row * 0x60C + column * 0xC);
            f32 x = *(f32*)(work + 4) + (f32)*(s32*)(work + 0x1C) * float_0p0078125_80428444 * (f32)column;
            f32 z = *(f32*)(work + 0xC) + (f32)*(s32*)(work + 0x20) * (f32)row;
            Vec upper = *(Vec*)(data + 0x00);
            Vec lower = *(Vec*)(data + 0x0C);
            Vec velocity = *(Vec*)(data + 0x18);
            upper.x = x;
            upper.z = z;
            lower.x = x;
            lower.z = z;
            *(Vec*)(vertex + 0x306) = upper;
            *(Vec*)vertex = lower;
            *(Vec*)(vertex + 0x60C) = velocity;
            *(f32*)(memory + 0x3060 + row * 0x204 + column * 4) = float_0p0078125_80428444 * (f32)column;
            *(f32*)(memory + 0x3468 + row * 0x204 + column * 4) = (f32)row;
        }
    }
    dispEntry(2, 0, yuka_capture, 0, float_0_80428424);
}

/* stub-fill: yuka_main | prototype_only | source_prototype */
void yuka_main(void) {
    extern void memcpy_as4(void* dst, const void* src, u32 size);
    extern void PSVECSubtract(const void* a, const void* b, void* out);
    extern void PSVECNormalize(const void* src, void* dst);
    extern f32 PSVECMag(const void* vec);
    extern void PSVECScale(const void* src, void* dst, f32 scale);
    extern void PSVECAdd(const void* a, const void* b, void* out);
    extern u32 vec3_80300ebc[];

    u8* work = get_ptr();
    u8* memory = **(u8***)(work + 0x2C);
    s32 i;
    s32 row;

    for (i = 0; i < 6; i++) {
        if (*(u8*)(work + 0x518 + i) == 1) {
            f32* position = (f32*)(work + 0x550 + i * 4);
            f32* velocity = (f32*)(work + 0x520 + i * 4);
            s32* point = (s32*)(work + 0x538 + i * 4);
            f32 impulse = *(f32*)(work + 0x568 + i * 4);

            *position += *velocity;
            for (;;) {
                s32 current = (s32)*position;

                if ((current <= *point || *velocity <= 0.0f) &&
                    (*point <= current || *velocity >= 0.0f)) {
                    break;
                }
                *(f32*)(memory + 0x1834 + *point * 0xC) += impulse;
                *(f32*)(memory + 0x1834 + 0x60C + *point * 0xC) += impulse;
                if (*velocity <= 0.0f) {
                    (*point)--;
                } else {
                    (*point)++;
                }
                if (*point <= 0 || *point >= 0x80) {
                    *(u8*)(work + 0x518 + i) = 0;
                    break;
                }
            }
        }
    }

    memcpy_as4(memory + 0x2448, memory, 0xC18);
    for (row = 0; row < 2; row++) {
        s32 rowOffset = row * 0x60C;

        for (i = 0; i < 0x81; i++) {
            u8* point = memory + rowOffset + i * 0xC;
            u8* backup = point + 0x2448;
            f32 force[3];
            f32 delta[3];
            f32 normal[3];
            f32 scaledVelocity[3];
            f32 magnitude;

            force[0] = *(f32*)&vec3_80300ebc[0];
            force[1] = *(f32*)&vec3_80300ebc[1];
            force[2] = *(f32*)&vec3_80300ebc[2];

            PSVECSubtract(backup, backup + 0xC, delta);
            PSVECNormalize(delta, normal);
            magnitude = PSVECMag(delta);
            PSVECScale(normal, normal, 1.0f - magnitude);
            PSVECAdd(force, normal, force);

            PSVECSubtract(backup, backup - 0xC, delta);
            PSVECNormalize(delta, normal);
            magnitude = PSVECMag(delta);
            PSVECScale(normal, normal, 1.0f - magnitude);
            PSVECAdd(force, normal, force);

            force[0] = 0.0f;
            force[2] = 0.0f;
            PSVECScale(force, force, 0.01f);
            PSVECAdd(point + 0x1830, force, point + 0x1830);
            PSVECScale(point + 0x1830, scaledVelocity, 0.1f);
            PSVECAdd(point, scaledVelocity, point);

            *(f32*)(point + 4) += *(f32*)(point + 0x1834);
            *(f32*)(point + 0x1834) -= 2.0f;
            if (*(f32*)(point + 4) <= 0.0f) {
                *(f32*)(point + 4) = 0.0f;
                *(f32*)(point + 0x1834) = 0.0f;
            }
        }
    }

    for (row = 0; row < 2; row++) {
        s32 rowOffset = row * 0x60C;

        for (i = 1; i < 0x80; i++) {
            f32* point = (f32*)(memory + rowOffset + i * 0xC);
            point[1] = 0.5f * (point[-2] + point[4]);
        }
    }

    for (row = 0; row < 2; row++) {
        s32 rowOffset = row * 0x60C;

        for (i = 0; i < 0x81; i++) {
            u32* point = (u32*)(memory + rowOffset + i * 0xC);
            u32* backup = (u32*)((u8*)point + 0x2448);
            backup[0] = point[0];
            backup[1] = point[1];
            backup[2] = point[2];
        }
    }
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

/* stub-fill: main_base | prototype_only | source_prototype */
void main_base(void) {
    extern void* get_ptr(void);
    extern void create_timing(void);
    extern u32 psndSFXOn(const char* name);
    extern void psndSFXOff(u32 id);
    extern void psndSFX_pit(u32 id, s32 pitch);
    extern void* evtEntry(const void* code, s32 priority, u32 flags);
    extern u16 keyGetButtonTrg(s32 controller);
    extern void BattleAudienceSoundHandBeat(void);
    extern void BattleAudienceSoundBooing(void);
    extern f64 intplGetValue(f64 start, f64 end, s32 type, s32 current, s32 max);
    extern const s32 nice_event[];
    extern const char str_SFX_BTL_SAC_DOKKAN1__80301010[];
    extern const char str_SFX_BTL_SAC_DOKKAN2_80301028[];
    extern const char str_SFX_BTL_SAC_DOKKAN3_8030103c[];

    u8* work = get_ptr();
    s32 state = *(s32*)(work + 0x30);
    f32 values[8];
    s32 order[8];
    s32 i;
    s32 j;

    switch (state) {
        case 1:
            *(s32*)(work + 0x30) = 2;
            *(u32*)(work + 0x40) = 1;
            *(s32*)(work + 0x44) = 0;
            *(f32*)(work + 0x38) = 180.0f;
            *(f32*)(work + 0x3C) = 0.0f;
            create_timing();
            *(s32*)(work + 0x50) = 1;
            /* fallthrough */
        case 2:
            if (*(s32*)(work + 0x50) != 3) {
                return;
            }
            *(s32*)(work + 0x30) = 3;
            *(u32*)(work + 0x6C) = psndSFXOn(str_SFX_BTL_SAC_DOKKAN1__80301010);
            break;
        case 3:
            break;
        case 4:
            *(s32*)(work + 0x34) -= 1;
            *(f32*)(work + 0x4C) =
                (f32)intplGetValue(-40.0, 0.0, 0,
                                   0x1E - *(s32*)(work + 0x34), 0x1E);
            if (*(s32*)(work + 0x34) > 0) {
                return;
            }
            *(s32*)(work + 0x30) = 3;
            *(u32*)(work + 0x6C) = psndSFXOn(str_SFX_BTL_SAC_DOKKAN1__80301010);
            return;
        default:
            return;
    }

    if (*(s32*)(work + 0x50) == 3) {
        s32 gaugeIndex = *(s32*)(work + 0x44);
        s32 pitch;

        *(f32*)(work + 0x3C) += 0.1f * (f32)gaugeIndex + 0.8f;
        pitch = (s32)(683.0f * (f32)gaugeIndex +
                      (4096.0f * *(f32*)(work + 0x3C)) / *(f32*)(work + 0x38));
        psndSFX_pit(*(u32*)(work + 0x6C), pitch);

        if (*(f32*)(work + 0x3C) >= *(f32*)(work + 0x38)) {
            void* event;
            s32 audienceValue;

            *(f32*)(work + 0x3C) = 0.0f;
            *(u32*)(work + 0x40) += 1;
            *(s32*)(work + 0x44) += 1;
            *(s32*)(work + 0x3F0) = 6;
            psndSFXOff(*(u32*)(work + 0x6C));
            event = evtEntry(nice_event, 0, 0x20);
            audienceValue = 6;
            if (*(s32*)(work + 0x44) + 1 < 6) {
                audienceValue = *(s32*)(work + 0x44) + 1;
            }
            *(s32*)((s32)event + 0x9C) = audienceValue;

            if (*(s32*)(work + 0x40) > 5) {
                *(s32*)(work + 0x30) = 5;
                *(s32*)(work + 0x50) = 5;
            } else {
                *(s32*)(work + 0x30) = 4;
                *(s32*)(work + 0x34) = 0x1E;
                *(f32*)(work + 0x4C) = -40.0f;
                create_timing();
            }
        }
    }

    for (i = 0; i < 8; i++) {
        order[i] = i;
        values[i] = *(f32*)(work + 0x70 + i * 0x38 + 0xC);
    }
    for (i = 0; i < 7; i++) {
        for (j = i + 1; j < 8; j++) {
            if (values[j] < values[i]) {
                f32 value = values[i];
                s32 index = order[i];
                values[i] = values[j];
                order[i] = order[j];
                values[j] = value;
                order[j] = index;
            }
        }
    }

    if ((keyGetButtonTrg(0) & 0x100) != 0) {
        for (i = 0; i < 8; i++) {
            u8* timing = work + 0x70 + order[i] * 0x38;

            if (*(s32*)timing == 2 &&
                *(f32*)(timing + 0xC) - 7.0f <= *(f32*)(work + 0x3C) &&
                *(f32*)(work + 0x3C) <= *(f32*)(timing + 0xC) + 7.0f) {
                if (*(s32*)(timing + 8) == 1) {
                    *(s32*)timing = 3;
                    *(u8*)(timing + 0x35) = 1;
                    psndSFXOn(str_SFX_BTL_SAC_DOKKAN2_80301028);
                    BattleAudienceSoundHandBeat();
                } else {
                    *(s32*)(work + 0x44) = 0;
                    *(s32*)(work + 0x30) = 5;
                    *(s32*)(work + 0x50) = 4;
                    *(s32*)(work + 0x3F0) = 8;
                    *(u8*)(timing + 0x35) = 2;
                    psndSFXOn(str_SFX_BTL_SAC_DOKKAN3_8030103c);
                    BattleAudienceSoundBooing();
                    psndSFXOff(*(u32*)(work + 0x6C));
                }
                break;
            }
        }
        if (i == 8) {
            for (i = 0; i < 8; i++) {
                u8* timing = work + 0x70 + order[i] * 0x38;

                if (*(s32*)timing == 2 && *(s32*)(timing + 8) == 1) {
                    *(s32*)(work + 0x30) = 5;
                    *(s32*)(work + 0x50) = 4;
                    *(u8*)(timing + 0x35) = 2;
                    psndSFXOn(str_SFX_BTL_SAC_DOKKAN3_8030103c);
                    BattleAudienceSoundBooing();
                    psndSFXOff(*(u32*)(work + 0x6C));
                    return;
                }
            }
        }
    }

    for (i = 0; i < 8; i++) {
        u8* timing = work + 0x70 + order[i] * 0x38;

        if (*(s32*)timing == 2 && *(s32*)(timing + 8) != 2 &&
            *(f32*)(work + 0x3C) > *(f32*)(timing + 0xC) + 7.0f) {
            *(s32*)(work + 0x30) = 5;
            *(s32*)(work + 0x50) = 4;
            *(u8*)(timing + 0x35) = 2;
            psndSFXOn(str_SFX_BTL_SAC_DOKKAN3_8030103c);
            BattleAudienceSoundBooing();
            psndSFXOff(*(u32*)(work + 0x6C));
            return;
        }
    }
}

/* stub-fill: main_gauge | prototype_only | source_prototype */
void main_gauge(void) {
    extern f32 intplGetValue(s32 type, s32 current, f32 start, f32 end, s32 max);
    extern f32 float_500_80428470;
    extern f32 float_neg1000_8042844c;
    extern u32 vec3_80300ec8[];

    void* work = get_ptr();
    s32 blink;

    switch (*(s32*)((s32)work + 0x50)) {
        case 1:
            *(s32*)((s32)work + 0x50) = 2;
            *(s32*)((s32)work + 0x54) = 0;
            *(f32*)((s32)work + 0x58) = float_500_80428470;
            *(u32*)((s32)work + 0x5C) = vec3_80300ec8[0];
            *(u32*)((s32)work + 0x60) = vec3_80300ec8[1];
            *(u32*)((s32)work + 0x64) = vec3_80300ec8[2];
            *(u8*)((s32)work + 0x68) = 0xFF;
            /* fallthrough */
        case 2:
            *(s32*)((s32)work + 0x54) += 1;
            *(f32*)((s32)work + 0x5C) = intplGetValue(4, *(s32*)((s32)work + 0x54), float_neg1000_8042844c, float_0_80428424, 0x1E);
            if (*(s32*)((s32)work + 0x54) >= 0x1E) {
                *(s32*)((s32)work + 0x50) = 3;
                *(s32*)((s32)work + 0x54) = 0;
            }
            break;
        case 3:
            *(s32*)((s32)work + 0x54) = 0;
            break;
        case 4:
            *(s32*)((s32)work + 0x54) += 1;
            blink = *(s32*)((s32)work + 0x54) % 12;
            if (blink >= 0 && blink <= 5) {
                *(u8*)((s32)work + 0x68) = 0;
            } else {
                *(u8*)((s32)work + 0x68) = 0xFF;
            }
            if (*(s32*)((s32)work + 0x54) >= 0x50) {
                *(s32*)((s32)work + 0x50) = 5;
                *(s32*)((s32)work + 0x54) = 0;
                *(u8*)((s32)work + 0x68) = 0xFF;
            }
            break;
        case 5:
            *(s32*)((s32)work + 0x54) += 1;
            *(f32*)((s32)work + 0x5C) = intplGetValue(4, *(s32*)((s32)work + 0x54), float_0_80428424, float_neg1000_8042844c, 0x1E);
            if (*(s32*)((s32)work + 0x54) >= 0x1E) {
                *(s32*)((s32)work + 0x50) = 3;
                *(s32*)((s32)work + 0x54) = 0;
            }
            break;
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

/* stub-fill: main_star | prototype_only | source_prototype */
void main_star(void) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void* _battleWorkPointer;
    extern void* get_ptr(void);
    extern void* BattleGetMarioPtr(void*);
    extern void BtlUnit_GetPos(void*, f32*, f32*, f32*);
    extern void* effStarStoneEntry(f32, f32, f32, f32, s32);
    extern f64 intplGetValue(f64, f64, s32, s32, s32);
    extern s32 BattleAudience_GetAudienceNum(void);
    extern void BattleAudienceSoundCallKind(s32);
    extern void BattleAudienceSoundWhistleKind(s32);
    extern void psndSFXOn(const char*);
    extern void btl_camera_shake_h(s32, f32, f32, s32, s32);
    extern const char str_SFX_BTL_SAC_DOKKAN5_80300ffc[];
    extern f32 float_5_804283f8;
    extern f32 float_neg0p5_80428464;
    extern f32 float_40_80428400;
    extern f32 float_255_80428430;
    extern f32 float_2_80428414;
    extern f32 float_neg1_8042842c;
    extern f32 float_43_80428468;
    extern f32 float_4_804283fc;
    extern f32 dat_80300f50[6];
    extern f32 dat_80300f68[6];
    extern s32 dat_80300ef8[11];
    extern s32 dat_80300f24[11];
    extern Vec vec3_80300ee0;
    extern Vec vec3_80300eec;
    u8* work = get_ptr();
    u8* mario = BattleGetMarioPtr(_battleWorkPointer);
    s32 timer;
    s32 audience = BattleAudience_GetAudienceNum();
    u8 alpha = 0xFF;

    switch (*(s32*)(work + 0x3F0)) {
    case 1:
        *(s32*)(work + 0x3F0) = 2;
        *(s32*)(work + 0x3F4) = 0;
        *(void**)(work + 0x44C) = effStarStoneEntry(0.0f, -1000.0f, 0.0f, 1.0f, 0);
        BtlUnit_GetPos(mario, (f32*)(work + 0x3FC), (f32*)(work + 0x400), (f32*)(work + 0x404));
        *(f32*)(work + 0x400) += *(f32*)(mario + 0x114) * (f32)*(s16*)(mario + 0xCE) + 37.0f;
        *(f32*)(work + 0x408) = *(f32*)(work + 0x3FC);
        *(f32*)(work + 0x40C) = *(f32*)(work + 0x400);
        *(f32*)(work + 0x410) = *(f32*)(work + 0x404);
        *(f32*)(work + 0x414) = *(f32*)(work + 0x3FC);
        *(f32*)(work + 0x418) = *(f32*)(work + 0x400) + 50.0f;
        *(f32*)(work + 0x41C) = *(f32*)(work + 0x404) - 1.0f;
        *(Vec*)(work + 0x42C) = vec3_80300ee0;
        *(Vec*)(work + 0x438) = vec3_80300eec;
        /* fallthrough */
    case 2:
        timer = ++*(s32*)(work + 0x3F4);
        if (timer < 101) {
            *(f32*)(work + 0x3FC) = (f32)intplGetValue(*(f32*)(work + 0x408), *(f32*)(work + 0x414), 0, timer, 100);
            *(f32*)(work + 0x400) = (f32)intplGetValue(*(f32*)(work + 0x40C), *(f32*)(work + 0x418), 0, timer, 100);
            *(f32*)(work + 0x404) = (f32)intplGetValue(*(f32*)(work + 0x410), *(f32*)(work + 0x41C), 0, timer, 100);
            *(f32*)(work + 0x42C) = *(f32*)(work + 0x430) = *(f32*)(work + 0x434) =
                (f32)intplGetValue(0.0, 2.0, 0, timer, 100);
        } else {
            *(f32*)(work + 0x3FC) = *(f32*)(work + 0x414);
            *(f32*)(work + 0x400) = *(f32*)(work + 0x418);
            *(f32*)(work + 0x404) = *(f32*)(work + 0x41C);
            *(f32*)(work + 0x42C) = *(f32*)(work + 0x430) = *(f32*)(work + 0x434) = 2.0f;
        }
        *(f32*)(work + 0x43C) = (f32)intplGetValue(0.0, 2160.0, 4, timer, 120);
        if (timer > 119) {
            *(s32*)(work + 0x3F0) = 3;
            *(s32*)(work + 0x3F4) = 0;
            *(f32*)(work + 0x408) = *(f32*)(work + 0x3FC);
            *(f32*)(work + 0x40C) = *(f32*)(work + 0x400);
            *(f32*)(work + 0x410) = *(f32*)(work + 0x404);
            *(f32*)(work + 0x414) = -100.0f;
            *(f32*)(work + 0x418) = 30.0f;
            *(f32*)(work + 0x41C) = 50.0f;
        }
        break;
    case 3:
        timer = ++*(s32*)(work + 0x3F4);
        *(f32*)(work + 0x3FC) = (f32)intplGetValue(*(f32*)(work + 0x408), *(f32*)(work + 0x414), 4, timer, 60);
        *(f32*)(work + 0x400) = (f32)intplGetValue(*(f32*)(work + 0x40C), *(f32*)(work + 0x418), 4, timer, 60);
        *(f32*)(work + 0x404) = (f32)intplGetValue(*(f32*)(work + 0x410), *(f32*)(work + 0x41C), 4, timer, 60);
        if (timer > 59) {
            *(s32*)(work + 0x3F0) = 4;
            *(s32*)(work + 0x3F4) = 0;
        }
        break;
    case 6:
        *(s32*)(work + 0x3F0) = 7;
        *(s32*)(work + 0x3F4) = 0;
        *(f32*)(work + 0x444) = *(f32*)(work + 0x42C);
        *(f32*)(work + 0x448) = *(f32*)(work + 0x42C) + 1.0f;
        psndSFXOn("SFX_BTL_SAC_DOKKAN4");
        if (audience > 0 && audience < 50) {
            BattleAudienceSoundCallKind(1);
        }
        if (audience > 49 && audience < 100) {
            BattleAudienceSoundCallKind(1);
            BattleAudienceSoundWhistleKind(1);
        }
        if (audience > 99 && audience < 150) {
            BattleAudienceSoundCallKind(2);
            BattleAudienceSoundWhistleKind(2);
        }
        if (audience > 149) {
            BattleAudienceSoundCallKind(2);
            BattleAudienceSoundWhistleKind(3);
        }
        /* fallthrough */
    case 7: {
        s32 bounceTiming[11];
        s32 copyIndex;
        for (copyIndex = 0; copyIndex < 11; copyIndex++) {
            bounceTiming[copyIndex] = dat_80300ef8[copyIndex];
        }
        timer = ++*(s32*)(work + 0x3F4);
        *(f32*)(work + 0x42C) = (f32)intplGetValue(
            *(f32*)(work + 0x444), *(f32*)(work + 0x448), 4,
            bounceTiming[timer >> 2], 5);
        *(f32*)(work + 0x430) = *(f32*)(work + 0x42C);
        *(f32*)(work + 0x434) = *(f32*)(work + 0x42C);
        *(f32*)(work + 0x400) =
            12.5f * (*(f32*)(work + 0x430) - 2.0f) + 30.0f;
        if (timer > 42) {
            *(s32*)(work + 0x3F0) = 5;
            *(s32*)(work + 0x3F4) = 0;
            *(f32*)(work + 0x42C) = *(f32*)(work + 0x448);
            *(f32*)(work + 0x430) = *(f32*)(work + 0x448);
            *(f32*)(work + 0x434) = *(f32*)(work + 0x448);
        }
        break;
    }
    case 8:
        *(s32*)(work + 0x3F0) = 9;
        *(s32*)(work + 0x3F4) = 0;
        *(f32*)(work + 0x444) = *(f32*)(work + 0x42C);
        *(f32*)(work + 0x448) = 2.0f;
        /* fallthrough */
    case 9: {
        s32 bounceTiming[11];
        s32 copyIndex;
        for (copyIndex = 0; copyIndex < 11; copyIndex++) {
            bounceTiming[copyIndex] = dat_80300f24[copyIndex];
        }
        timer = ++*(s32*)(work + 0x3F4);
        *(f32*)(work + 0x42C) = (f32)intplGetValue(
            *(f32*)(work + 0x444), *(f32*)(work + 0x448), 4,
            bounceTiming[timer >> 2], 5);
        *(f32*)(work + 0x430) = *(f32*)(work + 0x42C);
        *(f32*)(work + 0x434) = *(f32*)(work + 0x42C);
        *(f32*)(work + 0x400) =
            12.5f * (*(f32*)(work + 0x430) - 2.0f) + 30.0f;
        if (timer > 42) {
            *(s32*)(work + 0x3F0) = 5;
            *(s32*)(work + 0x3F4) = 0;
            *(f32*)(work + 0x42C) = *(f32*)(work + 0x448);
            *(f32*)(work + 0x430) = *(f32*)(work + 0x448);
            *(f32*)(work + 0x434) = *(f32*)(work + 0x448);
        }
        break;
    }
    case 10:
        *(s32*)(work + 0x3F0) = 11;
        *(f32*)(work + 0x418) = *(f32*)(work + 0x400);
        *(f32*)(work + 0x424) = float_5_804283f8;
        *(s32*)(work + 0x3F4) = 0;
        *(s32*)(work + 0x3F8) = 0;
        /* fallthrough */
    case 11:
        *(f32*)(work + 0x400) += *(f32*)(work + 0x424);
        *(f32*)(work + 0x424) += float_neg0p5_80428464;
        if (*(f32*)(work + 0x400) < *(f32*)(work + 0x418)) {
            f32 shakeScale[6];
            f32 launchScale[6];
            s32 side;
            s32 slot;
            for (slot = 0; slot < 6; slot++) {
                shakeScale[slot] = dat_80300f50[slot];
            }
            for (slot = 0; slot < 6; slot++) {
                launchScale[slot] = dat_80300f68[slot];
            }
            *(s32*)(work + 0x3F8) += 1;
            btl_camera_shake_h(0, (f32)*(s32*)(work + 0x3F8) *
                shakeScale[*(s32*)(work + 0x44)], 0.0f, 20, 0);
            psndSFXOn(str_SFX_BTL_SAC_DOKKAN5_80300ffc);
            for (side = 0; side < 2; side++) {
                for (slot = 0; slot < 6; slot++) {
                    if (*(u8*)(work + 0x518 + slot) == 0) {
                        *(u8*)(work + 0x518 + slot) = 1;
                        *(f32*)(work + 0x520 + slot * 4) = float_2_80428414;
                        if (side == 1) {
                            *(f32*)(work + 0x520 + slot * 4) *= float_neg1_8042842c;
                        }
                        *(s32*)(work + 0x538 + slot * 4) = side == 0 ? 42 : 41;
                        *(f32*)(work + 0x550 + slot * 4) = float_43_80428468;
                        *(f32*)(work + 0x568 + slot * 4) =
                            (f32)*(s32*)(work + 0x3F8) *
                            launchScale[*(s32*)(work + 0x44)] + float_4_804283fc;
                        break;
                    }
                }
            }
            if (*(s32*)(work + 0x3F8) < 6) {
                f32 bounce = float_4_804283fc * (f32)*(s32*)(work + 0x3F8) + 20.0f;
                *(f32*)(work + 0x400) = *(f32*)(work + 0x418);
                *(f32*)(work + 0x424) =
                    (0.5f * bounce * bounce) / (float_2_80428414 * bounce);
            } else {
                *(s32*)(work + 0x3F0) = 12;
                *(s32*)(work + 0x3F4) = 30;
                *(f32*)(work + 0x414) = *(f32*)(work + 0x3FC);
                *(f32*)(work + 0x418) = *(f32*)(work + 0x400);
                *(f32*)(work + 0x41C) = *(f32*)(work + 0x404);
                *(f32*)(work + 0x408) = *(f32*)(work + 0x414);
                *(f32*)(work + 0x40C) = *(f32*)(work + 0x418);
                *(f32*)(work + 0x410) = *(f32*)(work + 0x41C);
                *(f32*)(work + 0x418) += float_40_80428400;
            }
        }
        break;
    case 12:
        timer = --*(s32*)(work + 0x3F4);
        *(f32*)(work + 0x400) = (f32)intplGetValue(
            *(f32*)(work + 0x40C), *(f32*)(work + 0x418), 4,
            30 - timer, 30);
        if (timer < 1) {
            *(s32*)(work + 0x3F0) = 13;
            *(s32*)(work + 0x3F4) = 0;
        }
        break;
    case 13:
        timer = ++*(s32*)(work + 0x3F4);
        if (timer > 30) {
            timer = 30;
            *(s32*)(work + 0x3F4) = timer;
        }
        alpha = (u8)(s32)intplGetValue(float_255_80428430, 0.0, 0, timer, 30);
        break;
    }
    if (*(void**)(work + 0x44C) != 0) {
        u8* ew = *(u8**)((u8*)*(void**)(work + 0x44C) + 0xC);
        *(f32*)(ew + 8) = *(f32*)(work + 0x3FC);
        *(f32*)(ew + 0xC) = *(f32*)(work + 0x400);
        *(f32*)(ew + 0x10) = *(f32*)(work + 0x404);
        *(f32*)(ew + 0x14) = *(f32*)(work + 0x42C) * 1.5f;
        *(f32*)(ew + 0x18) = *(f32*)(work + 0x438);
        *(f32*)(ew + 0x1C) = *(f32*)(work + 0x43C);
        *(f32*)(ew + 0x20) = *(f32*)(work + 0x440);
        *(u8*)(ew + 0x5F) = alpha;
    }
}

void main_enemy(void) {
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
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
    for (i = count; i < 10; i++) {
        *(s32*)((s32)work + 0x450 + i * 4) = -1;
    }
}

/* stub-fill: main_enemy_sub | prototype_only | source_prototype */
void main_enemy_sub(void* unit, s32* count) {
    extern s32 BtlUnit_GetBodyPartsId(void* unit);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partId);
    extern s32 BtlUnit_CheckStatus(void* unit, s32 status);
    extern void BtlUnit_SetAnimType(void* part, s32 type);
    extern void effSmallStarEntry(s32, s32, f32, f32, f32, f32, f32, f32);
    extern void BtlUnit_snd_se(void* unit, const char* name, s32 volume, s16 pitch);
    extern s32 irand(s32 max);
    extern s32 BtlUnit_GetUnitId(void* unit);

    void* work = get_ptr();
    void* memory = **(void***)((s32)work + 0x2C);
    s32 index = *count;
    f32* position = (f32*)((s32)unit + 0x3C);
    f32* home = (f32*)((s32)unit + 0x30);
    f32* rotation = (f32*)((s32)unit + 0x6C);
    f32* rotateOffset = (f32*)((s32)unit + 0x78);
    f32* centerOffset = (f32*)((s32)unit + 0x84);
    f32* velocity = (f32*)((s32)work + 0x478 + index * 0xC);
    f32* angularVelocity = (f32*)((s32)work + 0x4F0 + index * 4);
    f32 originalX;
    f32 sampleX;
    s32 segment;
    s32 memoryOffset;

    rotateOffset[0] = centerOffset[0];
    rotateOffset[1] = centerOffset[1];
    rotateOffset[2] = centerOffset[2];
    velocity[1] -= 1.25f;

    originalX = position[0];
    if (originalX >= 250.0f) {
        sampleX = 250.0f;
    } else if (originalX <= 0.0f) {
        sampleX = 0.0f;
    } else {
        sampleX = originalX;
    }

    segment = 0x40;
    memoryOffset = 0x300;
    do {
        f32 segmentWidth = (f32)*(s32*)((s32)work + 0x1C) * 0.0078125f;
        f32 segmentStart = segmentWidth * (f32)segment + *(f32*)((s32)work + 4);

        if (segmentStart <= sampleX && sampleX < segmentStart + segmentWidth) {
            f32 leftHeight = *(f32*)((s32)memory + memoryOffset + 0xC1C);
            f32 height = ((sampleX - segmentStart) / segmentWidth) *
                             (*(f32*)((s32)memory + memoryOffset + 0xC28) - leftHeight) +
                         leftHeight;

            if (position[1] < height) {
                u32 attributes = *(u32*)((s32)unit + 0x104);

                if ((attributes & 2) != 0 && height < 120.0f) {
                    break;
                }
                if (*(f32*)((s32)memory + memoryOffset + 0x1834) > 0.0f) {
                    void* part;

                    if (velocity[1] < 0.0f) {
                        velocity[1] = 0.0f;
                    }
                    position[1] = height;
                    rotation[2] = 0.0f;
                    *angularVelocity = 0.0f;
                    velocity[1] +=
                        0.5f * *(f32*)((s32)memory + memoryOffset + 0x1834);

                    part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
                    if ((*(u32*)((s32)part + 0x1AC) & 0x20000000) == 0 &&
                        (*(u32*)((s32)part + 0x1AC) & 0x4000) == 0) {
                        if ((attributes & 0x20000) == 0 &&
                            BtlUnit_CheckStatus(unit, 0x1B) == 0) {
                            BtlUnit_SetAnimType(part, 0x27);
                        }
                        effSmallStarEntry(0, 2, sampleX, position[1], position[2],
                                          0.0f, -1.0f, 0.0f);
                        BtlUnit_snd_se(unit,
                            *(const char**)(*(s32*)((s32)unit + 0x10) + 0x9C),
                            -250000000, 0);

                        attributes = *(u32*)((s32)unit + 0x104);
                        if ((attributes & 0x200000) == 0 &&
                            (attributes & 0x20000) == 0 &&
                            BtlUnit_CheckStatus(unit, 0x1B) == 0) {
                            *angularVelocity = 20.0f;
                            if (irand(2) == 0) {
                                *angularVelocity *= -1.0f;
                            }
                        }
                    }
                }
            }
        }
        segment++;
        memoryOffset += 0xC;
    } while (segment < 0x81);

    position[0] = originalX;
    position[0] += velocity[0];
    position[1] += velocity[1];
    position[2] += velocity[2];
    rotation[2] += *angularVelocity;

    if (position[1] < home[1]) {
        position[1] = home[1];
        velocity[1] = 0.0f;
        if ((*(u32*)((s32)unit + 0x104) & 0x20000) == 0 &&
            BtlUnit_CheckStatus(unit, 0x1B) == 0) {
            rotation[2] = 0.0f;
            *angularVelocity = 0.0f;
        }
    }

    *(s32*)((s32)work + 0x450 + index * 4) = BtlUnit_GetUnitId(unit);
    *count = index + 1;
}

USER_FUNC(end_deka) {
    void* work = get_ptr();

    effDelete(*(void**)(*(s32*)(*(s32*)((s32)work + 0x44C) + 0xC) + 0x34));
    effDelete(*(void**)((s32)work + 0x44C));
    return 2;
}

/* stub-fill: disp_2D | prototype_only | source_prototype */
void disp_2D(void) {
    typedef f32 Mtx[3][4];
    typedef union {
        f64 value;
        struct { u32 high, low; } words;
    } IntToDouble;
    extern void* get_ptr(void);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void btlDispTexPlane2(Mtx, s32, u32*);
    extern void btlDispGXInit2DRasta(void);
    extern void btlDispGXQuads2DRasta(f32, f32, f32, f32, u8, u8, u8, u8);
    extern void* camGetPtr(s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetScissor(s32, s32, s32, s32);
    extern void iconDispGxCol(Mtx, u16, u16, u32*);
    extern u8* gp;
    extern u32 dat_804283d8;
    extern u32 dat_804283dc;
    extern u32 dat_804283e0;
    extern f32 float_255_80428430;
    extern f64 double_to_int_mask_80300fe0;
    u8* work = (u8*)get_ptr();
    f32 left = *(f32*)(work + 0x5C) - 0.5f * *(f32*)(work + 0x58) - 8.0f;
    f32 right = *(f32*)(work + 0x5C) + 0.5f * *(f32*)(work + 0x58) + 8.0f;
    u32 color;
    Mtx trans, scale, model;
    IntToDouble alphaConvert, itemConvert;
    s32 i;

    if (*(s32*)(work + 0x50) > 1 && *(s32*)(work + 0x50) < 6) {
        u32 drawColor;
        color = dat_804283d8;
        ((u8*)&color)[3] = *(u8*)(work + 0x68);
        PSMTXTrans(trans, left, *(f32*)(work + 0x60), *(f32*)(work + 0x64));
        drawColor = color;
        btlDispTexPlane2(trans, 0x59, &drawColor);
        PSMTXScale(scale, *(f32*)(work + 0x58) * 0.25f, 1.0f, 1.0f);
        PSMTXTrans(trans, *(f32*)(work + 0x5C), *(f32*)(work + 0x60), *(f32*)(work + 0x64));
        PSMTXConcat(trans, scale, model);
        drawColor = color;
        btlDispTexPlane2(model, 0x5A, &drawColor);
        PSMTXScale(scale, -1.0f, 1.0f, 1.0f);
        PSMTXTrans(trans, right, *(f32*)(work + 0x60), *(f32*)(work + 0x64));
        PSMTXConcat(trans, scale, model);
        drawColor = color;
        btlDispTexPlane2(model, 0x59, &drawColor);
    }
    if (*(s32*)(work + 0x50) > 1 && *(s32*)(work + 0x50) < 6) {
        f32 ratio = *(f32*)(work + 0x3C) / *(f32*)(work + 0x38);
        if (ratio >= 0.0f && ratio <= 1.0f) {
            btlDispGXInit2DRasta();
            GXLoadPosMtxImm((u8*)camGetPtr(8) + 0x11C, 0);
            btlDispGXQuads2DRasta(left, *(f32*)(work + 0x60) + 8.0f,
                left + (right - left) * ratio, *(f32*)(work + 0x60) - 8.0f,
                0xFF, 0, 0, *(u8*)(work + 0x68));
        }
    }
    GXSetScissor(0, 0x78, *(u16*)(gp + 0x170), *(u16*)(gp + 0x172));
    if (*(s32*)(work + 0x50) > 1 && *(s32*)(work + 0x50) < 6) {
        f32 span = right - left;
        for (i = 0; i < 8; i++) {
            u8* timing = work + 0x230 + i * 0x38;
            f32 x = *(f32*)(timing + 0x10) +
                (*(f32*)(timing + 0x0C) / *(f32*)(work + 0x38)) * span + left;
            u8 itemAlpha = *(u8*)(timing + 0x34);
            u32 itemColor = dat_804283dc;
            u32 drawColor;
            alphaConvert.words.high = 0x43300000;
            alphaConvert.words.low = *(u8*)(work + 0x68);
            itemConvert.words.high = 0x43300000;
            itemConvert.words.low = itemAlpha;
            ((u8*)&itemColor)[3] = (s32)((f32)(alphaConvert.value - double_to_int_mask_80300fe0) *
                ((f32)(itemConvert.value - double_to_int_mask_80300fe0) / float_255_80428430));
            if (*(s32*)(timing + 8) == 1) {
                PSMTXScale(scale, 0.75f, 0.75f, 0.75f);
                PSMTXTrans(trans, x, 40.0f + *(f32*)(work + 0x4C) + *(f32*)(work + 0x60) - 18.0f +
                    *(f32*)(timing + 0x14), *(f32*)(work + 0x64) + *(f32*)(timing + 0x18));
                PSMTXConcat(trans, scale, model);
                drawColor = itemColor;
                iconDispGxCol(model, 0x10, 0x9D, &drawColor);
            } else if (*(s32*)(timing + 8) == 2) {
                PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
                PSMTXTrans(trans, x, 40.0f + *(f32*)(work + 0x4C) + *(f32*)(work + 0x60) +
                    *(f32*)(timing + 0x14), *(f32*)(work + 0x64) + *(f32*)(timing + 0x18));
                PSMTXConcat(trans, scale, model);
                drawColor = itemColor;
                btlDispTexPlane2(model, 0x42, &drawColor);
            }
        }
        for (i = 0; i < 8; i++) {
            u8* timing = work + 0x70 + i * 0x38;
            f32 x = *(f32*)(timing + 0x10) +
                (*(f32*)(timing + 0x0C) / *(f32*)(work + 0x38)) * span + left;
            u8 itemAlpha = *(u8*)(timing + 0x34);
            u32 itemColor = dat_804283e0;
            u32 drawColor;
            alphaConvert.words.high = 0x43300000;
            alphaConvert.words.low = *(u8*)(work + 0x68);
            itemConvert.words.high = 0x43300000;
            itemConvert.words.low = itemAlpha;
            ((u8*)&itemColor)[3] = (s32)((f32)(alphaConvert.value - double_to_int_mask_80300fe0) *
                ((f32)(itemConvert.value - double_to_int_mask_80300fe0) / float_255_80428430));
            if (*(s32*)timing > 1 && *(s32*)timing < 6) {
                if (*(s32*)(timing + 4) == 1) {
                    u16 icon = *(f32*)(work + 0x3C) < *(f32*)(timing + 0x0C) - 7.0f ? 0x99 : 0x9D;
                    PSMTXScale(scale, 0.75f, 0.75f, 0.75f);
                    PSMTXTrans(trans, x, *(f32*)(work + 0x4C) + *(f32*)(work + 0x60) - 18.0f +
                        *(f32*)(timing + 0x14), *(f32*)(work + 0x64) + *(f32*)(timing + 0x18));
                    PSMTXConcat(trans, scale, model);
                    drawColor = itemColor;
                    iconDispGxCol(model, 0x10, icon, &drawColor);
                } else {
                    PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
                    PSMTXTrans(trans, x, *(f32*)(work + 0x4C) + *(f32*)(work + 0x60) +
                        *(f32*)(timing + 0x14), *(f32*)(work + 0x64) + *(f32*)(timing + 0x18));
                    PSMTXConcat(trans, scale, model);
                    drawColor = itemColor;
                    btlDispTexPlane2(model, 0x42, &drawColor);
                }
            }
            PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
            PSMTXTrans(trans, x, *(f32*)(work + 0x60) + *(f32*)(timing + 0x14) - 40.0f,
                *(f32*)(work + 0x64) + *(f32*)(timing + 0x18));
            PSMTXConcat(trans, scale, model);
            if (*(u8*)(timing + 0x35) == 1) {
                drawColor = itemColor;
                btlDispTexPlane2(model, 0x54, &drawColor);
            } else if (*(u8*)(timing + 0x35) == 2) {
                drawColor = itemColor;
                btlDispTexPlane2(model, 0x55, &drawColor);
            }
        }
    }
    GXSetScissor(0, 0, *(u16*)(gp + 0x170), *(u16*)(gp + 0x172));
}

void disp_3D(void) {
    ;
}

void disp_3D_alpha(void) {
    ;
}

/* stub-fill: yuka_capture | missing_definition | ghidra_signature */
void yuka_capture(s32 cameraId) {
    typedef f32 Mtx[3][4];
    typedef f32 Mtx44[4][4];
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void* camGetPtr(s32 cameraId);
    extern f64 tan(f64 value);
    extern void C_MTXLookAt(Mtx dst, Vec* pos, Vec* up, Vec* target);
    extern void C_MTXPerspective(Mtx44 dst, f32 fov, f32 aspect,
                                f32 nearZ, f32 farZ);
    extern void GXSetProjection(Mtx44 projection, s32 type);
    extern void GXSetViewport(f32 x, f32 y, f32 width, f32 height,
                              f32 nearZ, f64 farZ);
    extern void GXSetScissor(u32 x, u32 y, u32 width, u32 height);
    extern void test_kururing_mapdisp(s32 cameraId);
    extern void sysWaitDrawSync(void);
    extern void GXSetTexCopySrc(u16 x, u16 y, u16 width, u16 height);
    extern void GXSetTexCopyDst(u16 width, u16 height, u32 format, u8 mipmap);
    extern void GXSetZMode(u8 enable, u32 func, u8 update);
    extern void GXCopyTex(void* destination, u8 clear);
    extern void GXPixModeSync(void);
    extern f32 float_0p5_804283ec;
    extern f32 float_25_8042841c;
    extern f32 float_1_80428420;
    extern f32 float_32767_80428404;
    extern Vec vec3_80300f80[];

    u8 cameraCopy[0x260];
    u8* work = (u8*)get_ptr();
    u8* camera = (u8*)camGetPtr(cameraId);
    Vec position;

    memcpy(cameraCopy, camGetPtr(cameraId), 0x260);
    position.x = (*(f32*)(work + 4) + *(f32*)(work + 0x10)) * float_0p5_804283ec;
    position.y = (*(f32*)(work + 8) + *(f32*)(work + 0x14)) * float_0p5_804283ec;
    position.z = (*(f32*)(work + 0xC) + *(f32*)(work + 0x18)) * float_0p5_804283ec;
    *(Vec*)(camera + 0x24) = vec3_80300f80[0];
    *(Vec*)(camera + 0xC) = position;
    *(Vec*)(camera + 0x18) = *(Vec*)(camera + 0xC);
    *(f32*)(camera + 0x10) += ((f32)*(s32*)(work + 0x20) * float_0p5_804283ec) /
                   (f32)tan(0.21817);
    C_MTXLookAt((f32(*)[4])(camera + 0x11C), (Vec*)(camera + 0xC),
                (Vec*)(camera + 0x24), (Vec*)(camera + 0x18));
    C_MTXPerspective((f32(*)[4])(camera + 0x15C), float_25_8042841c,
                     (f32)*(s32*)(work + 0x1C) / (f32)*(s32*)(work + 0x20),
                     float_1_80428420, float_32767_80428404);
    *(s32*)(camera + 0x19C) = 0;
    GXSetProjection((f32(*)[4])(camera + 0x15C), *(s32*)(camera + 0x19C));
    GXSetViewport(0.0f, 0.0f, (f32)*(s32*)(work + 0x24),
                  (f32)*(s32*)(work + 0x28), 0.0f, 1.0);
    GXSetScissor(0, 0, *(u32*)(work + 0x24), *(u32*)(work + 0x28));
    test_kururing_mapdisp(cameraId);
    sysWaitDrawSync();
    GXSetTexCopySrc(0, 0, *(u16*)(work + 0x26), *(u16*)(work + 0x2A));
    GXSetTexCopyDst(*(u16*)(work + 0x26), *(u16*)(work + 0x2A), 4, 0);
    GXSetZMode(1, 3, 1);
    GXCopyTex(**(void***)work, 1);
    GXPixModeSync();
    memcpy(camGetPtr(cameraId), cameraCopy, 0x260);
}

/* stub-fill: yuka_disp | prototype_only | source_prototype */
void yuka_disp(s32 cameraId) {
    typedef struct LocalCamera {
        u8 pad[0x11C];
        f32 viewMtx[3][4];
    } LocalCamera;
    typedef struct LocalAlloc {
        void* pMemory;
    } LocalAlloc;

    extern LocalCamera* camGetPtr(s32 cameraId);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 type, s32 frac, s32 shift);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 color);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXInitTexObj(void* obj, void* data, s16 width, s16 height, s32 fmt, s32 wrapS, s32 wrapT, s32 mipmap);
    extern void GXLoadTexObj(void* obj, s32 mapId);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);

    void* work = get_ptr();
    LocalCamera* camera = camGetPtr(cameraId);
    LocalAlloc* texture = *(LocalAlloc**)work;
    void* image = texture->pMemory;
    void* capture;
    u8 texObj[0x20];
    volatile f32* fifo;
    s32 posOff;
    s32 texOff;
    s32 i;

    if (image != NULL) {
        capture = (*(LocalAlloc**)((s32)work + 0x2C))->pMemory;
        if (capture != NULL) {
            GXSetCullMode(0);
            GXClearVtxDesc();
            GXSetVtxDesc(9, 1);
            GXSetVtxDesc(0xD, 1);
            GXSetVtxAttrFmt(0, 9, 1, 4, 0);
            GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
            GXSetNumChans(0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
            GXSetNumTevStages(1);
            GXSetTevOrder(0, 0, 0, 0xFF);
            GXSetTevOp(0, 3);
            GXInitTexObj(texObj, image, (u16)*(s32*)((s32)work + 0x24), (u16)*(s32*)((s32)work + 0x28), 4, 0, 0, 0);
            GXLoadTexObj(texObj, 0);
            GXLoadPosMtxImm(camera->viewMtx, 0);
            GXSetCurrentMtx(0);

            fifo = (volatile f32*)0xCC008000;
            posOff = 0;
            texOff = 0;
            for (i = 0; i < 0x80; i++) {
                GXBegin(0x80, 0, 4);

                fifo[0] = *(f32*)((s32)capture + posOff + 0xC18);
                fifo[0] = *(f32*)((s32)capture + posOff + 0xC1C);
                fifo[0] = *(f32*)((s32)capture + posOff + 0xC20);
                fifo[0] = *(f32*)((s32)capture + texOff + 0x3060);
                fifo[0] = *(f32*)((s32)capture + texOff + 0x3468);

                fifo[0] = *(f32*)((s32)capture + posOff + 0x1224);
                fifo[0] = *(f32*)((s32)capture + posOff + 0x1228);
                fifo[0] = *(f32*)((s32)capture + posOff + 0x122C);
                fifo[0] = *(f32*)((s32)capture + texOff + 0x3264);
                fifo[0] = *(f32*)((s32)capture + texOff + 0x366C);

                fifo[0] = *(f32*)((s32)capture + posOff + 0x1230);
                fifo[0] = *(f32*)((s32)capture + posOff + 0x1234);
                fifo[0] = *(f32*)((s32)capture + posOff + 0x1238);
                fifo[0] = *(f32*)((s32)capture + texOff + 0x3268);
                fifo[0] = *(f32*)((s32)capture + texOff + 0x3670);

                fifo[0] = *(f32*)((s32)capture + posOff + 0xC24);
                fifo[0] = *(f32*)((s32)capture + posOff + 0xC28);
                fifo[0] = *(f32*)((s32)capture + posOff + 0xC2C);
                fifo[0] = *(f32*)((s32)capture + texOff + 0x3064);
                fifo[0] = *(f32*)((s32)capture + texOff + 0x346C);

                posOff += 0xC;
                texOff += 4;
            }
        }
    }
}

void* get_ptr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1F4C);
}


/* CHATGPT STUB FILL: main/action/star/sac_deka 20260624_184929 */

/* stub-fill: create_timing | missing_definition | ghidra_signature */
void create_timing(void) {
    extern s32 rand(void);
    extern f32 float_40_80428400;
    extern f32 float_30_80428408;
    extern f32 float_20_8042840c;
    extern f32 float_7_80428410;
    extern f32 float_2_80428414;
    extern f32 float_70_80428418;
    extern f64 double_15p75_80300fc8;

    u8* work = (u8*)get_ptr();
    s32 count;
    s32 retryCount;
    s32 i;
    s32 j;
    f32 sortValues[8];
    s32 sortOrder[9];

restart_all:
    memcpy(work + 0x230, work + 0x70, 0x1C0);

    count = *(s32*)(work + 0x40) + 3;
    retryCount = 0;

    for (i = 0; i < 8; i++) {
        u8* timing = work + 0x70 + i * 0x38;
        *(u8*)(timing + 0x35) = 0;

        if (i < count) {
            f32 width = *(f32*)(work + 0x38);
            f32 value;
            f32 maxValue;

            *(s32*)(timing + 0x08) = 1;
            value = ((width - float_40_80428400) * (f32)rand()) / float_32767_80428404;

            if (value <= float_30_80428408) {
                value = float_30_80428408;
            }

            maxValue = width - float_20_8042840c;
            if (maxValue <= value) {
                value = maxValue;
            }

            *(f32*)(timing + 0x0C) = value;
            *(s32*)(timing + 0x00) = 2;
        } else {
            *(s32*)(timing + 0x00) = 0;
        }
    }

rescan_timings:
    for (i = 0; i < count; i++) {
        u8* current = work + 0x70 + i * 0x38;

        for (j = 0; j < count; j++) {
            u8* other;
            f64 diff;
            f32 value;
            f32 width;
            f32 maxValue;

            if (i == j) {
                continue;
            }

            other = work + 0x70 + j * 0x38;
            if (*(s32*)(other + 0x00) != 2) {
                continue;
            }

            diff = (f64)(*(f32*)(other + 0x0C) - *(f32*)(current + 0x0C));
            if (diff < 0.0) {
                diff = -diff;
            }

            if (diff > double_15p75_80300fc8) {
                continue;
            }

            width = *(f32*)(work + 0x38);
            value = (width * (f32)rand()) / float_32767_80428404;

            if (value <= float_30_80428408) {
                value = float_30_80428408;
            }

            maxValue = width - float_20_8042840c;
            if (maxValue <= value) {
                value = maxValue;
            }

            retryCount++;
            *(f32*)(current + 0x0C) = value;

            if (retryCount != 100) {
                goto rescan_timings;
            }

            *(s32*)(current + 0x00) = 0;
            *(u8*)(current + 0x35) = 0;
            goto restart_all;
        }
    }

    for (i = 0; i < count; i++) {
        u8* timing = work + 0x70 + i * 0x38;
        sortOrder[i + 1] = i;
        sortValues[i] = *(f32*)(timing + 0x0C);
    }

    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (sortValues[j] < sortValues[i]) {
                f32 tempValue = sortValues[i];
                s32 tempOrder = sortOrder[i + 1];

                sortValues[i] = sortValues[j];
                sortOrder[i + 1] = sortOrder[j + 1];
                sortValues[j] = tempValue;
                sortOrder[j + 1] = tempOrder;
            }
        }
    }

    *(f32*)(work + 0x70 + sortOrder[count] * 0x38 + 0x0C) =
        (*(f32*)(work + 0x38) - float_7_80428410) - float_2_80428414;

    if (float_70_80428418 < *(f32*)(work + 0x70 + sortOrder[1] * 0x38 + 0x0C)) {
        *(f32*)(work + 0x70 + sortOrder[1] * 0x38 + 0x0C) = float_70_80428418;
    }
}

s32 weaponGetPower_Deka(void) {
    return *(s32*)((s32)get_ptr() + 0x44) + 1;
}

USER_FUNC(wait_star_stone_take_on) {
    s32 value = *(s32*)((s32)get_ptr() + 0x3F0);
    return value == 4 ? 2 : 0;
}

USER_FUNC(start_game) {
    void* wp;

    wp = get_ptr();
    *(s32*)((s32)wp + 0x30) = 1;

    return 2;
}

USER_FUNC(wait_game_end) {
    s32 value = *(s32*)((s32)get_ptr() + 0x30);
    return value == 5 ? 2 : 0;
}

USER_FUNC(star_stone_appear) {
    void* wp;

    wp = get_ptr();
    *(s32*)((s32)wp + 0x3F0) = 1;
    return 2;
}

USER_FUNC(get_ride_pos) {
    extern void evtSetFloat(EventEntry* event, s32 target, f32 value);
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

USER_FUNC(get_result) {
    void* work = get_ptr();
    evtSetValue(event, event->args[0], *(s32*)((s32)work + 0x44));
    return 2;
}
