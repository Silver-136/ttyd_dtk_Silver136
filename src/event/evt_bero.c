#include "event/evt_bero.h"

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

s32 bero_get_BeroEXEC(void) {
    extern s32 BeroEXEC;
    return BeroEXEC;
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

f32 bero_get_BeroEX(void) {
    extern f32 BeroEX;
    return BeroEX;
}

f32 bero_get_BeroEY(void) {
    extern f32 BeroEY;
    return BeroEY;
}

f32 bero_get_BeroEZ(void) {
    extern f32 BeroEZ;
    return BeroEZ;
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


USER_FUNC(evt_bero_get_entername) {
    extern void* gp;

    evtSetValue(event, event->args[0], (s32)gp + 0x11C);
    return EVT_RETURN_DONE;
}
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


USER_FUNC(evt_bero_exec_get) {
    extern s32 BeroEXEC;

    evtSetValue(event, event->args[0], BeroEXEC);
    return EVT_RETURN_DONE;
}

u32 evt_bero_exec_wait(int param_1) {
    extern s32 BeroEXEC;
    EventEntry* event = (EventEntry*)param_1;
    s32 ret = EVT_RETURN_DONE;
    s32 mask = evtGetValue(event, event->args[0]);
    s32 value = BeroEXEC & mask;
    s32 result = (-value | value) >> 31;

    return ret & result;
}
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


USER_FUNC(evt_bero_set_now_number) {
    extern s32 BeroNOWNUM;

    BeroNOWNUM = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_get_now_number) {
    extern s32 BeroNOWNUM;

    evtSetValue(event, event->args[0], BeroNOWNUM);
    return EVT_RETURN_DONE;
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

s32 evt_bero_get_into_info(void* pEvt) {
    typedef struct BeroInfoLocal {
        char* hitName;
        u16 type;
        u16 pad6;
        s32 direction;
        f32 center[4];
        void* enterEvt;
    } BeroInfoLocal;
    extern s32 strcmp(const char*, const char*);
    extern u32 strlen(const char*);
    extern s32 strncmp(const char*, const char*, u32);
    extern BeroInfoLocal* BeroINFOARR[];
    extern BeroInfoLocal bero_null;
    extern s32 BeroNUM;
    extern void* gp;
    extern u8 default_entevt;
    extern char float_0_8042051c;
    static const char* directions[8] = { "n", "ne", "e", "se", "s", "sw", "w", "nw" };
    u8* event = pEvt;
    s32* lw = (s32*)(event + 0x9C);
    char* currentName = (char*)((u8*)gp + 0x11C);
    BeroInfoLocal** cursor = BeroINFOARR;
    BeroInfoLocal* info;
    BeroInfoLocal* first;
    s32 index = 0;
    s32 direction;
    s32 i;

scan:
    info = *cursor;
    if (info == 0 || strcmp(currentName, &float_0_8042051c) == 0) {
        index = 0;
        info = &bero_null;
        goto found;
    }
    if (strcmp(info->hitName, currentName) == 0) {
        goto found;
    }
    cursor++;
    index++;
    goto scan;

found:
    first = BeroINFOARR[0];
    if (((*(u32*)gp & 2) == 0) || strcmp(currentName, &float_0_8042051c) != 0) {
        lw[1] = index;
        lw[2] = BeroNUM;
        lw[3] = (s32)currentName;
        lw[4] = info->type;
        direction = info->direction;
        if (direction == 20000) {
            if ((info->type & 0xFFF) < 2) {
                direction = 0;
                for (i = 0; i < 8; i++) {
                    if (strncmp(directions[i], info->hitName, strlen(directions[i])) == 0) {
                        break;
                    }
                    direction++;
                }
            } else {
                direction = 10000;
            }
        }
        lw[5] = direction;
        lw[6] = (s32)&info->center[0];
        lw[7] = (s32)(info->enterEvt != 0 ? info->enterEvt : &default_entevt);
        lw[8] = -0x10000;
    } else {
        lw[1] = 0;
        lw[2] = BeroNUM;
        lw[3] = (s32)first->hitName;
        lw[4] = first->type;
        direction = first->direction;
        if (direction == 20000) {
            if ((first->type & 0xFFF) < 2) {
                direction = 0;
                for (i = 0; i < 8; i++) {
                    if (strncmp(directions[i], first->hitName, strlen(directions[i])) == 0) {
                        break;
                    }
                    direction++;
                }
            } else {
                direction = 10000;
            }
        }
        lw[5] = direction;
        lw[6] = (s32)&first->center[0];
        lw[7] = (s32)(first->enterEvt != 0 ? first->enterEvt : &default_entevt);
        lw[8] = -0x10000;
    }
    return 2;
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

s32 evt_bero_id_filter(int param_1) {
    extern char* bero_id_filter(char* id);
    EventEntry* event = (EventEntry*)param_1;
    s32* args = event->args;
    s32 id = evtGetValue(event, args[0]);

    evtSetValue(event, args[0], (s32)bero_id_filter((char*)id));
    return EVT_RETURN_DONE;
}

s32 evt_bero_get_info_num(void* pEvt, s32 isFirstCall) {
    typedef struct BeroInfoLocal {
        char* hitName;
        u16 type;
        u16 pad6;
        s32 direction;
        f32 center[4];
        void* enterEvt;
        s32 enterAnime;
        void* exitEvt;
        s32 exitAnime;
        s32 length;
    } BeroInfoLocal;
    extern s32 evtGetValue(void*, s32);
    extern u32 strlen(const char*);
    extern s32 strncmp(const char*, const char*, u32);
    extern BeroInfoLocal* BeroINFOARR[];
    extern s32 BeroSW[];
    extern u8 default_entevt;
    extern u8 default_outevt;
    static const char* directions[8] = { "n", "ne", "e", "se", "s", "sw", "w", "nw" };
    u8* event = pEvt;
    s32* args = *(s32**)(event + 0x18);
    s32* lw = (s32*)(event + 0x9C);
    s32 id = evtGetValue(pEvt, args[0]);
    BeroInfoLocal* info = BeroINFOARR[id];
    s32 direction = info->direction;
    s32 i;
    u16 type;

    lw[3] = (s32)info->hitName;
    lw[4] = info->type;
    if (direction == 20000) {
        if ((info->type & 0xFFF) < 2) {
            direction = 0;
            for (i = 0; i < 8; i++) {
                if (strncmp(directions[i], info->hitName, strlen(directions[i])) == 0) {
                    direction = i;
                    break;
                }
                direction = i + 1;
            }
        } else {
            direction = 10000;
        }
    }
    lw[5] = direction;
    lw[6] = (s32)&info->center[0];
    lw[7] = (s32)(info->enterEvt != 0 ? info->enterEvt : &default_entevt);
    lw[8] = -0x10000;
    lw[9] = BeroSW[id];
    lw[10] = info->enterAnime;
    if (info->enterAnime == -1) {
        type = info->type & 0xFFF;
        if (type == 3) {
            lw[10] = 7;
        } else if (type == 1) {
            lw[10] = 9;
        } else if (type == 0) {
            lw[10] = 10;
        } else if (type < 3) {
            lw[10] = 4;
        } else if (type == 0xF00) {
            lw[10] = 0;
        }
    }
    lw[11] = (s32)(info->exitEvt != 0 ? info->exitEvt : &default_outevt);
    lw[12] = -0x10000;
    lw[13] = info->exitAnime;
    lw[14] = info->length;
    return 2;
}
u8 bero_get_position_normal(void* pEvt) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern void* marioGetPtr(void);
    extern s32 evtGetValue(void* event, s32 arg);
    extern f32 evtGetFloat(void* event, s32 arg);
    extern void hitObjGetPos(char* name, Vec* out);
    extern double sin(double x);
    extern double cos(double x);
    extern s32 hitCheckFilter(f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz, s32 flags,
                              f32* outZ, f32* outY, f32* outX, f32* outDist,
                              void* outA, void* outB, void* outC);
    extern void N_marioSetBottomlessRespawnPosOnBeroEntry(f32 x, f32 y, f32 z);
    extern void N_marioReloadMapOnBottomlessOn(void);
    extern f32 BeroSX;
    extern f32 BeroSY;
    extern f32 BeroSZ;
    extern f32 BeroEX;
    extern f32 BeroEY;
    extern f32 BeroEZ;
    extern f32 float_0_804204e8;
    extern f32 float_6p2832_804204e0;
    extern f32 float_360_804204e4;
    extern f32 float_1E06_8042050c;
    extern f32 float_0p125_80420510;
    extern f32 float_180_80420514;
    extern f32 float_30_80420518;
    extern f32 float_neg1_80420508;
    void* mario = marioGetPtr();
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 type = evtGetValue(pEvt, args[0]);
    f32 length = evtGetFloat(pEvt, args[1]);
    s32* posData = *(s32**)((s32)pEvt + 0xB4);
    s32 dirIndex = *(s32*)((s32)pEvt + 0xB0);
    Vec start;
    Vec end;
    Vec base;
    Vec hitPos;
    Vec rayStart;
    f32 hitDist;
    f32 outA;
    f32 outB;
    f32 outC;
    f32 angle;
    f32 rad;
    f32 s;
    f32 c;

    start.x = (f32)posData[0];
    start.y = (f32)posData[1];
    start.z = (f32)posData[2];
    end.x = start.x;
    end.y = start.y;
    end.z = start.z;

    if ((u32)(posData[0] - 0x10000) <= 0x86A0) {
        hitObjGetPos(*(char**)((s32)pEvt + 0xA8), &start);
    }

    if (float_0_804204e8 != length) {
        hitPos.x = float_0_804204e8;
        hitPos.y = float_0_804204e8;
        hitPos.z = float_0_804204e8;
        hitDist = float_1E06_8042050c;
        outA = float_0_804204e8;
        outB = float_0_804204e8;
        outC = float_0_804204e8;

        angle = float_360_804204e4 * (f32)dirIndex * float_0p125_80420510;
        if (type == 1) {
            angle += float_180_80420514;
        }
        rad = (float_6p2832_804204e0 * angle) / float_360_804204e4;

        s = (f32)sin(rad);
        base.y = start.y;
        base.x = start.x - (float_30_80420518 * s);

        c = (f32)cos(rad);
        base.z = start.z + (float_30_80420518 * c);

        if (hitCheckFilter(base.x, base.y, base.z,
                           float_0_804204e8, float_neg1_80420508, float_0_804204e8,
                           0, &hitPos.z, &hitPos.y, &hitPos.x, &hitDist,
                           &outA, &outB, &outC) != 0) {
            base.y = hitPos.y;
        }

        rayStart.x = base.x;
        rayStart.y = base.y;
        rayStart.z = base.z;

        s = (f32)sin(rad);
        length += float_30_80420518;
        end.x = (length * s) + rayStart.x;

        c = (f32)cos(rad);
        end.z = (length * -c) + rayStart.z;
        end.y = rayStart.y;

        start.x = base.x;
        start.y = base.y;
        start.z = base.z;
    }

    BeroSX = start.x;
    BeroSY = start.y;
    BeroSZ = start.z;
    BeroEX = end.x;
    BeroEY = end.y;
    BeroEZ = end.z;
    N_marioSetBottomlessRespawnPosOnBeroEntry(end.x, end.y, end.z);
    if (*(u16*)((s32)mario + 0x2E) == 0x19) {
        N_marioReloadMapOnBottomlessOn();
    }
    return 2;
}

void bero_get_position_pipe_sub(s32 side, u32 distance, char* hitName, s32 kind,
                                f32* start, f32* end) {
    extern void hitObjGetPos(char*, f32*);
    extern s32 hitCheckFilter(f32, f32, f32, f32, f32, f32, s32,
                              f32*, f32*, f32*, f32*, void*, void*, void*);
    extern f32 float_0_804204e8;
    extern f32 float_20_804204f8;
    extern f32 float_neg1_80420508;
    extern f32 float_10000_80420504;
    f32 hitX = float_0_804204e8;
    f32 hitY = float_0_804204e8;
    f32 hitZ = float_0_804204e8;
    f32 hitDist = float_10000_80420504;
    s32 hitA;
    s32 hitB;
    s32 hitC;
    f32 offset;

    if (kind == 2 || kind == 6) {
        if (kind == 6) {
            distance = -distance;
            offset = -float_20_804204f8;
        } else {
            offset = float_20_804204f8;
        }
        if (side == 0) {
            hitObjGetPos(hitName, start);
            start[0] -= offset;
            end[0] = start[0] + (f32)(s32)distance;
            end[1] = start[1];
            end[2] = start[2];
            hitCheckFilter(end[0], end[1], end[2], float_0_804204e8,
                           float_neg1_80420508, float_0_804204e8, 0,
                           &hitZ, &hitY, &hitX, &hitDist, &hitA, &hitB, &hitC);
        } else {
            hitObjGetPos(hitName, end);
            end[0] -= offset;
            start[0] = end[0] + (f32)(s32)distance;
            start[1] = end[1];
            start[2] = end[2];
            hitCheckFilter(start[0], start[1], start[2], float_0_804204e8,
                           float_neg1_80420508, float_0_804204e8, 0,
                           &hitZ, &hitY, &hitX, &hitDist, &hitA, &hitB, &hitC);
        }
        start[1] = hitY;
        end[1] = hitY;
        return;
    }

    if (kind == 8 || kind == 9) {
        if (kind == 9) {
            distance = -distance;
        }
        if (side == 0) {
            hitObjGetPos(hitName, start);
            end[0] = start[0];
            end[1] = start[1] + (f32)(s32)distance;
            end[2] = start[2];
        } else {
            hitObjGetPos(hitName, end);
            start[0] = end[0];
            start[1] = end[1] + (f32)(s32)distance;
            start[2] = end[2];
        }
        return;
    }

    hitObjGetPos(hitName, start);
    hitObjGetPos(hitName, end);
}
s32 bero_get_position_pipe_pure(void* pEvt) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void bero_get_position_pipe_sub(s32, s32, void*, s32, void*, void*);
    extern void N_marioSetBottomlessRespawnPosOnBeroEntry(f32, f32, f32);
    extern f32 BeroSX;
    extern f32 BeroSY;
    extern f32 BeroSZ;
    extern f32 BeroEX;
    extern f32 BeroEY;
    extern f32 BeroEZ;
    extern f32 float_37p5_80420500;
    EventEntry* event;
    s32* args;
    f32 start[3];
    f32 end[3];
    s32 id;
    s32 distance;
    s32 kind;

    event = pEvt;
    args = event->args;
    id = evtGetValue(event, args[0]);
    distance = (s32)evtGetFloat(event, args[1]);
    kind = *(s32*)((u8*)event + 0xB0);
    bero_get_position_pipe_sub(id, distance, *(void**)((u8*)event + 0xA8), kind, start, end);
    BeroSX = start[0];
    if (kind == 8) {
        start[1] -= float_37p5_80420500;
    }
    BeroSZ = start[2];
    BeroEX = end[0];
    if (kind == 8) {
        end[1] -= float_37p5_80420500;
    }
    BeroEZ = end[2];
    BeroEY = end[1];
    BeroSY = start[1];
    N_marioSetBottomlessRespawnPosOnBeroEntry(end[0], end[1], end[2]);
    return 2;
}

s32 bero_get_position_pipe2(s32 param) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void bero_get_position_pipe_sub(s32, s32, void*, s32, void*, void*);
    extern void camRoadReset(void);
    extern void camShiftReset(void);
    extern void camRoadMain(f64, f64, f64, f64, f64, f64, void*);
    extern f32 BeroPT[];
    extern f32 BeroAT[];
    extern f32 float_1_804204fc;
    s32* args;
    f32 start[3];
    f32 end[3];
    f32 road[9];
    s32 id;
    s32 distance;

    args = *(s32**)(param + 0x18);
    id = evtGetValue((void*)param, args[0]);
    distance = (s32)evtGetFloat((void*)param, args[1]);
    bero_get_position_pipe_sub(id, distance, *(void**)(param + 0xA8),
                               *(s32*)(param + 0xB0), start, end);
    end[1] += float_1_804204fc;
    camRoadReset();
    camShiftReset();
    camRoadMain(end[0], end[1], end[2], end[0], end[1], end[2], road);
    BeroPT[0] = road[3];
    BeroPT[1] = road[4];
    BeroPT[2] = road[5];
    BeroAT[0] = road[6];
    BeroAT[1] = road[7];
    BeroAT[2] = road[8];
    return 2;
}

s32 bero_set_disp_position_pipe(s32 param) {
    extern void* marioGetPtr(void);
    extern f32 float_3_804204ec;
    extern f32 float_15_804204f0;
    extern f32 float_13_804204f4;
    extern f32 float_20_804204f8;
    u8* mario;
    s32 kind;
    u32 flags;

    kind = *(s32*)(param + 0xB0);
    flags = *(u32*)(param + 0xAC);
    if (kind == 2 || kind == 6) {
        if ((flags & 0x1000) == 0) {
            mario = marioGetPtr();
            *(f32*)(mario + 0x9C) += float_3_804204ec;
            mario = marioGetPtr();
            *(f32*)(mario + 0xA0) -= float_15_804204f0;
        }
    } else if (kind == 8 || kind == 9) {
        mario = marioGetPtr();
        if ((flags & 0x1000) == 0) {
            *(f32*)(mario + 0xA0) -= float_13_804204f4;
        } else {
            *(f32*)(mario + 0xA0) -= float_20_804204f8;
        }
    }
    return 2;
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

s32 evt_bero_get_number(void* pEvt) {
    extern char* bero_id_filter(char* id);
    EventEntry* event = pEvt;
    s32* args = event->args;
    s32 id = evtGetValue(event, args[0]);

    *(s32*)((s32)event + 0xA0) = (s32)bero_id_filter((char*)id);
    return EVT_RETURN_DONE;
}
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

s32 evt_bero_move_mario_speed(void* pEvt, s32 isFirstCall) {
    extern f32 evtGetFloat(void*, s32);
    extern f32 sysFrame2SecFloat(f32);
    extern s32 sysMsec2Frame(s32);
    extern f32 angleABf(f32, f32, f32, f32);
    extern f32 reviseAngle(f32);
    extern double sin(double);
    extern double cos(double);
    extern void* marioGetPtr(void);
    extern void* gp;
    extern f32 BeroSX;
    extern f32 BeroSY;
    extern f32 BeroSZ;
    extern f32 BeroEX;
    extern f32 BeroEY;
    extern f32 BeroEZ;
    extern f32 float_6p2832_804204e0;
    extern f32 float_360_804204e4;
    extern f32 float_0_804204e8;
    u8* event = pEvt;
    s32* args = *(s32**)(event + 0x18);
    u8* player;
    u64 now;
    u64 start;
    f32 step;
    f32 angle;
    f32 radians;
    f32 sinv;
    f32 cosv;
    f32 elapsed;
    f32 dx;
    f32 dy;
    s32 reachedX;
    s32 reachedY;

    step = sysFrame2SecFloat(evtGetFloat(pEvt, args[0]));
    if (*(s32*)((u8*)gp + 0x14) == 0) {
        now = *(u64*)((u8*)gp + 0x40);
    } else {
        now = *(u64*)((u8*)gp + 0x38);
    }
    if (isFirstCall != 0) {
        *(u64*)(event + 0x198) = now;
    }
    start = *(u64*)(event + 0x198);

    angle = reviseAngle(angleABf(BeroSX, BeroSY, BeroEX, BeroEY));
    radians = (float_6p2832_804204e0 * angle) / float_360_804204e4;
    sinv = (f32)sin((double)radians);
    cosv = (f32)cos((double)radians);
    elapsed = (f32)sysMsec2Frame((s32)((now - start) / (*(u32*)0x800000F8 / 4000)));
    dx = step * sinv;
    dy = -(step * cosv);

    player = marioGetPtr();
    *(f32*)(player + 0x8C) = BeroSX + sinv * elapsed;
    player = marioGetPtr();
    *(f32*)(player + 0x90) = BeroSY - cosv * elapsed;
    player = marioGetPtr();
    *(f32*)(player + 0x94) = BeroSZ;

    reachedX = 0;
    if ((dx >= float_0_804204e8 && *(f32*)(player + 0x8C) >= BeroEX) ||
        (dx < float_0_804204e8 && *(f32*)(player + 0x8C) <= BeroEX)) {
        reachedX = 1;
    }
    reachedY = 0;
    if ((dy >= float_0_804204e8 && *(f32*)(player + 0x90) >= BeroEY) ||
        (dy < float_0_804204e8 && *(f32*)(player + 0x90) <= BeroEY)) {
        reachedY = 1;
    }
    if (reachedX + reachedY == 2) {
        player = marioGetPtr();
        *(f32*)(player + 0x8C) = BeroEX;
        player = marioGetPtr();
        *(f32*)(player + 0x90) = BeroEY;
        player = marioGetPtr();
        *(f32*)(player + 0x94) = BeroEZ;
        return 2;
    }
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


USER_FUNC(evt_bero_cam_shift_init) {
    extern void camRoadReset(void);
    extern void camShiftReset(void);

    camRoadReset();
    camShiftReset();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_mario_go_init) {
    extern s32 BeroMarioGO;

    BeroMarioGO = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_mario_go) {
    extern s32 BeroMarioGO;

    BeroMarioGO = 1;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_mario_go_wait) {
    extern s32 BeroMarioGO;

    return BeroMarioGO ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}
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


s32 evt_camera_change_event_from_road(void) {
    extern void* marioGetPtr(void);
    extern void* camGetPtr(s32);
    extern void camShiftMain(void*, void*, void*);
    extern void camRoadMain(f64, f64, f64, f64, f64, f64, void*);
    extern void* gp;
    u8 shiftWork[28];
    f32 road[9];
    u8* player;
    u8* cam;
    f32 x;
    f32 y;
    f32 z;

    player = marioGetPtr();
    cam = camGetPtr(4);
    camShiftMain(cam, player, shiftWork);
    x = *(f32*)(player + 0x8C);
    y = *(f32*)(cam + 0x94);
    z = *(f32*)(player + 0x94);
    camRoadMain(x, y, z, x, y, z, road);

    *(u32*)(cam + 0x58) = *(u32*)(cam + 0x0C);
    *(u32*)(cam + 0x5C) = *(u32*)(cam + 0x10);
    *(u32*)(cam + 0x60) = *(u32*)(cam + 0x14);
    *(u32*)(cam + 0x64) = *(u32*)(cam + 0x18);
    *(u32*)(cam + 0x68) = *(u32*)(cam + 0x1C);
    *(u32*)(cam + 0x6C) = *(u32*)(cam + 0x20);
    *(f32*)(cam + 0x40) = road[3];
    *(f32*)(cam + 0x44) = road[4];
    *(f32*)(cam + 0x48) = road[5];
    *(f32*)(cam + 0x4C) = road[6];
    *(f32*)(cam + 0x50) = road[7];
    *(f32*)(cam + 0x54) = road[8];
    *(u32*)(cam + 0x70) = *(u32*)((u8*)gp + 0x38);
    *(u32*)(cam + 0x74) = *(u32*)((u8*)gp + 0x3C);
    *(u32*)(cam + 0x78) = 0;
    *(u32*)(cam + 0x7C) = (*(volatile u32*)0x800000F8 / 4000) * 500;
    *(u16*)(cam + 4) = 3;
    *(u8*)(cam + 0x80) = 11;
    return 2;
}

