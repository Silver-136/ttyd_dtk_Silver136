#include "battle/battle_stage_object.h"

extern void* _battleWorkPointer;
void BattleAudience_Case_HaikeiSet(void);
s32 strcmp(const char*, const char*);
void evtDeleteID(s32 id);
s32 evtCheckID(s32 id);
void* evtEntry(void* script, s32 priority, s32 flags);
s32 evtGetValue(void* event, s32 value);
s32 evtSetValue(void* event, s32 target, s32 value);
void _mobj_shake_main(void* stageObject);
void _nozzle_type_init(void);
extern s32 _object_turn_event_A1;
extern s32 _object_turn_event_B;
extern s32 _nozzle_work_event_0;
extern s32 _nozzle_work_event_1;
extern s32 _nozzle_turn_event;
extern u8 _nozzle_at_base_mario[];
extern u8 _nozzle_at_base_monster[];
extern char str_if_body_802f4484[];
extern u32 dat_80422fc8;
extern u32 dat_80422fb8;
extern u32 dat_80422fbc;
extern u32 dat_80422fc0;
extern u32 dat_80422fc4;
extern s32 str_A1_80423028;
extern s32 str_A2_8042302c;
extern s32 str_B_80423030;
extern s32 str_C_80423034;
extern f32 float_0_80422fd0;
extern f32 float_5_80422fdc;

void BattleObjectConfig(void) {
    ;
}

void _rate_mix(s8* maxValue, s8* total, s8* current, s8* add) {
    s8 value = *current;

    if (value <= 0) {
        return;
    }
    if (value > *maxValue) {
        *maxValue = value;
    }
    *total += *add;
}

s32 _bgset_audience_bgset_access(void) {
    BattleAudience_Case_HaikeiSet();
    return 2;
}

void* BattleGetObjectPtr(s32 id) {
    s32 offset;
    void* battleWork;
    s32 i;
    void* obj;

    offset = 0;
    battleWork = _battleWorkPointer;
    for (i = 0; i < 0x20; i++) {
        obj = (void*)((s32)battleWork + offset + 0x1715C);
        if (*(s32*)obj == id) {
            return obj;
        }
        offset += 0x7C;
    }
    return 0;
}

void* BattleSearchObjectPtr(const char* name) {
    s32 offset;
    void* battleWork;
    s32 i;

    offset = 0;
    battleWork = _battleWorkPointer;
    for (i = 0; i < 0x20; i++) {
        s32 addr = offset + 0x10000;
        void* obj = (void*)((s32)battleWork + addr + 0x715C);
        if (*(s32*)obj != 0 && strcmp(**(const char***)((s32)obj + 0x64), name) == 0) {
            return obj;
        }
        offset += 0x7C;
    }
    return 0;
}


u8 _object_fall_attack(void) {
    return 0;
}


u8 BattleStageObjectInit(void) {
    return 0;
}


u8 _nozzle_work_effect(void) {
    return 0;
}


void BattleStageObjectMain(void) {
    ;
}


u32 _set_mobj_rotate_x(void* event, s32 isFirstCall) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* event, s32 value);
    extern void _mobj_shake_main(void* stageObject);
    extern void psndSFXOn_3D(char* name, void* pos);
    extern f32 intpl_sub(s32 type, f32 start, f32 end, s32 current, s32 duration);
    extern char vec3_802f42b8[];
    extern f32 float_0_80422fd0;
    extern f32 float_90_80422fd4;
    s32 type;
    void* battleWork;
    void* base;
    char* snd0;
    char* snd1;
    s32 active;
    s32 i;
    s32 offset;
    void* obj;
    void* data;
    s32 timer;
    f32 angle;
    s32 value;
    f32 pos[3];

    type = evtGetValue(event, **(s32**)((s32)event + 0x18));
    battleWork = _battleWorkPointer;
    base = (void*)((s32)battleWork + 0x20000);
    active = 0;

    if (isFirstCall != 0) {
        if (type == 2) {
            *(s32*)((s32)event + 0x7C) = 0;
            *(s32*)((s32)event + 0x80) = 0x3C;
            *(s32*)((s32)event + 0x84) = 0;
        } else if (type >= 0 && type < 2) {
            offset = 0;
            for (i = 0; i < 0x20; i++) {
                obj = (void*)((s32)battleWork + offset + 0x1715C);
                if (*(s32*)obj > 0 && *(u16*)((s32)*(void**)((s32)obj + 0x64) + 6) == type) {
                    *(u8*)((s32)obj + 0x70) = *(u8*)((s32)*(void**)((s32)obj + 0x64) + 0x14);
                    *(u8*)((s32)obj + 0x71) = 0;
                }
                offset += 0x7C;
            }
        }
    }

    if (type >= 0 && type < 2) {
        snd0 = vec3_802f42b8 + 0x240;
        snd1 = vec3_802f42b8 + 0x258;
        offset = 0;
        for (i = 0; i < 0x20; i++) {
            obj = (void*)((s32)battleWork + offset + 0x1715C);
            if (*(s32*)obj > 0 && *(u16*)((s32)*(void**)((s32)obj + 0x64) + 6) == type) {
                if (*(u8*)((s32)obj + 0x70) != 0) {
                    *(u8*)((s32)obj + 0x70) -= 1;
                    active = 1;
                    _mobj_shake_main(obj);
                } else {
                    if (*(u8*)((s32)obj + 0x71) == 0) {
                        if (type == 0) {
                            psndSFXOn_3D(snd0, (void*)((s32)obj + 4));
                        } else {
                            psndSFXOn_3D(snd1, (void*)((s32)obj + 4));
                        }
                    }
                    *(u8*)((s32)obj + 0x71) += 1;
                    data = *(void**)((s32)obj + 0x64);
                    angle = intpl_sub(2, *(f32*)((s32)obj + 0x78), float_90_80422fd4,
                                      *(u8*)((s32)obj + 0x71), *(u8*)((s32)data + 0x15));
                    *(f32*)((s32)obj + 0x1C) = angle;
                    if (*(u8*)((s32)obj + 0x71) < *(u8*)((s32)data + 0x15)) {
                        active = 1;
                    }
                }
            }
            offset += 0x7C;
        }
    } else if (type == 2) {
        timer = *(s32*)((s32)event + 0x7C);
        if (timer != 0) {
            *(s32*)((s32)event + 0x7C) = timer - 1;
            active = 1;
        } else {
            if (*(s32*)((s32)event + 0x84) == 0) {
                pos[0] = *(f32*)(vec3_802f42b8 + 0xFC);
                pos[1] = *(f32*)(vec3_802f42b8 + 0x100);
                pos[2] = *(f32*)(vec3_802f42b8 + 0x104);
                psndSFXOn_3D(vec3_802f42b8 + 0x270, pos);
            }
            *(s32*)((s32)event + 0x84) += 1;
            value = (s32)intpl_sub(2, float_0_80422fd0, float_90_80422fd4,
                                   *(s32*)((s32)event + 0x84), *(s32*)((s32)event + 0x80));
            *(f32*)((s32)base - 0x7F20) = (f32)value;
            if (*(s32*)((s32)event + 0x84) < *(s32*)((s32)event + 0x80)) {
                active = 1;
            }
        }
    }

    return ((u32)(-active) | (u32)active) >> 31 ? 0 : 2;
}

