#include "battle/battle_seq_end.h"

extern void* _battleWorkPointer;
extern s32 evtGetValue();
extern void dispEntry(s32, s32, void*, void*, f32);
extern void N__draw_select_one_to_upgrade_text();
extern void* BattleAudienceBaseGetPtr();
extern s32 pouchGetMaxAP();
extern void pouchSetAP();
extern void windowDispGX_Waku_col(s32, void*, f32, f32, f32, f32, f32);
extern u32 dat_80427158;
extern void* BattleGetMarioPtr();
extern void* evtEntry();
extern void _koura_delete_event();
extern void* pouchGetPtr();
extern void evtSetValue();
extern f32 evtGetFloat();
extern void* BattleSearchObjectPtr();
extern void iconDelete();
extern s32 irand();
extern void BattleAudienceSoundWhistle();
extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
extern void* BtlUnit_GetData(void* unit, s32 dataId);

typedef struct RankUpData {
    s16 level;
    s16 field_0x2;
    const char* name;
    const char* message;
} RankUpData;

extern RankUpData _rank_up_data[];

RankUpData* _get_rank_data(s32 level) {
    RankUpData* data = _rank_up_data;

    goto compare;
loop:
    if (data->message == NULL) {
        return NULL;
    }
    data++;
compare:
    if (level != data->level) {
        goto loop;
    }
    return data;
}

const char* BattleGetRankNameLabel(s32 level) {
    RankUpData* data = _rank_up_data;
    RankUpData* best = NULL;

    while (level >= data->level) {
        if (data->level < 0) {
            break;
        }
        best = data;
        data++;
    }
    if (best == NULL) {
        return NULL;
    }
    return best->name;
}

/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: btlseqEnd | missing_definition | ghidra_signature */
void btlseqEnd(void* battleWork) {
    extern void _audience_Whistle_control(void);
    extern s32 BattleGetSeq(void*, s32);
    extern void BattleSetSeq(void*, s32, s32);
    extern void btlsubResetMoveColorLvAll(void*);
    extern void* BattleAlloc(s32);
    extern void btlseqEnd_DispInit(void*);
    extern s32 irand(s32);
    extern void _ExecAllUnitBattleEndEvent(void);
    extern void btl_camera_set_moveSpeedLv(s32,s32);
    extern void btl_camera_set_mode(s32,s32);
    extern void btl_camera_set_posoffset(f64,f64,f64,s32);
    extern void fadeEntry(s32,s32,void*);
    extern void psndBGMOff(s32);
    extern s32 fadeIsFinish(void);
    extern void seqSetSeq(s32,void*,void*);
    extern void* fbatGetPointer(void);
    extern void* evtEntry(void*,s32,s32);
    extern s32 evtCheckID(s32);
    extern s32 BattleWaitAllActiveEvtEnd(void*);
    extern void _BattleMarioKouraDelete(void);
    extern void BattleFogForceStop(void);
    extern s32 BattleGetStockExp(void*);
    extern void _GetExpIcon_Init(void*);
    extern void BattleAudienceSoundCheer(s32,s32);
    extern void BattleAudienceSoundClap(s32,s32);
    extern void BattleAudienceSoundWhistle(void);
    extern u32 BattlePadCheckNow(u32);
    extern void psndSFXOn(void*);
    extern void _GetExpIcon_End(void*);
    extern void effSoftDelete(void*);
    extern void BattleAudience_WinSetActive(s32);
    extern void BattleBreakSlot_HideReel(void);
    extern void BattleStatusWindowSystemOff(void);
    extern void BattleStatusWindowSystemOn(void);
    extern void BattleStatusWindowEventOn(void);
    extern void psndBGMOn(s32, void*);
    extern s32 _curtain_close_event[];
    extern char str_BGM_BATTLE_WIN3_802feabc[];
    extern u32 BattlePadCheckTrigger(u32);
    extern void evtDeleteID(s32);
    extern s16* _get_rank_data(s32);
    extern s32 _rank_up_event[];
    u8* work = *(u8**)((u8*)battleWork + 0xF28);
    s32 state;
    void* fbat;
    void* evt;
    u32 fadeColor = 0;

    _audience_Whistle_control();
    state = BattleGetSeq(battleWork, 7);
    switch (state) {
        case 0x07000000:
            btlsubResetMoveColorLvAll(battleWork);
            BattleSetSeq(battleWork, 7, 0x07000005);
            work = BattleAlloc(0x2AC);
            *(u8**)((u8*)battleWork + 0xF28) = work;
            btlseqEnd_DispInit(battleWork);
            *(s32*)work = 0;
            *(void**)(work + 0x29C) = 0;
            *(s32*)(work + 0x2A0) = irand(120) + 60;
            *(s32*)(work + 0x2A4) = 0;
            break;

        case 0x07000001:
            _ExecAllUnitBattleEndEvent();
            btl_camera_set_moveSpeedLv(0, 2);
            btl_camera_set_mode(0, 0x13);
            btl_camera_set_posoffset(0.0, 0.0, 0.0, 0);
            fadeEntry(9, 200, &fadeColor);
            psndBGMOff(0x201);
            BattleSetSeq(battleWork, 7, 0x07000002);
            break;

        case 0x07000002:
            if (fadeIsFinish() != 0) {
                BattleSetSeq(battleWork, 7, 0x0700001F);
            }
            break;

        case 0x07000003:
            fbat = fbatGetPointer();
            evt = evtEntry(*(void**)((u8*)fbat + 0x3C), 10, 0);
            *(s32*)((u8*)fbat + 0x40) = *(s32*)evt;
            BattleSetSeq(battleWork, 7, 0x07000004);
            return;

        case 0x07000004:
            fbat = fbatGetPointer();
            if (!evtCheckID(*(s32*)((u8*)fbat + 0x40)) &&
                BattleWaitAllActiveEvtEnd(battleWork) != 0) {
                if (*(s32*)((u8*)fbat + 0x44) != 0) {
                    BattleSetSeq(battleWork, 3, 0x03000001);
                    BattleSetSeq(battleWork, 0, 2);
                    return;
                }
                BattleSetSeq(battleWork, 7, 0x07000005);
                _BattleMarioKouraDelete();
                work = BattleAlloc(0x2AC);
                *(u8**)((u8*)battleWork + 0xF28) = work;
                btlseqEnd_DispInit(battleWork);
                *(s32*)work = 0;
                return;
            }
            break;

        case 0x07000006:
            if (BattleWaitAllActiveEvtEnd(battleWork) != 0) {
                BattleAudienceSoundClap(-1, 0);
                _ExecAllUnitBattleEndEvent();
                BattleSetSeq(battleWork, 7, 0x07000008);
            }
            break;

        case 0x07000007:
            if (BattleWaitAllActiveEvtEnd(battleWork) != 0) {
                BattleSetSeq(battleWork, 7, 0x07000008);
            }
            break;

        case 0x07000008:
            BattleFogForceStop();
            *(s32*)(work + 0xC) = 0;
            *(s16*)(work + 8) = BattleGetStockExp(battleWork);
            if (*(s16*)(work + 8) > 100) *(s16*)(work + 8) = 100;
            *(s16*)(work + 0xA) = *(s16*)(work + 8);
            if (*(s16*)(work + 8) > 0) _GetExpIcon_Init(work);
            *(s32*)(work + 4) = 120;
            btl_camera_set_moveSpeedLv(0, 2);
            btl_camera_set_mode(0, 0x11);
            btl_camera_set_posoffset(0.0, 0.0, 0.0, 0);
            BattleSetSeq(battleWork, 7, 0x07000009);
            BattleAudienceSoundCheer(60, 60);
            BattleAudienceSoundClap(-1, 0);
            BattleAudienceSoundWhistle();
        case 0x07000009:
            if (--*(s32*)(work + 4) < 1) {
                BattleSetSeq(battleWork, 7, 0x0700000A);
            }
            break;

        case 0x0700000B:
            if (*(s16*)(work + 0xA) < 1) {
                if (*(s32*)(work + 4) < 1000) {
                    if (*(s32*)(work + 4) < 120) {
                        *(s32*)(work + 4) = 140 - *(s32*)(work + 4);
                    } else {
                        *(s32*)(work + 4) = 20;
                    }
                } else {
                    *(s32*)(work + 4) = 10;
                }
                BattleSetSeq(battleWork, 7, 0x0700000C);
            } else {
                *(s32*)(work + 4) += 1;
                if (BattlePadCheckNow(0x300) != 0 && *(s32*)(work + 4) < 1000) {
                    *(s32*)(work + 4) += 1000;
                }
                if ((*(s32*)(work + 4) & 3) == 0 || *(s32*)(work + 4) > 999) {
                    *(s16*)(work + 0xA) -= 1;
                    if ((*(s32*)(work + 4) & 7) == 0) {
                        psndSFXOn((void*)0x2000B);
                    }
                }
            }
            break;

        case 0x0700000C:
            if (--*(s32*)(work + 4) < 1) {
                *(s32*)(work + 4) = 60;
                _GetExpIcon_End(work);
                BattleSetSeq(battleWork, 7, 0x0700000D);
                effSoftDelete(*(void**)(work + 0x29C));
                *(s32*)(work + 4) = (*(s32*)(work + 0xC) == 0) ? 5 : 60;
            }
            break;

        case 0x0700000D:
            if (--*(s32*)(work + 4) < 1) {
                *(void**)(work + 0x29C) = 0;
                BattleSetSeq(battleWork, 7, 0x0700000E);
            }
            break;

        case 0x07000010:
            *(u32*)(work + 0x30) &= ~1;
            if (*(s32*)(work + 0xC) == 0) {
                BattleSetSeq(battleWork, 7, 0x07000019);
            } else {
                *(s32*)(work + 0x10) = 1;
                *(s32*)(work + 0x14) = 0;
                *(s32*)(work + 0x18) = 3;
                *(s32*)(work + 4) = 0;
                BattleAudience_WinSetActive(0);
                BattleBreakSlot_HideReel();
                BattleSetSeq(battleWork, 7, 0x07000011);
            }
            break;

        case 0x07000011:
            if (*(s32*)(work + 4) == 0) {
                evt = evtEntry(_curtain_close_event, 10, 0);
                *(s32*)(work + 0x1FC) = *(s32*)evt;
                BattleStatusWindowSystemOff();
                psndBGMOff(0x400);
                psndBGMOn(1, str_BGM_BATTLE_WIN3_802feabc);
            }
            *(s32*)(work + 4) += 1;
            if (!evtCheckID(*(s32*)(work + 0x1FC))) {
                *(s32*)(work + 0x1FC) = 0;
                BattleStatusWindowSystemOn();
                BattleStatusWindowEventOn();
                BattleSetSeq(battleWork, 7, 0x07000012);
            }
            break;

        case 0x07000013:
            if (BattlePadCheckTrigger(0x20000) != 0) {
                *(s32*)(work + 0x24) += 1;
                if (*(s32*)(work + 0x24) >= *(s32*)(work + 0x2C)) {
                    *(s32*)(work + 0x24) = 0;
                }
            }
            if (BattlePadCheckTrigger(0x10000) != 0) {
                *(s32*)(work + 0x24) -= 1;
                if (*(s32*)(work + 0x24) < 0) {
                    *(s32*)(work + 0x24) = *(s32*)(work + 0x2C) - 1;
                }
            }
            if (BattlePadCheckTrigger(0x200) != 0) {
                *(u32*)(work + 0x30) &= ~0x10;
                BattleSetSeq(battleWork, 7, 0x07000012);
            }
            if (BattlePadCheckTrigger(0x100) != 0) {
                if (*(s32*)(work + 0x24) == 0) {
                    *(u32*)(work + 0x30) &= ~0x18;
                    BattleSetSeq(battleWork, 7, 0x07000015);
                } else {
                    *(u32*)(work + 0x30) &= ~0x10;
                    BattleSetSeq(battleWork, 7, 0x07000012);
                }
            }
            break;

        case 0x07000014:
            if (!evtCheckID(*(s32*)(work + 0x1FC))) {
                *(s32*)(work + 0x1FC) = 0;
                BattleSetSeq(battleWork, 7, 0x07000015);
            }
            break;

        case 0x07000016:
            *(s32*)(work + 4) -= 1;
            if (*(s32*)(work + 4) < 1) {
                if (*(s32*)(work + 0x2A4) != 0) {
                    evtDeleteID(*(s32*)(work + 0x2A4));
                }
                BattleSetSeq(battleWork, 7, 0x07000017);
            }
            break;

        case 0x07000017:
            {
                s16* rank = _get_rank_data(0);
                if (rank == 0) {
                    BattleSetSeq(battleWork, 7, 0x0700001C);
                    break;
                }
                evt = evtEntry(_rank_up_event, 10, 0);
                *(s32*)(work + 0x2A8) = *(s32*)evt;
                BattleSetSeq(battleWork, 7, 0x07000018);
            }
        case 0x07000018:
            if (!evtCheckID(*(s32*)(work + 0x2A8))) {
                BattleSetSeq(battleWork, 7, 0x0700001C);
            }
            break;

        case 0x0700001A:
            if (--*(s32*)(work + 4) < 1) {
                BattleSetSeq(battleWork, 7, 0x0700001B);
            }
            break;

        case 0x0700001B:
            BattleSetSeq(battleWork, 7, 0x0700001C);
            break;

        case 0x0700001C:
            BattleSetSeq(battleWork, 7, 0x0700001D);
            break;

        case 0x0700001D:
            fadeEntry(9, 200, &fadeColor);
            BattleSetSeq(battleWork, 7, 0x0700001E);
            psndBGMOff(0x201);
        case 0x0700001E:
            if (fadeIsFinish() != 0) {
                BattleSetSeq(battleWork, 7, 0x0700001F);
            }
            break;
    }
}

