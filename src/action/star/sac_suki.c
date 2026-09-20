#include "action/star/sac_suki.h"
#include "battle/battle_audience.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "driver/camdrv.h"
#include "event/evt_cmd.h"
#include "system.h"
#include "effect/n64/eff_crystal_n64.h"

static void* get_ptr(void);
void main_button(s32 index);
static void disp_2D(void);
static void disp_3D(void);
static void disp_3D_alpha(void);

extern void* _battleWorkPointer;
void* mapGetMapObj(s32 name);
void effDelete(void* effect);
void* memcpy(void* dest, const void* src, u32 size);
char* strcpy(char* dest, const char* src);
void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
void GXSetTevColor(s32 reg, GXColor* color);
void GXSetScissor(s32 x, s32 y, s32 w, s32 h);
void PSMTXTransApply(Mtx src, Mtx dst, f32 x, f32 y, f32 z);
void iconDispGx2(Mtx mtx, s32 flags, s32 iconId);
void iconDispGx(Vec* pos, s32 flags, s32 iconId, f32 scale);
void btlGetScreenPoint(Vec* pos, Vec* out);
void N__disp_2D(s32 type, s32 index);
static void main_base(void);
static void main_star(void);
void dispEntry(s32 cameraId, s32 order, void* callback, f32 priority, void* param);
static u32 weapon[48];
static u32 nice_event[8];
extern u8 N_ohko_rate_tbl[6];
extern const u32 dat_80301384[];
extern const Vec vec3_80301394;
extern const Vec vec3_80301418;
extern const Vec vec3_80301424;
extern GXColor dat_80428638;
extern char str_btl_wn_sac_sukkari_s_803012d0[];
extern f32 float_0_80428680;
extern f32 float_1_80428670;
extern f32 float_10_80428678;
extern f32 float_20_80428674;
extern f32 float_26_80428688;
extern f32 float_30_8042866c;
extern f32 float_60_80428684;
extern f32 float_901_804286cc;
extern f32 float_215_8042868c;
extern void* gp;

USER_FUNC(main_suki) {
    void* work;
    s32* args;
    s32 i;
    char* base;
    u32 init[18];

    base = str_btl_wn_sac_sukkari_s_803012d0;
    work = get_ptr();
    args = event->args;
    for (i = 0; i < 18; i++) {
        init[i] = *(u32*)(base + 0x3C + i * 4);
    }
    if (isFirstCall != 0) {
        memset(work, 0, 0x4C8);
        *(s32*)((s32)work + 8) = evtGetValue(event, args[0]);
        memcpy((void*)((s32)work + 0x28), init, 0x48);
        strcpy((char*)((s32)work + 0x70), base + 0x260);
        strcpy((char*)((s32)work + 0x80), base + 0x270);
        strcpy((char*)((s32)work + 0x90), base + 0x280);
        strcpy((char*)((s32)work + 0xA0), base + 0x290);
        strcpy((char*)((s32)work + 0xB0), base + 0x2A0);
        strcpy((char*)((s32)work + 0xC0), base + 0x2B0);
        *(u32*)((s32)work + 0xD0) = *(u32*)(base + 0x84);
        *(u32*)((s32)work + 0xD4) = *(u32*)(base + 0x88);
        *(u32*)((s32)work + 0xD8) = *(u32*)(base + 0x8C);
    }
    main_base();
    for (i = 0; i < 7; i++) {
        main_button(i);
    }
    main_star();
    dispEntry(8, 1, disp_2D, float_901_804286cc, NULL);
    dispEntry(4, 1, disp_3D, float_0_80428680, NULL);
    dispEntry(4, 2, disp_3D_alpha, float_0_80428680, NULL);
    return 0;
}