s32 _nozzle_turn(void* event, s32 isFirstCall) {
    extern void* _battleWorkPointer;
    extern void* BattleSearchObjectPtr(char* name);
    extern f32 intpl_sub(s32 type, f32 start, f32 end, s32 current, s32 duration);
    extern void mapObjRotate(f32 x, f32 y, f32 z, char* name);
    extern char vec3_802f42b8[];
    extern f32 float_0_80422fd0;
    extern f32 float_neg80_80423008;
    extern f32 float_45_80422ff8;
    extern f32 float_neg45_80423018;
    void* battleWork;
    void* work;
    void* nozzleWork;
    char* base;
    char* name;
    s32 nozzle;
    u8 dir;
    f32 xrot;
    f32 yrot;
    f32 start;
    f32 end;

    battleWork = _battleWorkPointer;
    base = vec3_802f42b8;
    nozzle = *(s32*)((s32)event + 0x160);
    work = (void*)((s32)battleWork + 0x20000 - 0x7F24);

    if (isFirstCall != 0) {
        *(s32*)((s32)event + 0x80) = 0;
        nozzleWork = (void*)((s32)work + nozzle);
        *(s32*)((s32)event + 0x84) = 0;
        if (*(u8*)((s32)nozzleWork + 0x1C4) != 0) {
            *(s32*)((s32)event + 0x80) = 0x1E;
        }
    }

    switch (nozzle) {
        case 0:
            name = base + 0xC0;
            break;
        case 1:
            name = base + 0xCC;
            break;
        case 2:
            name = base + 0xD8;
            break;
        default:
            return 2;
    }

    BattleSearchObjectPtr(name);

    if (*(s32*)((s32)event + 0x80) < 0x1E) {
        *(s32*)((s32)event + 0x80) += 1;
        xrot = intpl_sub(2, float_0_80422fd0, float_neg80_80423008, *(s32*)((s32)event + 0x80), 0x1E);
    } else {
        xrot = float_neg80_80423008;
    }

    if (*(s32*)((s32)event + 0x84) < 0x1E) {
        *(s32*)((s32)event + 0x84) += 1;
        nozzleWork = (void*)((s32)work + nozzle);
        dir = *(u8*)((s32)nozzleWork + 0x1C4);
        switch (dir) {
            case 0:
                start = float_0_80422fd0;
                break;
            case 1:
                start = float_45_80422ff8;
                break;
            case 2:
                start = float_0_80422fd0;
                break;
            case 3:
                start = float_neg45_80423018;
                break;
            default:
                start = float_0_80422fd0;
                break;
        }

        nozzleWork = (void*)((s32)work + nozzle);
        dir = *(u8*)((s32)nozzleWork + 0x1C7);
        switch (dir) {
            case 0:
                end = float_0_80422fd0;
                break;
            case 1:
                end = float_45_80422ff8;
                break;
            case 2:
                end = float_0_80422fd0;
                break;
            case 3:
                end = float_neg45_80423018;
                break;
            default:
                end = float_0_80422fd0;
                break;
        }
        yrot = intpl_sub(0xC, start, end, *(s32*)((s32)event + 0x84), 0x1E);
    }

    switch (nozzle) {
        case 0:
            mapObjRotate(xrot, yrot, float_0_80422fd0, base + 0x1D4);
            break;
        case 1:
            mapObjRotate(xrot, yrot, float_0_80422fd0, base + 0x1E0);
            break;
        case 2:
            mapObjRotate(xrot, yrot, float_0_80422fd0, base + 0x1EC);
            break;
    }

    if (*(s32*)((s32)event + 0x80) >= 0x1E && *(s32*)((s32)event + 0x84) >= 0x1E) {
        nozzleWork = (void*)((s32)work + nozzle);
        *(u8*)((s32)nozzleWork + 0x1C4) = *(u8*)((s32)nozzleWork + 0x1C7);
        *(s32*)((s32)work + (nozzle << 2) + 0x1CC) = 0;
        return 2;
    }
    return 0;
}