void _ExecAllUnitBattleEndEvent(void) {
    void* battleWork = _battleWorkPointer;
    s32 i;

    for (i = 0; i < 0x40; i++) {
        void* unit = BattleGetUnitPtr(battleWork, i);

        if (unit != NULL) {
            void* data = BtlUnit_GetData(unit, 0x3F);

            if (data != NULL) {
                void* evt = evtEntry(data, 10, 0);

                *(s32*)((s32)evt + 0x160) = *(s32*)unit;
            }
        }
    }
}

void btlseqEnd_DispInit(void* unit) {
    *(u32*)((s32)*(void**)((s32)unit + 0xF28) + 0x30) = 0;
}

void btlseqEnd_DispMain(void* disp, void* battleWork) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    void* battle;
    void* work;
    Vec vec;
    u32 flags;

    extern f32 float_900_8042720c;
    extern const Vec vec3_802fe7cc;
    extern void _GetExpIcon_Main(void* battleWork);
    extern void _GetExpIcon_Disp(void* battleWork);
    extern void _MarioExpDisp(void* battleWork);
    extern void _lvup_select_object_disp(void* battleWork);
    extern void _LvupParamConfirmDisp(void* battleWork);
    extern void L__LvupParamHelpMsgDisp(void* battleWork);
    extern f32 dispCalcZ(Vec* pos);

    battle = battleWork;
    work = *(void**)((s32)battle + 0xF28);
    if (work != NULL) {
        flags = *(u32*)((s32)work + 0x30);
        if (flags & 2) {
            _GetExpIcon_Main(battle);
            dispEntry((s32)disp, 1, _GetExpIcon_Disp, battle, float_900_8042720c);
        }
        if (*(u32*)((s32)work + 0x30) & 4) {
            dispEntry((s32)disp, 1, _MarioExpDisp, battle, float_900_8042720c);
        }
        if (*(u32*)((s32)work + 0x30) & 8) {
            vec = vec3_802fe7cc;
            dispEntry(4, 2, _lvup_select_object_disp, battle, dispCalcZ(&vec));
        }
        if (*(u32*)((s32)work + 0x30) & 0x10) {
            dispEntry((s32)disp, 1, _LvupParamConfirmDisp, battle, float_900_8042720c);
        }
        if (*(u32*)((s32)work + 0x30) & 0x20) {
            dispEntry((s32)disp, 1, L__LvupParamHelpMsgDisp, battle, float_900_8042720c);
        }
    }
}

