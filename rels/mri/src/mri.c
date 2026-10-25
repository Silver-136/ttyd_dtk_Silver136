#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mariost.h"
#include "dolphin/os.h"
#include "system.h"
#include "dolphin/gx.h"
#include "evt/evt_npc.h"
#include "driver/npcdrv.h"
#include "dolphin/mtx.h"
#include "pmario_sound.h"
#include "stdlib.h"
#include "mri.h"
#include "mri_puni.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];
void mapObjTranslate(const char* name, f32 x, f32 y, f32 z);
void mapGrpSetColor(const char* name, GXColor color);
typedef struct HSV { u8 h; u8 s; u8 v; u8 a; } HSV;
GXColor HSV2RGB(HSV hsv);
PuniData* puniGetPunidat(void);
void puniMoveDecel(PuniData* puni);
PuniWork* puniGetPunigp(void);
void PuniDokuritsu(PuniData* puni);
void puniModeWinjump(PuniData* puni);

USER_FUNC(evt_bero_exec_get);
USER_FUNC(evt_bero_exec_onoff);
USER_FUNC(evt_bero_mario_go);
USER_FUNC(evt_cam_get_pos);
USER_FUNC(evt_cam_shake);
USER_FUNC(evt_eff64);
USER_FUNC(evt_fade_entry);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_flag_on);
USER_FUNC(evt_item_get_position);
USER_FUNC(evt_item_set_position);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_mario_get_dir);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mri_door_naname_func);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_toge);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_pouch_add_item);
USER_FUNC(evt_pouch_check_item);
USER_FUNC(evt_pouch_remove_item);
USER_FUNC(evt_puni_all_save);
USER_FUNC(evt_puni_flag_onoff);
USER_FUNC(evt_puni_get_lsw_index);
USER_FUNC(evt_puni_get_mode);
USER_FUNC(evt_puni_get_trap_index);
USER_FUNC(evt_puni_init_puni);
USER_FUNC(evt_puni_set_lsw_index);
USER_FUNC(evt_puni_set_position);
USER_FUNC(evt_puni_set_trap_index);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_seq_set_seq);
USER_FUNC(evt_snd_bgmoff_f);
USER_FUNC(evt_snd_envoff_f);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_countdown_get_status);
USER_FUNC(evt_sub_countdown_set_restart_time);
USER_FUNC(evt_sub_countdown_start);
USER_FUNC(evt_sub_get_mapname);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_init);
USER_FUNC(evt_target_flag_onoff);
USER_FUNC(evt_win_item_select);
USER_FUNC(hakari_color);
void _prolog(void);
void _epilog(void);
void _unresolved(void);
USER_FUNC(evt_mri_get16bitdata);
USER_FUNC(mri_npc_buruburu);
BOOL toge_searchParent(PuniData* self, s16 group);
void togeModeParent(PuniData* puni);
void togeMoveParent(PuniData* puni);
void puniModeWinjump(PuniData* puni);
void togeMakeRetsu(Vec* pos, s16 leader, s16 start, s16 end);
USER_FUNC(evt_mri_toge_make_retsu);
USER_FUNC(evt_mri_toge_retsu_stop);
USER_FUNC(evt_mri_toge_enter_init);
USER_FUNC(evt_mri_puni_win_init);
USER_FUNC(evt_mri_puni_win_attack);
USER_FUNC(evt_mri_puni_win_end);

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("mri_00", mri_00_init_evt);
    relSetEvtAddr("mri_01", mri_01_init_evt);
    relSetEvtAddr("mri_02", mri_02_init_evt);
    relSetEvtAddr("mri_03", mri_03_init_evt);
    relSetEvtAddr("mri_04", mri_04_init_evt);
    relSetEvtAddr("mri_05", mri_05_init_evt);
    relSetEvtAddr("mri_06", mri_06_init_evt);
    relSetEvtAddr("mri_07", mri_07_init_evt);
    relSetEvtAddr("mri_08", mri_08_init_evt);
    relSetEvtAddr("mri_09", mri_09_init_evt);
    relSetEvtAddr("mri_10", mri_10_init_evt);
    relSetEvtAddr("mri_11", mri_11_init_evt);
    relSetEvtAddr("mri_12", mri_12_init_evt);
    relSetEvtAddr("mri_13", mri_13_init_evt);
    relSetEvtAddr("mri_14", mri_14_init_evt);
    relSetEvtAddr("mri_15", mri_15_init_evt);
    relSetEvtAddr("mri_16", mri_16_init_evt);
    relSetEvtAddr("mri_17", mri_17_init_evt);
    relSetEvtAddr("mri_18", mri_18_init_evt);
    relSetEvtAddr("mri_19", mri_19_init_evt);
    relSetEvtAddr("mri_20", mri_20_init_evt);
    relSetBtlAddr("mri", btlsetup_mri_tbl, btl_mri_setup_no_tbl);
}

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}