void _combine_nozzle_weapon(s32 copyBaseOnly, void* tempWeapon, void* baseWeapon) {
    extern void _rate_mix(s8* maxValue, s8* total, s8* current, s8* add);
    extern u8 _rate_mix_3(char* param_1, char* param_2, char* param_3, char* param_4, char* param_5, char* param_6);
    s32 i;
    s32 offset;
    s32* dst;
    s32* src;

    if (copyBaseOnly != 0) {
        dst = (s32*)((s32)tempWeapon - 4);
        src = (s32*)((s32)baseWeapon - 4);
        for (i = 0; i < 0x18; i++) {
            dst[1] = src[1];
            dst[2] = src[2];
            dst += 2;
            src += 2;
        }
        return;
    }

    *(u8*)((s32)tempWeapon + 0x11) += *(u8*)((s32)baseWeapon + 0x11);
    *(u8*)((s32)tempWeapon + 0x12) += *(u8*)((s32)baseWeapon + 0x12);
    if (*(void**)((s32)tempWeapon + 0x1C) == 0) {
        *(void**)((s32)tempWeapon + 0x1C) = *(void**)((s32)baseWeapon + 0x1C);
    }

    offset = 0;
    for (i = 0; i < 8; i++) {
        *(s32*)((s32)tempWeapon + offset + 0x20) += *(s32*)((s32)baseWeapon + offset + 0x20);
        offset += 4;
    }

    *(u8*)((s32)tempWeapon + 0x6C) = *(u8*)((s32)baseWeapon + 0x6C);
    tempWeapon = (void*)((s32)tempWeapon + 0x80);
    baseWeapon = (void*)((s32)baseWeapon + 0x80);

    _rate_mix((s8*)tempWeapon, (s8*)((s32)tempWeapon + 1), (s8*)baseWeapon, (s8*)((s32)baseWeapon + 1));
    _rate_mix((s8*)((s32)tempWeapon + 2), (s8*)((s32)tempWeapon + 3), (s8*)((s32)baseWeapon + 2), (s8*)((s32)baseWeapon + 3));
    _rate_mix((s8*)((s32)tempWeapon + 4), (s8*)((s32)tempWeapon + 5), (s8*)((s32)baseWeapon + 4), (s8*)((s32)baseWeapon + 5));
    _rate_mix((s8*)((s32)tempWeapon + 6), (s8*)((s32)tempWeapon + 7), (s8*)((s32)baseWeapon + 6), (s8*)((s32)baseWeapon + 8));
    _rate_mix((s8*)((s32)tempWeapon + 9), (s8*)((s32)tempWeapon + 0xA), (s8*)((s32)baseWeapon + 9), (s8*)((s32)baseWeapon + 0xA));
    _rate_mix((s8*)((s32)tempWeapon + 0xB), (s8*)((s32)tempWeapon + 0xC), (s8*)((s32)baseWeapon + 0xB), (s8*)((s32)baseWeapon + 0xC));
    _rate_mix((s8*)((s32)tempWeapon + 0xD), (s8*)((s32)tempWeapon + 0xE), (s8*)((s32)baseWeapon + 0xD), (s8*)((s32)baseWeapon + 0xE));
    _rate_mix((s8*)((s32)tempWeapon + 0xF), (s8*)((s32)tempWeapon + 0x10), (s8*)((s32)baseWeapon + 0xF), (s8*)((s32)baseWeapon + 0x10));
    _rate_mix((s8*)((s32)tempWeapon + 0x11), (s8*)((s32)tempWeapon + 0x12), (s8*)((s32)baseWeapon + 0x11), (s8*)((s32)baseWeapon + 0x12));
    _rate_mix_3((char*)((s32)tempWeapon + 0x13), (char*)((s32)tempWeapon + 0x14), (char*)((s32)tempWeapon + 0x15),
                (char*)((s32)baseWeapon + 0x13), (char*)((s32)baseWeapon + 0x14), (char*)((s32)baseWeapon + 0x15));
    _rate_mix_3((char*)((s32)tempWeapon + 0x19), (char*)((s32)tempWeapon + 0x1A), (char*)((s32)tempWeapon + 0x1B),
                (char*)((s32)baseWeapon + 0x19), (char*)((s32)baseWeapon + 0x1A), (char*)((s32)baseWeapon + 0x1B));
    _rate_mix((s8*)((s32)tempWeapon + 0x1C), (s8*)((s32)tempWeapon + 0x1D), (s8*)((s32)baseWeapon + 0x1C), (s8*)((s32)baseWeapon + 0x1D));
    *(u8*)((s32)tempWeapon + 0x1F) += *(u8*)((s32)baseWeapon + 0x1F);
    _rate_mix((s8*)((s32)tempWeapon + 0x20), (s8*)((s32)tempWeapon + 0x21), (s8*)((s32)baseWeapon + 0x20), (s8*)((s32)baseWeapon + 0x21));
    _rate_mix((s8*)((s32)tempWeapon + 0x22), (s8*)((s32)tempWeapon + 0x23), (s8*)((s32)baseWeapon + 0x22), (s8*)((s32)baseWeapon + 0x23));
    *(u8*)((s32)tempWeapon + 0x2A) += *(u8*)((s32)baseWeapon + 0x2A);
    *(u8*)((s32)tempWeapon + 0x2B) += *(u8*)((s32)baseWeapon + 0x2B);
    *(u8*)((s32)tempWeapon + 0x2C) += *(u8*)((s32)baseWeapon + 0x2C);
    *(u8*)((s32)tempWeapon + 0x2D) += *(u8*)((s32)baseWeapon + 0x2D);
}