void _MarioExpDisp(void* disp, void* battleWork) {
    u32 color;
    f32 pos[3];
    char buf[16];
    void* work;
    s32 len;

    extern f32 float_200_8042727c;
    extern f32 float_neg200_80427280;
    extern f32 float_0_804271c0;
    extern char str_PCT2d_80427284;
    extern u32 dat_80427148;
    extern f32 float_12p5_80427254;
    extern f32 float_65_804271ec;
    extern f32 float_180_804271f0;
    extern f32 float_neg160_80427288;
    extern f32 float_32_80427258;
    extern f32 float_10_80427214;
    extern f32 float_1_804271a8;
    extern f32 float_225_8042728c;
    extern f32 float_neg163_80427290;
    extern s32 sprintf(char* str, const char* format, ...);
    extern s32 strlen(const char* str);
    extern void iconDispGx(f32* pos, s32 icon, s32 color, f32 scale);
    extern void FontDrawStart(void);
    extern void FontDrawString(const char* str, f32 x, f32 y);

    work = *(void**)((s32)battleWork + 0xF28);
    pouchGetPtr();
    pos[0] = float_200_8042727c;
    pos[1] = float_neg200_80427280;
    pos[2] = float_0_804271c0;

    if (work != NULL) {
        sprintf(buf, &str_PCT2d_80427284, *(s16*)((s32)work + 0xA));
        len = strlen(buf);
        color = dat_80427148;
        windowDispGX_Waku_col(
            0,
            &color,
            float_180_804271f0,
            float_neg160_80427288,
            float_12p5_80427254 * len + float_65_804271ec,
            float_32_80427258,
            float_10_80427214);
        iconDispGx(pos, 0x10, 0x194, float_1_804271a8);
        FontDrawStart();
        FontDrawString(buf, float_225_8042728c, float_neg163_80427290);
    }
}

void _LvupParamConfirmDisp(void* disp, void* battleWork) {
    char buf[24];
    u32 color;
    f32 pos[3];
    void* work;
    char* base;
    s32 len;
    f32 halfWidth;

    extern char str_msg_menu_mario_name__802fe738[];
    extern u32 dat_8042714c;
    extern u32 dat_80427150;
    extern f32 float_6p25_8042724c;
    extern f32 float_neg30_80427248;
    extern f32 float_12p5_80427254;
    extern f32 float_65_804271ec;
    extern f32 float_30_80427250;
    extern f32 float_32_80427258;
    extern f32 float_10_80427214;
    extern f32 float_neg70_8042725c;
    extern f32 float_neg10_80427260;
    extern f32 float_130_80427264;
    extern f32 float_60_80427268;
    extern f32 float_0_804271c0;
    extern f32 float_27_8042726c;
    extern f32 float_neg20_804271d4;
    extern f32 float_neg13_80427270;
    extern f32 float_neg38_80427274;
    extern f32 float_neg40_80427278;
    extern f32 float_1_804271a8;
    extern const char* msgSearch(const char* msg);
    extern s32 sprintf(char* str, const char* format, ...);
    extern s32 strlen(const char* str);
    extern void FontDrawStart(void);
    extern void FontDrawString(const char* str, f32 x, f32 y);
    extern void iconDispGx(f32* pos, s32 icon, s32 color, f32 scale);

    base = str_msg_menu_mario_name__802fe738;
    work = *(void**)((s32)battleWork + 0xF28);
    pouchGetPtr();
    sprintf(buf, msgSearch(base + 0x314));
    len = strlen(buf);
    halfWidth = float_6p25_8042724c * len;
    color = dat_8042714c;
    windowDispGX_Waku_col(
        0,
        &color,
        float_neg30_80427248 - halfWidth,
        float_30_80427250,
        float_12p5_80427254 * len + float_65_804271ec,
        float_32_80427258,
        float_10_80427214);
    color = dat_80427150;
    windowDispGX_Waku_col(
        0,
        &color,
        float_neg70_8042725c,
        float_neg10_80427260,
        float_130_80427264,
        float_60_80427268,
        float_10_80427214);
    FontDrawStart();
    FontDrawString(buf, float_0_804271c0 - halfWidth, float_27_8042726c);
    FontDrawString(msgSearch(base + 0x324), float_neg20_804271d4, float_neg13_80427270);
    FontDrawString(msgSearch(base + 0x338), float_neg20_804271d4, float_neg38_80427274);

    pos[0] = float_neg40_80427278;
    pos[1] = (f32)(-50 - (*(s32*)((s32)work + 0x24) * 25));
    pos[2] = float_0_804271c0;
    iconDispGx(pos, 0x10, 0x194, float_1_804271a8);
}

/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: L__LvupParamHelpMsgDisp | prototype_only | source_prototype */
void L__LvupParamHelpMsgDisp(void* disp, void* battleWork) {
    extern char str_msg_menu_mario_name__802fe738[];
    extern u32 dat_80427154;
    extern f32 float_0_804271c0;
    extern f32 float_20_8042718c;
    extern f32 float_neg145_80427210;
    extern f32 float_10_80427214;
    extern f32 float_neg148_80427218;
    extern void* pouchGetPtr(void);
    extern const char* msgSearch(const char* msg);
    extern u32 FontGetMessageWidthLine(const char* msg, u16* lines);
    extern void FontDrawStart_alpha(s32 alpha);
    extern void FontDrawMessage(s32 x, s32 y, const char* msg);
    void* work;
    void* pouch;
    char* base;
    const char* msg;
    u16 lines;
    u32 color;
    s32 width;
    s32 lineCount;
    f32 x;

    base = str_msg_menu_mario_name__802fe738;
    work = *(void**)((s32)battleWork + 0xF28);
    pouch = pouchGetPtr();

    switch (*(s32*)((s32)work + 0x10)) {
        case 0:
            msg = msgSearch(base + 0x290);
            if (*(s16*)((s32)pouch + 0x8E) >= 0xC8) {
                msg = msgSearch(base + 0x2A4);
            }
            break;
        case 1:
            msg = msgSearch(base + 0x2BC);
            if (*(s16*)((s32)pouch + 0x90) >= 0xC8) {
                msg = msgSearch(base + 0x2D0);
            }
            break;
        case 2:
            msg = msgSearch(base + 0x2E8);
            if (*(s16*)((s32)pouch + 0x94) >= 0x63) {
                msg = msgSearch(base + 0x2FC);
            }
            break;
    }

    width = FontGetMessageWidthLine(msg, &lines) & 0xFFFF;
    lineCount = (u16)(lines + 1);
    color = dat_80427154;
    x = float_0_804271c0 - (f32)(width / 2);
    lines = lineCount;
    windowDispGX_Waku_col(
        0,
        &color,
        x - float_20_8042718c,
        float_neg145_80427210 + (f32)((lineCount - 1) * 0x1D),
        (f32)(width + 0x28),
        (f32)(lineCount * 0x1D + 1),
        float_10_80427214);
    FontDrawStart_alpha(0xFF);
    FontDrawMessage((s32)x, (s32)float_neg148_80427218 + ((s32)lines - 1) * 0x1D, msg);
}

/* CHATGPT FALLBACK MISSING STUBS: main/battle/battle_seq_end 20260624_191429 */