void _unresolved(void) {
}

EvtWord evt_mri_door_open_pure[95];
EvtWord evt_mri_door_open[14];
EvtWord evt_mri_door_close_pure[104];
EvtWord evt_mri_door_close[10];
EvtWord evt_mri_door_naname_open_pure[52];
EvtWord evt_mri_door_naname_open[10];
EvtWord evt_mri_door_naname_close_pure[61];
EvtWord evt_mri_door_naname_close[10];
EvtWord _ueki_anime[64];
EvtWord mri_ueki_entry[32];
EvtWord mri_puni_save[6];
s32 mri_itemsel_table[2];
EvtWord mri_punipuni_setup[16];
EvtWord mri_punipuni_itemselect[40];
EvtWord mri_punipuni_itemset[36];
EvtWord mri_punipuni_itemremove[29];
EvtWord mri_hikari_color[66];
#ifdef VERSION_G8ME01
EvtWord mri_countdown[176];
#else
EvtWord mri_countdown[174];
#endif
EvtWord mri_waterfall_effect[26];
EvtWord mri_guide_hagure_check[50];
EvtWord mri_guide_goryu[63];
EvtWord mri_puni_walk_se[20];
EvtWord mri_puni_jump_se[15];

EVT_BEGIN(evt_mri_door_open_pure)
    IF_INT_EQUAL(LW(9), 0)
        SET_VALUE(LW(2), -5)
        ELSE
        SET_VALUE(LW(2), 5)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(10), LW(11), LW(12), LW(13))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_ELEC2_1"), LW(11), LW(12), LW(13), 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 11, 0, LW(2), 200)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, LW(10), LW(0), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(10), LW(11), LW(12), LW(13))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_ELEC2_2"), LW(11), LW(12), LW(13), 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 11, 0, -60, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, LW(10), LW(2), 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_mri_door_open)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_bero_mario_go)
        CALL(evt_bero_exec_onoff, 0, 268435456)
    END_INLINE_EVENT
    CALL_CHILD(PTR(evt_mri_door_open_pure))
    RETURN
EVT_END()

EVT_BEGIN(evt_mri_door_close_pure)
    IF_INT_EQUAL(LW(9), 0)
        SET_VALUE(LW(2), -5)
        ELSE
        SET_VALUE(LW(2), 5)
    END_IF
    CALL(evt_mapobj_trans, 1, LW(10), LW(2), 0, -60)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(10), LW(11), LW(12), LW(13))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_ELEC2"), LW(11), LW(12), LW(13), 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 11, -60, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, LW(10), LW(2), 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(10), LW(11), LW(12), LW(13))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_ELEC2_1"), LW(11), LW(12), LW(13), 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 11, LW(2), 0, 200)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, LW(10), LW(0), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_mri_door_close)
    BEGIN_INLINE_EVENT
#ifdef VERSION_G8ME01
        WAIT_MS(600)
#else
        WAIT_MS(200)
#endif
        CALL(evt_bero_mario_go)
    END_INLINE_EVENT
    CALL_CHILD(PTR(evt_mri_door_close_pure))
    RETURN
EVT_END()

EVT_BEGIN(evt_mri_door_naname_open_pure)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(10), LW(11), LW(12), LW(13))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_ELEC2_1"), LW(11), LW(12), LW(13), 0)
    END_INLINE_EVENT
    CALL(evt_mri_door_naname_func, LW(10), FLOAT(0.0f), FLOAT(0.0f), FLOAT(6.5f), FLOAT(-6.5f), 200, 11)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(10), LW(11), LW(12), LW(13))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_ELEC2_2"), LW(11), LW(12), LW(13), 0)
    END_INLINE_EVENT
    CALL(evt_mri_door_naname_func, LW(10), FLOAT(6.5f), FLOAT(-6.5f), FLOAT(-34.5f), FLOAT(-46.5f), 500, 11)
    RETURN