s32 _bgset_kemuri_effect(void* event) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* event, s32 value);
    extern void effKemuri1N64Entry(s32 type, f32 x, f32 y, f32 z, f32 arg4);
    extern void psndSFXOn_3D(char* name, void* pos);
    extern char vec3_802f42b8[];
    extern f32 float_0_80422fd0;
    extern f32 float_15_80422fe4;
    extern f32 float_30_80423024;
    void* battleWork;
    char* snd0;
    char* snd1;
    char* snd2;
    s32 type;
    s32 i;
    s32 offset;
    void* obj;
    f32 x;
    f32 y;
    f32 z;

    type = evtGetValue(event, **(s32**)((s32)event + 0x18));
    battleWork = _battleWorkPointer;
    snd0 = vec3_802f42b8 + 0x1F8;
    snd1 = vec3_802f42b8 + 0x210;
    snd2 = vec3_802f42b8 + 0x228;
    i = 0;
    offset = 0;
    do {
        obj = (void*)((s32)battleWork + offset + 0x1715C);
        if (*(s32*)obj > 0 && *(u16*)((s32)*(void**)((s32)obj + 0x64) + 6) == type) {
            x = *(f32*)((s32)obj + 4);
            y = *(f32*)((s32)obj + 8);
            z = *(f32*)((s32)obj + 0xC);
            switch (type) {
                case 0:
                    z += float_30_80423024;
                    effKemuri1N64Entry(3, x, y, z, float_0_80422fd0);
                    psndSFXOn_3D(snd0, (void*)((s32)obj + 4));
                    break;
                case 1:
                    z += float_15_80422fe4;
                    effKemuri1N64Entry(0, x, y, z, float_0_80422fd0);
                    psndSFXOn_3D(snd1, (void*)((s32)obj + 4));
                    break;
                case 2:
                    z += float_30_80423024;
                    effKemuri1N64Entry(3, x, y, z, float_0_80422fd0);
                    effKemuri1N64Entry(4, x, y, z, float_0_80422fd0);
                    psndSFXOn_3D(snd2, (void*)((s32)obj + 4));
                    break;
            }
        }
        i++;
        offset += 0x7C;
    } while (i < 0x20);
    return 2;
}

s32 _mobj_set_alpha(void* event) {
    extern void* mapGetMapObj(void* name);
    extern void mapGrpSetColor(void* name, void* color);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 type = evtGetValue(event, args[0]);
    u8 alpha = evtGetValue(event, args[1]);
    u32 colorA1;
    u32 tempA1;
    u32 colorA2;
    u32 tempA2;
    u32 colorB;
    u32 tempB;
    u32 colorC;
    u32 tempC;

    switch (type) {
        case 0:
            if (mapGetMapObj(&str_A1_80423028) != 0) {
                tempA1 = dat_80422fb8;
                *(u8*)((s32)&tempA1 + 3) = alpha;
                colorA1 = tempA1;
                mapGrpSetColor(&str_A1_80423028, &colorA1);
            }
            break;
        case 1:
            if (mapGetMapObj(&str_A2_8042302c) != 0) {
                tempA2 = dat_80422fbc;
                *(u8*)((s32)&tempA2 + 3) = alpha;
                colorA2 = tempA2;
                mapGrpSetColor(&str_A2_8042302c, &colorA2);
            }
            break;
        case 2:
            if (mapGetMapObj(&str_B_80423030) != 0) {
                tempB = dat_80422fc0;
                *(u8*)((s32)&tempB + 3) = alpha;
                colorB = tempB;
                mapGrpSetColor(&str_B_80423030, &colorB);
            }
            break;
        case 3:
            if (mapGetMapObj(&str_C_80423034) != 0) {
                tempC = dat_80422fc4;
                *(u8*)((s32)&tempC + 3) = alpha;
                colorC = tempC;
                mapGrpSetColor(&str_C_80423034, &colorC);
            }
            break;
    }
    return 2;
}