/* fallback stub-fill: map=_GetExpIcon_Init addr=0x802149ec size=0x0000025c */
void _GetExpIcon_Init(void* seqEndWork) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern int sprintf(char* s, const char* fmt, ...);
    extern void iconEntry2D(char* name, u16 id);
    extern void* iconNameToPtr(char* name);
    extern void iconSetScale(f64 scale, char* name);
    extern void iconFlagOn(char* name, u16 flags);
    extern void iconSetPos(f64 x, f64 y, f64 z, char* name);
    extern const char str_BgspPCTd_802fe9c0[];
    extern f32 float_0_804271c0;
    extern f32 float_25_804271e0;
    extern f32 float_24_8042722c;
    extern f32 float_18_80427230;
    extern f32 float_36_80427234;
    extern f32 float_0p9_80427238;
    extern f32 float_0p45_8042723c;
    extern f32 float_86p9_80427240;
    extern f32 float_16p2_80427244;

    char iconEntryName[16];
    void* iconEntry;
    s32 i;
    s32 iconOffset;
    s32 posOffset;
    f32 f36 = float_36_80427234;
    f32 f18 = float_18_80427230;
    f32 f24 = float_24_8042722c;
    f32 f25 = float_25_804271e0;
    f32 zero = float_0_804271c0;
    f32 f162 = float_16p2_80427244;
    f32 f869 = float_86p9_80427240;

    iconOffset = 0;
    posOffset = 0;
    for (i = 0; i < 10; i++) {
        sprintf(iconEntryName, str_BgspPCTd_802fe9c0, i);
        iconEntry2D(iconEntryName, 0x194);
        iconEntry = iconNameToPtr(iconEntryName);
        *(void**)((s32)seqEndWork + 0x34 + iconOffset) = iconEntry;
        iconSetScale(float_0p9_80427238, iconEntryName);
        iconFlagOn(iconEntryName, 2);
        *(s32*)((s32)seqEndWork + 0xCC + iconOffset) = 0;
        ((VecLocal*)((s32)seqEndWork + 0x118))[i].x =
            f36 * (f32)i - (f18 * (f32)(*(s16*)((s32)seqEndWork + 0xA) / 10) - f24);
        ((VecLocal*)((s32)seqEndWork + 0x118))[i].y = f25;
        ((VecLocal*)((s32)seqEndWork + 0x118))[i].z = zero;
        iconSetPos(((VecLocal*)((s32)seqEndWork + 0x118))[i].x,
                   ((VecLocal*)((s32)seqEndWork + 0x118))[i].y,
                   ((VecLocal*)((s32)seqEndWork + 0x118))[i].z,
                   iconEntryName);
        iconOffset += 4;
        posOffset += 0xC;
    }

    iconOffset = 0x28;
    posOffset = 0x78;
    for (i = 10; i < 0x13; i++) {
        sprintf(iconEntryName, str_BgspPCTd_802fe9c0, i);
        iconEntry2D(iconEntryName, 0x194);
        iconEntry = iconNameToPtr(iconEntryName);
        *(void**)((s32)seqEndWork + 0x34 + iconOffset) = iconEntry;
        iconSetScale(float_0p45_8042723c, iconEntryName);
        iconFlagOn(iconEntryName, 2);
        *(s32*)((s32)seqEndWork + 0xCC + iconOffset) = 0;
        *(f32*)((s32)seqEndWork + 0x118 + posOffset) =
            -(f162 * (f32)(i - 9) - f869);
        *(f32*)((s32)seqEndWork + 0x11C + posOffset) = zero;
        *(f32*)((s32)seqEndWork + 0x120 + posOffset) = zero;
        iconSetPos(*(f32*)((s32)seqEndWork + 0x118 + posOffset),
                   *(f32*)((s32)seqEndWork + 0x11C + posOffset),
                   *(f32*)((s32)seqEndWork + 0x120 + posOffset),
                   iconEntryName);
        iconOffset += 4;
        posOffset += 0xC;
    }
    *(u32*)((s32)seqEndWork + 0x30) |= 2;
}

/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: _GetExpIcon_Main | prototype_only | source_prototype */
void _GetExpIcon_Main(void* battleWork) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern void iconFlagOff(char* name, u16 flags);
    extern void iconFlagOn(char* name, u16 flags);
    extern void iconSetAlpha(char* name, u8 alpha);
    extern void iconSetPos(f32 x, f32 y, f32 z, char* name);
    extern f32 float_0_804271c0;
    extern f32 float_24_8042722c;
    extern f32 float_18_80427230;
    extern f32 float_36_80427234;

    void* seqEndWork;
    s16 remaining;
    s32 tens;
    s32 ones;
    s32 i;
    s32 iconOffset;
    s32 posOffset;
    s32 alpha;
    f32 baseY;

    seqEndWork = *(void**)((s32)battleWork + 0xF28);
    remaining = *(s16*)((s32)seqEndWork + 0xA);
    tens = remaining / 10;
    iconOffset = 0;
    posOffset = 0;
    for (i = 0; i < tens; i++) {
        iconFlagOff((char*)((s32)*(void**)((s32)seqEndWork + 0x34 + iconOffset) + 0x18), 2);
        alpha = *(s32*)((s32)seqEndWork + 0x80 + iconOffset);
        if (alpha < 1) {
            *(s32*)((s32)seqEndWork + 0x80 + iconOffset) = 0x40;
        } else {
            alpha += 0x40;
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            *(s32*)((s32)seqEndWork + 0x80 + iconOffset) = alpha;
        }
        if (*(s32*)((s32)seqEndWork + 0xCC + iconOffset) < 0x10) {
            *(s32*)((s32)seqEndWork + 0xCC + iconOffset) += 1;
        }
        *(f32*)((s32)seqEndWork + 0x118 + posOffset) =
            float_36_80427234 * (f32)i - (float_18_80427230 * (f32)tens - float_24_8042722c);
        iconOffset += 4;
        posOffset += 0xC;
    }

    iconOffset = i << 2;
    for (; i < 10; i++) {
        iconFlagOn((char*)((s32)*(void**)((s32)seqEndWork + 0x34 + iconOffset) + 0x18), 2);
        *(s32*)((s32)seqEndWork + 0x80 + iconOffset) = 0;
        *(s32*)((s32)seqEndWork + 0xCC + iconOffset) = 0;
        iconOffset += 4;
    }

    iconOffset = 0x28;
    ones = remaining % 10;
    for (i = 10; i - 10U < ones; i++) {
        iconFlagOff((char*)((s32)*(void**)((s32)seqEndWork + 0x34 + iconOffset) + 0x18), 2);
        alpha = *(s32*)((s32)seqEndWork + 0x80 + iconOffset);
        if (alpha < 1) {
            *(s32*)((s32)seqEndWork + 0x80 + iconOffset) = 0x40;
        } else {
            alpha += 0x40;
            if (alpha > 0xFF) {
                alpha = 0xFF;
            }
            *(s32*)((s32)seqEndWork + 0x80 + iconOffset) = alpha;
        }
        iconOffset += 4;
    }

    iconOffset = i << 2;
    for (; i < 0x13; i++) {
        iconFlagOn((char*)((s32)*(void**)((s32)seqEndWork + 0x34 + iconOffset) + 0x18), 2);
        *(s32*)((s32)seqEndWork + 0x80 + iconOffset) = 0;
        iconOffset += 4;
    }

    baseY = float_0_804271c0;
    iconOffset = 0;
    posOffset = 0;
    for (i = 0; i < 0x13; i++) {
        iconSetAlpha((char*)((s32)*(void**)((s32)seqEndWork + 0x34 + iconOffset) + 0x18),
                     (u8)*(s32*)((s32)seqEndWork + 0x80 + iconOffset));
        iconSetPos(*(f32*)((s32)seqEndWork + 0x118 + posOffset),
                   *(f32*)((s32)seqEndWork + 0x11C + posOffset) + baseY,
                   *(f32*)((s32)seqEndWork + 0x120 + posOffset),
                   (char*)((s32)*(void**)((s32)seqEndWork + 0x34 + iconOffset) + 0x18));
        iconOffset += 4;
        posOffset += 0xC;
    }
}

void _GetExpIcon_Disp(void) {
    u32 color = dat_80427158;

    windowDispGX_Waku_col(0, &color, -76.9f, 21.0f, 165.8f, 24.2f, 10.0f);
}

void _GetExpIcon_End(void* work) {
    s32 i;

    *(u32*)((s32)work + 0x30) &= ~2;
    for (i = 0; i < 19; i++) {
        iconDelete((void*)((s32)*(void**)((s32)work + i * 4 + 0x34) + 0x18));
    }
}

s32 N__set_initial_upgrade_cursor_position(void* event) {
    s32 value = evtGetValue(event, **(s32***)((s32)event + 0x18));
    void* work = *(void**)((s32)_battleWorkPointer + 0xF28);

    *(s32*)((s32)work + 0x10) = value;
    return 2;
}

void N__draw_select_one_to_upgrade_text(void) {
    const char* msg;
    u16 lines;
    u32 color;
    s32 width;
    s32 lineCount;
    f32 x;

    extern const char str_select_HPFPBP_802fe9b0[14];
    extern u32 dat_80427168;
    extern f32 float_0_804271c0;
    extern f32 float_20_8042718c;
    extern f32 float_neg145_80427210;
    extern f32 float_10_80427214;
    extern f32 float_neg148_80427218;
    extern const char* msgSearch(const char* msg);
    extern u32 FontGetMessageWidthLine(const char* msg, u16* lines);
    extern void FontDrawStart_alpha(s32 alpha);
    extern void FontDrawMessage(s32 x, s32 y, const char* msg);

    msg = msgSearch(str_select_HPFPBP_802fe9b0);
    width = FontGetMessageWidthLine(msg, &lines);
    lineCount = (u16)(lines + 1);
    color = dat_80427168;
    x = float_0_804271c0 - (f32)((s32)(u16)width / 2);
    lines = lineCount;
    windowDispGX_Waku_col(
        0,
        &color,
        x - float_20_8042718c,
        float_neg145_80427210 + (f32)((lineCount - 1) * 0x1D),
        (f32)((u16)width + 0x28),
        (f32)(lineCount * 0x1D + 1),
        float_10_80427214);
    FontDrawStart_alpha(0xFF);
    FontDrawMessage((s32)x, (s32)float_neg148_80427218 + ((s32)lines - 1) * 0x1D, msg);
}