/* stub-fill: main_base | prototype_only | source_prototype */
static void main_base(void) {
    extern s32 BattleAudience_GetAudienceNum(void);
    extern f64 intplGetValue(f64 start, f64 end, s32 type, s32 current, s32 max);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern u32 psndSFXOn(const char* name);
    extern u16 keyGetButtonTrg(s32 controller);
    extern void* evtEntry(const void* code, s32 priority, u32 flags);
    extern void BattleAudienceSoundCallKind(s32 kind);
    extern void BattleAudienceSoundWhistleKind(s32 kind);
    extern const char str_SFX_BTL_SAC_SUKKIRI1_80301518[];
    extern const char str_SFX_BTL_SAC_SUKKIRI2_80301500[];
    extern u32 DAT_80301364;
    extern u32 DAT_80301368;
    extern u32 vec3_8030136c[];
    extern u32 vec3_80301378[];
    u8* work = get_ptr();
    s32 audience = BattleAudience_GetAudienceNum();
    s32 i;

    switch (*(s32*)work) {
        case 1:
            *(s32*)work = 2;
            *(s32*)(work + 4) = 0;
        case 2:
            if (*(s32*)(work + 0xC) == 0) {
                *(f32*)(work + 0xD0) =
                    (f32)intplGetValue(-1000.0, 0.0, 4, *(s32*)(work + 4), 0xF);
                *(u32*)(work + 0xD4) = DAT_80301364;
                *(u32*)(work + 0xD8) = DAT_80301368;
            }
            if (*(s32*)(work + 4) == 0) {
                if (*(s32*)(work + 0x22C) != 4) {
                    return;
                }
                memcpy(work + 0x184, work + 0xDC, 0xA8);
                *(u32*)(work + 0x14) = *(u32*)(work + 0x10);
                if (*(s32*)(work + 0xC) < 5) {
                    *(s32*)(work + 0x10) = 0;
                    *(s32*)(work + 0x18) = *(s32*)(work + 0xC) + 3;
                    *(s32*)(work + 0x1C) = *(s32*)(work + 0xC) * 10 + 0xB4;
                    for (i = 0; i < 7; i++) {
                        *(s32*)(work + 0xDC + i * 0x18) =
                            i < *(s32*)(work + 0x18) ? 1 : 0;
                    }
                } else {
                    for (i = 0; i < 7; i++) {
                        *(s32*)(work + 0xDC + i * 0x18) = 0;
                    }
                }
            }
            *(f32*)(work + 0x24) =
                (f32)intplGetValue(-80.0, 0.0, 0, *(s32*)(work + 4), 0xF);
            *(s32*)(work + 4) += 1;
            if (*(s32*)(work + 4) > 0xE) {
                if (*(s32*)(work + 0xC) < 5) {
                    *(s32*)work = 4;
                } else {
                    *(s32*)work = 5;
                    *(s32*)(work + 4) = 0;
                    *(s32*)(work + 0x22C) = 8;
                }
                *(u32*)(work + 0xD0) = vec3_8030136c[0];
                *(u32*)(work + 0xD4) = vec3_8030136c[1];
                *(u32*)(work + 0xD8) = vec3_8030136c[2];
            }
            break;

        case 3:
            if (*(s32*)(work + 4) == 0) {
                if (*(s32*)(work + 0x22C) != 4) {
                    return;
                }
                psndSFXOn(str_SFX_BTL_SAC_SUKKIRI2_80301500);
                memcpy(work + 0x184, work + 0xDC, 0xA8);
                *(u32*)(work + 0x14) = *(u32*)(work + 0x10);
                for (i = 0; i < 7; i++) {
                    *(s32*)(work + 0xDC + i * 0x18) = 0;
                }
            }
            *(f32*)(work + 0x24) =
                (f32)intplGetValue(-60.0, 0.0, 0, *(s32*)(work + 4), 0xF);
            *(s32*)(work + 4) += 1;
            if (*(s32*)(work + 4) > 0xE) {
                *(s32*)work = 5;
                *(s32*)(work + 4) = 0;
                *(s32*)(work + 0x22C) = 8;
            }
            break;

        case 4:
            *(s32*)(work + 0x1C) -= 1;
            if (*(s32*)(work + 0x1C) < 1) {
                *(s32*)work = 3;
                *(s32*)(work + 4) = 0;
            } else {
                u16 buttons = keyGetButtonTrg(0);
                if ((buttons & 0xF00) != 0) {
                    buttons = keyGetButtonTrg(0);
                    if ((*(u32*)(work + 0xE4 + *(s32*)(work + 0x10) * 0x18) & buttons) == 0) {
                        *(s32*)work = 3;
                        *(s32*)(work + 4) = 0;
                    } else {
                        *(s32*)(work + 0x10) += 1;
                        psndSFXOn(str_SFX_BTL_SAC_SUKKIRI1_80301518);
                        if (*(s32*)(work + 0x10) >= *(s32*)(work + 0x18)) {
                            void* event;
                            s32 value;
                            *(s32*)work = 2;
                            *(s32*)(work + 4) = 0;
                            *(s32*)(work + 0xC) += 1;
                            *(s32*)(work + 0x22C) = 5;
                            event = evtEntry(nice_event, 0, 0x20);
                            value = *(s32*)(work + 0xC) + 1;
                            if (value >= 6) {
                                value = 6;
                            }
                            *(s32*)((s32)event + 0x9C) = value;
                            if (audience > 0 && audience < 0x32) {
                                BattleAudienceSoundCallKind(1);
                            }
                            if (audience > 0x31 && audience < 100) {
                                BattleAudienceSoundCallKind(1);
                                BattleAudienceSoundWhistleKind(1);
                            }
                            if (audience > 99 && audience < 0x96) {
                                BattleAudienceSoundCallKind(2);
                                BattleAudienceSoundWhistleKind(2);
                            }
                            if (audience > 0x95) {
                                BattleAudienceSoundCallKind(2);
                                BattleAudienceSoundWhistleKind(3);
                            }
                        }
                    }
                }
            }
            break;

        case 5:
            *(s32*)(work + 4) += 1;
            if (*(s32*)(work + 4) < 0x1F) {
                *(f32*)(work + 0xD0) =
                    (f32)intplGetValue(0.0, -1000.0, 4, *(s32*)(work + 4), 0x1E);
                *(u32*)(work + 0xD4) = vec3_80301378[1];
                *(u32*)(work + 0xD8) = vec3_80301378[2];
            }
            break;
    }
}

void main_button(s32 index) {
    void* work;
    void* button;
    s32 randoms[4];
    Vec pos;

    work = get_ptr();
    button = (void*)((s32)work + index * 0x18 + 0xDC);
    randoms[0] = dat_80301384[0];
    randoms[1] = dat_80301384[1];
    randoms[2] = dat_80301384[2];
    randoms[3] = dat_80301384[3];
    switch (*(s32*)button) {
        case 1:
            *(s32*)button = 2;
            pos = vec3_80301394;
            pos.x = (float_60_80428684 * index) - (float_30_8042866c * (*(s32*)((s32)work + 0x18) - 1));
            *(u32*)((s32)button + 0xC) = *(u32*)&pos.x;
            *(u32*)((s32)button + 0x10) = *(u32*)&pos.y;
            *(u32*)((s32)button + 0x14) = *(u32*)&pos.z;
            *(s32*)((s32)button + 8) = randoms[irand(4)];
            break;
    }
}