s32 _mobj_destroy(void* event) {
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 type;
    s32 offset;
    void* battleWork;
    s32 i;
    void* obj;
    void* unit;
    void* entry;

    battleWork = _battleWorkPointer;
    type = evtGetValue(event, args[0]);
    offset = 0;
    for (i = 0; i < 0x20; i++) {
        obj = (void*)((s32)battleWork + offset + 0x1715C);
        if (*(s32*)obj > 0 && *(u16*)((s32)*(void**)((s32)obj + 0x64) + 6) == type) {
            *(u32*)((s32)obj + 0x68) |= 1;
            unit = BattleGetUnitPtr(battleWork, *(s32*)((s32)obj + 0x6C));
            if (unit != 0) {
                entry = *(void**)((s32)unit + 0x14);
                while (entry != 0) {
                    *(u32*)((s32)entry + 0x1AC) |= 0x10000;
                    entry = *(void**)entry;
                }
            }
        }
        offset += 0x7C;
    }
    switch (type) {
        case 0:
            *(u8*)((s32)battleWork + 0x20000 - 0x7F08) |= 2;
            break;
        case 1:
            *(u8*)((s32)battleWork + 0x20000 - 0x7F08) |= 4;
            break;
        case 2:
            *(u8*)((s32)battleWork + 0x20000 - 0x7F08) |= 8;
            break;
    }
    return 2;
}


int _get_nozzle_attack_index(u8 nozzleIdx) {
    void* work = (void*)((s32)_battleWorkPointer + 0x20000 - 0x7F24);
    u8 dir;

    switch (nozzleIdx) {
        case 0:
            dir = *(u8*)((s32)work + nozzleIdx + 0x1C4);
            switch (dir) {
                case 1:
                    return -1;
                case 2:
                    return 0;
                case 3:
                    return 1;
            }
        case 2:
            dir = *(u8*)((s32)work + nozzleIdx + 0x1C4);
            switch (dir) {
                case 1:
                    return 0;
                case 2:
                    return 1;
                case 3:
                    return -1;
            }
        case 1:
            dir = *(u8*)((s32)work + nozzleIdx + 0x1C4);
            switch (dir) {
                case 1:
                    return 0;
                case 2:
                    return -1;
                case 3:
                    return 1;
            }
    }
    return -1;
}


s32 _fall_frame_wait_unit_ready(void* event, s32 isFirstCall) {
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
    extern s32 BtlUnit_CheckStatus(void* unit, s32 status);
    void* battleWork = _battleWorkPointer;
    void* unit;
    void* evt;
    s32 i;
    s32 zero;
    s32 evtId;

    if (isFirstCall != 0) {
        for (i = 0; i < 0x40; i++) {
            unit = BattleGetUnitPtr(battleWork, i);
            if (unit != 0 && (*(u32*)((s32)unit + 0x1C) & 0x80000000) == 0 &&
                BtlUnit_CheckStatus(unit, 0x1B) == 0 && *(void**)((s32)unit + 0x2C0) != 0) {
                evt = evtEntry(*(void**)((s32)unit + 0x2C0), 0xA, 0);
                if (evt != 0) {
                    *(s32*)((s32)evt + 0x160) = *(s32*)unit;
                }
                *(s32*)((s32)unit + 0x2C4) = *(s32*)((s32)evt + 0x15C);
            }
        }
    }

    zero = 0;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0 && (*(u32*)((s32)unit + 0x1C) & 0x80000000) == 0) {
            evtId = *(s32*)((s32)unit + 0x2C4);
            if (evtId != 0 && evtCheckID(evtId) == 0) {
                *(s32*)((s32)unit + 0x2C4) = zero;
            }
            if (*(s32*)((s32)unit + 0x2C4) != 0) {
                return 0;
            }
        }
    }
    return 2;
}


u8 _Nozzle_Change_Check(u32 nozzleIdx, void* turnWeights) {
    return 0;
}


s32 _nozzle_work_attack_set(void) {
    extern void* memset(void* ptr, s32 value, u32 size);
    void* work = (void*)((s32)_battleWorkPointer + 0x20000 - 0x7F24);
    s32 i;
    s32 attackIndex;
    s32 slotOffset;
    s32 sourceIndex;

    *(s32*)((s32)work + 0x3C) = 0;
    *(s32*)((s32)work + 0x40) = 0;
    memset((void*)((s32)work + 0x44), 0, 0xC0);
    memset((void*)((s32)work + 0x104), 0, 0xC0);
    _combine_nozzle_weapon(1, (void*)((s32)work + 0x44), _nozzle_at_base_mario);
    _combine_nozzle_weapon(1, (void*)((s32)work + 0x104), _nozzle_at_base_monster);
    for (i = 0; i < 3; i++) {
        attackIndex = _get_nozzle_attack_index((u8)i);
        if (attackIndex >= 0) {
            slotOffset = (attackIndex << 2) + 0x3C;
            if (*(void**)((s32)work + slotOffset) == 0) {
                *(void**)((s32)work + slotOffset) = (void*)((s32)work + attackIndex * 0xC0 + 0x44);
            }
            sourceIndex = *(s8*)((s32)work + 0x1CA);
            _combine_nozzle_weapon(
                0,
                *(void**)((s32)work + slotOffset),
                (void*)(*(s32*)((s32)work + 0x34) + sourceIndex * 0xC0 + 0x10)
            );
        }
    }
    return 2;
}