s32 N__display_select_one_to_upgrade_text(void) {
    dispEntry(8, 1, N__draw_select_one_to_upgrade_text, _battleWorkPointer, 900.0f);
    return 2;
}

s32 _nozzle_set_offset(void* event) {
    void* args = *(void**)((s32)event + 0x18);
    s32 id = evtGetValue(event, *(s32*)args);
    f32 offset = evtGetFloat(event, *(s32*)((s32)args + 4));
    void* obj = BattleSearchObjectPtr(id);

    if (obj != NULL) {
        *(f32*)((s32)obj + 0x14) = offset;
    }
    return 2;
}

s32 _lvup_ap_recover_full(void) {
    void* audience = BattleAudienceBaseGetPtr();

    *(s32*)((s32)audience + 0x13910) = 1;
    pouchSetAP(pouchGetMaxAP());
    return 2;
}

s32 _disable_enemy_unit_disp(void) {
    void* unit;
    s32 i;
    void* battleWork;
    s32 kind;
    void* data;
    void* evt;
    void* obj;

    extern const char str_if_body_802fe9a0[8];
    extern const char str_if_wire_802fe9a8[8];
    extern void mapGrpFlagOn(const char* name, s32 flag);

    battleWork = _battleWorkPointer;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != NULL && (*(u32*)((s32)unit + 0x1C) & 0x80000000) == 0) {
            kind = *(s32*)((s32)unit + 8);
            if (kind != 0xDC && kind != 0xDE) {
                if (!(kind >= 0xE0 && kind < 0xE7 && *(s8*)((s32)unit + 0xC) == 0) || kind == 0xE5) {
                    data = BtlUnit_GetData(unit, 0x42);
                    if (data != NULL) {
                        evt = evtEntry(data, 10, 0);
                        *(s32*)((s32)evt + 0x160) = *(s32*)unit;
                    } else if (*(s32*)((s32)unit + 8) != 0xE5) {
                        *(u32*)((s32)unit + 0x104) |= 0x21000000;
                    }
                }
            }
        }
    }

    obj = BattleSearchObjectPtr(str_if_body_802fe9a0);
    if (obj != NULL) {
        *(u8*)((s32)battleWork + 0x180F8) |= 0x20;
        *(u32*)((s32)obj + 0x68) |= 1;
        mapGrpFlagOn(str_if_body_802fe9a0, 1);
        mapGrpFlagOn(str_if_wire_802fe9a8, 1);
    }

    return 2;
}

s32 _set_confetti_control_evtid(void* event) {
    s32 value = **(s32**)((s32)event + 0x18);
    void* battleWork = _battleWorkPointer;
    void* work = *(void**)((s32)battleWork + 0xF28);

    *(s32*)((s32)work + 0x2A4) = evtGetValue(event, value);
    return 2;
}

s32 _lvup_select_obj_disp_on(void) {
    void* work = *(void**)((s32)_battleWorkPointer + 0xF28);

    *(s32*)((s32)work + 0x30) |= 8;
    return 2;
}

s32 _lvup_select_obj_lvup_param_disp(void) {
    s32 i;
    u8* work = *(u8**)((s32)_battleWorkPointer + 0xF28);

    for (i = 0; i < 3; i++) {
        work[i * 0x34 + 0x228] = 1;
    }
    return 2;
}

s32 _lvup_obj_set_color(void* event) {
    s32* args;
    void* work;
    s32 index;
    s32 entry;

    args = *(s32**)((s32)event + 0x18);
    work = *(void**)((s32)_battleWorkPointer + 0xF28);
    index = evtGetValue(event, args[0]);
    entry = (s32)work + index * 0x34 + 0x200;

    *(u8*)(entry + 0x30) = evtGetValue(event, args[1]);
    *(u8*)(entry + 0x31) = evtGetValue(event, args[2]);
    *(u8*)(entry + 0x32) = evtGetValue(event, args[3]);
    *(u8*)(entry + 0x33) = evtGetValue(event, args[4]);
    return 2;
}

s32 _lvup_disp_flag_onoff(void* event) {
    void* args = *(void**)((s32)event + 0x18);
    void* work = *(void**)((s32)_battleWorkPointer + 0xF28);
    s32 on = evtGetValue(event, *(s32*)args);
    s32 flag = *(s32*)((s32)args + 4);

    if (on) {
        *(s32*)((s32)work + 0x30) |= flag;
    } else {
        *(s32*)((s32)work + 0x30) &= ~flag;
    }
    return 2;
}

/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: _lvup_spot_on | missing_definition | ghidra_signature */
s32 _lvup_spot_on(void* event) {
    extern char str_msg_menu_mario_name__802fe738[];
    extern u32 dat_8042716c;
    extern f32 float_25_804271e0;
    extern f32 float_neg50_80427208;
    extern void* BatSpotGetPtr(void);
    u8* spot = BatSpotGetPtr();
    s32* args = *(s32**)((u8*)event + 0x18);
    u32 id;
    u8* entry;
    char* base = str_msg_menu_mario_name__802fe738;

    *(s32*)(spot + 8) = 1;
    id = evtGetValue(event, *args);
    entry = *(u8**)(spot + 4) + id * 0x2C;
    if (id == 2) {
        *(f32*)(entry + 0) = float_neg50_80427208 + float_25_804271e0 * 2.0f;
        *(u32*)(entry + 4) = *(u32*)(base + 0x1B8);
        *(u32*)(entry + 8) = *(u32*)(base + 0x1BC);
    } else if ((s32)id < 2) {
        if (id == 0) {
            *(f32*)(entry + 0) = float_neg50_80427208;
            *(u32*)(entry + 4) = *(u32*)(base + 0x1A0);
            *(u32*)(entry + 8) = *(u32*)(base + 0x1A4);
        } else if ((s32)id > -1) {
            *(f32*)(entry + 0) = float_neg50_80427208 + float_25_804271e0 * (f32)id;
            *(u32*)(entry + 4) = *(u32*)(base + 0x1AC);
            *(u32*)(entry + 8) = *(u32*)(base + 0x1B0);
        }
    } else if (id == 4) {
        *(f32*)(entry + 0) = float_neg50_80427208 + float_25_804271e0 * 4.0f;
        *(u32*)(entry + 4) = *(u32*)(base + 0x1D0);
        *(u32*)(entry + 8) = *(u32*)(base + 0x1D4);
    } else if ((s32)id < 4) {
        *(f32*)(entry + 0) = float_neg50_80427208 + float_25_804271e0 * (f32)id;
        *(u32*)(entry + 4) = *(u32*)(base + 0x1C4);
        *(u32*)(entry + 8) = *(u32*)(base + 0x1C8);
    }
    *(u32*)(entry + 0xC) = *(u32*)(base + 0x1D8);
    *(u32*)(entry + 0x10) = *(u32*)(base + 0x1DC);
    *(u32*)(entry + 0x14) = *(u32*)(base + 0x1E0);
    *(u32*)(entry + 0x18) = *(u32*)(base + 0x1E4);
    *(u32*)(entry + 0x1C) = *(u32*)(base + 0x1E8);
    *(u32*)(entry + 0x20) = *(u32*)(base + 0x1EC);
    *(u32*)(entry + 0x24) = dat_8042716c;
    *(u16*)(entry + 0x28) |= 2;
    return 2;
}

