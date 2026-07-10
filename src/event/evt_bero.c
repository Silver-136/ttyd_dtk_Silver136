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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_bero_cam3d_change(int param_1) {
    extern void* gp;
    extern f32 BeroPT[];
    extern f32 BeroAT[];
    extern void* camGetPtr(s32 camId);
    EventEntry* event = (EventEntry*)param_1;
    s32* args = event->args;
    s32 time = evtGetValue(event, args[0]);
    s32 type = evtGetValue(event, args[1]);
    void* cam = camGetPtr(4);

    *(u32*)((s32)cam + 0x58) = *(u32*)((s32)cam + 0x0C);
    *(u32*)((s32)cam + 0x5C) = *(u32*)((s32)cam + 0x10);
    *(u32*)((s32)cam + 0x60) = *(u32*)((s32)cam + 0x14);
    *(u32*)((s32)cam + 0x64) = *(u32*)((s32)cam + 0x18);
    *(u32*)((s32)cam + 0x68) = *(u32*)((s32)cam + 0x1C);
    *(u32*)((s32)cam + 0x6C) = *(u32*)((s32)cam + 0x20);

    *(u32*)((s32)cam + 0x40) = *(u32*)&BeroPT[0];
    *(u32*)((s32)cam + 0x44) = *(u32*)&BeroPT[1];
    *(u32*)((s32)cam + 0x48) = *(u32*)&BeroPT[2];
    *(u32*)((s32)cam + 0x4C) = *(u32*)&BeroAT[0];
    *(u32*)((s32)cam + 0x50) = *(u32*)&BeroAT[1];
    *(u32*)((s32)cam + 0x54) = *(u32*)&BeroAT[2];

    *(u32*)((s32)cam + 0x70) = *(u32*)((s32)gp + 0x38);
    *(u32*)((s32)cam + 0x74) = *(u32*)((s32)gp + 0x3C);
    *(u32*)((s32)cam + 0x78) = 0;
    *(u32*)((s32)cam + 0x7C) = time * ((*(u32*)0x800000F8) / 4000);
    *(u16*)((s32)cam + 0x04) = 3;
    *(u8*)((s32)cam + 0x80) = type;
    return EVT_RETURN_DONE;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_bero_switch_off(void* pEvt) {
    extern char* bero_id_filter(char* id);
    extern s32 BeroSW[];
    EventEntry* event = pEvt;
    s32* args = event->args;
    s32 id = evtGetValue(event, args[0]);

    id = (s32)bero_id_filter((char*)id);
    if (BeroSW[id] != 1) {
        *(s32*)((s32)event + 0x9C) = 1;
    } else {
        *(s32*)((s32)event + 0x9C) = 0;
    }
    BeroSW[id] = 0;
    *(s32*)((s32)event + 0xA0) = id;
    return EVT_RETURN_DONE;
}

s32 evt_bero_switch_on(void* pEvt) {
    extern char* bero_id_filter(char* id);
    extern s32 BeroSW[];
    EventEntry* event = pEvt;
    s32* args = event->args;
    s32 id = evtGetValue(event, args[0]);

    id = (s32)bero_id_filter((char*)id);
    if (BeroSW[id] != 0) {
        *(s32*)((s32)event + 0x9C) = 1;
    } else {
        *(s32*)((s32)event + 0x9C) = 0;
    }
    BeroSW[id] = 1;
    *(s32*)((s32)event + 0xA0) = id;
    return EVT_RETURN_DONE;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_bero_get_info_length(void* pEvt) {
    typedef struct BeroInfo {
        u8 pad0[4];
        u16 kind;
        u16 dir;
        u8 pad8[0x10];
        s32 length;
    } BeroInfo;
    extern BeroInfo* BeroINFOARR[];
    extern f32 float_30_80420518;
    extern f32 float_60_80420520;
    EventEntry* event = pEvt;
    s32* args = event->args;
    BeroInfo* info = BeroINFOARR[evtGetValue(event, args[0])];
    f32 value;

    if (info->length != -1) {
        evtSetFloat(event, args[1], (f32)info->length);
        return EVT_RETURN_DONE;
    }
    switch (info->kind & 0xFFF) {
        case 0:
        case 3:
        case 0xF00:
            value = float_30_80420518;
            break;
        case 1:
        case 2:
        default:
            value = float_60_80420520;
            break;
    }
    evtSetFloat(event, args[1], value);
    return EVT_RETURN_DONE;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 bero_get_position_pipe_pure(void* pEvt) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_bero_overwrite(void* pEvt) {
    extern s32 strcmp(const char* a, const char* b);
    extern void* BeroINFOARR[];
    EventEntry* event = (EventEntry*)pEvt;
    s32* args = event->args;
    void** info = BeroINFOARR;
    s32 id = evtGetValue(event, args[0]);
    s32 original = id;
    s32 value;

    if (id < 0 || id >= 16) {
        if (&id != NULL) {
            id = 0;
        }
        while (*(char**)*info != NULL && strcmp(*(char**)*info, (char*)original) != 0) {
            if (&id != NULL) {
                id++;
            }
            info++;
        }
    }
    value = evtGetValue(event, args[1]);
    BeroINFOARR[id] = (void*)value;
    return EVT_RETURN_DONE;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 bero_set_disp_position_pipe(int param_1) {
    return 0;
}


s32 evt_bero_id_filter(int param_1) {
    extern char* bero_id_filter(char* id);
    EventEntry* event = (EventEntry*)param_1;
    s32* args = event->args;
    s32 id = evtGetValue(event, args[0]);

    evtSetValue(event, args[0], (s32)bero_id_filter((char*)id));
    return EVT_RETURN_DONE;
}

s32 evt_bero_get_number(void* pEvt) {
    extern char* bero_id_filter(char* id);
    EventEntry* event = pEvt;
    s32* args = event->args;
    s32 id = evtGetValue(event, args[0]);

    *(s32*)((s32)event + 0xA0) = (s32)bero_id_filter((char*)id);
    return EVT_RETURN_DONE;
}

u32 evt_bero_read_mario_pera(int param_1, int param_2) {
    extern BOOL animGroupBaseAsync(const char* name, int a, int b);
    extern const char str_p_dokan_x_802c3060[];
    extern const char str_p_dokan_y_802c306c[];
    EventEntry* event = (EventEntry*)param_1;
    s32 state;

    if (param_2 != 0) {
        *(s32*)((s32)event + 0x78) = 0;
    }
    switch (*(s32*)((s32)event + 0x78)) {
        case 0:
            if (animGroupBaseAsync(str_p_dokan_x_802c3060, 0, 0)) {
                *(s32*)((s32)event + 0x78) = 1;
            }
            break;
        case 1:
            if (animGroupBaseAsync(str_p_dokan_y_802c306c, 0, 0)) {
                *(s32*)((s32)event + 0x78) = 100;
            }
            break;
    }
    state = *(s32*)((s32)event + 0x78);
    return ((~((state - 100) | (100 - state))) >> 31) & EVT_RETURN_DONE;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 N_evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry(EventEntry* event) {
    extern void N_marioSetBottomlessRespawnPosOnBeroEntry(f32 x, f32 y, f32 z);
    s32* args = event->args;
    s32 x = evtGetValue(event, args[0]);
    s32 y = evtGetValue(event, args[1]);
    s32 z = evtGetValue(event, args[2]);

    N_marioSetBottomlessRespawnPosOnBeroEntry((f32)x, (f32)y, (f32)z);
    return EVT_RETURN_DONE;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

char* bero_id_filter(char* param_1) {
    extern s32 strcmp(const char* a, const char* b);
    extern void* BeroINFOARR[];
    s32 id = (s32)param_1;
    void** info = BeroINFOARR;

    if (id < 0 || id >= 16) {
        if (&id != NULL) {
            id = 0;
        }
        while (*(char**)*info != NULL && strcmp(*(char**)*info, (char*)param_1) != 0) {
            if (&id != NULL) {
                id++;
            }
            info++;
        }
    }
    return (char*)id;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_bero_get_info_anime(EventEntry* event) {
    typedef struct BeroInfo {
        u8 pad0[0x30];
        u16 anime0;
        u16 anime1;
        s32 anime2;
        s32 anime3;
    } BeroInfo;
    extern BeroInfo* BeroINFOARR[];
    BeroInfo* info;
    s32* args = event->args;

    info = BeroINFOARR[evtGetValue(event, args[0])];
    evtSetValue(event, args[1], info->anime0);
    evtSetValue(event, args[2], info->anime1);
    evtSetValue(event, args[3], info->anime2);
    evtSetValue(event, args[4], info->anime3);
    return EVT_RETURN_DONE;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_bero_get_info_kinddir(EventEntry* event) {
    typedef struct BeroInfo {
        u8 pad0[4];
        u16 kind;
        u16 dir;
        s32 value;
    } BeroInfo;
    extern BeroInfo* BeroINFOARR[];
    BeroInfo* info;
    s32* args = event->args;

    info = BeroINFOARR[evtGetValue(event, args[0])];
    evtSetValue(event, args[1], info->kind);
    evtSetValue(event, args[2], info->dir);
    evtSetValue(event, args[3], info->value);
    return EVT_RETURN_DONE;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_bero_mapchange(void* pEvt, int param_2) {
    extern void seqSetSeq(s32 seq, s32 area, s32 map);
    EventEntry* event = (EventEntry*)pEvt;
    s32* args = event->args;
    s32 area = evtGetValue(event, args[0]);
    s32 map = evtGetValue(event, args[1]);

    if (param_2 != 0) {
        seqSetSeq(3, area, map);
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_bero_case_id_save(void* pEvt) {
    extern s32 BeroID[];
    EventEntry* event = (EventEntry*)pEvt;
    s32* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);

    BeroID[index] = value;
    return EVT_RETURN_DONE;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_bero_case_id_load(void* pEvt) {
    extern s32 BeroID[];
    EventEntry* event = (EventEntry*)pEvt;
    s32* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], BeroID[index]);
    return EVT_RETURN_DONE;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u32 evt_bero_exec_wait(int param_1) {
    extern s32 BeroEXEC;
    EventEntry* event = (EventEntry*)param_1;
    s32 ret = EVT_RETURN_DONE;
    s32 mask = evtGetValue(event, event->args[0]);
    s32 value = BeroEXEC & mask;
    s32 result = (-value | value) >> 31;

    return ret & result;
}


s32 evt_bero_get_info(void* evt, s32 isFirstCall) {
    typedef struct BeroInfo {
        const char* name;
        u8 data[0x38];
    } BeroInfo;
    extern BeroInfo* BeroINFOARR[];
    extern s32 BeroSW[];
    extern s32 BeroNUM;
    extern s32 BeroEXEC;
    extern void* hitNameToPtr(const char* name);
    extern void mapErrorEntry(s32);
    EventEntry* event = evt;
    BeroInfo* info;
    BeroInfo** infoArr = BeroINFOARR;
    s32* sw = BeroSW;
    s32 count = 0;
    int i;

    for (i = 0; i < 16; i++) {
        infoArr[i] = NULL;
    }
    info = *(BeroInfo**)((s32)event + 0x9C);
    while (info->name != NULL) {
        if (hitNameToPtr(info->name) == NULL) {
            mapErrorEntry(1);
        }
        info++;
    }
    info = *(BeroInfo**)((s32)event + 0x9C);
    while (info->name != NULL) {
        *infoArr = info;
        infoArr++;
        count++;
        info++;
        *sw = -1;
        sw++;
    }
    BeroNUM = count;
    BeroEXEC = 0;
    return EVT_RETURN_DONE;
}

s32 evt_bero_get_info_num(void* evt, s32 isFirstCall) {
    return 0;
}