EVT_END()

EVT_BEGIN(evt_mri_door_naname_open)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_bero_mario_go)
    END_INLINE_EVENT
    CALL_CHILD(PTR(evt_mri_door_naname_open_pure))
    RETURN
EVT_END()

EVT_BEGIN(evt_mri_door_naname_close_pure)
    CALL(evt_mapobj_trans, 1, LW(10), FLOAT(-34.5f), FLOAT(0.0f), FLOAT(-46.5f))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(10), LW(11), LW(12), LW(13))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_ELEC2"), LW(11), LW(12), LW(13), 0)
    END_INLINE_EVENT
    CALL(evt_mri_door_naname_func, LW(10), FLOAT(-34.5f), FLOAT(-46.5f), FLOAT(6.5f), FLOAT(-6.5f), 500, 11)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(10), LW(11), LW(12), LW(13))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_ELEC2_1"), LW(11), LW(12), LW(13), 0)
    END_INLINE_EVENT
    CALL(evt_mri_door_naname_func, LW(10), FLOAT(6.5f), FLOAT(-6.5f), FLOAT(0.0f), FLOAT(0.0f), 200, 11)
    RETURN
EVT_END()

EVT_BEGIN(evt_mri_door_naname_close)
    BEGIN_INLINE_EVENT
#ifdef VERSION_G8ME01
        WAIT_MS(600)
#else
        WAIT_MS(200)
#endif
        CALL(evt_bero_mario_go)
    END_INLINE_EVENT
    CALL_CHILD(PTR(evt_mri_door_naname_close_pure))
    RETURN
EVT_END()

USER_FUNC(evt_mri_door_naname_func) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x0 = evtGetFloat(event, args[1]);
    f32 z0 = evtGetFloat(event, args[2]);
    f32 x1 = evtGetFloat(event, args[3]);
    f32 z1 = evtGetFloat(event, args[4]);
    s32 frames = evtGetValue(event, args[5]);
    s32 mode = evtGetValue(event, args[6]);
    s32 elapsed;

    if (isFirstCall) {
        event->savedTime = gp->renderTime;
    }
    elapsed = OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime);
    if (elapsed < frames) {
        f32 x;
        f32 z;
        isFirstCall = elapsed;
        x = intplGetValue(mode, isFirstCall, frames, x0, x1);
        z = intplGetValue(mode, isFirstCall, frames, z0, z1);
        mapObjTranslate(name, x, 0.0f, z);
        return EVT_RETURN_BLOCK;
    }
    mapObjTranslate(name, x1, 0.0f, z1);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(_ueki_anime)
    CALL(evt_mapobj_get_position, LW(2), LW(4), LW(5), LW(6))
    CALL(evt_snd_sfxon_3d, STRING("SFX_GRASS_SHAKE1"), LW(4), LW(5), LW(6), 0)
    CALL(evt_mapobj_trans, 1, LW(2), 2, 0, 0)
    WAIT_MS(60)
    CALL(evt_mapobj_trans, 1, LW(2), -2, 0, 0)
    WAIT_MS(60)
    CALL(evt_mapobj_trans, 1, LW(2), 1, 0, 0)
    WAIT_MS(60)
    CALL(evt_mapobj_trans, 1, LW(2), -1, 0, 0)
    WAIT_MS(60)
    CALL(evt_mapobj_trans, 1, LW(2), 0, 0, 0)
    IF_INT_NOT_EQUAL(LW(10), 0)
        SPAWN_EVENT(LW(10))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(mri_ueki_entry)
    SET_INT_READ_ADDR(LW(0))
    LOOP(0)
        READ_INT3(LW(1), LW(2), LW(10))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_run_case_evt, 9, 1, LW(1), 0, PTR(_ueki_anime), 0)
        CALL(evt_run_case_evt, 13, 1, LW(1), 0, PTR(_ueki_anime), 0)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mri_puni_save)
    DEBUG_PUT_MSG(STRING("プニの状態をセーブします"))
    CALL(evt_puni_all_save)
    RETURN
EVT_END()

s32 mri_itemsel_table[] = {
    64, -1,
};