s32 _lvup_spot_move(void* event, s32 init) {
    typedef struct Vec3f_LvupSpotMove {
        f32 x;
        f32 y;
        f32 z;
    } Vec3f_LvupSpotMove;
    typedef struct BattleWorkStageSpotEntry_LvupSpotMove {
        u16 flags;
        u16 pad;
        Vec3f_LvupSpotMove translation;
        Vec3f_LvupSpotMove rotation;
        Vec3f_LvupSpotMove scale;
        u32 color;
    } BattleWorkStageSpotEntry_LvupSpotMove;
    typedef struct BattleWorkStageSpot_LvupSpotMove {
        s32 count;
        BattleWorkStageSpotEntry_LvupSpotMove* entries;
        s32 pad;
    } BattleWorkStageSpot_LvupSpotMove;

    extern BattleWorkStageSpot_LvupSpotMove* BatSpotGetPtr(void);
    extern void* BattleGetMarioPtr(void* battleWork);
    extern f32 reviseAngle(f32 angle);
    extern f32 sinfd(f32 angle);
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern f32 float_0p5_804271d0;
    extern f32 float_neg20_804271d4;
    extern f32 float_3_804271b0;
    extern f32 float_1p1_804271dc;
    extern f32 float_50_804271d8;
    extern f32 float_2p5_804271e8;
    extern f32 float_90_804271e4;
    extern f32 float_25_804271e0;
    extern f32 float_180_804271f0;
    extern f32 float_65_804271ec;
    extern f32 float_7p5_804271f4;
    extern f32 float_4_804271bc;
    extern f32 float_270_804271f8;
    extern f32 float_4p5_804271fc;
    extern f32 float_0_804271c0;
    extern f32 float_neg25_80427200;
    extern f32 float_255_804271cc;
    extern f32 float_128_80427204;
    extern u32 dat_80427170;

    BattleWorkStageSpot_LvupSpotMove* spot;
    BattleWorkStageSpotEntry_LvupSpotMove* entries;
    BattleWorkStageSpotEntry_LvupSpotMove* entry;
    void* mario;
    s32 id;
    s32 state;
    s32 timer;
    u32 color;
    f32 angle;
    f32 value;
    f32 wave;

    spot = BatSpotGetPtr();
    id = evtGetValue(event, **(s32**)((s32)event + 0x18));
    entries = spot->entries;
    entry = &entries[id];

    if (init != 0) {
        *(s32*)((s32)event + 0x78) = 0;
        *(s32*)((s32)event + 0x7C) = 0;
        *(s32*)((s32)event + 0x80) = (s32)entry->translation.x;
        *(s32*)((s32)event + 0x84) = (s32)entry->translation.y;
        *(s32*)((s32)event + 0x88) = 0;
    }

    *(s32*)((s32)event + 0x78) += 1;
    state = *(s32*)((s32)event + 0x7C);

    switch (state) {
        case 0:
            *(s32*)((s32)event + 0x78) = 1;
            *(s32*)((s32)event + 0x7C) += 1;
            /* fallthrough */
        case 1:
            switch (id) {
                case 0:
                    mario = BattleGetMarioPtr(_battleWorkPointer);
                    entry->translation.x = float_0p5_804271d0 * *(f32*)((s32)mario + 0x3C);
                    entry->translation.y = float_neg20_804271d4;
                    break;
                case 1:
                    angle = reviseAngle(float_1p1_804271dc * (float_3_804271b0 * (f32)*(s32*)((s32)event + 0x78)));
                    wave = sinfd(angle);
                    value = (f32)*(s32*)((s32)event + 0x80);
                    entry->translation.x = value + float_50_804271d8 * wave;
                    angle = reviseAngle(float_1p1_804271dc * (float_2p5_804271e8 * (f32)*(s32*)((s32)event + 0x78)) + float_90_804271e4);
                    wave = sinfd(angle);
                    value = (f32)(*(s32*)((s32)event + 0x84) + 25);
                    entry->translation.y = value - float_25_804271e0 * wave;
                    break;
                case 2:
                    angle = reviseAngle(float_1p1_804271dc * (float_2p5_804271e8 * (f32)*(s32*)((s32)event + 0x78)) + float_180_804271f0);
                    wave = sinfd(angle);
                    value = (f32)*(s32*)((s32)event + 0x80);
                    entry->translation.x = value + float_65_804271ec * wave;
                    angle = reviseAngle(float_1p1_804271dc * (float_7p5_804271f4 * (f32)*(s32*)((s32)event + 0x78)) + float_90_804271e4);
                    wave = sinfd(angle);
                    value = (f32)(*(s32*)((s32)event + 0x84) - 25);
                    entry->translation.y = value + float_25_804271e0 * wave;
                    break;
                case 3:
                    angle = reviseAngle(float_1p1_804271dc * (float_4_804271bc * (f32)*(s32*)((s32)event + 0x78)));
                    wave = sinfd(angle);
                    value = (f32)*(s32*)((s32)event + 0x80);
                    entry->translation.x = value - float_50_804271d8 * wave;
                    angle = reviseAngle(float_1p1_804271dc * (float_2p5_804271e8 * (f32)*(s32*)((s32)event + 0x78)) + float_90_804271e4);
                    wave = sinfd(angle);
                    value = (f32)(*(s32*)((s32)event + 0x84) + 25);
                    entry->translation.y = value - float_25_804271e0 * wave;
                    break;
                case 4:
                    angle = reviseAngle(float_1p1_804271dc * (float_3_804271b0 * (f32)*(s32*)((s32)event + 0x78)) + float_270_804271f8);
                    wave = sinfd(angle);
                    value = (f32)(*(s32*)((s32)event + 0x80) - 50);
                    entry->translation.x = value - float_50_804271d8 * wave;
                    angle = reviseAngle(float_1p1_804271dc * (float_4p5_804271fc * (f32)*(s32*)((s32)event + 0x78)) + float_90_804271e4);
                    wave = sinfd(angle);
                    value = (f32)(*(s32*)((s32)event + 0x84) - 25);
                    entry->translation.y = value + float_25_804271e0 * wave;
                    break;
            }
            break;
        case 2:
            *(s32*)((s32)event + 0x88) = 20;
            *(s32*)((s32)event + 0x80) = (s32)entry->translation.x;
            *(s32*)((s32)event + 0x84) = (s32)entry->translation.y;
            *(s32*)((s32)event + 0x7C) += 1;
            /* fallthrough */
        case 3:
            timer = *(s32*)((s32)event + 0x88) - 1;
            *(s32*)((s32)event + 0x88) = timer;
            entry->translation.x = intplGetValue(2, timer, 21, float_0_804271c0, (f32)*(s32*)((s32)event + 0x80));
            entry->translation.y = intplGetValue(2, timer, 21, float_neg25_80427200, (f32)*(s32*)((s32)event + 0x84));
            color = dat_80427170;
            ((u8*)&color)[3] = (s32)intplGetValue(0, timer, 21, float_255_804271cc, float_128_80427204);
            entry->color = color;
            if (*(s32*)((s32)event + 0x88) <= 0) {
                *(s32*)((s32)event + 0x7C) += 1;
            }
            break;
    }

    state = *(s32*)((s32)event + 0x7C);
    return (state >= 4) ? 2 : 0;
}

s32 _lvup_spot_off(void* event, s32 init) {
    void* spot;
    void* entry;
    s32 i;
    s32 offset;
    u32 color;
    s32 alpha;

    extern void* BatSpotGetPtr(void);
    extern u32 dat_80427174;
    extern f32 float_0_804271c0;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);

    spot = BatSpotGetPtr();
    if (init) {
        *(s32*)((s32)event + 0x78) = evtGetValue(event, **(s32**)((s32)event + 0x18));
        *(s32*)((s32)event + 0x7C) = *(s32*)((s32)event + 0x78);
        *(s32*)((s32)event + 0x80) = *(u8*)((s32)*(void**)((s32)spot + 4) + 0x2B);
    }

    *(s32*)((s32)event + 0x7C) -= 1;
    for (i = 0, offset = 0; i < 5; i++, offset += 0x2C) {
        entry = (void*)((s32)*(void**)((s32)spot + 4) + offset);
        alpha = (s32)intplGetValue(
            2,
            *(s32*)((s32)event + 0x7C),
            *(s32*)((s32)event + 0x78),
            float_0_804271c0,
            (f32)*(s32*)((s32)event + 0x80));
        color = dat_80427174;
        ((u8*)&color)[3] = alpha;
        *(u32*)((s32)entry + 0x28) = color;
        if (*(u8*)((s32)entry + 0x2B) == 0) {
            *(u16*)entry &= ~4;
        }
    }

    return (*(s32*)((s32)event + 0x7C) == 0) ? 2 : 0;
}

