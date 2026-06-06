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

void btlseqEnd_DispInit(void* unit) {
    *(u32*)((s32)*(void**)((s32)unit + 0xF28) + 0x30) = 0;
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

s32 N__set_initial_upgrade_cursor_position(void* event) {
    s32 value = evtGetValue(event, **(s32***)((s32)event + 0x18));
    void* work = *(void**)((s32)_battleWorkPointer + 0xF28);

    *(s32*)((s32)work + 0x10) = value;
    return 2;
}

s32 N__display_select_one_to_upgrade_text(void) {
    dispEntry(8, 1, N__draw_select_one_to_upgrade_text, _battleWorkPointer, 900.0f);
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

s32 _set_confetti_control_evtid(void* event) {
    s32 value = **(s32**)((s32)event + 0x18);
    void* battleWork = _battleWorkPointer;
    void* work = *(void**)((s32)battleWork + 0xF28);

    *(s32*)((s32)work + 0x2A4) = evtGetValue(event, value);
    return 2;
}

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

s32 _lvup_ap_recover_full(void) {
    void* audience = BattleAudienceBaseGetPtr();

    *(s32*)((s32)audience + 0x13910) = 1;
    pouchSetAP(pouchGetMaxAP());
    return 2;
}

void _GetExpIcon_Disp(void) {
    u32 color = dat_80427158;

    windowDispGX_Waku_col(0, &color, -76.9f, 21.0f, 165.8f, 24.2f, 10.0f);
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

void _BattleMarioKouraDelete(void) {
    void* mario = BattleGetMarioPtr(_battleWorkPointer);
    void* entry = evtEntry(_koura_delete_event, 10, 0);

    *(s32*)((s32)entry + 0x160) = *(s32*)mario;
}

s32 _get_rank_up_msg(void* event) {
    void* args = *(void**)((s32)event + 0x18);
    s32 out = *(s32*)args;
    RankUpData* data = _get_rank_data(*(s16*)((s32)pouchGetPtr() + 0x8A));

    evtSetValue(event, out, data->message);
    return 2;
}

void _GetExpIcon_End(void* work) {
    s32 i;

    *(u32*)((s32)work + 0x30) &= ~2;
    for (i = 0; i < 19; i++) {
        iconDelete((void*)((s32)*(void**)((s32)work + i * 4 + 0x34) + 0x18));
    }
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
