#include "action/star/sac_suki.h"
#include "event/evt_cmd.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"

void* get_ptr(void);
void main_button(s32 index);
void disp_2D(void);
void disp_3D(void);
void disp_3D_alpha(void);

extern void* _battleWorkPointer;
s32 evtGetValue(EventEntry* event, s32 value);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
f32 evtGetFloat(EventEntry* event, s32 value);
f32 evtSetFloat(EventEntry* event, s32 target, f32 value);
void* mapGetMapObj(s32 name);
void* BattleAudienceBaseGetPtr(void);
void effDelete(void* effect);
void* memset(void* dest, int value, u32 size);
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
void* camGetPtr(s32 cameraId);
s32 irand(s32 max);
void N__disp_2D(s32 type, s32 index);
void main_base(void);
void main_star(void);
void dispEntry(s32 cameraId, s32 order, void* callback, f32 priority, void* param);
extern u8 weapon[];
extern u32 N_ohko_rate_tbl;
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
void main_base(void) {
    extern void* get_ptr(void);
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
    extern const s32 nice_event[];
    extern u32 DAT_80301364;
    extern u32 DAT_80301368;
    extern u32 vec3_8030136c[];
    extern u32 vec3_80301378[];

    u8* work = get_ptr();
    s32 audience = BattleAudience_GetAudienceNum();
    s32 state = *(s32*)work;
    s32 i;

    if (state == 3) {
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
            (f32)intplGetValue(-60.0, 0.0, 0,
                               *(s32*)(work + 4), 0xF);
        *(s32*)(work + 4) += 1;
        if (*(s32*)(work + 4) > 0xE) {
            *(s32*)work = 5;
            *(s32*)(work + 4) = 0;
            *(s32*)(work + 0x22C) = 8;
        }
    } else if (state < 3) {
        if (state == 1) {
            *(s32*)work = 2;
            *(s32*)(work + 4) = 0;
        } else if (state < 1) {
            return;
        }

        if (*(s32*)(work + 0xC) == 0) {
            *(f32*)(work + 0xD0) =
                (f32)intplGetValue(-1000.0, 0.0, 4,
                                   *(s32*)(work + 4), 0xF);
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
            (f32)intplGetValue(-80.0, 0.0, 0,
                               *(s32*)(work + 4), 0xF);
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
    } else if (state == 5) {
        *(s32*)(work + 4) += 1;
        if (*(s32*)(work + 4) < 0x1F) {
            *(f32*)(work + 0xD0) =
                (f32)intplGetValue(0.0, -1000.0, 4,
                                   *(s32*)(work + 4), 0x1E);
            *(u32*)(work + 0xD4) = vec3_80301378[1];
            *(u32*)(work + 0xD8) = vec3_80301378[2];
        }
    } else if (state < 5) {
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
void main_star(void) {
    extern void* _battleWorkPointer;
    extern void* get_ptr(void);
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

void disp_2D(void) {
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
    extern void* get_ptr(void);
    extern void iconDispGx(f32 scale, Vec* pos, s32 flags, s32 iconId);
    extern Vec vec3_80301430;
    extern f32 float_0_80428680;
    extern f32 float_1_80428670;
    extern f32 float_60_80428684;
    extern f32 float_80_8042867c;

    void* work;
    void* entry;
    Vec pos;
    s32 selected;
    u16 icon;

    work = get_ptr();
    if (type == 0) {
        entry = (void*)((s32)work + index * 0x18 + 0x184);
        selected = *(s32*)((s32)work + 0x14);
    } else {
        entry = (void*)((s32)work + index * 0x18 + 0xDC);
        selected = *(s32*)((s32)work + 0x10);
    }

    if (type == 0) {
        if (*(s32*)entry == 0) {
            return;
        }
    } else if (type == 1) {
        if (*(s32*)entry != 2) {
            return;
        }
    }

    switch (*(s32*)((s32)entry + 8)) {
        case 0x100:
            if (index < selected) {
                icon = 0x6D;
            } else {
                icon = 0x6C;
            }
            break;
        case 0x200:
            if (index < selected) {
                icon = 0x6F;
            } else {
                icon = 0x6E;
            }
            break;
        case 0x400:
            if (index < selected) {
                icon = 0x71;
            } else {
                icon = 0x70;
            }
            break;
        case 0x800:
            if (index < selected) {
                icon = 0x73;
            } else {
                icon = 0x72;
            }
            break;
        case 0x40:
            if (index < selected) {
                icon = 0x87;
            } else {
                icon = 0x86;
            }
            break;
        case 0x20:
            if (index < selected) {
                icon = 0x89;
            } else {
                icon = 0x88;
            }
            break;
        case 0x10:
            if (index < selected) {
                icon = 0x8B;
            } else {
                icon = 0x8A;
            }
            break;
    }

    if (icon != 0) {
        pos = vec3_80301430;
        pos.x = *(f32*)((s32)entry + 0xC);
        if (type == 0) {
            pos.y = *(f32*)((s32)entry + 0x10) + *(f32*)((s32)work + 0x24) + float_60_80428684 + float_80_8042867c;
        } else {
            pos.y = *(f32*)((s32)entry + 0x10) + *(f32*)((s32)work + 0x24) + float_60_80428684 + float_0_80428680;
        }
        pos.z = *(f32*)((s32)entry + 0x14);
        iconDispGx(float_1_80428670, &pos, 0x10, icon);
    }
}

void disp_3D(void) {
    ;
}

void disp_3D_alpha(void) {
    ;
}

void* get_ptr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1F4C);
}

USER_FUNC(start_game) {
    void* wp;

    wp = get_ptr();
    *(s32*)wp = 1;
    return 2;
}

USER_FUNC(wait_game_end) {
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

USER_FUNC(star_stone_appear) {
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

USER_FUNC(star_stone_attack) {
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

USER_FUNC(set_weapon) {
    s32 type;
    s32* args;
    void* work;
    u8 rates[6];

    args = event->args;
    type = evtGetValue(event, args[0]);
    work = get_ptr();
    *(u32*)&rates[0] = N_ohko_rate_tbl;
    *(u16*)&rates[4] = *(u16*)((s32)&N_ohko_rate_tbl + 4);
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
    extern void* effCrystalN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 scale);

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