s32 _lvup_object_noselect_delete(void* event, s32 init) {
    void* work;
    s32 selected;
    s32 i;
    s32 offset;
    s32 alpha;

    extern f32 float_255_804271cc;
    extern f32 float_0_804271c0;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);

    work = *(void**)((s32)_battleWorkPointer + 0xF28);
    selected = *(s32*)((s32)work + 0x10);

    if (init) {
        *(s32*)((s32)event + 0x78) = 0;
        *(s32*)((s32)event + 0x80) = 0x1E;
        for (i = 0, offset = 0; i < *(s32*)((s32)work + 0x18); i++, offset += 0x34) {
            if (i != selected) {
                *(u8*)((s32)work + offset + 0x228) = 3;
            }
        }
    }

    *(s32*)((s32)event + 0x78) += 1;
    if (*(s32*)((s32)event + 0x78) < *(s32*)((s32)event + 0x80)) {
        for (i = 0, offset = 0; i < *(s32*)((s32)work + 0x18); i++, offset += 0x34) {
            if (i != selected) {
                alpha = (s32)intplGetValue(
                    5,
                    *(s32*)((s32)event + 0x78),
                    *(s32*)((s32)event + 0x80),
                    float_255_804271cc,
                    float_0_804271c0);
                *(u8*)((s32)work + offset + 0x233) = alpha;
            }
        }
        return 0;
    }

    for (i = 0, offset = 0; i < *(s32*)((s32)work + 0x18); i++, offset += 0x34) {
        if (i != selected) {
            *(u8*)((s32)work + offset + 0x233) = 0;
        }
    }
    return 2;
}

/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: _lvup_object_select_move | missing_definition | ghidra_signature */
s32 _lvup_object_select_move(void* event, s32 isFirstCall) {
    extern f32 float_0_804271c0;
    extern f32 float_2160_804271c8;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern f32 reviseAngle(f32 angle);
    void* battleWork;
    void* work;
    void* mario;
    s32 selected;
    u8* entry;
    s32 timer;
    s32 total;

    battleWork = _battleWorkPointer;
    work = *(void**)((s32)battleWork + 0xF28);
    selected = *(s32*)((s32)work + 0x10);
    mario = BattleGetMarioPtr(battleWork);
    entry = (u8*)((s32)work + selected * 0x34 + 0x200);
    if (isFirstCall != 0) {
        *(s32*)((s32)event + 0x78) = 0;
        *(s32*)((s32)event + 0x7C) = 0x3C;
        *(s32*)((s32)event + 0x80) = (s32)*(f32*)entry;
        *(s32*)((s32)event + 0x84) = (s32)*(f32*)(entry + 8);
        *(s32*)((s32)event + 0x88) = (s32)*(f32*)((s32)mario + 0x3C);
        *(s32*)((s32)event + 0x8C) = (s32)*(f32*)((s32)mario + 0x44);
    }

    *(s32*)((s32)event + 0x78) += 1;
    timer = *(s32*)((s32)event + 0x78);
    total = *(s32*)((s32)event + 0x7C);
    if (timer < total) {
        *(f32*)entry = intplGetValue(5, timer, total, (f32)*(s32*)((s32)event + 0x80), (f32)*(s32*)((s32)event + 0x88));
        *(f32*)(entry + 8) = intplGetValue(5, timer, total, (f32)*(s32*)((s32)event + 0x84), (f32)*(s32*)((s32)event + 0x8C));
        *(f32*)(entry + 0x1C) = reviseAngle(intplGetValue(5, timer, total, float_0_804271c0, float_2160_804271c8));
        return 0;
    }

    *(f32*)entry = (f32)*(s32*)((s32)event + 0x88);
    *(f32*)(entry + 8) = (f32)*(s32*)((s32)event + 0x8C);
    return 2;
}

/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: _lvup_object_decide_bound | missing_definition | ghidra_signature */
s32 _lvup_object_decide_bound(void* event, s32 isFirstCall) {
    extern f32 float_0_804271c0;
    extern f32 float_1p5_804271c4;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void* effPokopiPchargeN64Entry(s32 type, s32 duration, s32 unused, f32 x, f32 y, f32 z, f32 scale);
    void* work;
    s32 selected;
    u8* entry;
    s32 timer;
    s32 half;
    s32 startY;

    work = *(void**)((s32)_battleWorkPointer + 0xF28);
    selected = *(s32*)((s32)work + 0x10);
    entry = (u8*)((s32)work + selected * 0x34 + 0x200);
    if (isFirstCall != 0) {
        *(s32*)((s32)event + 0x78) = 0;
        *(s32*)((s32)event + 0x7C) = (s32)*(f32*)(entry + 4);
        *(s32*)((s32)event + 0x80) = 0x10;
        *(s32*)((s32)event + 0x84) = 0x1E;
        entry[0x28] = 2;
    }

    *(s32*)((s32)event + 0x78) += 1;
    timer = *(s32*)((s32)event + 0x78);
    if (timer < *(s32*)((s32)event + 0x80)) {
        half = *(s32*)((s32)event + 0x80) / 2;
        startY = *(s32*)((s32)event + 0x7C);
        if (timer < half) {
            *(f32*)(entry + 4) = (f32)startY + intplGetValue(5, timer, *(s32*)((s32)event + 0x84), float_0_804271c0, (f32)*(s32*)((s32)event + 0x84));
        } else {
            *(f32*)(entry + 4) = (f32)startY + intplGetValue(5, timer - half, *(s32*)((s32)event + 0x84), (f32)*(s32*)((s32)event + 0x84), float_0_804271c0);
        }
        return 0;
    }

    *(f32*)(entry + 4) = (f32)*(s32*)((s32)event + 0x7C);
    entry[0x28] = 4;
    effPokopiPchargeN64Entry(3, 0x78, 0, *(f32*)entry, *(f32*)(entry + 4), *(f32*)(entry + 8), float_1p5_804271c4);
    return 2;
}

/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: _lvup_select_object_init | missing_definition | ghidra_signature */
s32 _lvup_select_object_init(void) {
    extern char str_msg_menu_mario_name__802fe738[];
    extern u32 dat_80427178;
    extern u32 dat_8042717c;
    extern u32 dat_80427180;
    void* battleWork;
    u8* work;
    char* base;
    u16 hp;
    u16 fp;
    u16 bp;

    base = str_msg_menu_mario_name__802fe738;
    battleWork = _battleWorkPointer;
    work = *(u8**)((s32)battleWork + 0xF28);
    pouchGetPtr();

    *(u32*)(work + 0x200) = *(u32*)(base + 0x1F0);
    *(u32*)(work + 0x204) = *(u32*)(base + 0x1F4);
    *(u32*)(work + 0x208) = *(u32*)(base + 0x1F8);
    *(u32*)(work + 0x20C) = *(u32*)(base + 0x1FC);
    *(u32*)(work + 0x210) = *(u32*)(base + 0x200);
    *(u32*)(work + 0x214) = *(u32*)(base + 0x204);

    *(u32*)(work + 0x234) = *(u32*)(base + 0x208);
    *(u32*)(work + 0x238) = *(u32*)(base + 0x20C);
    *(u32*)(work + 0x23C) = *(u32*)(base + 0x210);
    *(u32*)(work + 0x240) = *(u32*)(base + 0x214);
    *(u32*)(work + 0x244) = *(u32*)(base + 0x218);
    *(u32*)(work + 0x248) = *(u32*)(base + 0x21C);

    *(u32*)(work + 0x268) = *(u32*)(base + 0x220);
    *(u32*)(work + 0x26C) = *(u32*)(base + 0x224);
    *(u32*)(work + 0x270) = *(u32*)(base + 0x228);
    *(u32*)(work + 0x274) = *(u32*)(base + 0x22C);
    *(u32*)(work + 0x278) = *(u32*)(base + 0x230);
    *(u32*)(work + 0x27C) = *(u32*)(base + 0x234);

    work[0x228] = 0;
    *(u32*)(work + 0x230) = dat_80427178;
    hp = *(s16*)((s32)battleWork + 0x8E);
    *(u16*)(work + 0x224) = hp;
    *(u32*)(work + 0x218) = *(u32*)(base + 0x238);
    *(u32*)(work + 0x21C) = *(u32*)(base + 0x23C);
    *(u32*)(work + 0x220) = *(u32*)(base + 0x240);
    *(u16*)(work + 0x226) = (hp >= 0xC8) ? hp : hp + 5;

    work[0x25C] = 0;
    *(u32*)(work + 0x264) = dat_8042717c;
    fp = *(s16*)((s32)battleWork + 0x90);
    *(u16*)(work + 0x258) = fp;
    *(u32*)(work + 0x24C) = *(u32*)(base + 0x244);
    *(u32*)(work + 0x250) = *(u32*)(base + 0x248);
    *(u32*)(work + 0x254) = *(u32*)(base + 0x24C);
    *(u16*)(work + 0x25A) = (fp >= 0xC8) ? fp : fp + 5;

    work[0x290] = 0;
    *(u32*)(work + 0x298) = dat_80427180;
    bp = *(s16*)((s32)battleWork + 0x94);
    *(u16*)(work + 0x28C) = bp;
    *(u32*)(work + 0x280) = *(u32*)(base + 0x250);
    *(u32*)(work + 0x284) = *(u32*)(base + 0x254);
    *(u32*)(work + 0x288) = *(u32*)(base + 0x258);
    *(u16*)(work + 0x28E) = (bp >= 0x63) ? bp : bp + 3;

    *(u32*)(work + 0x30) |= 8;
    return 2;
}