/* stub-fill: main_star | prototype_only | source_prototype */
static void main_star(void) {
    extern void* BattleGetMarioPtr(void* battleWork);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern void* effStarStoneEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
    extern void* effStardustN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale);
    extern f64 intplGetValue(f64 start, f64 end, s32 type, s32 current, s32 max);
    extern u32 psndSFXOn(const char* name);
    extern const char str_SFX_BTL_SAC_SUKKIRI3_803014e0[];
    extern u32 vec3_803013a0[];
    extern u32 vec3_803013ac[];
    extern u32 vec3_803013b8[];
    extern u32 vec3_803013c4[];
    extern u32 vec3_803013d0[];
    extern u32 vec3_803013dc[];
    extern u32 vec3_803013e8[];
    extern u32 vec3_803013f4[];
    extern u32 vec3_80301400[];
    extern u32 vec3_8030140c[];

    u8* work = get_ptr();
    void* mario = BattleGetMarioPtr(_battleWorkPointer);

    switch (*(s32*)(work + 0x22C)) {
        case 1:
            *(s32*)(work + 0x22C) = 2;
            *(s32*)(work + 0x230) = 0;
            *(void**)(work + 0x27C) = effStarStoneEntry(5, 0.0f, -1000.0f, 0.0f, 1.0f);
            BtlUnit_GetPos(mario,
                           (f32*)(work + 0x234),
                           (f32*)(work + 0x238),
                           (f32*)(work + 0x23C));
            *(f32*)(work + 0x238) +=
                *(f32*)((s32)mario + 0x114) * (f32)*(s16*)((s32)mario + 0xCE) + 37.0f;
            *(u32*)(work + 0x240) = *(u32*)(work + 0x234);
            *(u32*)(work + 0x244) = *(u32*)(work + 0x238);
            *(u32*)(work + 0x248) = *(u32*)(work + 0x23C);
            *(u32*)(work + 0x24C) = *(u32*)(work + 0x234);
            *(u32*)(work + 0x250) = *(u32*)(work + 0x238);
            *(u32*)(work + 0x254) = *(u32*)(work + 0x23C);
            *(f32*)(work + 0x250) += 50.0f;
            *(f32*)(work + 0x254) -= 1.0f;
            *(u32*)(work + 0x264) = vec3_803013a0[0];
            *(u32*)(work + 0x268) = vec3_803013a0[1];
            *(u32*)(work + 0x26C) = vec3_803013a0[2];
            *(u32*)(work + 0x270) = vec3_803013ac[0];
            *(u32*)(work + 0x274) = vec3_803013ac[1];
            *(u32*)(work + 0x278) = vec3_803013ac[2];
            *(s32*)(work + 0x280) = 4;
            /* fallthrough */
        case 2:
            *(s32*)(work + 0x230) += 1;
            if (*(s32*)(work + 0x230) < 0x65) {
                f32 scale = (f32)intplGetValue(0.0, 2.0, 0,
                                               *(s32*)(work + 0x230), 100);
                *(f32*)(work + 0x234) =
                    (f32)intplGetValue(*(f32*)(work + 0x240), *(f32*)(work + 0x24C),
                                       0, *(s32*)(work + 0x230), 100);
                *(f32*)(work + 0x238) =
                    (f32)intplGetValue(*(f32*)(work + 0x244), *(f32*)(work + 0x250),
                                       0, *(s32*)(work + 0x230), 100);
                *(f32*)(work + 0x23C) =
                    (f32)intplGetValue(*(f32*)(work + 0x248), *(f32*)(work + 0x254),
                                       0, *(s32*)(work + 0x230), 100);
                *(f32*)(work + 0x264) = scale;
                *(f32*)(work + 0x268) = scale;
                *(f32*)(work + 0x26C) = scale;
            } else {
                *(u32*)(work + 0x234) = *(u32*)(work + 0x24C);
                *(u32*)(work + 0x238) = *(u32*)(work + 0x250);
                *(u32*)(work + 0x23C) = *(u32*)(work + 0x254);
                *(f32*)(work + 0x264) = 2.0f;
                *(f32*)(work + 0x268) = 2.0f;
                *(f32*)(work + 0x26C) = 2.0f;
            }
            *(f32*)(work + 0x274) =
                (f32)intplGetValue(0.0, 2160.0, 4,
                                   *(s32*)(work + 0x230), 0x78);
            if (*(s32*)(work + 0x230) > 0x77) {
                *(s32*)(work + 0x22C) = 3;
                *(s32*)(work + 0x230) = 0;
                *(u32*)(work + 0x240) = *(u32*)(work + 0x234);
                *(u32*)(work + 0x244) = *(u32*)(work + 0x238);
                *(u32*)(work + 0x248) = *(u32*)(work + 0x23C);
                *(f32*)(work + 0x250) = 300.0f;
            }
            break;
        case 3:
            *(s32*)(work + 0x230) += 1;
            *(f32*)(work + 0x238) =
                (f32)intplGetValue(*(f32*)(work + 0x244), *(f32*)(work + 0x250),
                                   1, *(s32*)(work + 0x230), 0x3C);
            if (*(s32*)(work + 0x230) > 0x3B) {
                *(s32*)(work + 0x22C) = 4;
                *(s32*)(work + 0x230) = 0;
                *(u32*)(work + 0x234) = vec3_803013b8[0];
                *(u32*)(work + 0x238) = vec3_803013b8[1];
                *(u32*)(work + 0x23C) = vec3_803013b8[2];
                *(u32*)(work + 0x264) = vec3_803013c4[0];
                *(u32*)(work + 0x268) = vec3_803013c4[1];
                *(u32*)(work + 0x26C) = vec3_803013c4[2];
                *(u32*)(work + 0x270) = vec3_803013d0[0];
                *(u32*)(work + 0x274) = vec3_803013d0[1];
                *(u32*)(work + 0x278) = vec3_803013d0[2];
            }
            break;
        case 5:
            *(s32*)(work + 0x22C) = 6;
            *(s32*)(work + 0x230) = 0;
            BtlUnit_GetPos(mario,
                           (f32*)(work + 0x234),
                           (f32*)(work + 0x238),
                           (f32*)(work + 0x23C));
            *(f32*)(work + 0x238) += 100.0f;
            *(u32*)(work + 0x264) = vec3_803013dc[0];
            *(u32*)(work + 0x268) = vec3_803013dc[1];
            *(u32*)(work + 0x26C) = vec3_803013dc[2];
            *(u32*)(work + 0x270) = vec3_803013e8[0];
            *(u32*)(work + 0x274) = vec3_803013e8[1];
            *(u32*)(work + 0x278) = vec3_803013e8[2];
            *(u32*)(work + 0x244) = *(u32*)(work + 0x238);
            *(f32*)(work + 0x250) = 300.0f;
            effStardustN64Entry(0,
                                *(f32*)(work + 0x234),
                                *(f32*)(work + 0x238),
                                *(f32*)(work + 0x23C), 32.0f);
            break;
        case 6:
            *(s32*)(work + 0x230) += 1;
            *(f32*)(work + 0x274) =
                (f32)intplGetValue(0.0, 1440.0, 0,
                                   *(s32*)(work + 0x230), 0x28);
            if (*(s32*)(work + 0x230) > 0x27) {
                f32 px = *(f32*)(work + 0x234);
                f32 py = *(f32*)(work + 0x238);
                f32 pz = *(f32*)(work + 0x23C);

                *(s32*)(work + 0x22C) = 7;
                *(s32*)(work + 0x230) = 0;
                effStardustN64Entry(4, px, py - 5.0f, pz, 40.0f);
                effStardustN64Entry(4, px + 30.0f, py, pz, 40.0f);
                effStardustN64Entry(4, px - 30.0f, py, pz, 40.0f);
                psndSFXOn(str_SFX_BTL_SAC_SUKKIRI3_803014e0);
            }
            break;
        case 7:
            *(s32*)(work + 0x230) += 1;
            *(f32*)(work + 0x238) =
                (f32)intplGetValue(*(f32*)(work + 0x244), *(f32*)(work + 0x250),
                                   4, *(s32*)(work + 0x230), 0x1E);
            if (*(s32*)(work + 0x230) > 0x1D) {
                *(s32*)(work + 0x22C) = 4;
                *(s32*)(work + 0x230) = 0;
                *(u32*)(work + 0x234) = vec3_803013f4[0];
                *(u32*)(work + 0x238) = vec3_803013f4[1];
                *(u32*)(work + 0x23C) = vec3_803013f4[2];
                *(u32*)(work + 0x264) = vec3_80301400[0];
                *(u32*)(work + 0x268) = vec3_80301400[1];
                *(u32*)(work + 0x26C) = vec3_80301400[2];
                *(u32*)(work + 0x270) = vec3_8030140c[0];
                *(u32*)(work + 0x274) = vec3_8030140c[1];
                *(u32*)(work + 0x278) = vec3_8030140c[2];
            }
            break;
        case 8:
            *(s32*)(work + 0x280) = 4;
            break;
        case 9:
            *(s32*)(work + 0x230) += 1;
            *(f32*)(work + 0x234) =
                (f32)intplGetValue(*(f32*)(work + 0x240), *(f32*)(work + 0x24C),
                                   0, *(s32*)(work + 0x230), 0x14);
            *(f32*)(work + 0x238) =
                (f32)intplGetValue(*(f32*)(work + 0x244), *(f32*)(work + 0x250),
                                   1, *(s32*)(work + 0x230), 0x14);
            *(f32*)(work + 0x23C) =
                (f32)intplGetValue(*(f32*)(work + 0x248), *(f32*)(work + 0x254),
                                   0, *(s32*)(work + 0x230), 0x14);
            *(s32*)(work + 0x280) = 7;
            if (*(s32*)(work + 0x230) > 0x13) {
                *(s32*)(work + 0x22C) = 8;
                *(s32*)(work + 0x230) = 0;
            }
            break;
    }

    if (*(void**)(work + 0x27C) != NULL) {
        u8* effectWork = *(u8**)((s32)*(void**)(work + 0x27C) + 0xC);
        *(u32*)(effectWork + 8) = *(u32*)(work + 0x234);
        *(u32*)(effectWork + 0xC) = *(u32*)(work + 0x238);
        *(u32*)(effectWork + 0x10) = *(u32*)(work + 0x23C);
        *(f32*)(effectWork + 0x14) = 1.5f * *(f32*)(work + 0x264);
        *(u32*)(effectWork + 0x18) = *(u32*)(work + 0x270);
        *(u32*)(effectWork + 0x1C) = *(u32*)(work + 0x274);
        *(u32*)(effectWork + 0x20) = *(u32*)(work + 0x278);
        *(u32*)(effectWork + 0x38) = *(u32*)(work + 0x280);
    }
}