s32 _fall_frame(void* event, s32 isFirstCall) {
    extern f32 intpl_sub(s32 type, f32 start, f32 end, s32 current, s32 duration);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    s32 dst = **(s32**)((s32)event + 0x18);
    void* obj = BattleSearchObjectPtr(str_if_body_802f4484);
    s32 timer;

    if (isFirstCall != 0) {
        *(s32*)((s32)event + 0x80) = 0;
        *(s32*)((s32)event + 0x84) = (s32)*(f32*)((s32)obj + 8);
    }
    timer = *(s32*)((s32)event + 0x80) + 1;
    *(s32*)((s32)event + 0x80) = timer;
    *(f32*)((s32)obj + 8) = intpl_sub(
        2,
        (f32)*(s32*)((s32)event + 0x84),
        float_0_80422fd0,
        (*(s32*)((s32)event + 0x80) * *(s32*)((s32)event + 0x80)) / 0x3C,
        0x3C
    );
    if (*(s32*)((s32)event + 0x80) < 0x3C) {
        return 0;
    }
    evtSetValue(event, dst, *(s32*)((s32)base - 0x7EEC));
    return 2;
}


s32 _set_mobj_shake(void* event) {
    extern s32 BattleWaitAllActiveEvtEnd_NoBgSetEndWait(void* battleWork);
    void* battleWork = _battleWorkPointer;
    void* base = (void*)((s32)battleWork + 0x20000);
    s32* args = *(s32**)((s32)event + 0x18);
    s32 type = evtGetValue(event, args[0]);
    s32 offset;
    s32 i;
    void* obj;

    if (BattleWaitAllActiveEvtEnd_NoBgSetEndWait(battleWork) != 0) {
        if (*(s32*)((s32)base - 0x7D4C) == 0 || evtCheckID(*(s32*)((s32)base - 0x7D4C)) == 0) {
            if (*(s32*)((s32)base - 0x7D3C) == 0 || evtCheckID(*(s32*)((s32)base - 0x7D3C)) == 0) {
                return 2;
            }
        }
    }
    if (type < 2 && type >= 0) {
        offset = 0;
        for (i = 0; i < 0x20; i++) {
            obj = (void*)((s32)battleWork + offset + 0x1715C);
            if (*(s32*)obj > 0 && *(u16*)((s32)*(void**)((s32)obj + 0x64) + 6) == type) {
                _mobj_shake_main(obj);
            }
            offset += 0x7C;
        }
    }
    return 0;
}


s32 _set_mobj_shake_init(void* event) {
    extern s32 irand(s32 max);
    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 type = evtGetValue(event, args[0]);
    s32 offset;
    s32 i;
    s32 frame;
    void* obj;

    offset = 0;
    for (i = 0; i < 0x20; i++) {
        obj = (void*)((s32)battleWork + offset + 0x1715C);
        if (*(s32*)obj > 0 && *(u16*)((s32)*(void**)((s32)obj + 0x64) + 6) == type) {
            *(u8*)((s32)obj + 0x72) = 0;
            *(u8*)((s32)obj + 0x73) = irand(0xA) + 0x3C;
            if ((irand(0x64) & 1) != 0) {
                frame = 0;
            } else {
                frame = *(u8*)((s32)obj + 0x73) >> 1;
            }
            *(u8*)((s32)obj + 0x74) = frame;
            *(f32*)((s32)obj + 0x78) = float_0_80422fd0;
        }
        offset += 0x7C;
    }
    return 2;
}


s32 _object_fall_wait(void) {
    extern s32 BattleWaitAllActiveEvtEnd_NoBgSetEndWait(void* battleWork);
    void* battleWork = _battleWorkPointer;
    void* base = (void*)((s32)battleWork + 0x20000);
    s32 id;

    if (BattleWaitAllActiveEvtEnd_NoBgSetEndWait(battleWork) == 0) {
        return 0;
    }

    id = *(s32*)((s32)base - 0x7D4C);
    if (id != 0) {
        if (evtCheckID(id) != 0) {
            return 0;
        }
        *(s32*)((s32)base - 0x7D4C) = 0;
    }
    id = *(s32*)((s32)base - 0x7D3C);
    if (id != 0) {
        if (evtCheckID(id) != 0) {
            return 0;
        }
        *(s32*)((s32)base - 0x7D3C) = 0;
    }
    id = *(s32*)((s32)base - 0x7F04);
    if (id != 0) {
        if (evtCheckID(id) != 0) {
            return 0;
        }
        *(s32*)((s32)base - 0x7F04) = 0;
    }
    return 2;
}


void BattleStage_NozzleWorkCheck(s32 force) {
    extern void* pouchGetPtr(void);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    void* event;
    s32 id;

    if (*(s16*)((s32)pouchGetPtr() + 0x88) > 0) {
        _nozzle_type_init();
        if (force == 0 && *(s8*)((s32)base - 0x7D5A) == 0) {
            return;
        }
        id = *(s32*)((s32)base - 0x7D4C);
        if (id != 0 && evtCheckID(id) == 0) {
            *(s32*)((s32)base - 0x7D4C) = 0;
        }
        if (*(s32*)((s32)base - 0x7D4C) == 0) {
            if (force != 0) {
                event = evtEntry(&_nozzle_work_event_1, 0xA, 0);
            } else {
                event = evtEntry(&_nozzle_work_event_0, 0xA, 0);
            }
            *(s32*)((s32)base - 0x7D4C) = *(s32*)((s32)event + 0x15C);
        }
    }
}


