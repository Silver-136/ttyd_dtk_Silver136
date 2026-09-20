#include "event/evt_bero.h"
#include "cam_road.h"
#include "cam_shift.h"

typedef struct RoadVec {
    f32 x;
    f32 y;
    f32 z;
} RoadVec;
extern void* marioGetPtr(void);
extern void* gp;
extern f32 BeroSZ;
extern f32 BeroEY;
extern f32 BeroEZ;
extern f32 BeroEX;
extern s32 BeroEXEC;
extern f32 BeroSY;
extern f32 BeroSX;
extern s32 BeroSW[];
extern char* bero_id_filter(char* id);
extern s32 BeroMarioGO;
extern f32 BeroOX;
extern s32 BeroNUM;
extern f32 BeroOZ;
extern f32 BeroOY;
extern s32 BeroID[];
extern f32 BeroAT[];
extern void camRoadMain(f64, f64, f64, f64, f64, f64, void*);
extern f32 BeroPT[];
extern s32 BeroNOWNUM;
extern s32 default_entevt[];
extern const char* const lbl_802C2C28[8];


extern s32 strncmp(const char*, const char*, u32);
extern u32 strlen(const char*);
extern void N_marioSetBottomlessRespawnPosOnBeroEntry(f32 x, f32 y, f32 z);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 N_evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry(EventEntry* event) {
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
    void* mario = marioGetPtr();

    N_marioSetBottomlessRespawnPosOnBeroEntry(
        *(f32*)((s32)mario + 0x8C),
        *(f32*)((s32)mario + 0x90),
        *(f32*)((s32)mario + 0x94));
    return EVT_RETURN_DONE;
}

s32 bero_get_BeroEXEC(void) {
    return BeroEXEC;
}

s32 bero_get_BeroNUM(void) {
    return BeroNUM;
}

f32 bero_get_BeroSX(void) {
    return BeroSX;
}

f32 bero_get_BeroSY(void) {
    return BeroSY;
}

f32 bero_get_BeroSZ(void) {
    return BeroSZ;
}

f32 bero_get_BeroEX(void) {
    return BeroEX;
}

f32 bero_get_BeroEY(void) {
    return BeroEY;
}

f32 bero_get_BeroEZ(void) {
    return BeroEZ;
}

void* bero_get_ptr(void) {
    extern void* BeroINFOARR[];
    return BeroINFOARR;
}