USER_FUNC(end_suki) {
    void* wp;

    wp = get_ptr();
    effDelete(*(void**)(*(s32*)(*(s32*)((s32)wp + 0x27C) + 0xC) + 0x34));
    effDelete(*(void**)((s32)wp + 0x27C));
    return 2;
}

static void disp_2D(void) {
    Mtx mtx;
    Vec pos_a;
    Vec pos_b;
    void* work;
    s32 i;

    work = get_ptr();
    PSMTXScale(mtx, float_26_80428688, float_1_80428670, float_1_80428670);
    PSMTXTransApply(mtx, mtx,
                    *(f32*)((s32)work + 0xD0),
                    *(f32*)((s32)work + 0xD4) + float_60_80428684,
                    *(f32*)((s32)work + 0xD8));
    iconDispGx2(mtx, 0x10, 0x96);

    pos_a = vec3_80301418;
    pos_a.x = *(f32*)((s32)work + 0xD0) + float_215_8042868c;
    pos_a.y = *(f32*)((s32)work + 0xD4) + float_60_80428684;
    pos_a.z = *(f32*)((s32)work + 0xD8);
    iconDispGx(&pos_a, 0x10, 0x98, float_1_80428670);

    pos_b = vec3_80301424;
    pos_b.x = *(f32*)((s32)work + 0xD0) - float_215_8042868c;
    pos_b.y = *(f32*)((s32)work + 0xD4) + float_60_80428684;
    pos_b.z = *(f32*)((s32)work + 0xD8);
    iconDispGx(&pos_b, 0x10, 0x97, float_1_80428670);

    GXSetScissor(0, 0x91, *(u16*)((s32)gp + 0x170), 0x50);
    for (i = 0; i < 7; i++) {
        N__disp_2D(0, i);
        N__disp_2D(1, i);
    }
    GXSetScissor(0, 0, *(u16*)((s32)gp + 0x170), *(u16*)((s32)gp + 0x172));
}


/* CHATGPT STUB FILL: main/action/star/sac_suki 20260624_184929 */

/* stub-fill: N__disp_2D | prototype_only | source_prototype */
void N__disp_2D(s32 type, s32 index) {
    extern void iconDispGx(Vec* pos, s32 flags, s32 iconId, f32 scale);
    extern u32 vec3_80301430[];
    extern f32 float_80_8042867c;

    void* work;
    void* entry;
    Vec pos;
    s32 selected;
    f32 yOffset;
    u16 icon;

    work = get_ptr();
    if (type == 0) {
        entry = (void*)((s32)work + index * 0x18 + 0x184);
        selected = *(s32*)((s32)work + 0x14);
        yOffset = float_80_8042867c;
    } else {
        entry = (void*)((s32)work + index * 0x18 + 0xDC);
        selected = *(s32*)((s32)work + 0x10);
        yOffset = float_0_80428680;
    }

    if (type == 0) {
        if (*(s32*)entry == 0) {
            return;
        }
    } else if (type == 1 && *(s32*)entry != 2) {
        return;
    }

    switch (*(s32*)((s32)entry + 8)) {
        case 0x100:
            if (index < selected) icon = 0x6D;
            else icon = 0x6C;
            break;
        case 0x200:
            if (index < selected) icon = 0x6F;
            else icon = 0x6E;
            break;
        case 0x400:
            if (index < selected) icon = 0x71;
            else icon = 0x70;
            break;
        case 0x800:
            if (index < selected) icon = 0x73;
            else icon = 0x72;
            break;
        case 0x40:
            if (index < selected) icon = 0x87;
            else icon = 0x86;
            break;
        case 0x20:
            if (index < selected) icon = 0x89;
            else icon = 0x88;
            break;
        case 0x10:
            if (index < selected) icon = 0x8B;
            else icon = 0x8A;
            break;
    }

    if (icon != 0) {
        pos = *(Vec*)vec3_80301430;
        pos.x = *(f32*)((s32)entry + 0xC);
        pos.y = *(f32*)((s32)entry + 0x10) + yOffset +
                *(f32*)((s32)work + 0x24) + float_60_80428684;
        pos.z = *(f32*)((s32)entry + 0x14);
        iconDispGx(&pos, 0x10, icon, float_1_80428670);
    }
}

static void disp_3D(void) {
    ;
}

static void disp_3D_alpha(void) {
    ;
}

static void* get_ptr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1F4C);
}

static s32 start_game(EventEntry* event, BOOL isFirstCall) {
    void* wp;

    wp = get_ptr();
    *(s32*)wp = 1;
    return 2;
}

static s32 wait_game_end(EventEntry* event, BOOL isFirstCall) {
    void* wp;
    s32* args;

    wp = get_ptr();
    args = event->args;
    if (*(s32*)wp == 5) {
        evtSetValue(event, args[0], *(s32*)((s32)wp + 0xC) + 1);
        return 2;
    }
    return 0;
}