EVT_BEGIN(mri_punipuni_setup)
    CALL(evt_item_entry, LW(10), 64, 0, -20000, 0, 16, -1, 0)
    CALL(evt_item_flag_on, LW(10), 2)
    RETURN
EVT_END()

EVT_BEGIN(mri_punipuni_itemselect)
    SET_VALUE(LW(1), 0)
    CALL(evt_pouch_check_item, 64, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("sys_no_item_to_put_on"), 0, 0)
        RETURN
    END_IF
    CALL(evt_win_item_select, 0, PTR(mri_itemsel_table), LW(0), 0)
    IF_INT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    SET_VALUE(LW(1), 1)
    RETURN
EVT_END()

EVT_BEGIN(mri_punipuni_itemset)
    CALL(evt_target_flag_onoff, LW(11), 0, 3)
    CALL(evt_pouch_remove_item, 64, LW(0))
    CALL(evt_item_get_position, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNIDAMA_SET1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("sys_stg2_punitama_on"), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(mri_punipuni_itemremove)
    CALL(evt_target_flag_onoff, LW(11), 1, 1)
    CALL(evt_pouch_add_item, 64, LW(0))
    CALL(evt_item_set_position, LW(10), 0, -20000, 0)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("sys_stg2_punitama_off"), 0, 0)
    RETURN
EVT_END()

USER_FUNC(hakari_color) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 now = evtGetValue(event, args[1]);
    s32 max = evtGetValue(event, args[2]);

    if (now <= max) {
        mapGrpSetColor(name, HSV2RGB((HSV){gp->retraceCount, 0xFF, 0xFF, 0xFF}));
    } else {
        GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
        mapGrpSetColor(name, white);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mri_get16bitdata) {
    EvtWord* args = event->args;
    s16* table = (s16*)evtGetValue(event, args[0]);
    s32 index = evtGetValue(event, args[1]);
    evtSetValue(event, args[2], table[index]);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(mri_hikari_color)
    CALL(evt_mapobj_flag_onoff, 0, 0, LW(0), 256)
    GET_INT_INDEXED(LW(1), LW(11))
    LOOP(0)
        GET_INT_INDEXED(LW(1), LW(10))
        IF_INT_LESS_OR_EQUAL(LW(10), LW(2))
            IF_INT_GREATER(LW(11), LW(2))
                CALL(evt_mapobj_get_position, LW(0), LW(4), LW(5), LW(6))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNISWITCH_RIDE1"), LW(4), LW(5), LW(6), 0)
            END_IF
            ELSE
            IF_INT_LESS_OR_EQUAL(LW(11), LW(2))
                CALL(evt_mapobj_get_position, LW(0), LW(4), LW(5), LW(6))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNISWITCH_RIDE1"), LW(4), LW(5), LW(6), 0)
            END_IF
        END_IF
        SET_VALUE(LW(11), LW(10))
        CALL(hakari_color, LW(0), LW(10), LW(2))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

USER_FUNC(mri_npc_buruburu) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    NpcEntry* npc = evtNpcNameToPtr(event, name);
    s32 axis = evtGetValue(event, args[1]);
    f32 amp = evtGetFloat(event, args[2]);
    s32 period = evtGetValue(event, args[3]);
    s32 duration = evtGetValue(event, args[4]);
    f32* work = event->userdataf;
    s32 elapsed;
    f32 v;
    f32 delta;

    if (isFirstCall) {
        event->savedTime = gp->renderTime;
        work[0] = 0.0f;
        work[1] = npc->position.x;
        work[2] = npc->position.y;
        work[3] = npc->position.z;
    }
    elapsed = OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime);
    if (elapsed % (period * 2) >= period) {
        v = amp;
    } else {
        v = -amp;
    }
    delta = v - work[0];
    switch (axis) {
        case 0:
            npc->position.x += delta;
            npc->prevPosition.x = npc->position.x;
            break;
        case 1:
            npc->position.y += delta;
            npc->prevPosition.y = npc->position.y;
            break;
        case 2:
            npc->position.z += delta;
            npc->prevPosition.z = npc->position.z;
            break;
    }
    work[0] = v;
    if (duration == 0) {
        return EVT_RETURN_BLOCK;
    }
    if (elapsed < duration) {
        return EVT_RETURN_BLOCK;
    }
    {
        Vec pos = {0.0f, 0.0f, 0.0f};
        pos.x = work[1];
        pos.y = work[2];
        pos.z = work[3];
        npc->prevPosition = pos;
        npc->position = npc->prevPosition;
    }
    return EVT_RETURN_YIELD;
}