s32 _get_bgset_attack_data(void* event) {
    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 type = evtGetValue(event, args[0]);
    s32 dst = args[1];
    void* data;

    switch (type) {
        case 0:
            data = (void*)((s32)**(void***)((s32)*(void**)((s32)battleWork + 0x2738) + 0xC) + 0x10);
            break;
        case 2:
            data = (void*)((s32)**(void***)((s32)*(void**)((s32)battleWork + 0x2738) + 0xC) + 0xD0);
            break;
        default:
            data = 0;
            break;
    }
    if (data != 0 && *(void**)((s32)data + 0x1C) == 0) {
        data = 0;
    }
    evtSetValue(event, dst, (s32)data);
    return 2;
}


void BattleStage_DestroyB(void) {
    void* battleWork = _battleWorkPointer;
    void* data = *(void**)((s32)battleWork + 0x2738);
    void* event;
    void* script;

    script = *(void**)((s32)**(void***)((s32)data + 0xC) + 0x19C);
    evtDeleteID(*(s32*)((s32)battleWork + 0x20000 - 0x7EF8));
    *(s32*)((s32)battleWork + 0x20000 - 0x7EF8) = 0;
    if (*(s32*)((s32)battleWork + 0x20000 - 0x7F04) != 0 &&
        evtCheckID(*(s32*)((s32)battleWork + 0x20000 - 0x7F04)) != 0) {
        evtDeleteID(*(s32*)((s32)battleWork + 0x20000 - 0x7F04));
    }
    if (script == 0) {
        script = &_object_turn_event_B;
    }
    event = evtEntry(script, 0xA, 0);
    if (event != 0) {
        *(s32*)((s32)battleWork + 0x20000 - 0x7F04) = *(s32*)((s32)event + 0x15C);
    }
}


void BattleStage_DestroyA1(void) {
    void* battleWork = _battleWorkPointer;
    void* data = *(void**)((s32)battleWork + 0x2738);
    void* event;
    void* script;

    script = *(void**)((s32)**(void***)((s32)data + 0xC) + 0x194);
    evtDeleteID(*(s32*)((s32)battleWork + 0x20000 - 0x7F00));
    *(s32*)((s32)battleWork + 0x20000 - 0x7F00) = 0;
    if (*(s32*)((s32)battleWork + 0x20000 - 0x7F04) != 0 &&
        evtCheckID(*(s32*)((s32)battleWork + 0x20000 - 0x7F04)) != 0) {
        evtDeleteID(*(s32*)((s32)battleWork + 0x20000 - 0x7F04));
    }
    if (script == 0) {
        script = &_object_turn_event_A1;
    }
    event = evtEntry(script, 0xA, 0);
    if (event != 0) {
        *(s32*)((s32)battleWork + 0x20000 - 0x7F04) = *(s32*)((s32)event + 0x15C);
    }
}


void _nozzle_type_init(void) {
    extern s32 irand(s32 max);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    void* work = *(void**)((s32)base - 0x7EF0);
    s32 total;
    s32 value;
    s8 i;

    if (*(s8*)((s32)base - 0x7D5A) != -1) {
        return;
    }
    total = 0;
    for (i = 0; i < 4; i++) {
        total += *(u8*)((s32)work + i + 0xC);
    }
    value = irand(total);
    for (i = 0; i < 4; i++) {
        value -= *(u8*)((s32)work + i + 0xC);
        if (value < 0) {
            break;
        }
    }
    *(u8*)((s32)base - 0x7D5A) = i;
}


s32 _fall_frame_delete(void* event, s32 isFirstCall) {
    extern void mapGrpSetColor(const char* name, void* color);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    u32 color;
    u32 colorTemp;

    if (isFirstCall != 0) {
        *(s32*)((s32)event + 0x80) = 0xFF;
    }
    *(s32*)((s32)event + 0x80) -= 5;
    colorTemp = dat_80422fc8;
    *(u8*)((s32)&colorTemp + 3) = *(s32*)((s32)event + 0x80);
    color = colorTemp;
    mapGrpSetColor(str_if_body_802f4484, &color);
    if (*(s32*)((s32)event + 0x80) > 0) {
        return 0;
    }
    *(s32*)((s32)base - 0x7D3C) = 0;
    return 2;
}


s32 _nozzle_work_wait(void) {
    extern s32 BattleWaitAllActiveEvtEnd_NoBgSetEndWait(void* battleWork);
    void* battleWork = _battleWorkPointer;
    void* work = (void*)((s32)battleWork + 0x20000 - 0x7F24);
    s32 i;
    s32 offset;
    s32 id;

    if (BattleWaitAllActiveEvtEnd_NoBgSetEndWait(battleWork) == 0) {
        return 0;
    }

    offset = 0;
    for (i = 0; i < 3; i++) {
        id = *(s32*)((s32)work + offset + 0x1CC);
        if (id != 0) {
            if (evtCheckID(id) != 0) {
                return 0;
            }
            *(s32*)((s32)work + offset + 0x1CC) = 0;
        }
        offset += 4;
    }
    return 2;
}