static s32 star_stone_appear(EventEntry* event, BOOL isFirstCall) {
    void* wp;

    wp = get_ptr();
    *(s32*)((s32)wp + 0x22C) = 1;
    return 2;
}

USER_FUNC(wait_star_stone_up_end) {
    s32 value = *(s32*)((s32)get_ptr() + 0x22C);
    return value == 4 ? 2 : 0;
}

USER_FUNC(star_stone_get_attack_point) {
    void* work;
    s32* args;
    s32 offset;

    work = get_ptr();
    args = event->args;
    offset = evtGetValue(event, args[0]) * 0xC;
    evtSetFloat(event, args[1], *(f32*)((s32)work + offset + 0x28));
    evtSetFloat(event, args[2], *(f32*)((s32)work + offset + 0x2C));
    evtSetFloat(event, args[3], *(f32*)((s32)work + offset + 0x30));
    return 2;
}

static s32 star_stone_attack(EventEntry* event, BOOL isFirstCall) {
    void* work;
    void* cam;
    s32 index;
    s32 offset;

    work = get_ptr();
    index = evtGetValue(event, event->args[0]);
    *(s32*)((s32)work + 0x22C) = 9;
    *(s32*)((s32)work + 0x230) = 0;
    offset = index * 0xC;
    *(u32*)((s32)work + 0x240) = *(u32*)((s32)work + offset + 0x28);
    *(u32*)((s32)work + 0x244) = *(u32*)((s32)work + offset + 0x2C);
    *(u32*)((s32)work + 0x248) = *(u32*)((s32)work + offset + 0x30);
    *(u32*)((s32)work + 0x234) = *(u32*)((s32)work + 0x240);
    *(u32*)((s32)work + 0x238) = *(u32*)((s32)work + 0x244);
    *(u32*)((s32)work + 0x23C) = *(u32*)((s32)work + 0x248);
    cam = camGetPtr(4);
    *(u32*)((s32)work + 0x24C) = *(u32*)((s32)cam + 0xC);
    *(u32*)((s32)work + 0x250) = *(u32*)((s32)cam + 0x10);
    *(u32*)((s32)work + 0x254) = *(u32*)((s32)cam + 0x14);
    *(f32*)((s32)work + 0x250) += float_20_80428674;
    *(f32*)((s32)work + 0x254) += float_10_80428678;
    return 2;
}

USER_FUNC(wait_star_stone_attack_end) {
    s32 value = *(s32*)((s32)get_ptr() + 0x22C);
    return value == 8 ? 2 : 0;
}

static s32 set_weapon(EventEntry* event, BOOL isFirstCall) {
    s32 type;
    s32* args;
    void* work;
    u8 rates[6];

    args = event->args;
    type = evtGetValue(event, args[0]);
    work = get_ptr();
    *(u32*)&rates[0] = *(u32*)&N_ohko_rate_tbl[0];
    *(u16*)&rates[4] = *(u16*)&N_ohko_rate_tbl[4];
    memcpy((void*)((s32)work + 0x284), weapon, 0xC0);
    *(u8*)((s32)work + 0x322) = rates[type - 1];
    evtSetValue(event, args[1], (s32)work + 0x284);
    return 2;
}

USER_FUNC(aud_set_draw) {
    s32* args = event->args;
    void* base = BattleAudienceBaseGetPtr();
    s32 value = evtGetValue(event, args[0]);
    if (value == 0) {
        *(u32*)base |= 0x20000;
    } else {
        *(u32*)base &= ~0x20000;
    }
    return 2;
}