EVT_BEGIN(mri_countdown)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 118)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(2880), 1)
        CALL(evt_sub_countdown_start, 300000, 300000)
        SET_VALUE(GSWF(2880), 0)
    END_IF
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("mri_13"))
        CALL(evt_sub_countdown_set_restart_time, 300000)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("mri_06"))
        CALL(evt_sub_countdown_set_restart_time, 250000)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("mri_01"))
        CALL(evt_sub_countdown_set_restart_time, 100000)
    END_IF
#ifdef VERSION_G8ME01
    WAIT_FRAMES(5)
#endif
    LOOP(0)
        CALL(evt_sub_countdown_get_status, LW(0))
        CALL(evt_bero_exec_get, LW(1))
        IF_INT_EQUAL(LW(0), 1)
            IF_BITS_CLEAR(LW(1), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SUSPEND_TYPES_OTHER(4)
    CALL(evt_mario_key_onoff, 0)
    WAIT_FRAMES(5)
    IF_BITS_SET(LW(1), 1)
        CALL(evt_mario_key_onoff, 1)
        RESUME_TYPES_OTHER(4)
        RETURN
    END_IF
    SET_VALUE(GSW_STORY_PROGRESS, 127)
    CALL(evt_snd_bgmoff_f, 512, 500)
    CALL(evt_snd_envoff_f, 512, 500)
    CALL(evt_snd_sfxon, STRING("SFX_STG2_QUAKE1"), 0)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 500)
    CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 500)
    CALL(evt_cam_shake, 4, FLOAT(0.0146484375f), FLOAT(0.0f), 500)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.01953125f), FLOAT(0.0f), 10000)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(1), 150)
        CALL(evt_cam_get_pos, 4, LW(10), LW(11), LW(12))
        LOOP(0)
            CALL(evt_eff64, STRING(""), STRING("hokori_n64"), 0, LW(10), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(300)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(2000)
    CALL(evt_fade_entry, 17, 4000, 0, 0, 0)
    WAIT_MS(4000)
    CALL(evt_seq_set_seq, 5, 1, 0)
    RETURN
EVT_END()

EVT_BEGIN(mri_waterfall_effect)
    ADD(LW(1), -10)
    LOOP(0)
        CALL(evt_eff64, STRING(""), STRING("water_damage_n64"), 15, LW(0), LW(1), LW(2), LW(3), 18, 0, 0, 0, 0, 0, 0)
        WAIT_MS(150)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mri_guide_hagure_check)
    CALL(evt_puni_get_lsw_index, LSW(0), 0, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_puni_get_trap_index, 1, LW(4))
    SET_VALUE(LW(1), 0)
    IF_INT_NOT_EQUAL(LW(4), 0)
        SET_VALUE(LW(1), 3)
        DEBUG_PUT_MSG(STRING("ガイド・トラップにはまっている"))
    END_IF
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(1), 1)
        DEBUG_PUT_MSG(STRING("ガイド・別のエリアにはぐれている"))
    END_IF
    CALL(evt_puni_get_mode, 1, LW(0))
    IF_INT_EQUAL(LW(0), 16)
        SET_VALUE(LW(1), 2)
        DEBUG_PUT_MSG(STRING("ガイド・同じエリアではぐれている"))
    END_IF
    DEBUG_PUT_MSG(STRING("ガイド・普通の状態"))
    RETURN
EVT_END()

EVT_BEGIN(mri_guide_goryu)
    CALL_CHILD(PTR(mri_guide_hagure_check))
    IF_INT_EQUAL(LW(1), 0)
        RETURN
    END_IF
    CALL(evt_puni_set_lsw_index, LSW(0), 0, 0, 0, 0, 0)
    CALL(evt_puni_set_trap_index, 1, 0)
    CALL(evt_puni_init_puni, 1, 30, 0, -1000, 0, 0)
    CALL(evt_puni_flag_onoff, 1, 1, 65536)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_get_dir, LW(3))
    ADD(LW(2), -10)
    IF_INT_LESS_OR_EQUAL(LW(0), 180)
        ADD(LW(0), -10)
        ELSE
        ADD(LW(0), 10)
    END_IF
    CALL(evt_puni_set_position, 1, LW(0), LW(1), LW(2))
    RETURN
