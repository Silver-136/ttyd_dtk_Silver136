#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "win.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

USER_FUNC(evt_bero_mario_go);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_init);
void _unresolved(void);
void _epilog(void);
void _prolog(void);

EvtWord evt_win_door_open_pure[84];
EvtWord evt_win_door_open[16];
EvtWord evt_win_door_close_pure[66];
EvtWord evt_win_door_close[41];
EvtWord _tree_anime[106];
EvtWord win_tree_entry[25];
EvtWord win_puni_walk_se[20];

EVT_BEGIN(evt_win_door_open_pure)
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_mapobj_get_position, LW(8), LW(10), LW(11), LW(12))
            CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_WOOD3"), LW(10), LW(11), LW(12), 0)
            ELSE
            CALL(evt_mapobj_get_position, LW(8), LW(10), LW(11), LW(12))
            CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_WOOD2"), LW(10), LW(11), LW(12), 0)
        END_IF
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_sub_intpl_msec_init, 11, 0, -70, LW(1))
        ELSE
        CALL(evt_sub_intpl_msec_init, 11, -70, 0, LW(1))
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(2), 0)
        SUBTRACT(LW(2), LW(0))
        CALL(evt_mapobj_rotate, 1, LW(8), 0, LW(2), 0)
        CALL(evt_mapobj_rotate, 1, LW(9), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_win_door_open)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_bero_mario_go)
    END_INLINE_EVENT
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 800)
    CALL_CHILD(PTR(evt_win_door_open_pure))
    RETURN
EVT_END()

EVT_BEGIN(evt_win_door_close_pure)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(8), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_WOOD2"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_sub_intpl_msec_init, 11, 70, 0, LW(1))
        ELSE
        CALL(evt_sub_intpl_msec_init, 11, 0, 70, LW(1))
    END_IF
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(2), 0)
        SUBTRACT(LW(2), LW(0))
        CALL(evt_mapobj_rotate, 1, LW(8), 0, LW(2), 0)
        CALL(evt_mapobj_rotate, 1, LW(9), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_win_door_close)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_bero_mario_go)
    END_INLINE_EVENT
    SET_VALUE(LW(0), 70)
    SET_VALUE(LW(2), 0)
    SUBTRACT(LW(2), LW(0))
    CALL(evt_mapobj_rotate, 1, LW(8), 0, LW(2), 0)
    CALL(evt_mapobj_rotate, 1, LW(9), 0, LW(0), 0)
    WAIT_MS(500)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 800)
    CALL_CHILD(PTR(evt_win_door_close_pure))
    RETURN
EVT_END()

EVT_BEGIN(_tree_anime)
    CALL(evt_mapobj_get_position, LW(2), LW(4), LW(5), LW(6))
    CALL(evt_snd_sfxon_3d, STRING("SFX_HIT_TREE1"), LW(4), LW(5), LW(6), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_HIT_TREE1_LEAVES1"), LW(4), LW(5), LW(6), 0)
    CALL(evt_mapobj_trans, 1, LW(2), 1, 0, 0)
    CALL(evt_mapobj_trans, 1, LW(3), 4, 0, 0)
    WAIT_MS(60)
    CALL(evt_mapobj_trans, 1, LW(2), -1, 0, 0)
    CALL(evt_mapobj_trans, 1, LW(3), -4, 0, 0)
    WAIT_MS(60)
    CALL(evt_mapobj_trans, 1, LW(2), 1, 0, 0)
    CALL(evt_mapobj_trans, 1, LW(3), 2, 0, 0)
    WAIT_MS(60)
    CALL(evt_mapobj_trans, 1, LW(2), 0, 0, 0)
    CALL(evt_mapobj_trans, 1, LW(3), -2, 0, 0)
    WAIT_MS(60)
    CALL(evt_mapobj_trans, 1, LW(2), 0, 0, 0)
    CALL(evt_mapobj_trans, 1, LW(3), 0, 0, 0)
    IF_INT_NOT_EQUAL(LW(10), 0)
        SPAWN_EVENT(LW(10))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(win_tree_entry)
    SET_INT_READ_ADDR(LW(0))
    LOOP(0)
        READ_INT4(LW(1), LW(2), LW(3), LW(10))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_run_case_evt, 13, 1, LW(1), 0, PTR(_tree_anime), 0)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(win_puni_walk_se)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("ガイド"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_MOVE2_1"), LW(0), LW(1), LW(2), 0)
        WAIT_FRAMES(5)
    END_LOOP
    RETURN
EVT_END()

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("win_00", win_00_init_evt);
    relSetEvtAddr("win_01", win_01_init_evt);
    relSetEvtAddr("win_02", win_02_init_evt);
    relSetEvtAddr("win_03", win_03_init_evt);
    relSetEvtAddr("win_04", win_04_init_evt);
    relSetEvtAddr("win_05", win_05_init_evt);
    relSetEvtAddr("win_06", win_06_init_evt);
    relSetBtlAddr("win", btlsetup_win_tbl, btl_win_setup_no_tbl);
}

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}
void _unresolved(void) {
}