void bero_clear_Offset(void) {
    extern f32 float_0_804204e8;
    f32 zero = float_0_804204e8;

    BeroOX = zero;
    BeroOY = zero;
    BeroOZ = zero;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
char* bero_id_filter(char* param_1) {
    extern s32 strcmp(const char* a, const char* b);
    extern void* BeroINFOARR[];
    s32 id = (s32)param_1;
    void** info = BeroINFOARR;

    if ((s32)param_1 < 0 || (s32)param_1 >= 16) {
        s32 original = (s32)param_1;
        if (&id != NULL) {
            id = 0;
        }
    scan:
        if (*(char**)*info == NULL) {
            goto done;
        }
        if (strcmp(*(char**)*info, (char*)original) == 0) {
            goto done;
        }
        {
            info++;
            if (&id != NULL) {
                id++;
            }
        }
        goto scan;
    }
done:
    return (char*)id;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

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

    evtSetValue(event, event->args[0], (s32)gp + 0x11C);
    return EVT_RETURN_DONE;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_bero_exec_onoff) {
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

    evtSetValue(event, event->args[0], BeroEXEC);
    return EVT_RETURN_DONE;
}

u32 evt_bero_exec_wait(int param_1) {
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
    extern f32 float_30_80420518;
    typedef struct BeroInfo {
        u8 pad0[4];
        u16 kind;
        u16 dir;
        u8 pad8[0x10];
        s32 length;
    } BeroInfo;
    extern BeroInfo* BeroINFOARR[];
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

    BeroNOWNUM = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_get_now_number) {

    evtSetValue(event, event->args[0], BeroNOWNUM);
    return EVT_RETURN_DONE;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_bero_get_info(void* evt, s32 isFirstCall) {
    typedef struct BeroInfo {
        const char* name;
        u8 data[0x38];
    } BeroInfo;
    extern BeroInfo* BeroINFOARR[];
    extern void* hitNameToPtr(const char* name);
    extern void mapErrorEntry(s32);
    BeroInfo** infoArr = BeroINFOARR;
    EventEntry* event = evt;
    BeroInfo* info;
    s32* sw;
    s32 count;

    infoArr[0] = NULL;
    infoArr[1] = NULL;
    infoArr[2] = NULL;
    infoArr[3] = NULL;
    infoArr[4] = NULL;
    infoArr[5] = NULL;
    infoArr[6] = NULL;
    infoArr[7] = NULL;
    infoArr[8] = NULL;
    infoArr[9] = NULL;
    infoArr[10] = NULL;
    infoArr[11] = NULL;
    infoArr[12] = NULL;
    infoArr[13] = NULL;
    infoArr[14] = NULL;
    infoArr[15] = NULL;

    info = *(BeroInfo**)((s32)event + 0x9C);
    for (;;) {
        void* hit = hitNameToPtr(info->name);
        if (info->name == NULL) {
            break;
        }
        if (hit == NULL) {
            mapErrorEntry(1);
        }
        info++;
    }

    sw = BeroSW;
    info = *(BeroInfo**)((s32)event + 0x9C);
    count = 0;
fill:
    if (info->name != NULL) {
        *infoArr++ = info;
        count++;
        info++;
        *sw++ = -1;
        goto fill;
    }
    BeroNUM = count;
    BeroEXEC = 0;
    return EVT_RETURN_DONE;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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
    extern BeroInfoLocal* BeroINFOARR[];
    extern BeroInfoLocal bero_null;
    extern f32 float_0_8042051c;
    u8* event = pEvt;
    s32* lw = (s32*)(event + 0x9C);
    BeroInfoLocal** cursor = BeroINFOARR;
    BeroInfoLocal* info;
    BeroInfoLocal* first;
    s32 index = 0;
    s32 direction;
    s32 i;
    const char** name;
    const char* namesA[8];
    const char* namesB[8];

scan:
    info = *cursor;
    if (info == 0 || strcmp((char*)((u8*)gp + 0x11C), (char*)&float_0_8042051c) == 0) {
        index = 0;
        info = &bero_null;
        goto found;
    }
    if (strcmp(info->hitName, (char*)((u8*)gp + 0x11C)) == 0) {
        goto found;
    }
    cursor++;
    index++;
    goto scan;

found:
    first = BeroINFOARR[0];
    if (((*(u32*)gp & 2) == 0) || strcmp((char*)((u8*)gp + 0x11C), (char*)&float_0_8042051c) != 0) {
        lw[1] = index;
        lw[2] = BeroNUM;
        lw[3] = (s32)((u8*)gp + 0x11C);
        lw[4] = info->type;
        direction = info->direction;
        namesA[0] = lbl_802C2C28[0];
        namesA[1] = lbl_802C2C28[1];
        namesA[2] = lbl_802C2C28[2];
        namesA[3] = lbl_802C2C28[3];
        namesA[4] = lbl_802C2C28[4];
        namesA[5] = lbl_802C2C28[5];
        namesA[6] = lbl_802C2C28[6];
        namesA[7] = lbl_802C2C28[7];
        if (direction == 20000) {
            if (direction == 10000) {
                direction = 10000;
            } else if ((info->type & 0xFFF) < 2) {
                direction = 0;
                name = namesA;
                do {
                    if (strncmp(*name, info->hitName, strlen(*name)) == 0) {
                        break;
                    }
                    direction++;
                    name++;
                } while (direction < 8);
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
        namesB[0] = lbl_802C2C28[0];
        namesB[1] = lbl_802C2C28[1];
        namesB[2] = lbl_802C2C28[2];
        namesB[3] = lbl_802C2C28[3];
        namesB[4] = lbl_802C2C28[4];
        namesB[5] = lbl_802C2C28[5];
        namesB[6] = lbl_802C2C28[6];
        namesB[7] = lbl_802C2C28[7];
        if (direction == 20000) {
            if (direction == 10000) {
                direction = 10000;
            } else if ((first->type & 0xFFF) < 2) {
                direction = 0;
                for (i = 0; i < 8; i++) {
                    if (strncmp(namesB[i], first->hitName, strlen(namesB[i])) == 0) {
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

#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

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
s32 evt_bero_id_filter(int param_1) {
    extern void* BeroINFOARR[];
    extern s32 strcmp(const char* a, const char* b);
    EventEntry* event = (EventEntry*)param_1;
    s32* args = event->args;
    s32 filtered = evtGetValue(event, args[0]);
    void** info = BeroINFOARR;

    if (filtered < 0 || filtered >= 16) {
        s32 original = filtered;
        if (&filtered != NULL) {
            filtered = 0;
        }
    scan:
        if (*(char**)*info == NULL) {
            goto done;
        }
        if (strcmp(*(char**)*info, (char*)original) == 0) {
            goto done;
        }
        {
            info++;
            if (&filtered != NULL) {
                filtered++;
            }
        }
        goto scan;
    }
done:
    evtSetValue(event, args[0], filtered);
    return EVT_RETURN_DONE;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

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
    extern BeroInfoLocal* BeroINFOARR[];
    extern s32 default_outevt[];
    u8* event = pEvt;
    s32* args = *(s32**)(event + 0x18);
    s32* lw = (s32*)(event + 0x9C);
    const char* names[8];
    const char** name;
    s32 id = evtGetValue(pEvt, args[0]);
    BeroInfoLocal* info = BeroINFOARR[id];
    s32 direction = info->direction;
    s32 type;

    lw[3] = (s32)info->hitName;
    lw[4] = info->type;
    names[0] = lbl_802C2C28[0];
    names[1] = lbl_802C2C28[1];
    names[2] = lbl_802C2C28[2];
    names[3] = lbl_802C2C28[3];
    names[4] = lbl_802C2C28[4];
    names[5] = lbl_802C2C28[5];
    names[6] = lbl_802C2C28[6];
    names[7] = lbl_802C2C28[7];
    if (direction == 20000) {
        if ((info->type & 0xFFF) < 2) {
            direction = 0;
            name = names;
            do {
                if (strncmp(*name, info->hitName, strlen(*name)) == 0) {
                    break;
                }
                direction++;
                name++;
            } while (direction < 8);
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
    extern f32 float_0_804204e8;
    extern f32 float_30_80420518;
    extern f32 float_360_804204e4;
    extern f32 float_neg1_80420508;
    extern f32 float_6p2832_804204e0;
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern s32 evtGetValue(void* event, s32 arg);
    extern f32 evtGetFloat(void* event, s32 arg);
    extern void hitObjGetPos(char* name, f32* out);
    extern double sin(double x);
    extern double cos(double x);
    extern s32 hitCheckFilter(f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz, s32 flags,
                              f32* outZ, f32* outY, f32* outX, f32* outDist,
                              void* outA, void* outB, void* outC);
    extern void N_marioReloadMapOnBottomlessOn(void);
    extern f32 float_1E06_8042050c;
    extern f32 float_0p125_80420510;
    extern f32 float_180_80420514;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    void* mario = marioGetPtr();
    s32 type = evtGetValue(pEvt, args[0]);
    f32 length = evtGetFloat(pEvt, args[1]);
    s32* posData = *(s32**)((s32)pEvt + 0xB4);
    s32 dirIndex = *(s32*)((s32)pEvt + 0xB0);
    f32 start[3];
    f32 end[3];
    f32 base[3];
    f32 hitPos[3];
    f32 rayStart[3];
    f32 hitDist;
    f32 outA;
    f32 outB;
    f32 outC;
    f32 angle;
    f32 rad;
    f32 s;
    f32 c;

    start[0] = (f32)posData[0];
    start[1] = (f32)posData[1];
    start[2] = (f32)posData[2];
    end[0] = (f32)posData[0];
    end[1] = (f32)posData[1];
    end[2] = (f32)posData[2];

    if (posData[0] == 100000) {
        hitObjGetPos(*(char**)((s32)pEvt + 0xA8), start);
    }

    if (float_0_804204e8 != length) {
        hitPos[0] = float_0_804204e8;
        hitPos[1] = float_0_804204e8;
        hitPos[2] = float_0_804204e8;
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
        base[1] = start[1];
        base[0] = start[0] - (float_30_80420518 * s);

        c = (f32)cos(rad);
        base[2] = start[2] + (float_30_80420518 * c);

        if (hitCheckFilter(base[0], base[1], base[2],
                           float_0_804204e8, float_neg1_80420508, float_0_804204e8,
                           0, &hitPos[2], &hitPos[1], &hitPos[0], &hitDist,
                           &outA, &outB, &outC) != 0) {
            base[1] = hitPos[1];
        }

        rayStart[0] = base[0];
        rayStart[1] = base[1];
        rayStart[2] = base[2];

        s = (f32)sin(rad);
        length += float_30_80420518;
        end[0] = (length * s) + rayStart[0];

        c = (f32)cos(rad);
        end[2] = (length * -c) + rayStart[2];
        end[1] = rayStart[1];

        start[0] = base[0];
        start[1] = base[1];
        start[2] = base[2];
    }

    BeroSX = start[0];
    BeroSY = start[1];
    BeroSZ = start[2];
    BeroEX = end[0];
    BeroEY = end[1];
    BeroEZ = end[2];
    N_marioSetBottomlessRespawnPosOnBeroEntry(end[0], end[1], end[2]);
    if (*(u16*)((s32)mario + 0x2E) == 0x19) {
        N_marioReloadMapOnBottomlessOn();
    }
    return 2;
}

void bero_get_position_pipe_sub(s32 side, u32 distance, char* hitName, s32 kind,
                                f32* start, f32* end) {
    extern f32 float_0_804204e8;
    extern f32 float_20_804204f8;
    extern f32 float_neg1_80420508;
    extern void hitObjGetPos(char*, f32*);
    extern s32 hitCheckFilter(f32, f32, f32, f32, f32, f32, s32,
                              f32*, f32*, f32*, f32*, void*, void*, void*);
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 bero_get_position_pipe2(s32 param) {
    extern s32 evtGetValue(void*, s32);
    extern f32 evtGetFloat(void*, s32);
    extern void bero_get_position_pipe_sub(s32, s32, void*, s32, void*, void*);
    extern f32 float_1_804204fc;
    extern f32 float_0_804204e8;
    extern const RoadVec vec3_802c2c48;
    extern const RoadVec vec3_802c2c54;
    s32* args;
    f32 start[3];
    f32 end[3];
    f32 road[9];
    RoadVec position;
    RoadVec target;
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
    ((void (*)(f64, f64, f64, f64, f64, f64, f64, void*))camRoadMain)(
        end[0], end[1], end[2], end[0], end[1], end[2], float_0_804204e8, road);
    position = vec3_802c2c48;
    target = vec3_802c2c54;
    position.x = road[3];
    position.y = road[4];
    position.z = road[5];
    target.x = road[6];
    target.y = road[7];
    target.z = road[8];
    *(RoadVec*)BeroPT = position;
    *(RoadVec*)BeroAT = target;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 bero_set_disp_position_pipe(s32 param) {
    extern f32 float_20_804204f8;
    extern f32 float_3_804204ec;
    extern f32 float_15_804204f0;
    extern f32 float_13_804204f4;
    u8* mario;
    s32 kind;

    kind = *(s32*)(param + 0xB0);
    switch (kind) {
    case 2:
    case 6:
        if ((*(u32*)(param + 0xAC) & 0x1000) == 0) {
            mario = marioGetPtr();
            *(f32*)(mario + 0x9C) += float_3_804204ec;
            mario = marioGetPtr();
            *(f32*)(mario + 0xA0) -= float_15_804204f0;
        }
        break;
    case 8:
    case 9:
        if ((*(u32*)(param + 0xAC) & 0x1000) == 0) {
            mario = marioGetPtr();
            *(f32*)(mario + 0xA0) -= float_13_804204f4;
        } else {
            mario = marioGetPtr();
            *(f32*)(mario + 0xA0) -= float_20_804204f8;
        }
        break;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_bero_switch_on(void* pEvt) {
    extern s32 strcmp(const char* a, const char* b);
    extern void* BeroINFOARR[];
    EventEntry* event = pEvt;
    s32* args = event->args;
    void** info = BeroINFOARR;
    s32 id = evtGetValue(event, args[0]);

    if (id < 0 || id >= 16) {
        register s32 original = id;
        if (&id != NULL) {
            id = 0;
        }
        for (;;) {
            if (*(char**)*info == NULL) {
                break;
            }
            if (strcmp(*(char**)*info, (char*)original) == 0) {
                break;
            }
            info++;
            if (&id != NULL) {
                id++;
            }
        }
    }
    if (BeroSW[id] == 1) {
        *(s32*)((s32)event + 0x9C) = 1;
    } else {
        *(s32*)((s32)event + 0x9C) = 0;
    }
    BeroSW[id] = 1;
    *(s32*)((s32)event + 0xA0) = id;
    return EVT_RETURN_DONE;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_bero_switch_off(void* pEvt) {
    extern s32 strcmp(const char* a, const char* b);
    extern void* BeroINFOARR[];
    EventEntry* event = pEvt;
    s32* args = event->args;
    void** info = BeroINFOARR;
    s32 id = evtGetValue(event, args[0]);

    if (id < 0 || id >= 16) {
        register s32 original = id;
        if (&id != NULL) {
            id = 0;
        }
        for (;;) {
            if (*(char**)*info == NULL) {
                break;
            }
            if (strcmp(*(char**)*info, (char*)original) == 0) {
                break;
            }
            info++;
            if (&id != NULL) {
                id++;
            }
        }
    }
    if (BeroSW[id] != 1) {
        *(s32*)((s32)event + 0x9C) = 1;
    } else {
        *(s32*)((s32)event + 0x9C) = 0;
    }
    BeroSW[id] = 0;
    *(s32*)((s32)event + 0xA0) = id;
    return EVT_RETURN_DONE;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_bero_get_number) {
    extern s32 strcmp(const char*, const char*);
    extern void* BeroINFOARR[];
    s32* args = event->args;
    void** info = BeroINFOARR;
    s32 id = evtGetValue(event, args[0]);

    if (id < 0 || id >= 16) {
        register s32 original = id;
        if (&id != NULL) {
            id = 0;
        }
        for (;;) {
            if (*(char**)*info == NULL) {
                break;
            }
            if (strcmp(*(char**)*info, (char*)original) == 0) {
                break;
            }
            info++;
            if (&id != NULL) {
                id++;
            }
        }
    }
    *(s32*)((s32)event + 0xA0) = id;
    return EVT_RETURN_DONE;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_bero_case_id_save(void* pEvt) {
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
    EventEntry* event = (EventEntry*)pEvt;
    s32* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], BeroID[index]);
    return EVT_RETURN_DONE;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(evt_bero_disppos_save) {

    BeroOX = *(f32*)((s32)marioGetPtr() + 0x98);
    BeroOY = *(f32*)((s32)marioGetPtr() + 0x9C);
    BeroOZ = *(f32*)((s32)marioGetPtr() + 0xA0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_disppos_load) {
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
    extern f32 float_0_804204e8;
    extern f32 float_360_804204e4;
    extern f32 float_6p2832_804204e0;
    extern f32 evtGetFloat(void*, s32);
    extern f32 sysFrame2SecFloat(f32);
    extern s32 sysMsec2Frame(s32);
    extern f32 angleABf(f32, f32, f32, f32);
    extern f32 reviseAngle(f32);
    extern double sin(double);
    extern double cos(double);
    u8* event = pEvt;
    s32* args = *(s32**)(event + 0x18);
    u8* player;
    u64 now;
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

    step = evtGetFloat(pEvt, args[0]);
    if (*(s32*)((u8*)gp + 0x14) == 0) {
        now = *(u64*)((u8*)gp + 0x40);
    } else {
        now = *(u64*)((u8*)gp + 0x38);
    }
    if (isFirstCall != 0) {
        *(u64*)(event + 0x198) = now;
    }
    step = sysFrame2SecFloat(step);

    angle = reviseAngle(angleABf(BeroSX, BeroSY, BeroEX, BeroEY));
    radians = (float_6p2832_804204e0 * angle) / float_360_804204e4;
    sinv = (f32)sin((double)radians);
    cosv = (f32)cos((double)radians);
    elapsed = (f32)sysMsec2Frame((s32)((now - *(u64*)(event + 0x198)) /
        (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6)));
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
USER_FUNC(evt_bero_overwrite) {
    extern s32 strcmp(const char* a, const char* b);
    extern void* BeroINFOARR[];
    void** info = BeroINFOARR;
    s32* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 value;

    if (id < 0 || id >= 16) {
        register char* name = (char*)id;
        if (&id != NULL) {
            id = 0;
        }
        while (*(char**)*info != NULL && strcmp(*(char**)*info, name) != 0) {
            info++;
            if (&id != NULL) {
                id++;
            }
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
USER_FUNC(evt_bero_cam3d_change) {
    extern void* camGetPtr(s32 camId);
    s32* args = event->args;
    void* cam = camGetPtr(4);
    s32 time = evtGetValue(event, args[0]);
    s32 type = evtGetValue(event, args[1]);

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
    *(u32*)((s32)cam + 0x7C) = time * (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6);
    *(u16*)((s32)cam + 0x04) = 3;
    *(u8*)((s32)cam + 0x80) = type;
    return EVT_RETURN_DONE;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(evt_bero_cam_shift_init) {

    camRoadReset();
    camShiftReset();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_mario_go_init) {

    BeroMarioGO = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_mario_go) {

    BeroMarioGO = 1;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_mario_go_wait) {
    return (__cntlzw((u32)BeroMarioGO) & 0x20) ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
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


#pragma use_lmw_stmw off
s32 evt_camera_change_event_from_road(void) {
    extern void* camGetPtr(s32);
    extern void camShiftMain(void*, void*, void*);
    extern const RoadVec vec3_802c2f10;
    extern const RoadVec vec3_802c2f1c;
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
    y = *(f32*)(cam + 0x98);
    z = *(f32*)(player + 0x94);
    camRoadMain(x, y, z, x, y, z, road);
    {
    RoadVec roadPos = vec3_802c2f10;
    RoadVec roadTarget = vec3_802c2f1c;

    *(u32*)(cam + 0x58) = *(u32*)(cam + 0x0C);
    *(u32*)(cam + 0x5C) = *(u32*)(cam + 0x10);
    *(u32*)(cam + 0x60) = *(u32*)(cam + 0x14);
    *(u32*)(cam + 0x64) = *(u32*)(cam + 0x18);
    *(u32*)(cam + 0x68) = *(u32*)(cam + 0x1C);
    *(u32*)(cam + 0x6C) = *(u32*)(cam + 0x20);
    roadPos.x = road[3];
    roadPos.y = road[4];
    roadPos.z = road[5];
    roadTarget.x = road[6];
    roadTarget.y = road[7];
    roadTarget.z = road[8];
    *(RoadVec*)(cam + 0x40) = roadPos;
    *(RoadVec*)(cam + 0x4C) = roadTarget;
    *(u32*)(cam + 0x70) = *(u32*)((u8*)gp + 0x38);
    *(u32*)(cam + 0x74) = *(u32*)((u8*)gp + 0x3C);
    *(u32*)(cam + 0x78) = 0;
    *(u32*)(cam + 0x7C) = (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6) * 500;
    *(u16*)(cam + 4) = 3;
    *(u8*)(cam + 0x80) = 11;
    }
    return 2;
}
#pragma use_lmw_stmw on

extern s32 evt_mario_key_onoff(void);

s32 default_entevt[] = {
    0x00020032, 0xFE363C80, 0, 2, 1,
};

s32 default_outevt[] = {
    0x00020032, 0xFE363C80, 0, 2, 1,
};

s32 evt_bero_info_run_none[] = {
    0x0002005B, (s32)evt_mario_key_onoff, 1, 2, 1,
};

s32 evt_bero_info_run_gate[] = {
    0x0002005B, (s32)evt_mario_key_onoff, 1, 2, 1,
};

void* BeroINFOARR[16];
s32 BeroSW[16];
s32 BeroID[16];

s32 BeroEXEC;
s32 BeroNUM;
s32 BeroNOWNUM;
s32 BeroCAM;
f32 BeroOX;
f32 BeroOY;
f32 BeroOZ;
f32 BeroSX;
f32 BeroSY;
f32 BeroSZ;
f32 BeroEX;
f32 BeroEY;
f32 BeroEZ;
f32 BeroPT[3];
f32 BeroAT[3];
s32 BeroMarioGO;

const char str_n_804204c0[] = "n_";
const char str_ne_804204c4[] = "ne_";
const char str_e_804204c8[] = "e_";
const char str_se_804204cc[] = "se_";
const char str_s_804204d0[] = "s_";
const char str_sw_804204d4[] = "sw_";
const char str_w_804204d8[] = "w_";
const char str_nw_804204dc[] = "nw_";

/* Recovered evt_bero safe-event dependencies. */
extern const char str_sjis_00000018[];
extern const char str_sjis_000002f4[];
extern const char str_sjis_00000300[];
extern const char str_sjis_00000330[];
extern const char str_sjis_00000344[];
extern const char str_sjis_00000384[];
extern const char str_sjis_000003a8[];
extern const char str_sjis_000003cc[];
extern const char str_sjis_000003f0[];
extern u32 bero_case_switch_on[];
extern s32 evt_mario_get_motion();
extern s32 evt_del_case_evt();
extern s32 evt_sub_get_system_flag();
extern s32 evt_mario_kill_party();
extern s32 evt_mario_cam_y_off();
extern s32 evt_mario_set_prev_party_dokan();
extern s32 evt_mario_set_prev_party();
extern s32 evt_mario_dokan_prepare();
extern u32 bero_null[];
extern u32 bero_open_door_play_se[];
extern u32 bero_close_door_play_se[];
extern u32 bero_play_enter_anime[];
extern u32 bero_play_out_anime[];
extern u32 bero_case_init_check[];
extern u32 bero_case_exec_none[];
extern u32 bero_case_exec_road[];
extern u32 bero_case_exec_door[];
extern u32 bero_case_exec_pipe_sub[];
extern u32 bero_case_exec_pipe[];
extern u32 bero_case_exec_gate[];
extern u32 bero_case_entry[];
extern u32 bero_case_entry_sub[];
extern u32 evt_bero_info_run_road[];
extern u32 evt_bero_info_run_door[];
extern u32 evt_bero_info_run_pipe_sub00[];
extern u32 evt_bero_info_run_pipe[];
extern u32 evt_bero_initial_pos[];
extern u32 evt_bero_mario_enter[];
extern u32 bero_case_switch_off[];
extern u32 evt_bero_nonride_check[];
extern s32 evt_cam3d_evt_off();
extern s32 evt_cam_road_reset();
extern s32 evt_cam_road_reset2();
extern s32 evt_fade_end_wait();
extern s32 evt_fade_set_mapchange_type();
extern s32 evt_hit_get_position();
extern s32 evt_hitobj_attr_onoff();
extern s32 evt_map_checkanim();
extern s32 evt_map_pauseanim();
extern s32 evt_map_playanim();
extern s32 evt_map_replayanim();
extern s32 evt_mapobj_rotate();
extern s32 evt_mario_bgmode_off();
extern s32 evt_mario_bgmode_on();
extern s32 evt_mario_cam_off();
extern s32 evt_mario_cam_on();
extern s32 evt_mario_cont_onoff();
extern s32 evt_mario_dokan_end();
extern s32 evt_mario_get_mode();
extern s32 evt_mario_get_pos();
extern s32 evt_mario_mov_pos2();
extern s32 evt_mario_paper_pose_dokan();
extern s32 evt_mario_paper_pose_off();
extern s32 evt_mario_party_bero_move();
extern s32 evt_mario_set_dispdir();
extern s32 evt_mario_set_force_dir();
extern s32 evt_mario_set_pos();
extern s32 evt_mario_wait_landon();
extern s32 evt_npc_change_fbat_mode();
extern s32 evt_npc_start_for_event();
extern s32 evt_npc_stop_for_event();
extern s32 evt_party_run();
extern s32 evt_party_set_homing_dist();
extern s32 evt_party_set_pos();
extern s32 evt_party_stop();
extern s32 evt_run_case_evt_bero();
extern s32 evt_seq_wait();
extern s32 evt_snd_sfxon_3d();
extern s32 evt_sub_intpl_msec_get_value_para();
extern s32 evt_sub_intpl_msec_init();
extern const char str_SFX_DOOR_OPEN1_802c2c70[];
extern const char str_SFX_DOOR_OPEN_CASTLE_802c2d54[];
extern const char str_SFX_DOOR_OPEN_CASTLE_802c2d6c[];
extern const char str_SFX_DOOR_OPEN_ELEC1_802c2d28[];
extern const char str_SFX_DOOR_OPEN_GRILLE_802c2ce4[];
extern const char str_SFX_DOOR_OPEN_IRON1_802c2cbc[];
extern const char str_SFX_DOOR_OPEN_IRON2_802c2cd0[];
extern const char str_SFX_DOOR_OPEN_METAL1_802c2d84[];
extern const char str_SFX_DOOR_OPEN_SLIDE1_802c2cfc[];
extern const char str_SFX_DOOR_OPEN_SLIDE1_802c2dc8[];
extern const char str_SFX_DOOR_OPEN_STONE1_802c2d3c[];
extern const char str_SFX_DOOR_OPEN_TURN1_802c2d14[];
extern const char str_SFX_DOOR_OPEN_WOOD1_802c2c80[];
extern const char str_SFX_DOOR_OPEN_WOOD2_802c2c94[];
extern const char str_SFX_DOOR_OPEN_WOOD3_802c2ca8[];
extern const char str_SFX_DOOR_SHUT1_802c2de0[];
extern const char str_SFX_DOOR_SHUT_CASTLE_802c2e98[];
extern const char str_SFX_DOOR_SHUT_CASTLE_802c2eb0[];
extern const char str_SFX_DOOR_SHUT_ELEC1_802c2e84[];
extern const char str_SFX_DOOR_SHUT_GRILLE_802c2e40[];
extern const char str_SFX_DOOR_SHUT_IRON1_802c2e18[];
extern const char str_SFX_DOOR_SHUT_IRON2_802c2e2c[];
extern const char str_SFX_DOOR_SHUT_METAL1_802c2ec8[];
extern const char str_SFX_DOOR_SHUT_SLIDE1_802c2e58[];
extern const char str_SFX_DOOR_SHUT_SLIDE1_802c2ef8[];
extern const char str_SFX_DOOR_SHUT_TURN1_802c2e70[];
extern const char str_SFX_DOOR_SHUT_WOOD1_802c2df0[];
extern const char str_SFX_DOOR_SHUT_WOOD2_802c2e04[];
extern const char str_SFX_MARIO_DOKAN1_802c2f28[];
extern const char str_SFX_STG3_AUTO_DOOR1_802c2d9c[];
extern const char str_SFX_STG3_LOCKER_CLOS_802c2ee0[];
extern const char str_SFX_STG3_LOCKER_OPEN_802c2db0[];

/* Recovered target evt_bero safe event/event-like arrays, in target order. */
u32 bero_null[15] = {
    0x00000000, 0x0F000000, 0x00004E20, 0x000186A0, 0x00000000, 0x00000000,
    0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000,
    0x00010001, 0x00000000, 0x00000000
};

u32 evt_bero_nonride_check[37] = {
    0x00020019, 0xFE363C8A, 0x00000006, 0x00020032, 0xFE363C80, 0xFE363C83,
    0x0001005E, (u32)bero_case_switch_on, 0x00000002, 0x00000021, 0x00010005, 0x00000000,
    0x0002005B, (u32)evt_mario_get_motion, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000003,
    0x00000007, 0x00000021, 0x00020018, 0xFE363C80, 0x0000000A, 0x00000007,
    0x00000021, 0x00010009, 0x00000001, 0x00000006, 0x00010071, (u32)(char*)str_sjis_00000018,
    0x00020032, 0xFE363C80, 0xFE363C83, 0x0001005E, (u32)bero_case_switch_on, 0x00000002,
    0x00000001
};

u32 bero_open_door_play_se[188] = {
    0x0002005B, (u32)evt_bero_get_info_num, 0xFE363C81, 0x00020032, 0xFE363C84, 0x00000000,
    0x00020032, 0xFE363C85, 0x00000000, 0x00020032, 0xFE363C86, 0x00000000,
    0x00020019, 0xFE363C83, 0x00000000, 0x0005005B, (u32)evt_hit_get_position, 0xFE363C83,
    0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000021, 0x0005005B, (u32)evt_bero_get_info_kinddir,
    0xFE363C81, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00010022, 0xFE363C8B,
    0x00010024, 0x00000000, 0x00010024, 0x00000001, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_OPEN1_802c2c70, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x00000002, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_WOOD1_802c2c80, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x00000003, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_OPEN_WOOD2_802c2c94, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x00000004, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_WOOD3_802c2ca8, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x00000005, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_OPEN_IRON1_802c2cbc, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x00000006, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_IRON2_802c2cd0, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x00000007, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_OPEN_GRILLE_802c2ce4, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x00000008, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_SLIDE1_802c2cfc, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x00000009, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_OPEN_TURN1_802c2d14, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x0000000A, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_ELEC1_802c2d28, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x0000000B, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_OPEN_STONE1_802c2d3c, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x0000000C, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_CASTLE_802c2d54, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x0000000D, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_OPEN_CASTLE_802c2d6c, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x0000000E, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_OPEN_METAL1_802c2d84, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x0000000F, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_STG3_AUTO_DOOR1_802c2d9c, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x00000010, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_STG3_LOCKER_OPEN_802c2db0, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x00000011, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_OPEN_SLIDE1_802c2dc8, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00000031,
    0x00000002, 0x00000001
};

u32 bero_close_door_play_se[188] = {
    0x0002005B, (u32)evt_bero_get_info_num, 0xFE363C81, 0x00020032, 0xFE363C84, 0x00000000,
    0x00020032, 0xFE363C85, 0x00000000, 0x00020032, 0xFE363C86, 0x00000000,
    0x00020019, 0xFE363C83, 0x00000000, 0x0005005B, (u32)evt_hit_get_position, 0xFE363C83,
    0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000021, 0x0005005B, (u32)evt_bero_get_info_kinddir,
    0xFE363C81, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00010022, 0xFE363C8B,
    0x00010024, 0x00000000, 0x00010024, 0x00000001, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_SHUT1_802c2de0, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x00000002, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_WOOD1_802c2df0, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x00000003, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_SHUT_WOOD2_802c2e04, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x00000004, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_WOOD2_802c2e04, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x00000005, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_SHUT_IRON1_802c2e18, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x00000006, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_IRON2_802c2e2c, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x00000007, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_SHUT_GRILLE_802c2e40, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x00000008, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_SLIDE1_802c2e58, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x00000009, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_SHUT_TURN1_802c2e70, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x0000000A, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_ELEC1_802c2e84, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x0000000B, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_OPEN_STONE1_802c2d3c, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x0000000C, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_CASTLE_802c2e98, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x0000000D, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_SHUT_CASTLE_802c2eb0, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x0000000E, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_DOOR_SHUT_METAL1_802c2ec8, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x0000000F, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_STG3_AUTO_DOOR1_802c2d9c, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00010024,
    0x00000010, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_STG3_LOCKER_CLOS_802c2ee0, 0xFE363C84, 0xFE363C85,
    0xFE363C86, 0x00000000, 0x00010024, 0x00000011, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_DOOR_SHUT_SLIDE1_802c2ef8, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x00000000, 0x00000031,
    0x00000002, 0x00000001
};

u32 bero_play_enter_anime[147] = {
    0x0006005B, (u32)evt_bero_get_info_anime, 0xFE363C81, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0xFE363C8D, 0x00010022, 0xFE363C8A, 0x00010024, 0x00000000, 0x00010024,
    0x00000001, 0x0000006B, 0x0001000A, 0x000000C8, 0x0001005B, (u32)evt_bero_mario_go,
    0x0000006D, 0x00020019, 0xFE363C8C, 0x00000000, 0x0004005B, (u32)evt_map_playanim,
    0xFE363C8C, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_map_pauseanim, 0x00000000,
    0xFE363C8C, 0x0001000A, 0x000002BC, 0x0001005C, (u32)bero_close_door_play_se, 0x0003005B,
    (u32)evt_map_replayanim, 0x00000000, 0xFE363C8C, 0x00000021, 0x00010024, 0x00000002,
    0x00020018, 0xFE363C8C, 0x00000000, 0x00000030, 0x00000021, 0x00020032,
    0xFE363C88, 0xFE363C8C, 0x0006005B, (u32)evt_mapobj_rotate, 0x00000001, 0xFE363C88,
    0x00000000, 0xFFFFFFB0, 0x00000000, 0x0001005B, (u32)evt_bero_mario_go, 0x0001000A,
    0x000001F4, 0x0001005C, (u32)bero_close_door_play_se, 0x0005005B, (u32)evt_sub_intpl_msec_init, 0x0000000B,
    0xFFFFFFB0, 0x00000000, 0x000001F4, 0x00010005, 0x00000000, 0x0003005B,
    (u32)evt_sub_intpl_msec_get_value_para, 0xFE363C80, 0xFE363C81, 0x0006005B, (u32)evt_mapobj_rotate, 0x00000001,
    0xFE363C88, 0x00000000, 0xFE363C80, 0x00000000, 0x00020018, 0xFE363C81,
    0x00000000, 0x00000007, 0x00000021, 0x00010009, 0x00000001, 0x00000006,
    0x00010024, 0x00000004, 0x00020019, 0xFE363C8C, 0x00000000, 0x0001005C,
    0xFE363C8C, 0x00000021, 0x00010024, 0x00000005, 0x00020019, 0xFE363C8C,
    0x00000000, 0x00020034, 0xFE363C80, 0xF24D4A80, 0x0001005E, 0xFE363C8C,
    0x0001005B, (u32)evt_bero_mario_go, 0x0001000A, 0x000001F4, 0x0001005C, (u32)bero_close_door_play_se,
    0x00020032, 0xFE363C89, 0xFE363C8C, 0x0005005B, (u32)evt_sub_intpl_msec_init, 0x0000000B,
    0x000000B4, 0x00000000, 0x000001F4, 0x00010005, 0x00000000, 0x0003005B,
    (u32)evt_sub_intpl_msec_get_value_para, 0xFE363C80, 0xFE363C8A, 0x00020018, 0xFE363C8A, 0x00000000,
    0x00020034, 0xFE363C80, 0xF24A7A80, 0x0001005E, 0xFE363C89, 0x00000007,
    0x00000020, 0x0001005E, 0xFE363C89, 0x00000021, 0x00000006, 0x00000021,
    0x00000031, 0x00000002, 0x00000001
};

u32 bero_play_out_anime[133] = {
    0x0006005B, (u32)evt_bero_get_info_anime, 0xFE363C81, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0xFE363C8D, 0x00010022, 0xFE363C8B, 0x00010024, 0x00000000, 0x00010024,
    0x00000001, 0x00020019, 0xFE363C8D, 0x00000000, 0x0001005C, (u32)bero_open_door_play_se,
    0x0004005B, (u32)evt_map_playanim, 0xFE363C8D, 0x00000000, 0x00000000, 0x0000006B,
    0x0004005B, (u32)evt_map_checkanim, 0xFE363C8D, 0xFE363C88, 0xFE363C89, 0x0001000A,
    0xFE363C89, 0x0001005B, (u32)evt_bero_mario_go, 0x0000006D, 0x00000020, 0x0000006B,
    0x0001000A, 0x000001F4, 0x0001005B, (u32)evt_bero_mario_go, 0x0000006D, 0x00000021,
    0x00010024, 0x00000002, 0x00020018, 0xFE363C8C, 0x00000000, 0x00000030,
    0x00000021, 0x00020032, 0xFE363C88, 0xFE363C8C, 0x0001005C, (u32)bero_open_door_play_se,
    0x0005005B, (u32)evt_sub_intpl_msec_init, 0x0000000B, 0x00000000, 0x00000050, 0x000001F4,
    0x00010005, 0x00000000, 0x0003005B, (u32)evt_sub_intpl_msec_get_value_para, 0xFE363C80, 0xFE363C81,
    0x0006005B, (u32)evt_mapobj_rotate, 0x00000001, 0xFE363C88, 0x00000000, 0xFE363C80,
    0x00000000, 0x00020018, 0xFE363C81, 0x00000000, 0x00000007, 0x00000021,
    0x00010009, 0x00000001, 0x00000006, 0x0001005B, (u32)evt_bero_mario_go, 0x00010024,
    0x00000004, 0x00020019, 0xFE363C8D, 0x00000000, 0x0001005C, 0xFE363C8D,
    0x00000021, 0x00010024, 0x00000005, 0x00020019, 0xFE363C8D, 0x00000000,
    0x0001005C, (u32)bero_open_door_play_se, 0x00020032, 0xFE363C89, 0xFE363C8D, 0x0005005B,
    (u32)evt_sub_intpl_msec_init, 0x0000000B, 0x00000000, 0x000000B4, 0x000001F4, 0x00010005,
    0x00000000, 0x0003005B, (u32)evt_sub_intpl_msec_get_value_para, 0xFE363C80, 0xFE363C8A, 0x00020018,
    0xFE363C8A, 0x00000000, 0x00020034, 0xFE363C80, 0xF24D4A80, 0x0001005E,
    0xFE363C89, 0x00000007, 0x00000020, 0x0001005E, 0xFE363C89, 0x00000021,
    0x00000006, 0x0001005B, (u32)evt_bero_mario_go, 0x00000021, 0x00000031, 0x00000002,
    0x00000001
};

u32 bero_case_init_check[99] = {
    0x0003005B, (u32)evt_bero_exec_onoff, 0x00000001, 0x00010000, 0x0002005B, (u32)evt_bero_exec_get,
    0xFE363C80, 0x0002004E, 0xFE363C80, 0x00000001, 0x00020019, 0xFE363C80,
    0x00000000, 0x00020032, 0xFE363C80, 0x00000001, 0x00000002, 0x00000021,
    0x0002005B, (u32)evt_npc_change_fbat_mode, 0x00000007, 0x0001005B, (u32)evt_npc_stop_for_event, 0x00020032,
    0xFE363C80, 0xFE363C84, 0x0002004E, 0xFE363C80, 0x00000FFF, 0x00010022,
    0xFE363C80, 0x0001002B, 0x00000000, 0x0001002B, 0x00000001, 0x0003005B,
    (u32)evt_bero_get_info_length, 0xFE363C81, 0xFE363C80, 0x0003005B, (u32)bero_get_position_normal, 0x00000000,
    0xFE363C80, 0x0000002E, 0x0001002B, 0x00000F00, 0x0001002B, 0x00000003,
    0x0003005B, (u32)bero_get_position_normal, 0x00000000, 0xF24A7A80, 0x0000002E, 0x00010024,
    0x00000002, 0x0003005B, (u32)evt_bero_get_info_length, 0xFE363C81, 0xFE363C80, 0x0003005B,
    (u32)bero_get_position_pipe_pure, 0x00000000, 0xFE363C80, 0x00000031, 0x0002005B, (u32)evt_bero_set_now_number,
    0xFE363C81, 0x0003005B, (u32)evt_bero_exec_onoff, 0x00000000, 0x00000001, 0x00020032,
    0xFE363C8F, 0x00000000, 0x0001005E, 0xFE363C8B, 0x00020018, 0xFE363C80,
    0x00000001, 0x0002005B, (u32)evt_npc_change_fbat_mode, 0x00000001, 0x0001005B, (u32)evt_npc_start_for_event,
    0x0003005B, (u32)evt_bero_exec_onoff, 0x00000000, 0x00010000, 0x0003005B, (u32)evt_bero_exec_onoff,
    0x00000001, 0x00000001, 0x00020032, 0xFE363C80, 0x00000001, 0x00000002,
    0x00000021, 0x00000002, 0x00000001
};

u32 bero_case_exec_none[45] = {
    0x0002005B, (u32)evt_mario_key_onoff, 0x00000000, 0x0001005E, (u32)bero_case_init_check, 0x0002005B,
    (u32)evt_mario_key_onoff, 0x00000001, 0x00020018, 0xFE363C80, 0x00000001, 0x00000002,
    0x00000021, 0x0002005B, (u32)evt_mario_key_onoff, 0x00000000, 0x0003005B, (u32)evt_bero_exec_onoff,
    0x00000000, 0x00010000, 0x0003005B, (u32)evt_bero_exec_onoff, 0x00000001, 0x00000001,
    0x0003005B, (u32)evt_bero_exec_onoff, 0x00000000, 0x00000008, 0x00010009, 0x00000002,
    0x0002005B, (u32)evt_bero_1stcheck, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001,
    0x0001000A, 0x000003E8, 0x00000021, 0x0003005B, (u32)evt_bero_mapchange, 0xFE363C8D,
    0xFE363C8E, 0x00000002, 0x00000001
};

u32 bero_case_exec_road[75] = {
    0x0002005B, (u32)evt_mario_key_onoff, 0x00000000, 0x0001005E, (u32)bero_case_init_check, 0x0002005B,
    (u32)evt_mario_key_onoff, 0x00000001, 0x00020018, 0xFE363C80, 0x00000001, 0x00000002,
    0x00000021, 0x0002005B, (u32)evt_mario_key_onoff, 0x00000000, 0x0000006B, 0x00020032,
    0xFE363C80, 0xFE363C85, 0x00020037, 0xFE363C80, 0x0000002D, 0x0002005B,
    (u32)evt_mario_set_force_dir, 0xFE363C80, 0x0001005B, (u32)evt_mario_wait_landon, 0x0003005B, (u32)bero_get_position_normal,
    0x00000000, 0xF24CD280, 0x00020056, 0xFE363C8A, (u32)&BeroEX, 0x00020056,
    0xFE363C8C, (u32)&BeroEZ, 0x0004005B, (u32)evt_mario_mov_pos2, 0xFE363C8A, 0xFE363C8C,
    0xF24CD280, 0x0003005B, (u32)evt_bero_exec_onoff, 0x00000000, 0x00010000, 0x0003005B,
    (u32)evt_bero_exec_onoff, 0x00000001, 0x00000001, 0x0000006D, 0x0001000A, 0x00000096,
    0x0003005B, (u32)evt_bero_exec_onoff, 0x00000000, 0x00000008, 0x00010009, 0x00000002,
    0x0002005B, (u32)evt_bero_1stcheck, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001,
    0x0001000A, 0x000003E8, 0x00000021, 0x0003005B, (u32)evt_bero_mapchange, 0xFE363C8D,
    0xFE363C8E, 0x00000002, 0x00000001
};

u32 bero_case_exec_door[89] = {
    0x0002005B, (u32)evt_mario_key_onoff, 0x00000000, 0x0001005E, (u32)bero_case_init_check, 0x0002005B,
    (u32)evt_mario_key_onoff, 0x00000001, 0x00020018, 0xFE363C80, 0x00000001, 0x00000002,
    0x00000021, 0x0002005B, (u32)evt_mario_key_onoff, 0x00000000, 0x0005005B, (u32)evt_hitobj_attr_onoff,
    0x00000000, 0x00000001, 0xFE363C83, 0x00800000, 0x0001005B, (u32)evt_bero_mario_go_init,
    0x0001005C, (u32)bero_play_out_anime, 0x0000006B, 0x00020032, 0xFE363C80, 0xFE363C85,
    0x00020037, 0xFE363C80, 0x0000002D, 0x0002005B, (u32)evt_mario_set_force_dir, 0xFE363C80,
    0x0001005B, (u32)evt_mario_wait_landon, 0x0001005B, (u32)evt_bero_mario_go_wait, 0x0003005B, (u32)bero_get_position_normal,
    0x00000000, 0xF24CD280, 0x00020056, 0xFE363C8A, (u32)&BeroEX, 0x00020056,
    0xFE363C8C, (u32)&BeroEZ, 0x0004005B, (u32)evt_mario_mov_pos2, 0xFE363C8A, 0xFE363C8C,
    0xF24CD280, 0x0003005B, (u32)evt_bero_exec_onoff, 0x00000000, 0x00010000, 0x0003005B,
    (u32)evt_bero_exec_onoff, 0x00000001, 0x00000001, 0x0000006D, 0x0001005B, (u32)evt_bero_mario_go_wait,
    0x0001000A, 0x00000096, 0x0003005B, (u32)evt_bero_exec_onoff, 0x00000000, 0x00000008,
    0x00010009, 0x00000002, 0x0002005B, (u32)evt_bero_1stcheck, 0xFE363C80, 0x00020018,
    0xFE363C80, 0x00000001, 0x0001000A, 0x000003E8, 0x00000021, 0x0003005B,
    (u32)evt_bero_mapchange, 0xFE363C8D, 0xFE363C8E, 0x00000002, 0x00000001
};

u32 bero_case_exec_pipe_sub[333] = {
    0x0000006B, 0x0001005B, (u32)evt_camera_change_event_from_road, 0x0000006D, 0x00020018, 0xFE363C8D,
    0x00000000, 0x0000006B, 0x00020032, 0xFE363C80, 0xFE363C84, 0x0002004E,
    0xFE363C80, 0x00001000, 0x00020018, 0xFE363C80, 0x00000000, 0x00020032,
    0xFBD3E280, 0x00000000, 0x00000020, 0x00020032, 0xFBD3E280, 0x00000001,
    0x00000021, 0x0002005B, (u32)evt_bero_get_number, 0xFE363C8E, 0x0002005B, (u32)evt_bero_set_now_number,
    0xFE363C81, 0x0002005B, (u32)evt_bero_get_info_num, 0xFE363C81, 0x00020032, 0xFE363C80,
    0xFE363C84, 0x0002004E, 0xFE363C80, 0x00001000, 0x00020018, 0xFE363C80,
    0x00000000, 0x00020032, 0xFBD3E281, 0x00000000, 0x00000020, 0x00020032,
    0xFBD3E281, 0x00000001, 0x00000021, 0x00020032, 0xFBD3E282, 0x00000000,
    0x00020018, 0xFBD3E280, 0x00000000, 0x00020018, 0xFBD3E281, 0x00000001,
    0x00020032, 0xFBD3E282, 0x00000001, 0x00000021, 0x00000021, 0x00020018,
    0xFBD3E282, 0x00000000, 0x00020053, 0x00000000, (u32)&BeroCAM, 0x0003005B,
    (u32)evt_bero_get_info_length, 0xFE363C81, 0xFE363C8A, 0x0003005B, (u32)bero_get_position_pipe2, 0x00000001,
    0xFE363C8A, 0x00010005, 0x00000000, 0x00020055, 0xFE363C80, (u32)&BeroCAM,
    0x00020018, 0xFE363C80, 0x00000001, 0x00000007, 0x00000021, 0x00010009,
    0x00000001, 0x00000006, 0x0003005B, (u32)evt_bero_cam3d_change, 0x000001F4, 0x0000000B,
    0x00000021, 0x0000006D, 0x00000021, 0x0001005B, (u32)evt_bero_disppos_save, 0x0003005B,
    (u32)evt_bero_get_info_length, 0xFE363C81, 0xFE363C8A, 0x0003005B, (u32)bero_get_position_pipe_pure, 0x00000000,
    0xFE363C8A, 0x0001005B, (u32)bero_set_disp_position_pipe, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_MARIO_DOKAN1_802c2f28,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00010022, 0xFE363C85,
    0x00010024, 0x00000006, 0x0002005B, (u32)evt_mario_paper_pose_dokan, 0x00000005, 0x0002005B,
    (u32)evt_bero_move_mario_speed, 0xF24B6A80, 0x00010024, 0x00000002, 0x0002005B, (u32)evt_mario_paper_pose_dokan,
    0x00000005, 0x0002005B, (u32)evt_bero_move_mario_speed, 0xF24B6A80, 0x00010024, 0x00000008,
    0x0002005B, (u32)evt_mario_paper_pose_dokan, 0x00000002, 0x0002005B, (u32)evt_bero_move_mario_speed, 0xF24B6A80,
    0x00010024, 0x00000009, 0x0002005B, (u32)evt_mario_paper_pose_dokan, 0x00000003, 0x0002005B,
    (u32)evt_bero_move_mario_speed, 0xF24B6A80, 0x00000031, 0x00020053, 0x00000001, (u32)&BeroCAM,
    0x00020018, 0xFE363C8D, 0x00000000, 0x00020032, 0xFE363C80, 0xFE363C84,
    0x0002004E, 0xFE363C80, 0x00001000, 0x00020018, 0xFE363C80, 0x00000000,
    0x00020032, 0xFBD3E280, 0x00000000, 0x00000020, 0x00020032, 0xFBD3E280,
    0x00000001, 0x00000021, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_bero_get_number,
    0xFE363C8E, 0x0002005B, (u32)evt_bero_get_info_num, 0xFE363C81, 0x0001005E, 0xFE363C87,
    0x00020032, 0xFE363C80, 0xFE363C84, 0x0002004E, 0xFE363C80, 0x00001000,
    0x00020018, 0xFE363C80, 0x00000000, 0x00020032, 0xFBD3E281, 0x00000000,
    0x00000020, 0x00020032, 0xFBD3E281, 0x00000001, 0x00000021, 0x0001005B,
    (u32)evt_bero_disppos_load, 0x0001005B, (u32)evt_mario_paper_pose_off, 0x0001005B, (u32)evt_bero_disppos_save, 0x0002005B,
    (u32)evt_bero_exec_get, 0xFE363C80, 0x0002001F, 0xFE363C80, 0x00000004, 0x0003005B,
    (u32)evt_bero_exec_onoff, 0x00000000, 0x00000002, 0x00000021, 0x0003005B, (u32)evt_bero_get_info_length,
    0xFE363C81, 0xFE363C8A, 0x0003005B, (u32)bero_get_position_pipe_pure, 0x00000001, 0xFE363C8A,
    0x0001005B, (u32)bero_set_disp_position_pipe, 0x0001000A, 0x000001F4, 0x00020019, 0xFBD3E281,
    0x00000000, 0x0001005B, (u32)evt_mario_bgmode_on, 0x00000020, 0x0001005B, (u32)evt_mario_bgmode_off,
    0x00000021, 0x00020056, 0xFE363C80, (u32)&BeroEX, 0x00020056, 0xFE363C81,
    (u32)&BeroEY, 0x00020056, 0xFE363C82, (u32)&BeroEZ, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_MARIO_DOKAN1_802c2f28, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x0001005E,
    (u32)evt_bero_info_run_pipe_sub00, 0x0002005B, (u32)evt_bero_exec_get, 0xFE363C80, 0x0002001F, 0xFE363C80,
    0x00000004, 0x0003005B, (u32)evt_bero_exec_onoff, 0x00000001, 0x00000002, 0x00000021,
    0x0001005B, (u32)evt_bero_disppos_load, 0x0002005B, (u32)evt_mario_cont_onoff, 0x00000001, 0x0002005B,
    (u32)evt_mario_key_onoff, 0x00000001, 0x00020032, 0xFE363C80, 0xFE363C83, 0x0001005E,
    (u32)bero_case_switch_off, 0x0001005C, (u32)evt_bero_nonride_check, 0x0001005B, (u32)evt_bero_cam_shift_init, 0x0003005B,
    (u32)evt_cam3d_evt_off, 0x000001F4, 0x0000000B, 0x00000020, 0x0001005B, (u32)evt_bero_disppos_load,
    0x0001005B, (u32)evt_mario_paper_pose_off, 0x00020032, 0xFE363C80, 0xFE363C84, 0x0002004E,
    0xFE363C80, 0x00001000, 0x00020019, 0xFE363C80, 0x00000000, 0x0001005B,
    (u32)evt_mario_bgmode_on, 0x00000020, 0x0001005B, (u32)evt_mario_bgmode_off, 0x00000021, 0x00000021,
    0x0002005B, (u32)evt_npc_change_fbat_mode, 0x00000001, 0x0001005B, (u32)evt_npc_start_for_event, 0x0003005B,
    (u32)evt_bero_exec_onoff, 0x00000000, 0x00010000, 0x0003005B, (u32)evt_bero_exec_onoff, 0x00000001,
    0x00000001, 0x00000002, 0x00000001
};

u32 bero_case_exec_pipe[178] = {
    0x0002005B, (u32)evt_mario_get_mode, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x00000002, 0x00000021, 0x0002005B, (u32)evt_mario_key_onoff, 0x00000000, 0x0001005E,
    (u32)bero_case_init_check, 0x0002005B, (u32)evt_mario_key_onoff, 0x00000001, 0x00020018, 0xFE363C80,
    0x00000001, 0x00000002, 0x00000021, 0x0002005B, (u32)evt_mario_key_onoff, 0x00000000,
    0x0001005B, (u32)evt_mario_wait_landon, 0x0002005B, (u32)evt_mario_cont_onoff, 0x00000000, 0x00020018,
    0xFE363C8D, 0x00000000, 0x0001005C, (u32)bero_case_exec_pipe_sub, 0x00000002, 0x00000021,
    0x0001005B, (u32)evt_mario_cam_off, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_MARIO_DOKAN1_802c2f28, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0x00010022, 0xFE363C85, 0x00010024,
    0x00000006, 0x0002005B, (u32)evt_mario_paper_pose_dokan, 0x00000005, 0x0002005B, (u32)evt_bero_move_mario_speed,
    0xF24B6A80, 0x00010024, 0x00000002, 0x0002005B, (u32)evt_mario_paper_pose_dokan, 0x00000005,
    0x0002005B, (u32)evt_bero_move_mario_speed, 0xF24B6A80, 0x00010024, 0x00000008, 0x0002005B,
    (u32)evt_mario_paper_pose_dokan, 0x00000002, 0x0002005B, (u32)evt_bero_move_mario_speed, 0xF24B6A80, 0x00010024,
    0x00000009, 0x0002005B, (u32)evt_mario_paper_pose_dokan, 0x00000003, 0x0002005B, (u32)evt_bero_move_mario_speed,
    0xF24B6A80, 0x00000031, 0x0002001F, 0xFE363C84, 0x00002000, 0x00010022,
    0xFE363C85, 0x00010024, 0x00000006, 0x0006005B, (u32)evt_fade_set_mapchange_type, 0x00000001,
    0x0000002D, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x0006005B, (u32)evt_fade_set_mapchange_type,
    0x00000000, 0x0000002D, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010024,
    0x00000002, 0x0006005B, (u32)evt_fade_set_mapchange_type, 0x00000001, 0x0000002F, 0x00000000,
    0xFFFFFFFF, 0xFFFFFFFF, 0x0006005B, (u32)evt_fade_set_mapchange_type, 0x00000000, 0x0000002F,
    0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010024, 0x00000008, 0x0006005B,
    (u32)evt_fade_set_mapchange_type, 0x00000001, 0x00000029, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF,
    0x0006005B, (u32)evt_fade_set_mapchange_type, 0x00000000, 0x00000029, 0x00000000, 0xFFFFFFFF,
    0xFFFFFFFF, 0x00010024, 0x00000009, 0x0006005B, (u32)evt_fade_set_mapchange_type, 0x00000001,
    0x0000002B, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x0006005B, (u32)evt_fade_set_mapchange_type,
    0x00000000, 0x0000002B, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000031,
    0x00000021, 0x0003005B, (u32)evt_bero_exec_onoff, 0x00000000, 0x00000008, 0x00010009,
    0x00000002, 0x0002005B, (u32)evt_bero_1stcheck, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000001, 0x0001000A, 0x000003E8, 0x00000021, 0x0003005B, (u32)evt_bero_mapchange,
    0xFE363C8D, 0xFE363C8E, 0x00000002, 0x00000001
};

u32 bero_case_exec_gate[45] = {
    0x0002005B, (u32)evt_mario_key_onoff, 0x00000000, 0x0001005E, (u32)bero_case_init_check, 0x0002005B,
    (u32)evt_mario_key_onoff, 0x00000001, 0x00020018, 0xFE363C80, 0x00000001, 0x00000002,
    0x00000021, 0x0002005B, (u32)evt_mario_key_onoff, 0x00000000, 0x0003005B, (u32)evt_bero_exec_onoff,
    0x00000000, 0x00010000, 0x0003005B, (u32)evt_bero_exec_onoff, 0x00000001, 0x00000001,
    0x0003005B, (u32)evt_bero_exec_onoff, 0x00000000, 0x00000008, 0x00010009, 0x00000002,
    0x0002005B, (u32)evt_bero_1stcheck, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001,
    0x0001000A, 0x000003E8, 0x00000021, 0x0003005B, (u32)evt_bero_mapchange, 0xFE363C8D,
    0xFE363C8E, 0x00000002, 0x00000001
};

u32 bero_case_entry[100] = {
    0x00020055, 0xFE363C82, (u32)&BeroNUM, 0x00020032, 0xFE363C81, 0x00000000,
    0x00010005, 0x00000000, 0x0002005B, (u32)evt_bero_get_info_num, 0xFE363C81, 0x00020018,
    0xFE363C89, 0xFFFFFFFF, 0x00020032, 0xFE363C80, 0xFE363C84, 0x0002004E,
    0xFE363C80, 0x00000FFF, 0x00010022, 0xFE363C80, 0x00010024, 0x00000F00,
    0x0007005B, (u32)evt_run_case_evt_bero, 0xFE363C8A, 0x00000001, 0xFE363C83, 0x00000000,
    (u32)bero_case_exec_none, 0xFE363C8F, 0x00010024, 0x00000000, 0x0005005B, (u32)evt_hitobj_attr_onoff,
    0x00000000, 0x00000001, 0xFE363C83, 0x00800000, 0x0007005B, (u32)evt_run_case_evt_bero,
    0xFE363C8A, 0x00000001, 0xFE363C83, 0x00000000, (u32)bero_case_exec_road, 0xFE363C8F,
    0x00010024, 0x00000001, 0x0007005B, (u32)evt_run_case_evt_bero, 0xFE363C8A, 0x00000001,
    0xFE363C83, 0x00000000, (u32)bero_case_exec_door, 0xFE363C8F, 0x00010024, 0x00000002,
    0x0007005B, (u32)evt_run_case_evt_bero, 0xFE363C8A, 0x00000001, 0xFE363C83, 0x00000000,
    (u32)bero_case_exec_pipe, 0xFE363C8F, 0x00010024, 0x00000003, 0x0007005B, (u32)evt_run_case_evt_bero,
    0xFE363C8A, 0x00000001, 0xFE363C83, 0x00000000, (u32)bero_case_exec_gate, 0xFE363C8F,
    0x00000031, 0x0003005B, (u32)evt_bero_case_id_save, 0xFE363C81, 0xFE363C8F, 0x0000006B,
    0x0002005B, (u32)evt_bero_switch_on, 0xFE363C81, 0x0000006D, 0x00000021, 0x00020035,
    0xFE363C81, 0x00000001, 0x00020018, 0xFE363C81, 0xFE363C82, 0x00000007,
    0x00000021, 0x00000006, 0x00000002, 0x00000001
};

u32 bero_case_entry_sub[71] = {
    0x00020032, 0xFE363C80, 0xFE363C84, 0x0002004E, 0xFE363C80, 0x00000FFF,
    0x00010022, 0xFE363C80, 0x00010024, 0x00000F00, 0x0007005B, (u32)evt_run_case_evt_bero,
    0xFE363C8A, 0x00000001, 0xFE363C83, 0x00000000, (u32)bero_case_exec_none, 0xFE363C8F,
    0x00010024, 0x00000000, 0x0005005B, (u32)evt_hitobj_attr_onoff, 0x00000000, 0x00000001,
    0xFE363C83, 0x00800000, 0x0007005B, (u32)evt_run_case_evt_bero, 0xFE363C8A, 0x00000001,
    0xFE363C83, 0x00000000, (u32)bero_case_exec_road, 0xFE363C8F, 0x00010024, 0x00000001,
    0x0007005B, (u32)evt_run_case_evt_bero, 0xFE363C8A, 0x00000001, 0xFE363C83, 0x00000000,
    (u32)bero_case_exec_door, 0xFE363C8F, 0x00010024, 0x00000002, 0x0007005B, (u32)evt_run_case_evt_bero,
    0xFE363C8A, 0x00000001, 0xFE363C83, 0x00000000, (u32)bero_case_exec_pipe, 0xFE363C8F,
    0x00010024, 0x00000003, 0x0007005B, (u32)evt_run_case_evt_bero, 0xFE363C8A, 0x00000001,
    0xFE363C83, 0x00000000, (u32)bero_case_exec_gate, 0xFE363C8F, 0x00000031, 0x0003005B,
    (u32)evt_bero_case_id_save, 0xFE363C81, 0xFE363C8F, 0x00000002, 0x00000001
};

u32 bero_case_switch_on[19] = {
    0x0002005B, (u32)evt_bero_switch_on, 0xFE363C80, 0x0002005B, (u32)evt_bero_get_info_num, 0xFE363C81,
    0x00020018, 0xFE363C80, 0x00000000, 0x00010071, (u32)(char*)str_sjis_000002f4, 0x0001005E,
    (u32)bero_case_entry_sub, 0x00000020, 0x00010071, (u32)(char*)str_sjis_00000300, 0x00000021, 0x00000002,
    0x00000001
};

u32 bero_case_switch_off[31] = {
    0x0002005B, (u32)evt_bero_switch_off, 0xFE363C80, 0x0002005B, (u32)evt_bero_get_info_num, 0xFE363C81,
    0x00020018, 0xFE363C80, 0x00000000, 0x00010071, (u32)(char*)str_sjis_00000330, 0x0003005B,
    (u32)evt_bero_case_id_load, 0xFE363C81, 0xFE363C8F, 0x0003005B, (u32)evt_del_case_evt, 0x00000000,
    0xFE363C8F, 0x0005005B, (u32)&evt_hitobj_attr_onoff, 0x00000000, 0x00000000, 0xFE363C83,
    0x00800000, 0x00000020, 0x00010071, (u32)(char*)str_sjis_00000344, 0x00000021, 0x00000002,
    0x00000001
};

u32 evt_bero_info_run_road[41] = {
    0x00020056, 0xFE363C8D, (u32)&BeroEX, 0x00020056, 0xFE363C8E, (u32)&BeroEY,
    0x00020056, 0xFE363C8F, (u32)&BeroEZ, 0x0003005B, (u32)evt_party_set_homing_dist, 0x00000000,
    0xF24ACA80, 0x0003005B, (u32)evt_party_set_homing_dist, 0x00000001, 0xF24ACA80, 0x0004005B,
    (u32)evt_mario_mov_pos2, 0xFE363C8D, 0xFE363C8F, 0xF24CD280, 0x0002005B, (u32)evt_mario_key_onoff,
    0x00000001, 0x0002005B, (u32)evt_party_run, 0x00000000, 0x0002005B, (u32)evt_party_run,
    0x00000001, 0x0003005B, (u32)evt_party_set_homing_dist, 0x00000000, 0xF24B4280, 0x0003005B,
    (u32)evt_party_set_homing_dist, 0x00000001, 0xF24B4280, 0x00000002, 0x00000001
};

u32 evt_bero_info_run_door[47] = {
    0x0001005B, (u32)evt_bero_mario_go_init, 0x0001005C, (u32)bero_play_enter_anime, 0x0001005B, (u32)evt_bero_mario_go_wait,
    0x00020056, 0xFE363C8D, (u32)&BeroEX, 0x00020056, 0xFE363C8E, (u32)&BeroEY,
    0x00020056, 0xFE363C8F, (u32)&BeroEZ, 0x0003005B, (u32)evt_party_set_homing_dist, 0x00000000,
    0xF24ACA80, 0x0003005B, (u32)evt_party_set_homing_dist, 0x00000001, 0xF24ACA80, 0x0004005B,
    (u32)evt_mario_party_bero_move, 0xFE363C8D, 0xFE363C8F, 0xF24CD280, 0x0002005B, (u32)evt_mario_key_onoff,
    0x00000001, 0x0002005B, (u32)evt_party_run, 0x00000000, 0x0002005B, (u32)evt_party_run,
    0x00000001, 0x0003005B, (u32)evt_party_set_homing_dist, 0x00000000, 0xF24B4280, 0x0003005B,
    (u32)evt_party_set_homing_dist, 0x00000001, 0xF24B4280, 0x00000002, 0x00000001
};

u32 evt_bero_info_run_pipe_sub00[51] = {
    0x0002005B, (u32)evt_party_stop, 0x00000000, 0x0002005B, (u32)evt_party_stop, 0x00000001,
    0x00010022, 0xFE363C85, 0x00010024, 0x00000006, 0x0002005B, (u32)evt_mario_set_dispdir,
    0x000000B4, 0x0002005B, (u32)evt_mario_paper_pose_dokan, 0x00000004, 0x0002005B, (u32)evt_bero_move_mario_speed,
    0xF24B6A80, 0x00010024, 0x00000002, 0x0002005B, (u32)evt_mario_set_dispdir, 0x00000000,
    0x0002005B, (u32)evt_mario_paper_pose_dokan, 0x00000004, 0x0002005B, (u32)evt_bero_move_mario_speed, 0xF24B6A80,
    0x00010024, 0x00000009, 0x0002005B, (u32)evt_mario_paper_pose_dokan, 0x00000001, 0x0002005B,
    (u32)evt_bero_move_mario_speed, 0xF24B6A80, 0x00010024, 0x00000008, 0x0002005B, (u32)evt_mario_paper_pose_dokan,
    0x00000000, 0x0002005B, (u32)evt_bero_move_mario_speed, 0xF24B6A80, 0x00000031, 0x0001005B,
    (u32)evt_mario_dokan_end, 0x00000002, 0x00000001
};

u32 evt_bero_info_run_pipe[53] = {
    0x00020056, 0xFE363C80, (u32)&BeroEX, 0x00020056, 0xFE363C81, (u32)&BeroEY,
    0x00020056, 0xFE363C82, (u32)&BeroEZ, 0x0004005B, (u32)evt_cam_road_reset2, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0001005B, (u32)evt_mario_cam_off, 0x0002005B, (u32)evt_mario_cont_onoff,
    0x00000000, 0x0001005B, (u32)evt_fade_end_wait, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_MARIO_DOKAN1_802c2f28,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x0001005E, (u32)evt_bero_info_run_pipe_sub00,
    0x0002005B, (u32)evt_mario_cont_onoff, 0x00000001, 0x00020032, 0xFE363C80, 0xFE363C83,
    0x0001005E, (u32)bero_case_switch_off, 0x0001005C, (u32)evt_bero_nonride_check, 0x0001005B, (u32)evt_mario_cam_on,
    0x0002005B, (u32)evt_mario_key_onoff, 0x00000001, 0x00000002, 0x00000001
};

u32 evt_bero_info_run[249] = {
    0x0001005B, (u32)evt_bero_get_into_info, 0x0002005B, (u32)evt_bero_set_now_number, 0xFE363C81, 0x0001005B,
    (u32)evt_bero_read_mario_pera, 0x0002005B, (u32)evt_sub_get_system_flag, 0xFE363C80, 0x0002001E, 0xFE363C80,
    0x00000001, 0x00010071, (u32)(char*)str_sjis_00000384, 0x0003005B, (u32)evt_bero_exec_onoff, 0x00000000,
    0x00000001, 0x00010009, 0x00000001, 0x0002005B, (u32)evt_bero_exec_get, 0xFE363C80,
    0x0002001F, 0xFE363C80, 0x00000004, 0x0003005B, (u32)evt_bero_exec_onoff, 0x00000000,
    0x00000002, 0x00000021, 0x00010009, 0x00000001, 0x0002005B, (u32)evt_bero_exec_get,
    0xFE363C80, 0x0002001F, 0xFE363C80, 0x00000004, 0x0003005B, (u32)evt_bero_exec_onoff,
    0x00000001, 0x00000002, 0x00000021, 0x0002005B, (u32)&evt_npc_change_fbat_mode, 0x00000001,
    0x0001005B, (u32)&evt_npc_start_for_event, 0x0003005B, (u32)evt_bero_exec_onoff, 0x00000001, 0x00000001,
    0x0003005B, (u32)evt_bero_exec_onoff, 0x00000000, 0x00010000, 0x0001005E, (u32)bero_case_entry,
    0x00000002, 0x00000021, 0x0002005B, (u32)evt_bero_exec_get, 0xFE363C80, 0x00020018,
    0xFE363C80, 0x00000001, 0x00000002, 0x00000021, 0x0003005B, (u32)evt_bero_exec_onoff,
    0x00000000, 0x00000001, 0x0002005B, (u32)&evt_mario_key_onoff, 0x00000000, 0x00020032,
    0xFE363C80, 0xFE363C84, 0x0002004E, 0xFE363C80, 0x00001000, 0x00020019,
    0xFE363C80, 0x00000000, 0x00010071, (u32)(char*)str_sjis_000003a8, 0x0001005B, (u32)&evt_mario_bgmode_on,
    0x0002005B, (u32)evt_mario_kill_party, 0x00000000, 0x0002005B, (u32)evt_mario_kill_party, 0x00000001,
    0x0001005B, (u32)evt_mario_cam_y_off, 0x00000020, 0x00010071, (u32)(char*)str_sjis_000003cc, 0x0001005B,
    (u32)&evt_mario_bgmode_off, 0x00020032, 0xFE363C80, 0xFE363C84, 0x0002004E, 0xFE363C80,
    0x00000FFF, 0x00020018, 0xFE363C80, 0x00000002, 0x0002005B, (u32)evt_mario_set_prev_party_dokan,
    0x00000000, 0x0002005B, (u32)evt_mario_set_prev_party_dokan, 0x00000001, 0x00000020, 0x0002005B,
    (u32)evt_mario_set_prev_party, 0x00000000, 0x0002005B, (u32)evt_mario_set_prev_party, 0x00000001, 0x00000021,
    0x00000021, 0x00020032, 0xFE363C80, 0xFE363C84, 0x0002004E, 0xFE363C80,
    0x00000FFF, 0x00020018, 0xFE363C80, 0x00000002, 0x00010022, 0xFE363C85,
    0x00010024, 0x00000006, 0x0002005B, (u32)evt_mario_dokan_prepare, 0x00000004, 0x00010024,
    0x00000002, 0x0002005B, (u32)evt_mario_dokan_prepare, 0x00000004, 0x00010024, 0x00000009,
    0x0002005B, (u32)evt_mario_dokan_prepare, 0x00000001, 0x00010024, 0x00000008, 0x0002005B,
    (u32)evt_mario_dokan_prepare, 0x00000000, 0x00000031, 0x00000021, 0x0001005E, (u32)evt_bero_initial_pos,
    0x0002001F, 0xFE363C84, 0x00002000, 0x00020032, 0xFE363C80, 0xFE363C84,
    0x0002004E, 0xFE363C80, 0x00000FFF, 0x00020018, 0xFE363C80, 0x00000002,
    0x00010022, 0xFE363C85, 0x00010024, 0x00000006, 0x0006005B, (u32)&evt_fade_set_mapchange_type,
    0x00000001, 0xFFFFFFFF, 0xFFFFFFFF, 0x0000002E, 0x00000000, 0x0006005B,
    (u32)&evt_fade_set_mapchange_type, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x0000002E, 0x00000000,
    0x00010024, 0x00000002, 0x0006005B, (u32)&evt_fade_set_mapchange_type, 0x00000001, 0xFFFFFFFF,
    0xFFFFFFFF, 0x00000030, 0x00000000, 0x0006005B, (u32)&evt_fade_set_mapchange_type, 0x00000000,
    0xFFFFFFFF, 0xFFFFFFFF, 0x00000030, 0x00000000, 0x00010024, 0x00000008,
    0x0006005B, (u32)&evt_fade_set_mapchange_type, 0x00000001, 0xFFFFFFFF, 0xFFFFFFFF, 0x0000002A,
    0x00000000, 0x0006005B, (u32)&evt_fade_set_mapchange_type, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF,
    0x0000002A, 0x00000000, 0x00010024, 0x00000009, 0x0006005B, (u32)&evt_fade_set_mapchange_type,
    0x00000001, 0xFFFFFFFF, 0xFFFFFFFF, 0x0000002C, 0x00000000, 0x0006005B,
    (u32)&evt_fade_set_mapchange_type, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x0000002C, 0x00000000,
    0x00000031, 0x00000021, 0x00000021, 0x00010071, (u32)(char*)str_sjis_000003f0, 0x0001005C,
    (u32)evt_bero_mario_enter, 0x00000002, 0x00000001
};

u32 evt_bero_initial_pos[158] = {
    0x0001005B, (u32)evt_bero_get_into_info, 0x00020032, 0xFE363C80, 0xFE363C84, 0x0002004E,
    0xFE363C80, 0x00000FFF, 0x00010022, 0xFE363C80, 0x00010024, 0x00000F00,
    0x00010024, 0x00000003, 0x0003005B, (u32)bero_get_position_normal, 0x00000000, 0xF24A7A80,
    0x00020056, 0xFE363C8A, (u32)&BeroSX, 0x00020056, 0xFE363C8B, (u32)&BeroSY,
    0x00020056, 0xFE363C8C, (u32)&BeroSZ, 0x0004005B, (u32)evt_mario_set_pos, 0xFE363C8A,
    0xFE363C8B, 0xFE363C8C, 0x0005005B, (u32)evt_party_set_pos, 0x00000000, 0xFE363C8A,
    0xFE363C8B, 0xFE363C8C, 0x0005005B, (u32)evt_party_set_pos, 0x00000001, 0xFE363C8A,
    0xFE363C8B, 0xFE363C8C, 0x0000002E, 0x00010024, 0x00000000, 0x0003005B,
    (u32)evt_bero_get_info_length, 0xFE363C81, 0xFE363C8A, 0x0003005B, (u32)bero_get_position_normal, 0x00000001,
    0xFE363C8A, 0x00020056, 0xFE363C8A, (u32)&BeroSX, 0x00020056, 0xFE363C8B,
    (u32)&BeroSY, 0x00020056, 0xFE363C8C, (u32)&BeroSZ, 0x0004005B, (u32)evt_mario_set_pos,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x0005005B, (u32)evt_party_set_pos, 0x00000000,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x0005005B, (u32)evt_party_set_pos, 0x00000001,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00010024, 0x00000001, 0x0003005B,
    (u32)evt_bero_get_info_length, 0xFE363C81, 0xFE363C8A, 0x0003005B, (u32)bero_get_position_normal, 0x00000001,
    0xFE363C8A, 0x00020056, 0xFE363C8A, (u32)&BeroSX, 0x00020056, 0xFE363C8B,
    (u32)&BeroSY, 0x00020056, 0xFE363C8C, (u32)&BeroSZ, 0x0004005B, (u32)evt_mario_set_pos,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x0005005B, (u32)evt_party_set_pos, 0x00000000,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x0005005B, (u32)evt_party_set_pos, 0x00000001,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00010024, 0x00000002, 0x0003005B,
    (u32)evt_bero_get_info_length, 0xFE363C81, 0xFE363C8A, 0x0003005B, (u32)bero_get_position_pipe_pure, 0x00000001,
    0xFE363C8A, 0x00020056, 0xFE363C8A, (u32)&BeroSX, 0x00020056, 0xFE363C8B,
    (u32)&BeroSY, 0x00020056, 0xFE363C8C, (u32)&BeroSZ, 0x0004005B, (u32)evt_mario_set_pos,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x0005005B, (u32)evt_party_set_pos, 0x00000000,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x0005005B, (u32)evt_party_set_pos, 0x00000001,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000031, 0x0001005B, (u32)evt_cam_road_reset,
    0x00000002, 0x00000001
};

u32 evt_bero_mario_enter[95] = {
    0x0002005B, (u32)evt_seq_wait, 0x00000002, 0x0002005B, (u32)evt_npc_change_fbat_mode, 0x00000007,
    0x0001005B, (u32)evt_npc_stop_for_event, 0x0001005E, 0xFE363C87, 0x00020018, 0xFE363C80,
    0x00000001, 0x0002005B, (u32)evt_mario_key_onoff, 0x00000001, 0x00010004, 0x00000064,
    0x00000021, 0x0002005B, (u32)evt_bero_exec_get, 0xFE363C80, 0x0002001F, 0xFE363C80,
    0x00000004, 0x0003005B, (u32)evt_bero_exec_onoff, 0x00000000, 0x00000002, 0x00000021,
    0x0001005B, (u32)evt_bero_get_into_info, 0x00020032, 0xFE363C80, 0xFE363C84, 0x0002004E,
    0xFE363C80, 0x00000FFF, 0x00010022, 0xFE363C80, 0x00010024, 0x00000F00,
    0x0001005E, (u32)evt_bero_info_run_none, 0x00010024, 0x00000000, 0x0001005E, (u32)evt_bero_info_run_road,
    0x00010024, 0x00000001, 0x0001005E, (u32)evt_bero_info_run_door, 0x00010024, 0x00000002,
    0x0001005E, (u32)evt_bero_info_run_pipe, 0x00010024, 0x00000003, 0x0001005E, (u32)evt_bero_info_run_gate,
    0x0000002A, 0x0002005B, (u32)evt_mario_key_onoff, 0x00000001, 0x00000031, 0x0002005B,
    (u32)evt_bero_exec_get, 0xFE363C80, 0x0002001F, 0xFE363C80, 0x00000004, 0x0003005B,
    (u32)evt_bero_exec_onoff, 0x00000001, 0x00000002, 0x00000021, 0x00010003, 0x00000064,
    0x0002005B, (u32)evt_npc_change_fbat_mode, 0x00000001, 0x0001005B, (u32)evt_npc_start_for_event, 0x0001005E,
    (u32)bero_case_entry, 0x0003005B, (u32)evt_bero_exec_onoff, 0x00000001, 0x00000001, 0x0003005B,
    (u32)evt_bero_exec_onoff, 0x00000000, 0x00010000, 0x00000002, 0x00000001
};

/* Recovered target evt_bero ASCII .rodata strings, defined after all consumers. */
/* Recovered target evt_bero zero vectors, interleaved in target .rodata order. */
const RoadVec vec3_802c2c48 = {0.0f, 0.0f, 0.0f};
const RoadVec vec3_802c2c54 = {0.0f, 0.0f, 0.0f};
/* Recovered target evt_bero Shift-JIS strings with ASCII-safe source identities. */
const char str_sjis_00000018[15] = {
    0x93, 0x79, 0x8A, 0xC7, 0x82, 0xA9, 0x82, 0xE7, 0x8D, 0x7E, 0x82, 0xE8,
    0x82, 0xBD, 0x00
};
const char str_SFX_DOOR_OPEN1_802c2c70[] = "SFX_DOOR_OPEN1";
const char str_SFX_DOOR_OPEN_WOOD1_802c2c80[] = "SFX_DOOR_OPEN_WOOD1";
const char str_SFX_DOOR_OPEN_WOOD2_802c2c94[] = "SFX_DOOR_OPEN_WOOD2";
const char str_SFX_DOOR_OPEN_WOOD3_802c2ca8[] = "SFX_DOOR_OPEN_WOOD3";
const char str_SFX_DOOR_OPEN_IRON1_802c2cbc[] = "SFX_DOOR_OPEN_IRON1";
const char str_SFX_DOOR_OPEN_IRON2_802c2cd0[] = "SFX_DOOR_OPEN_IRON2";
const char str_SFX_DOOR_OPEN_GRILLE_802c2ce4[] = "SFX_DOOR_OPEN_GRILLE1";
const char str_SFX_DOOR_OPEN_SLIDE1_802c2cfc[] = "SFX_DOOR_OPEN_SLIDE1";
const char str_SFX_DOOR_OPEN_TURN1_802c2d14[] = "SFX_DOOR_OPEN_TURN1";
const char str_SFX_DOOR_OPEN_ELEC1_802c2d28[] = "SFX_DOOR_OPEN_ELEC1";
const char str_SFX_DOOR_OPEN_STONE1_802c2d3c[] = "SFX_DOOR_OPEN_STONE1";
const char str_SFX_DOOR_OPEN_CASTLE_802c2d54[] = "SFX_DOOR_OPEN_CASTLE1";
const char str_SFX_DOOR_OPEN_CASTLE_802c2d6c[] = "SFX_DOOR_OPEN_CASTLE2";
const char str_SFX_DOOR_OPEN_METAL1_802c2d84[] = "SFX_DOOR_OPEN_METAL1";
const char str_SFX_STG3_AUTO_DOOR1_802c2d9c[] = "SFX_STG3_AUTO_DOOR1";
const char str_SFX_STG3_LOCKER_OPEN_802c2db0[] = "SFX_STG3_LOCKER_OPEN1";
const char str_SFX_DOOR_OPEN_SLIDE1_802c2dc8[] = "SFX_DOOR_OPEN_SLIDE1_3";
const char str_SFX_DOOR_SHUT1_802c2de0[] = "SFX_DOOR_SHUT1";
const char str_SFX_DOOR_SHUT_WOOD1_802c2df0[] = "SFX_DOOR_SHUT_WOOD1";
const char str_SFX_DOOR_SHUT_WOOD2_802c2e04[] = "SFX_DOOR_SHUT_WOOD2";
const char str_SFX_DOOR_SHUT_IRON1_802c2e18[] = "SFX_DOOR_SHUT_IRON1";
const char str_SFX_DOOR_SHUT_IRON2_802c2e2c[] = "SFX_DOOR_SHUT_IRON2";
const char str_SFX_DOOR_SHUT_GRILLE_802c2e40[] = "SFX_DOOR_SHUT_GRILLE1";
const char str_SFX_DOOR_SHUT_SLIDE1_802c2e58[] = "SFX_DOOR_SHUT_SLIDE1";
const char str_SFX_DOOR_SHUT_TURN1_802c2e70[] = "SFX_DOOR_SHUT_TURN1";
const char str_SFX_DOOR_SHUT_ELEC1_802c2e84[] = "SFX_DOOR_SHUT_ELEC1";
const char str_SFX_DOOR_SHUT_CASTLE_802c2e98[] = "SFX_DOOR_SHUT_CASTLE1";
const char str_SFX_DOOR_SHUT_CASTLE_802c2eb0[] = "SFX_DOOR_SHUT_CASTLE2";
const char str_SFX_DOOR_SHUT_METAL1_802c2ec8[] = "SFX_DOOR_SHUT_METAL1";
const char str_SFX_STG3_LOCKER_CLOS_802c2ee0[] = "SFX_STG3_LOCKER_CLOSE1";
const char str_SFX_DOOR_SHUT_SLIDE1_802c2ef8[] = "SFX_DOOR_SHUT_SLIDE1_3";
const RoadVec vec3_802c2f10 = {0.0f, 0.0f, 0.0f};
const RoadVec vec3_802c2f1c = {0.0f, 0.0f, 0.0f};
const char str_SFX_MARIO_DOKAN1_802c2f28[] = "SFX_MARIO_DOKAN1";
const char str_sjis_000002f4[9] = {
    0x83, 0x78, 0x83, 0x8D, 0x93, 0x6F, 0x98, 0x5E, 0x00
};
const char str_sjis_00000300[47] = {
    0x83, 0x78, 0x83, 0x8D, 0x82, 0xCC, 0x8F, 0x6F, 0x82, 0xC4, 0x8D, 0x73,
    0x82, 0xAD, 0x83, 0x66, 0x81, 0x5B, 0x83, 0x5E, 0x82, 0xCC, 0x93, 0xF1,
    0x8F, 0x64, 0x93, 0x6F, 0x98, 0x5E, 0x96, 0x68, 0x8E, 0x7E, 0x82, 0xF0,
    0x82, 0xB5, 0x82, 0xC4, 0x82, 0xA2, 0x82, 0xDC, 0x82, 0xB7, 0x00
};
const char str_sjis_00000330[17] = {
    0x83, 0x78, 0x83, 0x8D, 0x8D, 0xED, 0x8F, 0x9C, 0x82, 0xB5, 0x82, 0xDC,
    0x82, 0xB5, 0x82, 0xBD, 0x00
};
const char str_sjis_00000344[61] = {
    0x83, 0x78, 0x83, 0x8D, 0x82, 0xCC, 0x8D, 0xED, 0x8F, 0x9C, 0x97, 0x5C,
    0x96, 0xF1, 0x82, 0xE0, 0x82, 0xB5, 0x82, 0xAD, 0x82, 0xCD, 0x83, 0x78,
    0x83, 0x8D, 0x82, 0xCC, 0x8F, 0x6F, 0x82, 0xC4, 0x8D, 0x73, 0x82, 0xAD,
    0x83, 0x66, 0x81, 0x5B, 0x83, 0x5E, 0x82, 0xCC, 0x93, 0xF1, 0x8F, 0x64,
    0x8D, 0xED, 0x8F, 0x9C, 0x96, 0x68, 0x8E, 0x7E, 0x82, 0xC5, 0x82, 0xB7,
    0x00
};
const char str_sjis_00000384[35] = {
    0x83, 0x5A, 0x81, 0x5B, 0x83, 0x75, 0x83, 0x75, 0x83, 0x8D, 0x83, 0x62,
    0x83, 0x4E, 0x82, 0xA9, 0x82, 0xE7, 0x82, 0xCC, 0x83, 0x8D, 0x81, 0x5B,
    0x83, 0x68, 0x8E, 0x9E, 0x82, 0xCC, 0x8F, 0x88, 0x97, 0x9D, 0x00
};
const char str_sjis_000003a8[33] = {
    0x94, 0x77, 0x8C, 0x69, 0x82, 0xC9, 0x93, 0xFC, 0x82, 0xE9, 0x82, 0xC6,
    0x82, 0xAB, 0x82, 0xCC, 0x83, 0x7D, 0x83, 0x8A, 0x83, 0x49, 0x82, 0xCC,
    0x8F, 0x89, 0x8A, 0xFA, 0x8F, 0x88, 0x97, 0x9D, 0x00
};
const char str_sjis_000003cc[33] = {
    0x95, 0x81, 0x92, 0xCA, 0x82, 0xC9, 0x93, 0xFC, 0x82, 0xE9, 0x82, 0xC6,
    0x82, 0xAB, 0x82, 0xCC, 0x83, 0x7D, 0x83, 0x8A, 0x83, 0x49, 0x82, 0xCC,
    0x8F, 0x89, 0x8A, 0xFA, 0x8F, 0x88, 0x97, 0x9D, 0x00
};
const char str_sjis_000003f0[19] = {
    0x82, 0xD9, 0x82, 0xA9, 0x82, 0xCC, 0x8F, 0x89, 0x8A, 0xFA, 0x90, 0xDD,
    0x92, 0xE8, 0x8A, 0x4A, 0x8E, 0x6E, 0x00
};
const char str_sys_01_802c304c[] = "sys_01";
const char str_p_dokan_x_802c3060[] = "p_dokan_x";
const char str_p_dokan_y_802c306c[] = "p_dokan_y";

/* Recovered target evt_bero .sdata2 floats; consumers retain original extern f32 view. */
const f32 float_6p2832_804204e0 = 6.2831855f;
const f32 float_360_804204e4 = 360.0f;
const f32 float_0_804204e8 = 0.0f;
const f32 float_3_804204ec = 3.0f;
const f32 float_15_804204f0 = 15.0f;
const f32 float_13_804204f4 = 13.0f;
const f32 float_20_804204f8 = 20.0f;
const f32 float_1_804204fc = 1.0f;
const f32 float_37p5_80420500 = 37.5f;
const f32 float_10000_80420504 = 10000.0f;
const f32 float_neg1_80420508 = -1.0f;
const f32 float_1E06_8042050c = 1000000.0f;
const f32 float_0p125_80420510 = 0.125f;
const f32 float_180_80420514 = 180.0f;
const f32 float_30_80420518 = 30.0f;
/* Target object is f32 zero; its address is also used as an empty C string. */
const f32 float_0_8042051c = 0.0f;
const f32 float_60_80420520 = 60.0f;