EVT_END()

EVT_BEGIN(mri_puni_walk_se)
    LOOP(0)
        CALL(evt_npc_get_position, LW(0), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_MOVE2_1"), LW(10), LW(11), LW(12), 0)
        WAIT_FRAMES(5)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mri_puni_jump_se)
    CALL(evt_npc_get_position, LW(0), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    RETURN
EVT_END()

BOOL toge_searchParent(PuniData* self, s16 group) {
    s32 i;
    s32 order = 10000;
    PuniData* parent;

    for (i = 110; i <= 209; i++) {
        PuniData* p = &puniGetPunidat()[i];
        if ((p->flags & 0x400) && (p->flags & 0x80000) && p->groupId == group && p->child1 == NULL && p != self &&
            (s16)order > p->depth) {
            parent = p;
            order = p->depth + 1;
        }
    }
    if ((s16)order == 10000) {
        return FALSE;
    }
    self->parent = parent;
    self->depth = order;
    self->groupId = parent->groupId;
    parent->child1 = self;
    return TRUE;
}

void togeModeParent(PuniData* puni) {
    Vec dir;

    PSVECSubtract(&puni->parent->pos, &puni->pos, &dir);
    puni->parentDist = PSVECMag(&dir);
    if (puni->parentDist > 3.0f + (2.0f + (puni->radius.x + puni->parent->radius.x))) {
        PSVECNormalize(&dir, &dir);
        dir.y = 0.0f;
        PSVECScale(&dir, &dir, 1.0f);
        PSVECAdd(&puni->vel, &dir, &puni->vel);
        puni->maxSpeed = 3.0f;
    }
}

void togeMoveParent(PuniData* puni) {
    puniMoveDecel(puni);
}

void puniModeWinjump(PuniData* puni) {
    s32 elapsed;
    f32 t;

    switch (puni->modeStep) {
        case 0:
            puni->startTime = gp->renderTime;
            puni->modeVar1 = 0;
            puni->modeF0 = puni->pos.y;
            puni->modeF1 = -72.0f;
            puni->modeF2 = -puni->modeF1;
            puni->modeStep = 1;
            psndSFXOn_3D("SFX_STG2_PUNI_JUMP1", &puni->pos);
        case 1:
            elapsed = OSTicksToMilliseconds((u32)gp->renderTime - (u32)puni->startTime);
            if (elapsed > 100 && puni->modeVar1 == 0) {
                puni->modeVar1 = 1;
                puni->animTime = 0;
            }
            if (elapsed < 200) {
                t = elapsed / 200.0f;
                puni->pos.y = puni->modeF0 + t * (puni->modeF1 * t + puni->modeF2);
            } else {
                puni->pos.y = puni->modeF0;
                puni->mode = 0x1B;
                puni->anim = 1;
                puni->modeStep = 0;
            }
            break;
    }
}

void togeMakeRetsu(Vec* pos, s16 leader, s16 start, s16 end) {
    s32 i;
    PuniData* puni = &puniGetPunidat()[leader];

    puni->pos.x = pos->x;
    puni->pos.y = pos->y;
    puni->pos.z = pos->z;
    puni->modeFunc = NULL;
    puni->moveFunc = NULL;
    puni->groupId = puni->index;
    puni->anim = 1;
    puni->flags |= 0x80000;
    for (i = start; i <= end; i++) {
        puni = &puniGetPunidat()[i];
        puni->modeFunc = togeModeParent;
        puni->moveFunc = togeMoveParent;
        puni->pos.x = pos->x;
        puni->pos.y = pos->y;
        puni->pos.z = pos->z;
        puni->anim = 1;
        puni->flags |= 0x80000;
        toge_searchParent(puni, leader);
    }
}

USER_FUNC(evt_mri_toge_make_retsu) {
    EvtWord* args = event->args;
    Vec pos;
    s16 leader;
    s16 start;
    s16 end;

    pos.x = evtGetValue(event, args[0]);
    pos.y = evtGetValue(event, args[1]);
    pos.z = evtGetValue(event, args[2]);
    leader = evtGetValue(event, args[3]);
    start = evtGetValue(event, args[4]);
    end = evtGetValue(event, args[5]);
    togeMakeRetsu(&pos, leader, start, end);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mri_toge_retsu_stop) {
    EvtWord* args = event->args;
    s16 leader = evtGetValue(event, args[0]);
    s16 start = evtGetValue(event, args[1]);
    s16 end = evtGetValue(event, args[2]);
    s32 i;
    PuniData* puni = &puniGetPunidat()[leader];

    puni->modeFunc = NULL;
    puni->moveFunc = NULL;
    puni->anim = 0;
    puni->flags &= ~0x80000;
    for (i = start; i <= end; i++) {
        puni = &puniGetPunidat()[i];
        puni->modeFunc = NULL;
        puni->moveFunc = NULL;
        puni->anim = 0;
        puni->flags &= ~0x80000;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mri_toge_enter_init) {
    s32 i;

    for (i = 110; i <= 209; i++) {
        PuniData* puni = &puniGetPunidat()[i];
        if (puni->flags & 0x400) {
            puni->pos.x = 0.0f;
            puni->pos.y = -1000.0f;
            puni->pos.z = 0.0f;
            puni->mode = 0x1D;
            puni->anim = 0;
            puni->flags &= ~0x100000;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mri_puni_win_init) {
    s32 i;

    for (i = 1; i <= 102; i++) {
        PuniData* puni = &puniGetPunidat()[i];
        if (puni->flags & 0x400) {
            puni->hp = puni->maxHp;
            puni->rotOffsetY = 0.0f;
            puni->spin = 0.0f;
            puni->dispOfs.y = 0.0f;
            puni->flags2 &= ~1;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mri_puni_win_attack) {
    EvtWord* args = event->args;
    f32 x0 = evtGetValue(event, args[0]);
    f32 y0 = evtGetValue(event, args[1]);
    f32 z0 = evtGetValue(event, args[2]);
    f32 x1 = evtGetValue(event, args[3]);
    f32 y1 = evtGetValue(event, args[4]);
    f32 z1 = evtGetValue(event, args[5]);
    PuniWork* wp = puniGetPunigp();
    s32 i;

    wp->beroMode = 2;
    wp->beroStart.x = x0;
    wp->beroStart.y = y0;
    wp->beroStart.z = z0;
    wp->beroStartRange = 100.0f;
    wp->beroEnd.x = x1;
    wp->beroEnd.y = y1;
    wp->beroEnd.z = z1;
    wp->beroEndRange = 100.0f;
    for (i = 1; i <= 102; i++) {
        PuniData* puni = &puniGetPunidat()[i];
        if (puni->flags & 0x400) {
            PuniDokuritsu(puni);
            puni->anim = 1;
            puni->modeStep = 0;
            if (puni->flags & 0x4000) {
                puni->flags &= ~0x4000;
                puni->mode = 0x1D;
                puni->modeFunc = puniModeWinjump;
                puni->moveFunc = puniMoveDecel;
            } else {
                puni->mode = 0x1B;
            }
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mri_puni_win_end) {
    EvtWord* args = event->args;
    f32 x0 = evtGetValue(event, args[0]);
    f32 y0 = evtGetValue(event, args[1]);
    f32 z0 = evtGetValue(event, args[2]);
    f32 x1 = evtGetValue(event, args[3]);
    f32 y1 = evtGetValue(event, args[4]);
    f32 z1 = evtGetValue(event, args[5]);
    PuniWork* wp = puniGetPunigp();
    s32 i;

    wp->allMode = 0;
    for (i = 1; i <= 102; i++) {
        PuniData* puni = &puniGetPunidat()[i];
        if (puni->flags & 0x400) {
            f32 rx;
            f32 rz;
            PuniDokuritsu(puni);
            rx = rand() % 21;
            rz = rand() % 21;
            puni->pos.x = x0 + rx - 10.0f;
            puni->pos.y = y0;
            puni->pos.z = z0 + rz - 10.0f;
            puni->moveStart.x = x1 + rx - 10.0f;
            puni->moveStart.y = y1;
            puni->moveStart.z = z1 + rz - 10.0f;
            puni->mode = 0x19;
            puni->modeStep = 0;
        }
    }
    return EVT_RETURN_DONE;
}