s32 BattleFogEndCheck(void) {
    extern void* _battleWorkPointer;
    extern void effSoftDelete(void* effect);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    s32 timer;
    void* effect;

    timer = *(s32*)((s32)base - 0x7D48);
    if (timer == 0) {
        return 1;
    }

    *(s32*)((s32)base - 0x7D48) = timer - 1;
    if (*(s32*)((s32)base - 0x7D48) > 0) {
        return 0;
    }

    effect = *(void**)((s32)base - 0x7D44);
    if (effect != NULL) {
        effSoftDelete(effect);
        *(void**)((s32)base - 0x7D44) = NULL;
    }
    return 1;
}

s32 _fall_frame_wait(void) {
    return 0;
}


u8 BattleStage_DestroyA2(void) {
    return 0;
}


void BattleStage_IronFrameFallCheck(void) {
    extern s32 _fall_frame_event[];
    extern void* evtEntry(void* script, s32 priority, s32 flags);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    u8 flags = *(u8*)((s32)base - 0x7F08);
    void* event;

    if ((flags & 0x40) == 0 && (flags & 0x20) == 0) {
        flags |= 0x20;
        *(u8*)((s32)base - 0x7F08) = flags;
        event = evtEntry(_fall_frame_event, 0xA, 0);
        *(s32*)((s32)base - 0x7D3C) = *(s32*)((s32)event + 0x15C);
    }
}

void BattleStage_NozzleDirChangeCheck(void) {
    extern void _Nozzle_Change_Check(s32 nozzleIdx, void* work);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    s32 i;

    if (*(void**)((s32)base - 0x7EF0) != NULL) {
        for (i = 0; (u8)i < 3; i++) {
            _Nozzle_Change_Check(i, (void*)((s32)*(void**)((s32)base - 0x7EF0) + ((u8)i << 2)));
        }
    }
}

s32 BattleStage_ObjectFallCheck(void) {
    extern s32 _object_fall_event[];
    extern void* evtEntry(void* script, s32 priority, s32 flags);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    void* event;

    if ((*(u32*)((s32)_battleWorkPointer + 0xEF4) & 0x1000) != 0) {
        return 0;
    }

    event = evtEntry(_object_fall_event, 0xA, 0);
    *(s32*)((s32)base - 0x7D38) = *(s32*)((s32)event + 0x15C);
    return *(s32*)((s32)event + 0x15C);
}

u8 _rate_mix_3(char* param_1, char* param_2, char* param_3, char* param_4, char* param_5, char* param_6) {
    return 0;
}


s32 _nozzle_work_get_weapon_address(void* event) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 evtSetValue(void* event, s32 target, s32 value);
    void* evt = event;
    s32* args = *(s32**)((s32)event + 0x18);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    s32 index;

    base = (void*)((s32)base - 0x7F24);
    index = evtGetValue(event, args[0]);
    index <<= 2;
    index = (s32)base + index;
    evtSetValue(evt, args[1], *(s32*)(index + 0x3C));
    return 2;
}


void BattleFogForceStop(void) {
    ;
}


void BattleStage_WallCloseCheck(void) {
    extern void* _battleWorkPointer;
    extern void* _close_wall_event;
    extern void* evtEntry(void* script, s32 priority, s32 flags);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    u8 flags = *(u8*)((s32)base - 0x7F08);
    void* event;

    if ((flags & 0x10) == 0) {
        flags |= 0x10;
        *(u8*)((s32)base - 0x7F08) = flags;
        event = evtEntry(&_close_wall_event, 0xA, 0);
        *(s32*)((s32)base - 0x7D40) = *(s32*)((s32)event + 0x15C);
    }
}

f32 _get_nozzle_dir(int nozzleIdx) {
    extern void* _battleWorkPointer;
    extern f32 float_neg45_80423018;
    extern f32 float_neg135_8042301c;
    extern f32 float_neg90_80423020;
    s32 base = (s32)_battleWorkPointer + 0x20000;
    s32 value;

    nozzleIdx = base + nozzleIdx;
    value = *(u8*)(nozzleIdx - 0x7D60);

    switch (value) {
        case 1:
            return float_neg45_80423018;
        case 3:
            return float_neg135_8042301c;
        default:
            return float_neg90_80423020;
    }
}

void _mobj_shake_main(void* stageObject) {
    extern f32 sinfd(f64 value);
    s32 angle;

    *(u8*)((s32)stageObject + 0x74) += 1;
    if (*(u8*)((s32)stageObject + 0x74) >= *(u8*)((s32)stageObject + 0x73)) {
        *(u8*)((s32)stageObject + 0x74) = 0;
    }
    angle = (*(u8*)((s32)stageObject + 0x74) * 0x168) / *(u8*)((s32)stageObject + 0x73);
    *(f32*)((s32)stageObject + 0x78) = float_5_80422fdc * sinfd((f32)angle);
    *(f32*)((s32)stageObject + 0x1C) = *(f32*)((s32)stageObject + 0x78);
}