s32 _keta_get(s32 value) {
    s32 digits = 0;

    if (value < 0) {
        value *= -1;
    }
    if (value == 0) {
        return 1;
    }
    while (value > 0) {
        value /= 10;
        digits++;
    }
    return digits;
}


/* CHATGPT STUB FILL: main/battle/battle_seq_end 20260624_184128 */

/* stub-fill: _lvup_select_object_disp | prototype_only | source_prototype */
void _lvup_select_object_disp(void) {
    typedef f32 Mtx[3][4];
    typedef struct VecLocal { f32 x,y,z; } VecLocal;
    typedef struct ColorLocal { u8 r,g,b,a; } ColorLocal;
    extern void* _battleWorkPointer;
    extern void PSMTXIdentity(Mtx);
    extern void PSMTXTrans(Mtx,f64,f64,f64);
    extern void PSMTXScale(Mtx,f32,f32,f32);
    extern void PSMTXRotRad(Mtx,f64,char);
    extern void PSMTXConcat(void*,void*,void*);
    extern void btlDispTexPlane2(void*,s32,ColorLocal*);
    extern void btlDispTexPlane(f64,f64,f64,f64,f64,s32,ColorLocal*,s32);
    extern void iconNumberDispGx3D(void*,s32,s32,ColorLocal*);
    extern void iconDispGxCol(void*,s32,s32,ColorLocal*);
    extern s32 _keta_get(u32);
    u8* battle = (u8*)_battleWorkPointer;
    u8* seq = *(u8**)(battle + 0xF28);
    Mtx model, trans, scale, rx, ry, rz;
    ColorLocal color;
    s32 i;
    u8* obj;
    s32 texture;

    for (i=0; i<3; i++) {
        obj = seq + 0x34 + i*0x34;
        if (obj[0x28] == 4) { obj[0x28]=5; obj[0x29]=1; }
        if (obj[0x28] == 5) {
            obj[0x29]--;
            if (obj[0x29] == 0) { obj[0x29]=8; *(s32*)(obj+0x2C)^=1; }
        } else {
            *(s32*)(obj+0x2C)=1;
        }
    }
    for (i=0; i<3; i++) {
        obj = seq + 0x34 + i*0x34;
        if (*(s32*)(obj+0x2C)==0) continue;
        PSMTXIdentity(model);
        PSMTXTrans(trans,*(f32*)(obj+0),*(f32*)(obj+4),*(f32*)(obj+8));
        PSMTXScale(scale,*(f32*)(obj+0x18),*(f32*)(obj+0x1C),*(f32*)(obj+0x20));
        PSMTXRotRad(rx,0.017453292f**(f32*)(obj+0xC),'x');
        PSMTXRotRad(ry,0.017453292f**(f32*)(obj+0x10),'y');
        PSMTXRotRad(rz,0.017453292f**(f32*)(obj+0x14),'z');
        PSMTXConcat(rz,scale,model); PSMTXConcat(rz,model,model);
        PSMTXConcat(rx,model,model); PSMTXConcat(ry,model,model); PSMTXConcat(trans,model,model);
        color=*(ColorLocal*)(obj+0x30);
        texture=0x31+i;
        btlDispTexPlane2(model,texture,&color);
        btlDispTexPlane(*(f32*)obj-16.0f,*(f32*)(obj+4)+20.0f,*(f32*)(obj+8)+2.0f,
                        *(f32*)(obj+0x18),*(f32*)(obj+0x1C),texture,&color,0);
        if (obj[0x28]==1 && *(u16*)(obj+0x24)!=*(u16*)(obj+0x26)) {
            PSMTXTrans(trans,*(f32*)obj-14.0f,*(f32*)(obj+4)-96.0f**(f32*)(obj+0x1C)-2.0f,*(f32*)(obj+8)+5.0f);
            PSMTXScale(scale,0.8f,0.8f,1.0f); PSMTXConcat(trans,scale,trans);
            iconNumberDispGx3D(trans,*(u16*)(obj+0x24),0,&color);
            PSMTXTrans(trans,*(f32*)obj,*(f32*)(obj+4)-96.0f**(f32*)(obj+0x1C),*(f32*)(obj+8)+5.0f);
            PSMTXScale(scale,-0.6f,0.6f,1.0f); PSMTXConcat(trans,scale,trans);
            iconDispGxCol(trans,0,0x16B,&color);
        } else if (obj[0x28]==2 || obj[0x28]==5) {
            PSMTXTrans(trans,*(f32*)obj,*(f32*)(obj+4)-96.0f**(f32*)(obj+0x1C)-2.0f,*(f32*)(obj+8)+5.0f);
            iconNumberDispGx3D(trans,*(u16*)(obj+0x26),0,&color);
        }
    }
}

void _check_audience_battle_end_exec(void) {
    void* battleWork;
    void* entries;
    s32 firstLevel;
    s32 rank;
    s32 maxLevel;
    s32 sum;
    s32 count;
    s32 i;
    void* unit;

    extern void BattleAudience_Case_GreatVictory(void);
    extern void BattleAudience_Case_FastVictory(f32 value);

    battleWork = _battleWorkPointer;
    entries = *(void**)((s32)*(void**)((s32)*(void**)((s32)battleWork + 0x2738) + 0xC) + 0x18);
    firstLevel = *(u8*)((s32)*(void**)*(void**)((s32)entries + 4) + 0xE);
    if (firstLevel > *(s16*)((s32)pouchGetPtr() + 0x8A) + 1) {
        BattleAudience_Case_GreatVictory();
    }

    rank = *(s16*)battleWork;
    maxLevel = *(s16*)((s32)pouchGetPtr() + 0x8A) + 1;
    sum = 0;
    count = *(s32*)entries;
    entries = *(void**)((s32)entries + 4);
    for (i = 0; i < count; i++) {
        unit = *(void**)entries;
        entries = (void*)((s32)entries + 0x30);
        sum += *(u8*)((s32)unit + 0xE);
    }

    count = ((sum * 2) / maxLevel) / 3;
    if (rank < count) {
        BattleAudience_Case_FastVictory((f32)(count - rank));
    }
}

void _audience_Whistle_control(void) {
    void* work = *(void**)((s32)_battleWorkPointer + 0xF28);

    if (work != NULL && *(s32*)work < 0x700001C && *(s32*)work >= 0x7000007) {
        s32 timer = *(s32*)((s32)work + 0x2A0) - 1;

        *(s32*)((s32)work + 0x2A0) = timer;
        if (timer < 0) {
            *(s32*)((s32)work + 0x2A0) = irand(120) + 60;
            BattleAudienceSoundWhistle();
        }
    }
}

void _BattleMarioKouraDelete(void) {
    void* mario = BattleGetMarioPtr(_battleWorkPointer);
    void* entry = evtEntry(_koura_delete_event, 10, 0);

    *(s32*)((s32)entry + 0x160) = *(s32*)mario;
}

s32 _check_guard_koura(void* event) {
    void* battleWork;
    s32* args;
    s32 value;
    s32 out;
    void* unit;
    void* guardKoura;

    extern s32 BattleTransID(void* event, s32 id);
    extern void* BtlUnit_GetGuardKouraPtr(void* unit);

    battleWork = _battleWorkPointer;
    args = *(s32**)((s32)event + 0x18);
    value = evtGetValue(event, args[0]);
    out = args[1];
    value = BattleTransID(event, value);
    unit = BattleGetUnitPtr(battleWork, value);

    if ((*(u32*)((s32)unit + 0x104) & 0x20) == 0) {
        evtSetValue(event, out, -1);
        return 2;
    }

    guardKoura = BtlUnit_GetGuardKouraPtr(unit);
    if (guardKoura == NULL) {
        evtSetValue(event, out, -1);
        return 2;
    }

    evtSetValue(event, out, *(s32*)guardKoura);
    return 2;
}

s32 _get_rank_up_msg(void* event) {
    void* args = *(void**)((s32)event + 0x18);
    s32 out = *(s32*)args;
    RankUpData* data = _get_rank_data(*(s16*)((s32)pouchGetPtr() + 0x8A));

    evtSetValue(event, out, data->message);
    return 2;
}