USER_FUNC(flash_update) {
    u8 value = evtGetValue(event, event->args[0]);
    *(u8*)((s32)get_ptr() + 0x20) = value;
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

USER_FUNC(get_screen_point) {
    Vec pos;
    s32* args;

    args = event->args;
    pos.x = evtGetFloat(event, args[0]);
    pos.y = evtGetFloat(event, args[1]);
    pos.z = evtGetFloat(event, args[2]);
    btlGetScreenPoint(&pos, &pos);
    evtSetFloat(event, args[3], pos.x);
    evtSetFloat(event, args[4], pos.y);
    return 2;
}

USER_FUNC(eff_crystal) {

    s32* args;
    f32 x;
    f32 y;
    void* effect;

    args = event->args;
    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    effect = effCrystalN64Entry(6, 0x3C, x, y, float_30_8042866c, float_1_80428670);
    *(s32*)(*(s32*)((s32)effect + 0xC) + 0x38) = 7;
    return 2;
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

void set_tev(void* param) {
    GXColor out;
    GXColor color;
    u8 alpha;

    alpha = *(u8*)((s32)get_ptr() + 0x20);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 2);
    GXSetTevAlphaIn(0, 7, 7, 7, 1);
    color = dat_80428638;
    color.r = alpha;
    color.g = alpha;
    color.b = alpha;
    out = color;
    GXSetTevColor(1, &out);
    *(s32*)((s32)param + 0xC) = 1;
    *(s32*)((s32)param + 0x14) = 0;
    *(s32*)((s32)param + 0x10) = 0;
}

const u32 dat_80301384[] = { 0x100, 0x200, 0x400, 0x800 };

/* Exact US Showstopper weapon/Event reconstruction dependencies. */
extern char str_A1_80428654[];
extern char str_A2_80428658[];
extern char str_A_1_80428644[];
extern char str_B_8042865c[];
extern char str_C_80428660[];
extern char str_M_I_Y_8042863c[];
extern char str_OFF_d_yabu_small_80301478[];
extern char str_SFX_BTL_SAC_START1_8030143c[];
extern char str_SFX_BTL_SAC_START2_80301450[];
extern char str_SFX_BTL_SAC_START3_80301464[];
extern char str_SFX_BTL_SAC_SUKKIRI4_80301498[];
extern char str_SFX_BTL_SAC_SUKKIRI4_803014b0[];
extern char str_SFX_BTL_SAC_SUKKIRI5_803014c8[];
extern char str_bti_S_8042864c[];
extern char str_gamen_80428664[];
extern char str_indirect_8030148c[];
extern char str_msg_sac_7_80301300[];
extern char str_msg_sukkari_sukkiri_803012ec[];
extern char zero_80428648[];
extern u32 sac_ac_help_off[];
extern u32 sac_ac_help_on[];
extern u32 sac_cheer[];
extern u32 sac_cheer_end[];
extern u32 sac_mario_back_home[];
extern u32 sac_mario_item_on_end_evt[];
extern u32 sac_mario_item_on_evt[];
extern u32 sac_start_camera_evt[];
extern u32 sac_start_stardust[];
extern s32 btlevtcmd_ACSuccessEffect();
extern s32 btlevtcmd_AnimeChangePose();
extern s32 btlevtcmd_AnimeChangePoseType();
extern s32 btlevtcmd_AttackDeclare();
extern s32 btlevtcmd_CheckDamage();
extern s32 btlevtcmd_CheckPartsAttribute();
extern s32 btlevtcmd_CheckStatus();
extern s32 btlevtcmd_ChoiceSamplingEnemy();
extern s32 btlevtcmd_CommandGetWeaponAddress();
extern s32 btlevtcmd_CommandPayWeaponCost();
extern s32 btlevtcmd_GetBodyId();
extern s32 btlevtcmd_GetEnemyBelong();
extern s32 btlevtcmd_GetSelectEnemy();
extern s32 btlevtcmd_GetSelectNextEnemy();
extern s32 btlevtcmd_GetUnitId();
extern s32 btlevtcmd_OffPartsAttribute();
extern s32 btlevtcmd_OnPartsAttribute();
extern s32 btlevtcmd_SamplingEnemy();
extern s32 btlevtcmd_SetAlpha();
extern s32 btlevtcmd_SetRGB();
extern s32 btlevtcmd_StartWaitEvent();
extern s32 btlevtcmd_WaitGuardMove();
extern s32 btlevtcmd_WeaponAftereffect();
extern s32 btlevtcmd_reset_move_color_lv_all();
extern s32 btlevtcmd_snd_se();
extern s32 evt_batstage_return_aud_dark_base();
extern s32 evt_batstage_return_stg_dark_base();
extern s32 evt_batstage_set_aud_dark();
extern s32 evt_batstage_set_stg_dark();
extern s32 evt_btl_camera_set_mode();
extern s32 evt_btl_camera_set_moveSpeedLv();
extern s32 evt_btl_camera_shake_h();
extern s32 evt_eff();
extern s32 evt_img_alloc_capture();
extern s32 evt_img_clear_virtual_point();
extern s32 evt_img_entry();
extern s32 evt_img_onoff();
extern s32 evt_img_release();
extern s32 evt_img_set_paper();
extern s32 evt_img_set_paper_anim();
extern s32 evt_map_set_tevcallback();
extern s32 evt_mapobj_flag_onoff();
extern s32 evt_paper_delete();
extern s32 evt_paper_entry();
extern s32 evt_snd_sfxoff();
extern s32 evt_snd_sfxon();
extern s32 tou_gamen_screen_tev();
extern u32 flash_evt[];
extern u32 flash_main_evt[];
extern u32 flash_cancel_evt[];
extern u32 marioAttackEvent_Suki[];

/* Exact US local BattleWeapon payload followed by documented Event arrays. */
static u32 weapon[48] = {
    (u32)(char*)str_btl_wn_sac_sukkari_s_803012d0, 0x01A10000, 0x00000000, (u32)(char*)str_msg_sukkari_sukkiri_803012ec, 0x64000200, 0x3F800000,
    0x01056432, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02101260, 0x20000000, 0x00000302, (u32)(char*)str_msg_sac_7_80301300, 0x00020008,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00006400, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, (u32)marioAttackEvent_Suki, 0x00000064, 0x00000000, 0x00000000,
};

u32 marioAttackEvent_Suki[439] = {
    0x0003005B, (u32)btlevtcmd_GetUnitId, 0xFFFFFFFC, 0xFE363C8E, 0x0003005B, (u32)btlevtcmd_GetBodyId,
    0xFE363C8E, 0xFE363C8F, 0x0004005B, (u32)btlevtcmd_CheckStatus, 0xFE363C8E, 0x0000001B,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000, 0x00020032, 0xFE363C8E,
    0xFFFFFFFF, 0x00000021, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy, (u32)weapon, 0xFE363C83,
    0xFE363C84, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00020018,
    0xFE363C83, 0xFFFFFFFF, 0x00010004, 0x00000063, 0x00000021, 0x0004005B,
    (u32)btlevtcmd_AttackDeclare, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84, 0x0001005B, (u32)btlevtcmd_WaitGuardMove,
    0x0002005B, (u32)btlevtcmd_CommandPayWeaponCost, 0xFFFFFFFE, 0x0003005B, (u32)btlevtcmd_CommandGetWeaponAddress, 0xFFFFFFFE,
    0xFE363C80, 0x0002005B, (u32)btlevtcmd_WeaponAftereffect, 0xFE363C80, 0x0001005B, (u32)btlevtcmd_reset_move_color_lv_all,
    0x0000006E, 0x0002005B, (u32)main_suki, 0xFE363C8E, 0x00000070, 0x0001005E,
    (u32)sac_start_camera_evt, 0x0001000A, 0x000001F4, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_BTL_SAC_START1_8030143c, 0xF1194D80, 0x00000000, 0xF1194D80, 0x0001005E, (u32)sac_mario_item_on_evt,
    0x0001000A, 0x000001F4, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_SAC_START2_80301450,
    0xF1194D80, 0x00000000, 0xFE363C8B, 0x0001005B, (u32)star_stone_appear, 0x0000006E,
    0x0001005E, (u32)sac_start_stardust, 0x00000070, 0x00010009, 0x00000064, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_SAC_START3_80301464, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8B, 0x00010009, 0x00000028, 0x0001005E,
    (u32)sac_mario_item_on_end_evt, 0x0001005E, (u32)sac_mario_back_home, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_M_I_Y_8042863c, 0x0001005B, (u32)wait_star_stone_up_end, 0x0001000A, 0x00000064,
    0x0000006E, 0x0002005B, (u32)sac_cheer, 0x00000000, 0x00000070, 0x0001005B,
    (u32)sac_ac_help_on, 0x0001005B, (u32)start_game, 0x0002005B, (u32)wait_game_end, 0xFE363C80,
    0x0001005B, (u32)sac_ac_help_off, 0x0001005B, (u32)sac_cheer_end, 0x0003005B, (u32)set_weapon,
    0xFE363C80, 0xFE363C89, 0x0001000A, 0x000001F4, 0x0003005B, (u32)evt_btl_camera_set_mode,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002,
    0x0001000A, 0x000003E8, 0x00020032, 0xFE363C8A, 0x00000000, 0x00020032,
    0xFE363C8B, 0xFE363C80, 0x0002005B, (u32)evt_paper_entry, (u32)(char*)str_OFF_d_yabu_small_80301478, 0x00010005,
    0xFE363C8B, 0x0005005B, (u32)star_stone_get_attack_point, 0xFE363C8A, 0xFE363C81, 0xFE363C82,
    0xFE363C83, 0x0003005B, (u32)get_img_name, 0xFE363C8A, 0xFE363C8D, 0x0002005B,
    (u32)evt_img_entry, 0xFE363C8D, 0x0003005B, (u32)evt_img_set_paper, 0xFE363C8D, (u32)(char*)str_OFF_d_yabu_small_80301478,
    0x0006005B, (u32)get_screen_point, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0xFE363C84,
    0xFE363C85, 0x00020035, 0xFE363C84, 0x00000100, 0x00020037, 0xFE363C85,
    0xFFFFFFFF, 0x00020035, 0xFE363C85, 0x000000CC, 0x0009005B, (u32)evt_img_alloc_capture,
    0xFE363C8D, 0x00000000, 0x00000000, 0x00000000, 0xFE363C84, 0xFE363C85,
    0x00000060, 0x00000048, 0x0002005B, (u32)evt_img_clear_virtual_point, 0xFE363C8D, 0x0003005B,
    (u32)evt_img_onoff, 0xFE363C8D, 0x00000001, 0x0003005B, (u32)evt_img_set_paper_anim, 0xFE363C8D,
    (u32)(char*)str_A_1_80428644, 0x0000006E, 0x0002005B, (u32)star_stone_attack, 0xFE363C8A, 0x0006005B,
    (u32)evt_btl_camera_shake_h, 0x00000000, 0x00000008, 0x00000000, 0x0000000A, 0x00000000,
    0x00000070, 0x0003005B, (u32)eff_crystal, 0xFE363C81, 0xFE363C82, 0x000F005B,
    (u32)evt_eff, (u32)(char*)zero_80428648, (u32)(char*)str_indirect_8030148c, 0x00000000, 0xFE363C81, 0xFE363C82,
    0x0000001E, 0x0000003C, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_SAC_SUKKIRI4_80301498,
    0x00000000, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_SAC_SUKKIRI4_803014b0, 0x00000000, 0x00020018,
    0xFE363C8A, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C81,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C81, (u32)weapon, 0x0004005B,
    (u32)btlevtcmd_ChoiceSamplingEnemy, (u32)weapon, 0xFE363C83, 0xFE363C84, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy,
    0xFE363C83, 0xFE363C84, 0x00010003, 0x00000005, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000008, 0x00000021, 0x0005005B, (u32)btlevtcmd_CheckPartsAttribute,
    0xFE363C83, 0xFE363C84, 0x20000000, 0xFE363C81, 0x00020018, 0xFE363C81,
    0x00000000, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFE363C83, 0xFE363C84, 0x00000027,
    0x00000021, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00010004,
    0x00000005, 0x00000021, 0x00010003, 0x00000008, 0x00010009, 0x0000001E,
    0x0001005B, (u32)wait_star_stone_attack_end, 0x00020035, 0xFE363C8A, 0x00000001, 0x00000006,
    0x0002005B, (u32)evt_paper_delete, (u32)(char*)str_OFF_d_yabu_small_80301478, 0x00020032, 0xFE363C8A, 0x00000000,
    0x00020032, 0xFE363C8B, 0xFE363C80, 0x00010005, 0xFE363C8B, 0x0003005B,
    (u32)get_img_name, 0xFE363C8A, 0xFE363C8D, 0x0002005B, (u32)evt_img_release, 0xFE363C8D,
    0x00020035, 0xFE363C8A, 0x00000001, 0x00000006, 0x0001005E, (u32)flash_evt,
    0x00020032, 0xFE363C8A, 0x00000003, 0x00010005, 0x00000004, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_BTL_SAC_SUKKIRI5_803014c8, 0x00000000, 0x00010005, 0x00000004, 0x00020035,
    0xFE363C8A, 0x0000003F, 0x0002005B, (u32)flash_update, 0xFE363C8A, 0x0001005E,
    (u32)flash_main_evt, 0x00010009, 0x00000001, 0x00000006, 0x00010005, 0x00000004,
    0x00020036, 0xFE363C8A, 0x0000003F, 0x0002005B, (u32)flash_update, 0xFE363C8A,
    0x0001005E, (u32)flash_main_evt, 0x00010009, 0x00000001, 0x00000006, 0x00000006,
    0x0001005E, (u32)flash_cancel_evt, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, (u32)weapon, 0x0004005B,
    (u32)btlevtcmd_ChoiceSamplingEnemy, (u32)weapon, 0xFE363C83, 0xFE363C84, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy,
    0xFE363C83, 0xFE363C84, 0x00010003, 0x0000000A, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000014, 0x00000021, 0x0005005B, (u32)btlevtcmd_CheckPartsAttribute,
    0xFE363C83, 0xFE363C84, 0x00004000, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000000, 0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83, 0xFE363C84,
    0xFE363C89, 0x00000100, 0xFE363C85, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy,
    0xFE363C83, 0xFE363C84, 0x00010004, 0x0000000A, 0x00010003, 0x00000014,
    0x00010009, 0x0000001E, 0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000000,
    0x0001005B, (u32)end_suki, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

u32 flash_evt[182] = {
    0x0003005B, (u32)evt_map_set_tevcallback, 0x00000001, (u32)set_tev, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000001, (u32)(char*)str_bti_S_8042864c, 0x01000000, 0x0005005B, (u32)evt_mapobj_flag_onoff,
    0x00000001, 0x00000000, (u32)(char*)str_bti_S_8042864c, 0x00000100, 0x0003005B, (u32)exist_map,
    (u32)(char*)str_A1_80428654, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001, 0x0005005B,
    (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, (u32)(char*)str_A1_80428654, 0x0F000000, 0x0005005B,
    (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (u32)(char*)str_A1_80428654, 0x01000000, 0x0005005B,
    (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, (u32)(char*)str_A1_80428654, 0x00000100, 0x00000021,
    0x0003005B, (u32)exist_map, (u32)(char*)str_A2_80428658, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000001, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (u32)(char*)str_A2_80428658,
    0x01000000, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, (u32)(char*)str_A2_80428658,
    0x00000100, 0x00000021, 0x0003005B, (u32)exist_map, (u32)(char*)str_B_8042865c, 0xFE363C80,
    0x00020018, 0xFE363C80, 0x00000001, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, (u32)(char*)str_B_8042865c, 0x01000000, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000000, (u32)(char*)str_B_8042865c, 0x00000100, 0x00000021, 0x0003005B, (u32)exist_map,
    (u32)(char*)str_C_80428660, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001, 0x0005005B,
    (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (u32)(char*)str_C_80428660, 0x01000000, 0x0005005B,
    (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, (u32)(char*)str_C_80428660, 0x00000100, 0x00000021,
    0x0004005B, (u32)btlevtcmd_SetAlpha, 0xFFFFFFFE, 0x00000001, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0x00000001, 0x02000000, 0x00020019, 0xFE363C8E,
    0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_SetAlpha, 0xFE363C8E, 0xFE363C8F, 0x00000000,
    0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFE363C8E, 0xFE363C8F, 0x02000000, 0x00000021,
    0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy,
    0xFFFFFFFE, 0xFE363C80, (u32)weapon, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy, (u32)weapon,
    0xFE363C83, 0xFE363C84, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84,
    0x00010003, 0x00000005, 0x00020018, 0xFE363C83, 0xFFFFFFFF, 0x00010004,
    0x00000008, 0x00000021, 0x0006005B, (u32)btlevtcmd_SetRGB, 0xFE363C83, 0xFE363C84,
    0x00000000, 0x00000000, 0x00000000, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83,
    0xFE363C84, 0x00010004, 0x00000005, 0x00010003, 0x00000008, 0x0002005B,
    (u32)aud_set_draw, 0x00000000, 0x0004005B, (u32)evt_batstage_set_stg_dark, 0x00000000, 0x00000000,
    0x00000000, 0x0004005B, (u32)evt_batstage_set_aud_dark, 0x00000000, 0x00000000, 0x00000000,
    0x00000002, 0x00000001,
};

u32 flash_main_evt[52] = {
    0x00020032, 0xFE363C8B, 0x000000FF, 0x00020036, 0xFE363C8B, 0xFE363C8A,
    0x0002005B, (u32)flash_update, 0xFE363C8A, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE,
    0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, (u32)weapon,
    0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy, (u32)weapon, 0xFE363C83, 0xFE363C84, 0x0003005B,
    (u32)btlevtcmd_GetSelectEnemy, 0xFE363C83, 0xFE363C84, 0x00010003, 0x00000005, 0x00020018,
    0xFE363C83, 0xFFFFFFFF, 0x00010004, 0x00000008, 0x00000021, 0x0006005B,
    (u32)btlevtcmd_SetRGB, 0xFE363C83, 0xFE363C84, 0xFE363C8B, 0xFE363C8B, 0xFE363C8B,
    0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00010004, 0x00000005,
    0x00010003, 0x00000008, 0x00000002, 0x00000001,
};

u32 flash_cancel_evt[194] = {
    0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, (u32)(char*)str_bti_S_8042864c, 0x0F000000,
    0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (u32)(char*)str_bti_S_8042864c, 0x00000100,
    0x0003005B, (u32)exist_map, (u32)(char*)str_A1_80428654, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000001, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, (u32)(char*)str_A1_80428654,
    0x0F000000, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (u32)(char*)str_A1_80428654,
    0x00000100, 0x00000021, 0x0003005B, (u32)exist_map, (u32)(char*)str_A2_80428658, 0xFE363C80,
    0x00020018, 0xFE363C80, 0x00000001, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000000, (u32)(char*)str_A2_80428658, 0x0F000000, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, (u32)(char*)str_A2_80428658, 0x00000100, 0x00000021, 0x0003005B, (u32)exist_map,
    (u32)(char*)str_B_8042865c, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001, 0x0005005B,
    (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, (u32)(char*)str_B_8042865c, 0x0F000000, 0x0005005B,
    (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (u32)(char*)str_B_8042865c, 0x00000100, 0x00000021,
    0x0003005B, (u32)exist_map, (u32)(char*)str_C_80428660, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000001, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, (u32)(char*)str_C_80428660,
    0x0F000000, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (u32)(char*)str_C_80428660,
    0x00000100, 0x00000021, 0x0003005B, (u32)exist_map, (u32)(char*)str_gamen_80428664, 0xFE363C80,
    0x00020018, 0xFE363C80, 0x00000001, 0x0003005B, (u32)evt_map_set_tevcallback, 0x00000008,
    (u32)tou_gamen_screen_tev, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (u32)(char*)str_gamen_80428664,
    0x08000000, 0x0005005B, (u32)evt_mapobj_flag_onoff, 0x00000001, 0x00000000, (u32)(char*)str_gamen_80428664,
    0x00000100, 0x00000021, 0x0004005B, (u32)btlevtcmd_SetAlpha, 0xFFFFFFFE, 0x00000001,
    0x000000FF, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0x00000001, 0x02000000,
    0x00020019, 0xFE363C8E, 0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_SetAlpha, 0xFE363C8E,
    0xFE363C8F, 0x000000FF, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFE363C8E, 0xFE363C8F,
    0x02000000, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFE, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE, 0xFE363C80, (u32)weapon, 0x0004005B,
    (u32)btlevtcmd_ChoiceSamplingEnemy, (u32)weapon, 0xFE363C83, 0xFE363C84, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy,
    0xFE363C83, 0xFE363C84, 0x00010003, 0x00000005, 0x00020018, 0xFE363C83,
    0xFFFFFFFF, 0x00010004, 0x00000008, 0x00000021, 0x0006005B, (u32)btlevtcmd_SetRGB,
    0xFE363C83, 0xFE363C84, 0x000000FF, 0x000000FF, 0x000000FF, 0x0003005B,
    (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00010004, 0x00000005, 0x00010003,
    0x00000008, 0x0002005B, (u32)aud_set_draw, 0x00000001, 0x0003005B, (u32)evt_batstage_return_stg_dark_base,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_batstage_return_aud_dark_base, 0x00000000, 0x00000000,
    0x00000002, 0x00000001,
};

/* Exact local 8-word success Event in .sdata. */
__declspec(section ".sdata") static u32 nice_event[8] = {
    0x0005005B, (u32)btlevtcmd_ACSuccessEffect, 0xFE363C80, 0xFFFFFF88, 0x00000078, 0x00000000,
    0x00000002, 0x00000001,
};

/* Exact US Showstopper small-data prefix before the compiler scalar pool.
 * Real objects occupy target +0x00..+0x3B; natural alignment supplies the
 * target two-byte gaps at +0x06 and +0x3A.  Scalar storage from +0x3C onward
 * remains compiler/function-source-shape owned in this breadth-first pass. */
__declspec(section ".sdata2") u8 N_ohko_rate_tbl[6] = { 50, 60, 70, 80, 90, 100 };
__declspec(section ".sdata2") GXColor dat_80428638 = { 0, 0, 0, 0xFF };
__declspec(section ".sdata2") char str_M_I_Y_8042863c[] = "M_I_Y";
__declspec(section ".sdata2") char str_A_1_80428644[] = "A_1";
__declspec(section ".sdata2") char zero_80428648[] = "";
__declspec(section ".sdata2") char str_bti_S_8042864c[] = "bti_S";
__declspec(section ".sdata2") char str_A1_80428654[] = "A1";
__declspec(section ".sdata2") char str_A2_80428658[] = "A2";
__declspec(section ".sdata2") char str_B_8042865c[] = "B";
__declspec(section ".sdata2") char str_C_80428660[] = "C";
__declspec(section ".sdata2") char str_gamen_80428664[] = "gamen";
