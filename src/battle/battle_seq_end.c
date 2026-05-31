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
