#include "event/evt_bero.h"

f32 bero_get_BeroEX(void) {
    extern f32 BeroEX;
    return BeroEX;
}

s32 bero_get_BeroEXEC(void) {
    extern s32 BeroEXEC;
    return BeroEXEC;
}

f32 bero_get_BeroEY(void) {
    extern f32 BeroEY;
    return BeroEY;
}

f32 bero_get_BeroEZ(void) {
    extern f32 BeroEZ;
    return BeroEZ;
}

s32 bero_get_BeroNUM(void) {
    extern s32 BeroNUM;
    return BeroNUM;
}

f32 bero_get_BeroSX(void) {
    extern f32 BeroSX;
    return BeroSX;
}

f32 bero_get_BeroSY(void) {
    extern f32 BeroSY;
    return BeroSY;
}

f32 bero_get_BeroSZ(void) {
    extern f32 BeroSZ;
    return BeroSZ;
}

void* bero_get_ptr(void) {
    extern void* BeroINFOARR[];
    return BeroINFOARR;
}

void bero_clear_Offset(void) {
    extern f32 BeroOX;
    extern f32 BeroOY;
    extern f32 BeroOZ;
    extern f32 float_0_804204e8;
    f32 zero = float_0_804204e8;

    BeroOX = zero;
    BeroOY = zero;
    BeroOZ = zero;
}

USER_FUNC(evt_bero_mario_go) {
    extern s32 BeroMarioGO;

    BeroMarioGO = 1;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_mario_go_init) {
    extern s32 BeroMarioGO;

    BeroMarioGO = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_cam_shift_init) {
    extern void camRoadReset(void);
    extern void camShiftReset(void);

    camRoadReset();
    camShiftReset();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_exec_get) {
    extern s32 BeroEXEC;

    evtSetValue(event, event->args[0], BeroEXEC);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_get_now_number) {
    extern s32 BeroNOWNUM;

    evtSetValue(event, event->args[0], BeroNOWNUM);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_set_now_number) {
    extern s32 BeroNOWNUM;

    BeroNOWNUM = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos) {
    extern void* marioGetPtr(void);
    extern void N_marioSetBottomlessRespawnPosOnBeroEntry(f32 x, f32 y, f32 z);
    void* mario = marioGetPtr();

    N_marioSetBottomlessRespawnPosOnBeroEntry(
        *(f32*)((s32)mario + 0x8C),
        *(f32*)((s32)mario + 0x90),
        *(f32*)((s32)mario + 0x94));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_get_entername) {
    extern void* gp;

    evtSetValue(event, event->args[0], (s32)gp + 0x11C);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_disppos_save) {
    extern void* marioGetPtr(void);
    extern f32 BeroOX;
    extern f32 BeroOY;
    extern f32 BeroOZ;

    BeroOX = *(f32*)((s32)marioGetPtr() + 0x98);
    BeroOY = *(f32*)((s32)marioGetPtr() + 0x9C);
    BeroOZ = *(f32*)((s32)marioGetPtr() + 0xA0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_disppos_load) {
    extern void* marioGetPtr(void);
    extern f32 BeroOX;
    extern f32 BeroOY;
    extern f32 BeroOZ;
    f32 value;

    value = BeroOX;
    *(f32*)((s32)marioGetPtr() + 0x98) = value;
    value = BeroOY;
    *(f32*)((s32)marioGetPtr() + 0x9C) = value;
    value = BeroOZ;
    *(f32*)((s32)marioGetPtr() + 0xA0) = value;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_mario_go_wait) {
    extern s32 BeroMarioGO;

    return BeroMarioGO ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

USER_FUNC(evt_bero_get_end_position) {
    extern f32 BeroEX;
    extern f32 BeroEY;
    extern f32 BeroEZ;
    s32* args = event->args;

    evtSetFloat(event, args[0], BeroEX);
    evtSetFloat(event, args[1], BeroEY);
    evtSetFloat(event, args[2], BeroEZ);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_get_start_position) {
    extern f32 BeroSX;
    extern f32 BeroSY;
    extern f32 BeroSZ;
    s32* args = event->args;

    evtSetFloat(event, args[0], BeroSX);
    evtSetFloat(event, args[1], BeroSY);
    evtSetFloat(event, args[2], BeroSZ);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_exec_onoff) {
    extern s32 BeroEXEC;
    s32* args = event->args;
    s32 clear = evtGetValue(event, args[0]);
    s32 flags = evtGetValue(event, args[1]);

    if (clear != 0) {
        BeroEXEC &= ~flags;
    } else {
        BeroEXEC |= flags;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_1stcheck) {
    extern s32 strcmp(const char* a, const char* b);
    extern const char str_sys_01_802c304c[7];
    s32* args = event->args;

    if (strcmp(*(const char**)((s32)event + 0xD0), str_sys_01_802c304c) == 0) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_get_info_nextarea) {
    typedef struct BeroInfo {
        u8 pad[0x28];
        s32 nextArea;
        s32 nextMap;
    } BeroInfo;
    extern BeroInfo* BeroINFOARR[];
    BeroInfo* info;
    s32* args = event->args;

    info = BeroINFOARR[evtGetValue(event, args[0])];
    evtSetValue(event, args[1], info->nextArea);
    evtSetValue(event, args[2], info->nextMap);
    return EVT_RETURN_DONE;
}


u8 bero_get_position_normal(void) {
    return 0;
}


s32 evt_bero_get_into_info(void* pEvt) {
    return 0;
}


u8 bero_get_position_pipe_sub(void) {
    return 0;
}


u8 evt_bero_move_mario_speed(void) {
    return 0;
}


s32 evt_camera_change_event_from_road(void) {
    return 0;
}


s32 bero_get_position_pipe2(int param_1) {
    return 0;
}


s32 evt_bero_cam3d_change(int param_1) {
    return 0;
}


s32 evt_bero_switch_off(void* pEvt) {
    return 0;
}


s32 evt_bero_switch_on(void* pEvt) {
    return 0;
}


s32 evt_bero_get_info_length(void* pEvt) {
    return 0;
}


s32 bero_get_position_pipe_pure(void* pEvt) {
    return 0;
}


s32 evt_bero_overwrite(void* pEvt) {
    return 0;
}


s32 bero_set_disp_position_pipe(int param_1) {
    return 0;
}


s32 evt_bero_id_filter(int param_1) {
    return 0;
}


s32 evt_bero_get_number(void* pEvt) {
    return 0;
}


u32 evt_bero_read_mario_pera(int param_1, int param_2) {
    return 0;
}


s32 N_evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry(void* pEvt) {
    return 0;
}


char* bero_id_filter(char* param_1) {
    return 0;
}


s32 evt_bero_get_info_anime(int param_1) {
    return 0;
}


s32 evt_bero_get_info_kinddir(void* pEvt) {
    return 0;
}


s32 evt_bero_mapchange(void* pEvt, int param_2) {
    return 0;
}


s32 evt_bero_case_id_save(void* pEvt) {
    return 0;
}


s32 evt_bero_case_id_load(void* pEvt) {
    return 0;
}


u32 evt_bero_exec_wait(int param_1) {
    return 0;
}


s32 evt_bero_get_info(void* evt, s32 isFirstCall) {
    return 0;
}


s32 evt_bero_get_info_num(void* evt, s32 isFirstCall) {
    return 0;
}
