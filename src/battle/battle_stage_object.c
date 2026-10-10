#include "battle/battle_stage_object.h"
#include "battle/battle_audience.h"
#include "manager/evtmgr.h"

extern void* _battleWorkPointer;
s32 strcmp(const char*, const char*);
void* evtEntry(void* script, s32 priority, s32 flags);
s32 evtGetValue(void* event, s32 value);
s32 evtSetValue(void* event, s32 target, s32 value);
void _mobj_shake_main(void* stageObject);
void _nozzle_type_init(void);
void _combine_nozzle_weapon(s32 copyBaseOnly, void* tempWeapon, void* baseWeapon);
int _get_nozzle_attack_index(u8 nozzleIdx);
extern u32 _object_turn_event_A1[];
extern u32 _object_turn_event_B[];
extern u32 _nozzle_work_event_0[];
extern u32 _nozzle_work_event_1[];
extern u32 _nozzle_turn_event[];
extern u8 _nozzle_at_base_mario[];
extern u8 _nozzle_at_base_monster[];
extern char str_if_body_802f4484[];
extern u32 dat_80422fc8;
extern u32 dat_80422fb8;
extern u32 dat_80422fbc;
extern u32 dat_80422fc0;
extern u32 dat_80422fc4;
extern const char str_A1_80423028[];
extern const char str_A2_8042302c[];
extern const char str_B_80423030[];
extern const char str_C_80423034[];
extern const char str_den_80423038[4];
extern f32 float_5_80422fdc;

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
    extern void* _battleWorkPointer;
    extern s32 strcmp(const char* str1, const char* str2);

    void* battleWork;
    s32 offset;
    s32 i;
    void* obj;

    battleWork = _battleWorkPointer;
    offset = 0;
    for (i = 0; i < 0x20; i++) {
        obj = (void*)((s32)battleWork + offset + 0x1715C);
        if (*(s32*)obj != 0 && strcmp(**(const char***)((s32)obj + 0x64), name) == 0) {
            return obj;
        }
        offset += 0x7C;
    }

    return 0;
}

void BattleStageObjectInit(void) {
    extern void* pouchGetPtr(void);
    extern void* mapGetWork(void);
    extern void mapPlayAnimationLv(char*, s32, s32);
    extern void* mapGetMapObj(char*);
    extern void mapGrpFlagOn(char*, s32);
    extern void mapGrpFlagOff(char*, s32);
    extern void* BattleStageGetPtr(void);
    extern void* BtlUnit_Entry(void*);
    extern void BtlUnit_SetPos(void*, f32, f32, f32);
    extern void BtlUnit_SetHomePos(void*, f32, f32, f32);
    extern const char str_A1_80423028[];
    extern const char str_A2_8042302c[];
    extern const char str_B_80423030[];
    extern const char str_C_80423034[];
    extern const char str_den_80423038[4];
    extern char lbl_80373E38[];
    extern char vec3_802f42b8[];
    extern u32 _object_wall_data[6];
    extern u32 _object_spot_light_data[12];
    extern u32 _entry_tree_data[12];
    extern u32 _entry_switch_data[12];

    typedef struct LocalVec { u32 x, y, z; } LocalVec;
    u8* battle;
    u8* stageInfo;
    u8* objectData;
    u8* objectWork;
    u8* slot;
    u8* pouch;
    u8* mapWork;
    u8* dataBase;
    u8* vecBase;
    u8* stage;
    void* unit;
    void* event;
    s32 objectCount;
    s32 id;
    s32 layerA1;
    s32 layerA2;
    s32 layerB;
    s32 layerBRotate;
    s32 hasIronFrame;
    s32 i;
    s32 j;
    s32 offset;
    u16 layer;
    u16 spawnType;
    f32 zero;

    battle = (u8*)_battleWorkPointer;
    dataBase = (u8*)lbl_80373E38;
    vecBase = (u8*)vec3_802f42b8;

    layerA1 = 0;
    layerA2 = 0;
    layerB = 0;
    layerBRotate = 0;
    hasIronFrame = 0;

    stageInfo = *(u8**)(battle + 0x2738);
    stageInfo = *(u8**)(stageInfo + 0x0C);
    stageInfo = *(u8**)stageInfo;
    objectCount = *(s32*)(stageInfo + 0x08);
    objectData = *(u8**)(stageInfo + 0x0C);

    objectWork = battle + 0x1715C;
    memset(objectWork, 0, 0xF80);

    pouch = (u8*)pouchGetPtr();
    if (*(s16*)(pouch + 0x88) < 4 && *(s16*)(pouch + 0x88) >= 1) {
        mapPlayAnimationLv((char*)&str_den_80423038, 1, 0);
    }

    pouch = (u8*)pouchGetPtr();
    if (*(s16*)(pouch + 0x88) < 4 && *(s16*)(pouch + 0x88) >= 1) {
        pouch = (u8*)pouchGetPtr();
        *(void**)(battle + 0x18110) =
            dataBase + 0x1C0 + *(s16*)(pouch + 0x88) * 0x310;
    } else {
        *(void**)(battle + 0x18110) = 0;
    }

    pouch = (u8*)pouchGetPtr();
    *(void**)(battle + 0x18114) =
        dataBase + 0xE00 + *(s16*)(pouch + 0x88) * 0x78C;
    *(s8*)(battle + 0x182A6) = -1;
    *(s32*)(battle + 0x182B8) = 0;
    *(s32*)(battle + 0x182BC) = 0;

    mapWork = (u8*)mapGetWork();
    mapWork = *(u8**)(mapWork + 0xA8);
    mapWork = *(u8**)(mapWork + 0x08);
    mapGrpFlagOff(*(char**)mapWork, 2);

    if (mapGetMapObj((char*)&str_A1_80423028) != 0) {
        mapGrpFlagOn((char*)&str_A1_80423028, 1);
        mapGrpFlagOff((char*)&str_A1_80423028, 2);
    }
    if (mapGetMapObj((char*)&str_A2_8042302c) != 0) {
        mapGrpFlagOn((char*)&str_A2_8042302c, 1);
        mapGrpFlagOff((char*)&str_A2_8042302c, 2);
    }
    if (mapGetMapObj((char*)&str_B_80423030) != 0) {
        mapGrpFlagOn((char*)&str_B_80423030, 1);
        mapGrpFlagOff((char*)&str_B_80423030, 2);
    }
    if (mapGetMapObj((char*)&str_C_80423034) != 0) {
        mapGrpFlagOn((char*)&str_C_80423034, 1);
        mapGrpFlagOff((char*)&str_C_80423034, 2);
    }
    mapGrpFlagOn((char*)(vecBase + 0xE4), 1);

    zero = 0.0f;
    id = 1;

    if (objectData != 0) {
        while (objectCount != 0) {
            offset = 0;
            for (j = 0; j < 0x20; j++, offset += 0x7C) {
                slot = battle + offset + 0x1715C;
                if (*(s32*)slot < 1) {
                    break;
                }
            }
            if (j >= 0x20) {
                break;
            }

            *(void**)(slot + 0x64) = objectData;
            *(s32*)(slot + 0x00) = id;
            *(s32*)(slot + 0x6C) = -1;

            layer = *(u16*)(objectData + 0x06);
            switch (layer) {
                case 0:
                    layerA1++;
                    break;
                case 1:
                    layerA2++;
                    break;
                case 2:
                    layerB++;
                    break;
                case 3:
                    layerBRotate++;
                    break;
                case 6:
                    hasIronFrame = 1;
                    break;
            }

            switch (layer) {
            case 0:
            case 1:
            case 6:
            case 7:
                *(LocalVec*)(slot + 0x04) = *(LocalVec*)(objectData + 0x08);
                break;
            case 2:
            case 3:
                *(LocalVec*)(slot + 0x04) = *(LocalVec*)(objectData + 0x08);
                break;
            }

            *(f32*)(slot + 0x24) = zero;
            *(f32*)(slot + 0x20) = zero;
            *(f32*)(slot + 0x1C) = zero;

            *(LocalVec*)(slot + 0x34) = *(LocalVec*)(vecBase + 0x00);
            *(LocalVec*)(slot + 0x40) = *(LocalVec*)(vecBase + 0x0C);
            *(LocalVec*)(slot + 0x4C) = *(LocalVec*)(vecBase + 0x18);
            *(LocalVec*)(slot + 0x58) = *(LocalVec*)(vecBase + 0x24);

            mapGrpFlagOff(*(char**)(*(u8**)(slot + 0x64)), 1);

            spawnType = *(u16*)(*(u8**)(slot + 0x64) + 0x04);
            if (spawnType == 1) {
                unit = BtlUnit_Entry(&_entry_switch_data);
                *(s32*)((u8*)unit + 0xB30) = *(s32*)slot;
                *(s32*)(slot + 0x6C) = *(s32*)unit;
                BtlUnit_SetPos(unit,
                               *(f32*)(slot + 0x04),
                               *(f32*)(slot + 0x08),
                               *(f32*)(slot + 0x0C));
                BtlUnit_SetHomePos(unit,
                                   *(f32*)(slot + 0x04),
                                   *(f32*)(slot + 0x08),
                                   *(f32*)(slot + 0x0C));
            } else if (spawnType == 0) {
                unit = BtlUnit_Entry(&_entry_tree_data);
                *(s32*)((u8*)unit + 0xB30) = *(s32*)slot;
                *(s32*)(slot + 0x6C) = *(s32*)unit;
                BtlUnit_SetPos(unit,
                               *(f32*)(slot + 0x04),
                               *(f32*)(slot + 0x08),
                               *(f32*)(slot + 0x0C));
                BtlUnit_SetHomePos(unit,
                                   *(f32*)(slot + 0x04),
                                   *(f32*)(slot + 0x08),
                                   *(f32*)(slot + 0x0C));
            }

            id++;
            objectCount--;
            objectData += 0x18;
        }
    }

    pouch = (u8*)pouchGetPtr();
    if (*(s16*)(pouch + 0x88) > 0) {
        objectData = dataBase + 0x2C30;
        zero = 0.0f;

        for (i = 0; i < 3; i++, objectData += 0x18) {
            offset = 0;
            for (j = 0; j < 0x20; j++, offset += 0x7C) {
                slot = battle + offset + 0x1715C;
                if (*(s32*)slot < 1) {
                    break;
                }
            }
            if (j >= 0x20) {
                break;
            }

            *(void**)(slot + 0x64) = objectData;
            *(s32*)(slot + 0x00) = id;
            *(s32*)(slot + 0x6C) = -1;

            *(LocalVec*)(slot + 0x04) = *(LocalVec*)(objectData + 0x08);

            *(f32*)(slot + 0x24) = zero;
            *(f32*)(slot + 0x20) = zero;
            *(f32*)(slot + 0x1C) = zero;

            *(LocalVec*)(slot + 0x34) = *(LocalVec*)(vecBase + 0x30);
            *(LocalVec*)(slot + 0x40) = *(LocalVec*)(vecBase + 0x3C);
            *(LocalVec*)(slot + 0x4C) = *(LocalVec*)(vecBase + 0x48);
            *(LocalVec*)(slot + 0x58) = *(LocalVec*)(vecBase + 0x54);

            mapGrpFlagOff(*(char**)(*(u8**)(slot + 0x64)), 1);
            id++;
        }
    }

    offset = 0;
    for (j = 0; j < 0x20; j++, offset += 0x7C) {
        slot = battle + offset + 0x1715C;
        if (*(s32*)slot < 1) {
            break;
        }
    }

    objectData = (u8*)&_object_wall_data;
    *(void**)(slot + 0x64) = objectData;
    *(s32*)(slot + 0x00) = id;
    *(s32*)(slot + 0x6C) = -1;
    *(LocalVec*)(slot + 0x04) = *(LocalVec*)(objectData + 0x08);

    stage = (u8*)BattleStageGetPtr();
    *(f32*)(slot + 0x08) = *(f32*)(stage + 0xB28);

    zero = 0.0f;
    *(f32*)(slot + 0x24) = zero;
    *(f32*)(slot + 0x20) = zero;
    *(f32*)(slot + 0x1C) = zero;

    *(LocalVec*)(slot + 0x34) = *(LocalVec*)(vecBase + 0x60);
    *(LocalVec*)(slot + 0x40) = *(LocalVec*)(vecBase + 0x6C);
    *(LocalVec*)(slot + 0x4C) = *(LocalVec*)(vecBase + 0x78);
    *(LocalVec*)(slot + 0x58) = *(LocalVec*)(vecBase + 0x84);

    mapGrpFlagOn(*(char**)(*(u8**)(slot + 0x64)), 1);
    *(u32*)(slot + 0x68) |= 1;

    id++;

    zero = 0.0f;
    offset = 0;
    while (*(char**)((u8*)&_object_spot_light_data + offset) != 0) {
        objectData = (u8*)&_object_spot_light_data + offset;

        i = 0;
        for (j = 0; j < 0x20; j++, i += 0x7C) {
            slot = battle + i + 0x1715C;
            if (*(s32*)slot < 1) {
                break;
            }
        }

        *(void**)(slot + 0x64) = objectData;
        *(s32*)(slot + 0x00) = id;
        *(s32*)(slot + 0x6C) = -1;

        *(LocalVec*)(slot + 0x04) = *(LocalVec*)(objectData + 0x08);

        *(f32*)(slot + 0x24) = zero;
        *(f32*)(slot + 0x20) = zero;
        *(f32*)(slot + 0x1C) = zero;

        *(LocalVec*)(slot + 0x34) = *(LocalVec*)(vecBase + 0x90);
        *(LocalVec*)(slot + 0x40) = *(LocalVec*)(vecBase + 0x9C);
        *(LocalVec*)(slot + 0x4C) = *(LocalVec*)(vecBase + 0xA8);
        *(LocalVec*)(slot + 0x58) = *(LocalVec*)(vecBase + 0xB4);

        mapGrpFlagOn(*(char**)(*(u8**)(slot + 0x64)), 1);
        *(u32*)(slot + 0x68) |= 1;

        id++;
        offset += 0x18;
    }

    if (*(void**)(stageInfo + 0x190) != 0) {
        evtEntry(*(void**)(stageInfo + 0x190), 10, 0);
    }

    if (layerA1 != 0 && *(void**)(stageInfo + 0x1A0) != 0) {
        event = evtEntry(*(void**)(stageInfo + 0x1A0), 10, 0);
        *(s32*)(battle + 0x18100) = *(s32*)((u8*)event + 0x15C);
    }
    if (layerA2 != 0 && *(void**)(stageInfo + 0x1A4) != 0) {
        event = evtEntry(*(void**)(stageInfo + 0x1A4), 10, 0);
        *(s32*)(battle + 0x18104) = *(s32*)((u8*)event + 0x15C);
    }
    if (layerB != 0 && *(void**)(stageInfo + 0x1A8) != 0) {
        event = evtEntry(*(void**)(stageInfo + 0x1A8), 10, 0);
        *(s32*)(battle + 0x18108) = *(s32*)((u8*)event + 0x15C);
    }
    if (layerBRotate != 0 && *(void**)(stageInfo + 0x1AC) != 0) {
        event = evtEntry(*(void**)(stageInfo + 0x1AC), 10, 0);
        *(s32*)(battle + 0x1810C) = *(s32*)((u8*)event + 0x15C);
    }

    if (layerA1 == 0) {
        *(u8*)(battle + 0x180F8) |= 2;
    }
    if (layerA2 == 0) {
        *(u8*)(battle + 0x180F8) |= 4;
    }
    if (layerB == 0) {
        *(u8*)(battle + 0x180F8) |= 8;
    }

    if (*(s8*)(stageInfo + 0x1B1) != 0) {
        *(u8*)(battle + 0x180F8) |= 2;
    }
    if (*(s8*)(stageInfo + 0x1B2) != 0) {
        *(u8*)(battle + 0x180F8) |= 4;
    }
    if (*(s8*)(stageInfo + 0x1B3) != 0) {
        *(u8*)(battle + 0x180F8) |= 8;
    }

    if (hasIronFrame == 0) {
        *(u8*)(battle + 0x180F8) |= 0x20;
        mapGrpFlagOn((char*)(vecBase + 0x1CC), 1);
        mapGrpFlagOn((char*)(vecBase + 0x288), 1);
    }
}

void BattleStageObjectMain(void) {
    typedef struct RawVec3 {
        s32 x;
        s32 y;
        s32 z;
    } RawVec3;
    extern void* _battleWorkPointer;
    extern void BattleObjectConfig(void);
    extern void mapObjTranslate(f32 x, f32 y, f32 z, char* name);
    extern void mapObjRotate(f32 x, f32 y, f32 z, char* name);
    extern void* mapGetMapObj(char* name);
    extern void mapGrpFlagOn(char* name, s32 flag);
    extern void mapGrpFlagOff(char* name, s32 flag);
    extern f32 float_0_80422fd0;
    void* battleWork;
    void* base;
    void* obj;
    void* data;
    void* mapObj;
    s32 i;
    s32 offset;
    s32 changed;
    s32 layer;
    f32 x;
    f32 y;
    f32 z;
    char* name;

    battleWork = _battleWorkPointer;
    base = (void*)((s32)battleWork + 0x20000);
    i = 0;
    offset = 0;
    for (; i < 0x20; i++, offset += 0x7C) {
        obj = (void*)((s32)battleWork + offset + 0x1715C);
        if (*(s32*)obj <= 0) {
            continue;
        }

        changed = 0;
        if (*(f32*)((s32)obj + 0x4) != *(f32*)((s32)obj + 0x34) ||
            *(f32*)((s32)obj + 0x8) != *(f32*)((s32)obj + 0x38) ||
            *(f32*)((s32)obj + 0xC) != *(f32*)((s32)obj + 0x3C)) {
            changed = 1;
            *(RawVec3*)((s32)obj + 0x34) = *(RawVec3*)((s32)obj + 0x4);
        }
        if (*(f32*)((s32)obj + 0x10) != *(f32*)((s32)obj + 0x40) ||
            *(f32*)((s32)obj + 0x14) != *(f32*)((s32)obj + 0x44) ||
            *(f32*)((s32)obj + 0x18) != *(f32*)((s32)obj + 0x48)) {
            changed = 1;
            *(RawVec3*)((s32)obj + 0x40) = *(RawVec3*)((s32)obj + 0x10);
        }
        if (*(f32*)((s32)obj + 0x28) != *(f32*)((s32)obj + 0x58) ||
            *(f32*)((s32)obj + 0x2C) != *(f32*)((s32)obj + 0x5C) ||
            *(f32*)((s32)obj + 0x30) != *(f32*)((s32)obj + 0x60)) {
            changed = 1;
            *(RawVec3*)((s32)obj + 0x58) = *(RawVec3*)((s32)obj + 0x28);
        }
        if (*(f32*)((s32)obj + 0x1C) != *(f32*)((s32)obj + 0x4C) ||
            *(f32*)((s32)obj + 0x20) != *(f32*)((s32)obj + 0x50) ||
            *(f32*)((s32)obj + 0x24) != *(f32*)((s32)obj + 0x54)) {
            changed = 1;
            *(RawVec3*)((s32)obj + 0x4C) = *(RawVec3*)((s32)obj + 0x1C);
        }

        if ((*(u32*)((s32)obj + 0x68) & 2) != 0) {
            *(u32*)((s32)obj + 0x68) &= ~2;
            changed = 1;
        }

        data = *(void**)((s32)obj + 0x64);
        switch (*(u16*)((s32)data + 0x6)) {
        case 2:
            if (*(f32*)((s32)base - 0x7F20) != *(f32*)((s32)base - 0x7F14) ||
                *(f32*)((s32)base - 0x7F1C) != *(f32*)((s32)base - 0x7F10) ||
                *(f32*)((s32)base - 0x7F18) != *(f32*)((s32)base - 0x7F0C)) {
                changed = 1;
                *(RawVec3*)((s32)base - 0x7F14) = *(RawVec3*)((s32)base - 0x7F20);
            }
            break;
        }

        switch (*(u16*)((s32)*(void**)((s32)obj + 0x64) + 6)) {
        case 0:
        case 1:
        case 4:
        case 5:
        case 6:
        case 7:
            if (changed != 0) {
                mapObjTranslate(
                    *(f32*)((s32)obj + 0x28) + (*(f32*)((s32)obj + 0x4) + *(f32*)((s32)obj + 0x10)),
                    *(f32*)((s32)obj + 0x2C) + (*(f32*)((s32)obj + 0x8) + *(f32*)((s32)obj + 0x14)),
                    *(f32*)((s32)obj + 0x30) + (*(f32*)((s32)obj + 0xC) + *(f32*)((s32)obj + 0x18)),
                    *(char**)*(s32*)((s32)obj + 0x64));
                mapObjRotate(*(f32*)((s32)obj + 0x1C), *(f32*)((s32)obj + 0x20), *(f32*)((s32)obj + 0x24), *(char**)*(s32*)((s32)obj + 0x64));
            }
                    break;
        case 2:
            if (changed != 0) {
                mapObjTranslate(
                    *(f32*)((s32)obj + 0x28) + (*(f32*)((s32)obj + 0x4) + *(f32*)((s32)obj + 0x10)),
                    *(f32*)((s32)obj + 0x2C) + (*(f32*)((s32)obj + 0x8) + *(f32*)((s32)obj + 0x14)),
                    *(f32*)((s32)obj + 0x30) + (*(f32*)((s32)obj + 0xC) + *(f32*)((s32)obj + 0x18)),
                    *(char**)*(s32*)((s32)obj + 0x64));
                mapObjTranslate(float_0_80422fd0, -*(f32*)((s32)obj + 0x14), float_0_80422fd0, *(char**)*(s32*)((s32)obj + 0x64));
                mapObjRotate(*(f32*)((s32)base - 0x7F20), *(f32*)((s32)base - 0x7F1C), *(f32*)((s32)base - 0x7F18), *(char**)*(s32*)((s32)obj + 0x64));
                mapObjTranslate(float_0_80422fd0, *(f32*)((s32)obj + 0x14), float_0_80422fd0, *(char**)*(s32*)((s32)obj + 0x64));
            }
                    break;
        case 3:
            if (changed != 0) {
                mapObjTranslate(
                    *(f32*)((s32)obj + 0x28) + (*(f32*)((s32)obj + 0x4) + *(f32*)((s32)obj + 0x10)),
                    *(f32*)((s32)obj + 0x2C) + (*(f32*)((s32)obj + 0x8) + *(f32*)((s32)obj + 0x14)),
                    *(f32*)((s32)obj + 0x30) + (*(f32*)((s32)obj + 0xC) + *(f32*)((s32)obj + 0x18)),
                    *(char**)*(s32*)((s32)obj + 0x64));
                mapObjTranslate(float_0_80422fd0, -*(f32*)((s32)obj + 0x8), float_0_80422fd0, *(char**)*(s32*)((s32)obj + 0x64));
                mapObjRotate(*(f32*)((s32)obj + 0x1C), *(f32*)((s32)obj + 0x20), *(f32*)((s32)obj + 0x24), *(char**)*(s32*)((s32)obj + 0x64));
                mapObjTranslate(float_0_80422fd0, *(f32*)((s32)obj + 0x8), float_0_80422fd0, *(char**)*(s32*)((s32)obj + 0x64));
            }
                    break;
        }

        if ((*(u32*)((s32)obj + 0x68) & 1) != 0) {
            mapObj = mapGetMapObj(*(char**)*(s32*)((s32)obj + 0x64));
            if ((*(u32*)mapObj & 1) == 0) {
                mapGrpFlagOn(*(char**)*(s32*)((s32)obj + 0x64), 1);
            }
        } else {
            mapObj = mapGetMapObj(*(char**)*(s32*)((s32)obj + 0x64));
            if ((*(u32*)mapObj & 1) != 0) {
                mapGrpFlagOff(*(char**)*(s32*)((s32)obj + 0x64), 1);
            }
        }
    }
    BattleObjectConfig();
}

void BattleObjectConfig(void) {
    ;
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

void BattleStage_DestroyA2(void) {
    extern u32 _object_turn_event_A2[7];
    void* battleWork = _battleWorkPointer;
    void* data = *(void**)((s32)battleWork + 0x2738);
    void* script;

    script = *(void**)((s32)**(void***)((s32)data + 0xC) + 0x198);
    evtDeleteID(*(s32*)((s32)battleWork + 0x20000 - 0x7EFC));
    *(s32*)((s32)battleWork + 0x20000 - 0x7EFC) = 0;
    if (script == 0) {
        script = &_object_turn_event_A2;
    }
    evtEntry(script, 0xA, 0);
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

u32 _set_mobj_rotate_x(void* event, s32 isFirstCall) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* event, s32 value);
    extern void psndSFXOn_3D(char* name, void* pos);
    extern f32 intpl_sub(s32 type, f32 start, f32 end, s32 current, s32 duration);
    extern char vec3_802f42b8[];
    extern f32 float_0_80422fd0;
    extern f32 float_90_80422fd4;
    s32 type;
    char* strings;
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
    typedef struct LocalVec { f32 x, y, z; } LocalVec;
    LocalVec pos;

    strings = vec3_802f42b8;
    battleWork = _battleWorkPointer;
    base = (void*)((s32)battleWork + 0x20000);
    active = 0;
    type = evtGetValue(event, **(s32**)((s32)event + 0x18));

    if (isFirstCall != 0) {
        switch (type) {
        case 0:
        case 1:
            offset = 0;
            for (i = 0; i < 0x20; i++) {
                obj = (void*)((s32)battleWork + offset + 0x1715C);
                if (*(s32*)obj > 0 && *(u16*)((s32)*(void**)((s32)obj + 0x64) + 6) == type) {
                    *(u8*)((s32)obj + 0x70) = *(u8*)((s32)*(void**)((s32)obj + 0x64) + 0x14);
                    *(u8*)((s32)obj + 0x71) = 0;
                }
                offset += 0x7C;
            }
            break;
        case 2:
            *(s32*)((s32)event + 0x7C) = 0;
            *(s32*)((s32)event + 0x80) = 0x3C;
            *(s32*)((s32)event + 0x84) = 0;
            break;
        }
    }

    switch (type) {
    case 0:
    case 1:
        snd0 = strings + 0x240;
        snd1 = strings + 0x258;
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
                    if (*(u8*)((s32)obj + 0x71) < *(u8*)((s32)*(void**)((s32)obj + 0x64) + 0x15)) {
                        active = 1;
                    }
                }
            }
            offset += 0x7C;
        }
        break;
    case 2:
        timer = *(s32*)((s32)event + 0x7C);
        if (timer != 0) {
            *(s32*)((s32)event + 0x7C) = timer - 1;
            active = 1;
        } else {
            if (*(s32*)((s32)event + 0x84) == 0) {
                pos = *(LocalVec*)(strings + 0xFC);
                psndSFXOn_3D(strings + 0x270, &pos);
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

    return active ? 0 : 2;
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
            if (mapGetMapObj((char*)&str_A1_80423028) != 0) {
                tempA1 = dat_80422fb8;
                *(u8*)((s32)&tempA1 + 3) = alpha;
                colorA1 = tempA1;
                mapGrpSetColor((char*)&str_A1_80423028, &colorA1);
            }
            break;
        case 1:
            if (mapGetMapObj((char*)&str_A2_8042302c) != 0) {
                tempA2 = dat_80422fbc;
                *(u8*)((s32)&tempA2 + 3) = alpha;
                colorA2 = tempA2;
                mapGrpSetColor((char*)&str_A2_8042302c, &colorA2);
            }
            break;
        case 2:
            if (mapGetMapObj((char*)&str_B_80423030) != 0) {
                tempB = dat_80422fc0;
                *(u8*)((s32)&tempB + 3) = alpha;
                colorB = tempB;
                mapGrpSetColor((char*)&str_B_80423030, &colorB);
            }
            break;
        case 3:
            if (mapGetMapObj((char*)&str_C_80423034) != 0) {
                tempC = dat_80422fc4;
                *(u8*)((s32)&tempC + 3) = alpha;
                colorC = tempC;
                mapGrpSetColor((char*)&str_C_80423034, &colorC);
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

s32 _bgset_audience_bgset_access(void) {
    BattleAudience_Case_HaikeiSet();
    return 2;
}


s32 _set_mobj_shake_init(void* event) {
    extern s32 irand(s32 max);
    extern f32 float_0_80422fd0;
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


#pragma opt_common_subs off
void _Nozzle_Change_Check(u32 nozzleIdx, u8* turnWeights) {
    extern s32 irand(s32 max);
    void* work = (void*)((s32)_battleWorkPointer + 0x20000 - 0x7F24);
    u32 idx = nozzleIdx & 0xFF;
    u32 directionOffset = idx + 0x1C4;
    u8* weights;
    s32 total;
    s32 i;
    s32 roll;
    u8 direction;
    s32 evtId;
    u8* indexedWork;
    void* evt;

    total = 0;
    weights = turnWeights;
    for (i = 0; i < 4; i++) {
        total += *weights++;
    }

    roll = irand(total);
    weights = turnWeights;
    direction = 0;
    while ((u8)direction < 4) {
        roll -= *weights;
        if (roll < 0) {
            break;
        }
        weights++;
        direction++;
    }

    if ((u8)direction != *(u8*)((s32)work + directionOffset)) {
        indexedWork = (u8*)work + idx * 4;
        evtId = *(s32*)((s32)indexedWork + 0x1CC);
        if (evtId != 0 && evtCheckID(evtId) == 0) {
            *(s32*)((s32)indexedWork + 0x1CC) = 0;
        }
        if (*(s32*)((s32)indexedWork + 0x1CC) == 0) {
            *(u8*)((s32)work + idx + 0x1C7) = direction;
            evt = evtEntry(&_nozzle_turn_event, 0xA, 0);
            *(s32*)((s32)indexedWork + 0x1CC) = *(s32*)((s32)evt + 0x15C);
            if (evt != 0) {
                *(s32*)((s32)evt + 0x160) = nozzleIdx & 0xFF;
            }
        }
    }
}
#pragma opt_common_subs reset

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

void BattleStage_WallCloseCheck(void) {
    extern void* _battleWorkPointer;
    extern u32 _close_wall_event[11];
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

s32 _fall_frame_wait(void) {
    extern s32 BattleWaitAllActiveEvtEnd_NoBgSetEndWait(void* battleWork);
    void* battleWork = _battleWorkPointer;
    void* base = (void*)((s32)battleWork + 0x20000);
    s32 evtId;

    if (BattleWaitAllActiveEvtEnd_NoBgSetEndWait(battleWork) == 0) {
        return 0;
    }

    evtId = *(s32*)((s32)base - 0x7D4C);
    if (evtId != 0) {
        if (evtCheckID(evtId) != 0) {
            return 0;
        }
        *(s32*)((s32)base - 0x7D4C) = 0;
    }
    return 1;
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


s32 _fall_frame(void* event, s32 isFirstCall) {
    extern f32 intpl_sub(s32 type, f32 start, f32 end, s32 current, s32 duration);
    extern f32 float_0_80422fd0;
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

s32 _nozzle_work_effect(void* event, s32 isFirstCall) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 evtSetValue(void* event, s32 target, s32 value);
    extern s32 BattleCheckConcluded(void* battleWork);
    extern void* BattleSearchObjectPtr(char* name);
    extern f32 _get_nozzle_dir(s32 nozzleIdx);
    extern s32 psndSFXOn_3D(char* name, void* pos);
    extern void psndSFXOff(s32 id);
    extern f32 reviseAngle(f32 angle);
    extern f32 sinfd(f32 angle);
    extern f32 cosfd(f32 angle);
    extern void* effNozleEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 rotX, f32 rotY);
    extern void effConfettiN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale);
    extern char vec3_802f42b8[];
    extern f32 float_0_80422fd0;
    extern f32 float_90_80422fd4;
    extern f32 float_neg80_80423008;
    extern f32 float_10_80423000;
    extern f32 float_12_80423004;
    extern f32 float_20_8042300c;
    extern f32 float_8_80423010;
    extern f32 float_1_80423014;
    void* battleWork;
    void* base;
    s32* args;
    s32 typeArg;
    s32 dst;
    s32 nozzleType;
    char* soundName;
    s32 effType;
    s32 active;
    void* obj;
    char* objName;
    s32 soundOff;
    s32 effOff;
    f32 dir;
    f32 angle;
    f32 x;
    f32 y;
    f32 z;
    char* strings;

    battleWork = _battleWorkPointer;
    strings = vec3_802f42b8;
    args = *(s32**)((s32)event + 0x18);
    typeArg = evtGetValue(event, args[0]);
    dst = args[1];
    base = (void*)((s32)battleWork + 0x20000);
    active = 0;

    if (isFirstCall != 0) {
        if (BattleCheckConcluded(battleWork) != 0) {
            evtSetValue(event, dst, 0);
            return 2;
        }

        *(s32*)((s32)event + 0x78) = 0x5A;
        nozzleType = *(s8*)((s32)base - 0x7D5A);
        soundName = 0;
        effType = 0;
        switch (nozzleType) {
            case 1:
                soundName = strings + 0x188;
                effType = 0;
                break;
            case 2:
                soundName = 0;
                effType = 1;
                break;
            case 3:
                soundName = strings + 0x1A0;
                effType = 2;
                break;
        }

        if (nozzleType >= 1 && nozzleType < 4) {
#define DO_NOZZLE(INDEX, NAMEOFF, SOUNDOFF, EFFOFF) \
            obj = BattleSearchObjectPtr(strings + (NAMEOFF)); \
            dir = _get_nozzle_dir(INDEX); \
            if (*(u8*)((s32)base - 0x7D60 + (INDEX)) != 0) { \
                if (soundName != 0) *(s32*)((s32)event + (SOUNDOFF)) = psndSFXOn_3D(soundName, (void*)((s32)obj + 4)); \
                else *(s32*)((s32)event + (SOUNDOFF)) = -1; \
                angle = reviseAngle(float_90_80422fd4 + dir); \
                z = float_10_80423000 * sinfd(dir) + *(f32*)((s32)obj + 0xC); \
                x = *(f32*)((s32)obj + 4) - float_10_80423000 * cosfd(dir); \
                y = float_12_80423004 + *(f32*)((s32)obj + 8); \
                *(void**)((s32)event + (EFFOFF)) = effNozleEntry(effType, 0x5A, x, y, z, float_neg80_80423008, angle); \
            } else { \
                if (soundName != 0) *(s32*)((s32)event + (SOUNDOFF)) = psndSFXOn_3D(soundName, (void*)((s32)obj + 4)); \
                else *(s32*)((s32)event + (SOUNDOFF)) = -1; \
                x = *(f32*)((s32)obj + 4); \
                y = float_20_8042300c + *(f32*)((s32)obj + 8); \
                z = float_8_80423010 + *(f32*)((s32)obj + 0xC); \
                *(void**)((s32)event + (EFFOFF)) = effNozleEntry(effType, 0x5A, x, y, z, float_0_80422fd0, float_0_80422fd0); \
                if (typeArg == 0) effConfettiN64Entry(4, 0x5A, *(f32*)((s32)obj + 4), float_20_8042300c + *(f32*)((s32)obj + 8), float_8_80423010 + *(f32*)((s32)obj + 0xC), float_1_80423014); \
            }
            DO_NOZZLE(0, 0xC0, 0x8C, 0x7C);
            DO_NOZZLE(1, 0xCC, 0x90, 0x80);
            DO_NOZZLE(2, 0xD8, 0x94, 0x84);
#undef DO_NOZZLE
            active = 1;
        } else if (typeArg != 0 && *(void**)((s32)base - 0x7D44) == 0) {
            *(s32*)((s32)event + 0x78) = 0x78;
            *(s32*)((s32)event + 0x7C) = 0;
            *(s32*)((s32)event + 0x80) = 0;
            *(s32*)((s32)event + 0x84) = 0;
            *(void**)((s32)base - 0x7D44) = effNozleEntry(3, 0x3E8, float_0_80422fd0, float_0_80422fd0, float_0_80422fd0, float_0_80422fd0, float_0_80422fd0);
            *(void**)((s32)event + 0x84) = *(void**)((s32)base - 0x7D44);
            *(s32*)((s32)base - 0x7D48) = 2;
            obj = BattleSearchObjectPtr(strings + 0xCC);
            *(s32*)((s32)event + 0x8C) = psndSFXOn_3D(strings + 0x1B4, (void*)((s32)obj + 4));
            *(s32*)((s32)event + 0x90) = -1;
            *(s32*)((s32)event + 0x94) = -1;
            active = 1;
        }

        if (active == 0) {
            evtSetValue(event, dst, 0);
            return 2;
        }
    }

    if (*(s32*)((s32)event + 0x78) > 0) {
        *(s32*)((s32)event + 0x78) -= 1;
        if (*(s32*)((s32)event + 0x78) <= 0) {
            if (*(s32*)((s32)event + 0x8C) != -1) {
                psndSFXOff(*(s32*)((s32)event + 0x8C));
            }
            if (*(s32*)((s32)event + 0x90) != -1) {
                psndSFXOff(*(s32*)((s32)event + 0x90));
            }
            if (*(s32*)((s32)event + 0x94) != -1) {
                psndSFXOff(*(s32*)((s32)event + 0x94));
            }
        }
        return 0;
    }

    evtSetValue(event, dst, 1);
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


s32 BattleFogForceStop(void) {
    extern void effSoftDelete(void* eff);
    void* base = (void*)((s32)_battleWorkPointer + 0x20000);
    void* eff;

    if (*(s32*)((s32)base - 0x7D48) == 0) {
        return 0;
    }

    eff = *(void**)((s32)base - 0x7D44);
    if (eff != 0) {
        effSoftDelete(eff);
        *(void**)((s32)base - 0x7D44) = 0;
    }
    return 1;
}

#pragma opt_common_subs off
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
#pragma opt_common_subs reset


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

void _rate_mix_3(s8* maxValue, s8* totalA, s8* totalB, s8* current, s8* addA, s8* addB) {
    s8 value = *current;

    if (value <= 0) {
        return;
    }
    if (value > *maxValue) {
        *maxValue = value;
    }
    *totalA += *addA;
    *totalB += *addB;
    if (*totalB != 0) {
        return;
    }
    *maxValue = 0;
    *totalA = 0;
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

s32 _object_fall_attack(void* evt, s32 firstCall) {
    extern void* BattleAlloc(s32 size);
    extern void BattleFree(void* ptr);
    extern s32 BattleCheckConcluded(void* battle);
    extern s32 irand(s32 limit);
    extern void* pouchGetPtr(void);
    extern s32 BattleAudience_HaitiRandForFallObject(void);
    extern void* BattleGetSystemPtr(void* battle);
    extern s32 BattleSamplingEnemy(void* targetWork, void* weapon, s32 attackerIdx,
                                   s32 enemyBelong, u32 targetClassFlags,
                                   u32 targetPropertyFlags, s32 direction);
    extern void BattleChoiceSamplingEnemy(void* targetWork, u32 weighting, s32* unitIdx,
                                          s32* partIdx);
    extern void* BattleGetUnitPartsPtr(s32 unitId, s32 partId);
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
    extern s32 BattlePreCheckDamage(void* attacker, void* target, void* part, void* weapon,
                                    u32 flags);
    extern void BtlUnit_GetHitPos(void* unit, void* part, f32* x, f32* y, f32* z);
    extern s32 BtlUnit_GetWidth(void* unit);
    extern void* BattleStageGetPtr(void);
    extern void btl_camera_set_mode(s32 priority, s32 mode);
    extern void btl_camera_set_moveSpeedLv(s32 priority, s32 level);
    extern f32 BattleGetFloorHeight(void* battleWork, f32 x, f32 y, f32 z);
    extern s32 effFallEntry(s32 type, s32 count, s32 time, f32 x, f32 y, f32 z,
                            f32 width, f32 deltaY, f32 angle);
    extern s32 BtlUnit_GetBelong(void* unit);
    extern s32 effFallCheckHit(s32 effect);
    extern void BattleAudience_Case_FallObject_Aud(s32 memberIdx, u8 objectType);
    extern void psndSFXOff(s32 id);
    extern void BattleAudience_GetHomePosition(s32 index, f32* x, f32* y, f32* z);
    extern s32 psndSFXOn_3D(char* id, void* pos);
    extern void BattleAudience_Case_FallObject_Stage(void);
    extern s32 BtlUnit_GetACPossibility(void* unit);
    extern void BattleActionCommandCheckDefence(void* unit, void* weapon);
    extern s32 BattleCheckDamage(void* attacker, void* target, void* part, void* weapon,
                                 u32 flags);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern u8* effNiceEntry(s32 type, f32 x, f32 y, f32 z);
    extern s32 BattleRunHitEvent(void* unit, u32 eventId);
    extern void* BattleAudienceBaseGetPtr(void);
    extern u8 vec3_802f42b8[];
    extern f32 float_0p1_80422fcc;
    extern f32 float_0_80422fd0;
    extern f32 float_90_80422fd4;
    extern f32 float_270_80422fd8;
    extern f32 float_5_80422fdc;
    extern f32 float_neg300_80422fe0;
    extern f32 float_15_80422fe4;
    extern f32 float_300_80422fe8;
    extern f32 float_85_80422fec;
    extern f32 float_50_80422ff0;
    extern f32 float_neg50_80422ff4;
    extern f32 float_45_80422ff8;
    extern f32 float_0p75_80422ffc;

    s32 partChoice;
    s32 unitChoice;
    typedef struct LocalVec { f32 x, y, z; } LocalVec;
    LocalVec fallPos;
    LocalVec position;
    LocalVec hitPos;
    LocalVec nicePos;
    u8 initialTargets[0xACC];
    u8 runtimeTargets[0xACC];
    u32 convA[2];
    u32 convB[2];
    u32 convC[2];
    u8* battle;
    u8* fallData;
    u8* work;
    u8* entry;
    u8* other;
    u8* weapon;
    u8* rodata;
    u8* audience;
    u8* pouch;
    u8* targetRec;
    void* systemUnit;
    void* unit;
    void* parts;
    void* stage;
    u8* nice;
    char* sfxDamage;
    char* sfxDamageAlt;
    char* sfxRest;
    s32 resultArg;
    s32 i;
    s32 j;
    s32 slot;
    s32 total;
    s32 random;
    s32 active;
    s32 flags;
    s32 precheck;
    s32 delay;
    s32 guardCount;
    s32 targetIndex;
    s32 type;
    s32 mode;
    s32 effect;
    s32 done;
    s32 widthInt;
    f32 width;
    f32 angle;

    battle = (u8*)_battleWorkPointer;
    fallData = *(u8**)(battle + 0x18114);
    resultArg = (*(s32**)((u8*)evt + 0x18))[0];
    rodata = vec3_802f42b8;
    active = 0;

    if (firstCall != 0) {
        work = (u8*)BattleAlloc(0xEC);
        *(u8**)((u8*)evt + 0x78) = work;
        *(s32*)((u8*)evt + 0x7C) = 0;
        memset(work, 0, 0xEC);
        work[4] = 0xFF;
        for (i = 0; i < 3; i++) {
            work[i + 1] = 0xFF;
        }

        if (BattleCheckConcluded(battle) != 0) {
            evtSetValue(evt, resultArg, *(s32*)((u8*)evt + 0x7C));
            return 2;
        }

        work[0] = 1;
        for (i = 0; i < (s32)(u8)work[0]; i++) {
            entry = work + 4 + i * 0xE8;
            entry[0] = (u8)i;

            for (slot = 0; slot < 3; slot++) {
                if ((s8)work[slot + 1] == -1) {
                    break;
                }
            }

            if (slot < 3) {
                total = 0;
                for (j = 0; j < 9; j++) {
                    total += (u8)fallData[0xC0 + j];
                }
                random = irand(total);
                entry[1] = 0;
                while ((s8)entry[1] < 8) {
                    random -= (u8)fallData[0xC0 + (s8)entry[1]];
                    if (random < 0) {
                        break;
                    }
                    entry[1]++;
                }
                work[slot + 1] = entry[1];
            } else {
                entry[1] = work[irand(3) + 1];
            }

            {
                u32* dst;
                u32* src;
                dst = (u32*)(entry + 0x28);
                src = (u32*)(fallData + (s8)entry[1] * 0xC0 + 0xCC);
                for (j = 0; j < 24; j++) {
                    dst[0] = src[0];
                    dst[1] = src[1];
                    dst += 2;
                    src += 2;
                }
            }

            if ((s8)entry[1] == 6) {
                if (irand(100) < 50) {
                    *(u32*)(entry + 0x8C) |= 0x100;
                    *(u32*)(entry + 0x8C) &= ~0x200;
                } else {
                    *(u32*)(entry + 0x8C) &= ~0x100;
                    *(u32*)(entry + 0x8C) |= 0x200;
                }
            }

            *(s32*)(entry + 4) = 0;
            pouch = (u8*)pouchGetPtr();
            if (*(u16*)(pouch + 0x8C) != 0) {
                audience = (u8*)BattleAudienceBaseGetPtr();
                if ((*(u32*)audience & 0x4000) == 0 && (s8)entry[1] != 6 &&
                    (s8)entry[1] != 7 &&
                    ((*(u32*)(battle + 0x19078) & 2) != 0 || irand(100) < 10)) {
                    *(s32*)(entry + 4) = 1;
                    *(s32*)(entry + 0x0C) = -1;
                    *(s32*)(entry + 8) = BattleAudience_HaitiRandForFallObject();
                    active = 1;
                    *(s32*)((u8*)evt + 0x7C) = 1;
                }
            }

            if (*(s32*)(entry + 4) == 0) {
                systemUnit = BattleGetSystemPtr(battle);
                BattleSamplingEnemy(initialTargets, entry + 0x28, *(s32*)systemUnit, 1,
                                    *(u32*)(entry + 0x8C), *(u32*)(entry + 0x90),
                                    (s8)(-(s32)battle[0x12]));
                if (*(s8*)(initialTargets + 0xA6C) <= 0) {
                    evtSetValue(evt, resultArg, *(s32*)((u8*)evt + 0x7C));
                    return 2;
                }

                BattleChoiceSamplingEnemy(initialTargets, *(u32*)(entry + 0xA4),
                                          &unitChoice, &partChoice);
                active = 1;
                *(s32*)(entry + 8) = unitChoice;
                *(s32*)(entry + 0x0C) = partChoice;
                entry[0x14] = 1;

                for (j = i; j >= 0; j--) {
                    other = work + 4 + j * 0xE8;
                    if (other != entry && *(s32*)(other + 4) == 0 &&
                        *(s32*)(other + 8) == *(s32*)(entry + 8)) {
                        other[0x14] = 0;
                    }
                }
            }
        }

        if (!active) {
            evtSetValue(evt, resultArg, *(s32*)((u8*)evt + 0x7C));
            return 2;
        }

        for (i = 0; i < (s32)(u8)work[0]; i++) {
            entry = work + 4 + i * 0xE8;
            weapon = entry + 0x28;
            if (*(s32*)(entry + 4) == 0) {
                flags = 0;
                if (entry[0x14] != 0) {
                    flags |= 0x100;
                }
                if (entry[0x15] != 0) {
                    flags |= 0x100000;
                }

                parts = BattleGetUnitPartsPtr(*(s32*)(entry + 8), *(s32*)(entry + 0x0C));
                unit = BattleGetUnitPtr(battle, *(s32*)(entry + 8));
                systemUnit = BattleGetSystemPtr(battle);
                precheck = BattlePreCheckDamage(systemUnit, unit, parts, weapon, flags);
                *(s32*)(entry + 0x1C) = precheck;

                if (precheck == 1 && entry[0x14] == 0) {
                    delay = (s16)irand(0);
                    for (j = 0; j < i; j++) {
                        other = work + 4 + j * 0xE8;
                        if (*(s32*)(entry + 8) == *(s32*)(other + 8)) {
                            *(s16*)(other + 0x10) = (s16)delay;
                            delay = (s16)(delay + irand(0x16) + 0x14);
                            if (other[0x14] != 0) {
                                other[0x15] = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    work = *(u8**)((u8*)evt + 0x78);
    sfxDamage = (char*)(rodata + 0x130);
    sfxDamageAlt = (char*)(rodata + 0x14C);
    sfxRest = (char*)(rodata + 0x16C);
    done = 1;

    for (i = 0; i < (s32)(u8)work[0]; i++) {
        entry = work + 4 + i * 0xE8;
        if ((s8)entry[0] == -1) {
            continue;
        }

        guardCount = 0;
        done = 0;
        weapon = entry + 0x28;

        if (*(s32*)(entry + 4) == 0) {
            unit = BattleGetUnitPtr(battle, *(s32*)(entry + 8));
            parts = BattleGetUnitPartsPtr(*(s32*)(entry + 8), *(s32*)(entry + 0x0C));
        }

        effect = *(s32*)(entry + 0x24);
        if (effect == 0 || *(s16*)(entry + 0x10) > 0) {
            *(s16*)(entry + 0x10) -= 1;
            if (*(s16*)(entry + 0x10) > 0) {
                continue;
            }

            mode = *(s32*)(entry + 4);
            switch (mode) {
            case 0: {
                BtlUnit_GetHitPos(unit, parts, &position.x, &position.y, &position.z);
                widthInt = BtlUnit_GetWidth(unit);
                convA[1] = ((u32)widthInt) ^ 0x80000000;
                convA[0] = 0x43300000;
                width = (f32)(*(f64*)convA - *(f64*)(rodata + 0x180));
                break;
            }
            case 1: {
                unitChoice = *(s32*)(entry + 8);
                convA[0] = 0x43300000;
                convA[1] = ((u32)((unitChoice % 20) * 25 - 230)) ^ 0x80000000;
                position.x = (f32)(*(f64*)convA - *(f64*)(rodata + 0x180));

                stage = BattleStageGetPtr();
                position.y = *(f32*)((u8*)stage + 0xB34);

                convB[0] = 0x43300000;
                convC[0] = 0x43300000;
                convB[1] = ((u32)(unitChoice % 20)) ^ 0x80000000;
                convC[1] = ((u32)((unitChoice / 20) * 30 + 115)) ^ 0x80000000;
                position.z = float_0p1_80422fcc *
                           (f32)(*(f64*)convB - *(f64*)(rodata + 0x180)) +
                       (f32)(*(f64*)convC - *(f64*)(rodata + 0x180));

                btl_camera_set_mode(0, 6);
                btl_camera_set_moveSpeedLv(0, 2);
                width = float_0_80422fd0;
                break;
            }
            default: {
                *(u32*)&position.x = *(u32*)(rodata + 0x124);
                *(u32*)&position.y = *(u32*)(rodata + 0x128);
                *(u32*)&position.z = *(u32*)(rodata + 0x12C);
                width = float_0_80422fd0;
                break;
            }
            }

            fallPos = position;
            fallPos.y = BattleGetFloorHeight(battle, position.x, position.y, position.z);
            angle = (position.x >= float_0_80422fd0) ? float_90_80422fd4 : float_270_80422fd8;

            precheck = *(s32*)(entry + 0x1C);
            if ((precheck >= 2 && precheck < 5) || precheck == 6) {
                position.y = fallPos.y;
            }

            *(s32*)(entry + 0x20) = -1;
            *(s16*)(entry + 0x12) = 0;
            type = (s8)entry[1];
            switch (type) {
                case 0:
                    *(s32*)(entry + 0x24) = effFallEntry(4, 1, 90, fallPos.x, fallPos.y, fallPos.z,
                                                        width, position.y - fallPos.y, angle);
                    break;
                case 1:
                    *(s32*)(entry + 0x24) = effFallEntry(5, 1, 90, fallPos.x, fallPos.y, fallPos.z,
                                                        width, position.y - fallPos.y, angle);
                    break;
                case 2:
                    *(s32*)(entry + 0x24) = effFallEntry(0, 100, 90, fallPos.x, fallPos.y, fallPos.z,
                                                        width, position.y - fallPos.y, angle);
                    break;
                case 3:
                    *(s32*)(entry + 0x24) = effFallEntry(1, 1, 90, fallPos.x, fallPos.y, fallPos.z,
                                                        width, position.y - fallPos.y, angle);
                    break;
                case 4:
                    *(s32*)(entry + 0x24) = effFallEntry(2, 1, 90, fallPos.x, fallPos.y,
                                                        fallPos.z + float_5_80422fdc, width,
                                                        position.y - fallPos.y, angle);
                    break;
                case 8:
                    *(s32*)(entry + 0x24) = effFallEntry(8, 1, 90, fallPos.x, fallPos.y, fallPos.z,
                                                        width, position.y - fallPos.y, angle);
                    break;
                case 5:
                    *(s32*)(entry + 0x24) = effFallEntry(3, 1, 90, fallPos.x, fallPos.y, fallPos.z,
                                                        width, position.y - fallPos.y, angle);
                    break;
                case 6:
                    if ((s8)BtlUnit_GetBelong(unit) == 0) {
                        *(s32*)(entry + 0x24) = effFallEntry(
                            6, 1, 180, float_neg300_80422fe0, fallPos.y,
                            fallPos.z + float_15_80422fe4, float_0_80422fd0,
                            float_0_80422fd0, angle);
                    } else {
                        *(s32*)(entry + 0x24) = effFallEntry(
                            6, 1, 180, float_300_80422fe8, fallPos.y,
                            fallPos.z + float_15_80422fe4, float_0_80422fd0,
                            float_0_80422fd0, angle);
                    }
                    break;
                case 7:
                    *(s32*)(entry + 0x24) = effFallEntry(7, 1, 180, fallPos.x, fallPos.y,
                                                        fallPos.z + float_5_80422fdc, width,
                                                        position.y - fallPos.y, angle);
                    break;
            }
            continue;
        }

        if (effFallCheckHit(effect) == 0) {
            continue;
        }

        if (*(s32*)(entry + 4) == 1) {
            BattleAudience_Case_FallObject_Aud(*(s32*)(entry + 8), entry[1]);
            psndSFXOff(*(s32*)(entry + 0x20));
            BattleAudience_GetHomePosition(*(s32*)(entry + 8), &hitPos.x, &hitPos.y, &hitPos.z);
            type = (s8)entry[1];
            if (type != 5) {
                if (type >= 0 && type < 2) {
                    psndSFXOn_3D(sfxDamage, &hitPos.x);
                } else {
                    psndSFXOn_3D(sfxDamageAlt, &hitPos.x);
                }
            }
        } else {
            BattleAudience_Case_FallObject_Stage();
            if ((*(u32*)(weapon + 0x64) & 0x01000000) != 0) {
                BtlUnit_GetHitPos(unit, parts, &hitPos.x, &hitPos.y, &hitPos.z);
                flags = 0;
                if (entry[0x14] != 0) {
                    flags |= 0x100;
                }
                psndSFXOff(*(s32*)(entry + 0x20));
                precheck = *(s32*)(entry + 0x1C);

                switch (precheck) {
                case 1:
                    type = (s8)entry[1];
                    if (type == 5) {
                        psndSFXOn_3D(sfxRest, &hitPos.x);
                    } else if (type >= 0 && type < 2) {
                        psndSFXOn_3D(sfxDamage, &hitPos.x);
                    } else {
                        psndSFXOn_3D(sfxDamageAlt, &hitPos.x);
                    }
                    if (BtlUnit_GetACPossibility(unit) != 0) {
                        BattleActionCommandCheckDefence(unit, weapon);
                    }
                    BattleCheckDamage(BattleGetSystemPtr(battle), unit, parts, weapon, flags);
                    break;
                case 2:
                case 3:
                case 4:
                case 6:
                    type = (s8)entry[1];
                    if (type >= 0 && type < 2) {
                        psndSFXOn_3D(sfxDamage, &hitPos.x);
                    }
                    break;
                default:
                    break;
                }
            } else {
                psndSFXOff(*(s32*)(entry + 0x20));
                systemUnit = BattleGetSystemPtr(battle);
                BattleSamplingEnemy(runtimeTargets, weapon, *(s32*)systemUnit, 1,
                                    *(u32*)(weapon + 0x64), *(u32*)(weapon + 0x68),
                                    (s8)(-(s32)battle[0x12]));

                if (*(s8*)(runtimeTargets + 0xA6C) > 0) {
                while (*(s8*)(runtimeTargets + 0xAB7) < *(s8*)(runtimeTargets + 0xA6C)) {
                    targetIndex = (s8)runtimeTargets[0xA6D + *(s8*)(runtimeTargets + 0xAB7)];
                    targetRec = runtimeTargets + 4 + targetIndex * 0x24;
                    unit = BattleGetUnitPtr(battle, *(s16*)(targetRec + 0));
                    parts = BattleGetUnitPartsPtr(*(s16*)(targetRec + 0),
                                                  *(s16*)(targetRec + 2));
                    precheck = BattlePreCheckDamage(BattleGetSystemPtr(battle), unit, parts,
                                                    weapon, 0x100);

                    switch (precheck) {
                    case 3:
                        BtlUnit_GetPos(unit, &nicePos.x, &nicePos.y, &nicePos.z);
                        if (nicePos.y < float_85_80422fec) {
                            nicePos.y += float_50_80422ff0;
                        } else {
                            nicePos.y += float_neg50_80422ff4;
                        }
                        nicePos.x -= float_45_80422ff8;
                        nice = effNiceEntry(6, nicePos.x, nicePos.y, nicePos.z);
                        *(f32*)(*(u8**)(nice + 0x0C) + 0x1C) = float_0p75_80422ffc;
                        *(s32*)((u8*)unit + 0x274) = 0;
                        BattleRunHitEvent(unit, 0x126);
                        break;
                    case 6:
                        *(s32*)((u8*)unit + 0x274) = 0;
                        BattleRunHitEvent(unit, 0x127);
                        break;
                    case 1:
                        if (BtlUnit_GetACPossibility(unit) != 0 && guardCount < 1) {
                            BattleActionCommandCheckDefence(unit, weapon);
                            guardCount++;
                        }
                        parts = BattleGetUnitPartsPtr(*(s16*)(targetRec + 0),
                                                      *(s16*)(targetRec + 2));
                        BattleCheckDamage(BattleGetSystemPtr(battle), unit, parts, weapon, 0x100);
                        break;
                    case 2:
                    case 4:
                    default:
                        break;
                    }

                    (*(s8*)(runtimeTargets + 0xAB7))++;
                }
                }
            }
        }

        entry[0] = 0xFF;
    }

    if (done) {
        BattleFree(*(void**)((u8*)evt + 0x78));
        evtSetValue(evt, resultArg, *(s32*)((u8*)evt + 0x7C));
        return 2;
    }
    return 0;
}


/* Exact US stage-hazard parameter catalogs.  Kept as raw storage-layout arrays
 * so the current executable source can continue using its established byte-offset
 * accesses while preserving target data and relocations exactly. */
extern s32 weaponGetPowerDefault();

u8 _nozzle_at_base_mario[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x64, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x70, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u8 _nozzle_at_base_monster[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x64, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x6F, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u32 battle_stage_nozzle_data[] = {
    0x0000283C, 0x001E0046, 0x00283C00, 0x64000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02100060,
    0x20000000, 0x00000302, 0x00000000, 0x00000000, 0x000007FF, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02100060,
    0x20000000, 0x00000302, 0x00000000, 0x00000000, 0x000007FF, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02100060,
    0x20000000, 0x00000302, 0x00000000, 0x00000000, 0x000007FF, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02100060,
    0x20000000, 0x00000302, 0x00000000, 0x00000000, 0x000007FF, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000064, 0x00000000, 0x00000000, 0x00001E46, 0x001E0046,
    0x00283C00, 0x41230000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x02100060, 0x20000000, 0x00000302,
    0x00000000, 0x00000000, 0x000007FF, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000064,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x64000002, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x02100060, 0x20000000, 0x02000302,
    0x00000000, 0x00000000, 0x000007FF, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00640100, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000064,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x02100060, 0x20000000, 0x00000302,
    0x00000000, 0x00000000, 0x000007FF, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000064,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x02100060, 0x20000000, 0x00000302,
    0x00000000, 0x00000000, 0x000007FF, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000064,
    0x00000000, 0x00000000, 0x0000283C, 0x001E0046, 0x00283C00, 0x23192800,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100060, 0x20000000, 0x00000302, 0x00000000, 0x00000000,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100060, 0x20000000, 0x02000302, 0x00000000, 0x00000000,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00640100, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000002, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100060, 0x20000000, 0x03000302, 0x00000000, 0x00000000,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100060, 0x20000000, 0x00000302, 0x00000000, 0x00000000,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x0000283C, 0x001E0046, 0x00283C00, 0x14141923, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02100060,
    0x20000000, 0x00000302, 0x00000000, 0x00000000, 0x000007FF, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x64000002, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02100060,
    0x20000000, 0x02000302, 0x00000000, 0x00000000, 0x000007FF, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00640100, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault,
    0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02100060,
    0x20000000, 0x03000302, 0x00000000, 0x00000000, 0x000007FF, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02100060,
    0x20000000, 0x01000302, 0x00000000, 0x00000000, 0x000007FF, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x03000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000064, 0x00000000, 0x00000000,
};

u32 battle_stage_fall_object_data[] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100060, 0x20000000, 0x000A0302, 0x00000000, 0x00000000,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x000A0000, 0x00000000, 0x0A000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01100060, 0x20000000,
    0x00000302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x00000000,
    0x32030000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01100060, 0x20000000,
    0x00000302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x00000000,
    0x32030000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01100060, 0x20000000,
    0x00010302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x00000000,
    0x00000000, 0x00320300, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01100060, 0x20000000,
    0x00000302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x00000000,
    0x00000000, 0x00320300, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000002,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01100060, 0x20000000,
    0x00000302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x64000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01100060, 0x20000000,
    0x00000302, 0x00000000, 0x00000100, 0x000007FF, 0x80000000, 0x64006400,
    0x64006400, 0x00640064, 0x00000064, 0x00640000, 0x00000000, 0x00000000,
    0x0000009D, 0x00000000, 0x00009D00, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000005,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02100060, 0x20000000,
    0x000A0302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x00000000,
    0x00000000, 0x00640300, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000003,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02100060, 0x20000000,
    0x000A0302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x00000000,
    0x32030000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01100060, 0x20000000,
    0x000A0302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x00000000,
    0x00000000, 0x00000019, 0x03000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02100060, 0x20000000,
    0x000A0302, 0x00000000, 0x00000000, 0x000007FF, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x0A0A0000, 0x000A0000, 0x0A000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100060, 0x20000000, 0x00000302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x00000000, 0x32030000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100060, 0x20000000, 0x00000302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x00000000, 0x32030000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100060, 0x20000000, 0x00010302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00320300, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100060, 0x20000000, 0x00000302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00320300, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000002, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100060, 0x20000000, 0x00000302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x64000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100060, 0x20000000, 0x00000302, 0x00000000, 0x00000100,
    0x000007FF, 0x80000000, 0x64006400, 0x64006400, 0x00640064, 0x00000064,
    0x00640000, 0x00000000, 0x00000000, 0x0000009D, 0x00000000, 0x00009D00,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000005, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100060, 0x20000000, 0x000A0302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00640300, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000003, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100060, 0x20000000, 0x000A0302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x00000000, 0x32030000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100060, 0x20000000, 0x000A0302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000019, 0x03000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100060, 0x20000000, 0x000A0302, 0x00000000, 0x00000000,
    0x000007FF, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x0A0A0A0A, 0x0A0A0000, 0x0A000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01100060, 0x20000000,
    0x00000302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x00000000,
    0x32030000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01100060, 0x20000000,
    0x00000302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x00000000,
    0x32030000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01100060, 0x20000000,
    0x00010302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x00000000,
    0x00000000, 0x00320300, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01100060, 0x20000000,
    0x00000302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x00000000,
    0x00000000, 0x00320300, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000002,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01100060, 0x20000000,
    0x00000302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x64000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01100060, 0x20000000,
    0x00000302, 0x00000000, 0x00000100, 0x000007FF, 0x80000000, 0x64006400,
    0x64006400, 0x00640064, 0x00000064, 0x00640000, 0x00000000, 0x00000000,
    0x0000009D, 0x00000000, 0x00009D00, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000005,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02100060, 0x20000000,
    0x000A0302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x00000000,
    0x00000000, 0x00640300, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000003,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02100060, 0x20000000,
    0x000A0302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x00000000,
    0x32030000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01100060, 0x20000000,
    0x000A0302, 0x00000000, 0x00000000, 0x000007FF, 0x80000000, 0x00000000,
    0x00000000, 0x00000019, 0x03000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x64000002, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x02100060, 0x20000000,
    0x000A0302, 0x00000000, 0x00000000, 0x000007FF, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000064, 0x00000000, 0x00000000, 0x0A0A0A0A, 0x0A0A0A0A, 0x0A000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100060, 0x20000000, 0x00000302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x00000000, 0x32030000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100060, 0x20000000, 0x00000302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x00000000, 0x32030000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100060, 0x20000000, 0x00010302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00320300, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100060, 0x20000000, 0x00000302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00320300, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000002, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100060, 0x20000000, 0x00000302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x64000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100060, 0x20000000, 0x00000302, 0x00000000, 0x00000100,
    0x000007FF, 0x80000000, 0x64006400, 0x64006400, 0x00640064, 0x00000064,
    0x00640000, 0x00000000, 0x00000000, 0x0000009D, 0x00000000, 0x00009D00,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000005, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100060, 0x20000000, 0x000A0302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00640300, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000003, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02100060, 0x20000000, 0x000A0302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x00000000, 0x32030000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x64000002, 0x00000000,
    0x00000000, (u32)weaponGetPowerDefault, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x01100060, 0x20000000, 0x000A0302, 0x00000000, 0x00000000,
    0x000007FF, 0x80000000, 0x00000000, 0x00000000, 0x00000019, 0x03000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000064, 0x00000000, 0x00000000,
};


/* Exact US nozzle stage-object descriptors.
 * Three 24-byte records; raw word layout preserves target storage and relocations. */
extern char str_nozzle_l_802f4378[];
extern char str_nozzle_c_802f4384[];
extern char str_nozzle_r_802f4390[];

u32 _object_nozzle_data[] = {
    (u32)str_nozzle_l_802f4378, 0x00030004, 0xC2F00000, 0x00000000, 0x42B40000, 0x00000000,
    (u32)str_nozzle_c_802f4384, 0x00030004, 0x00000000, 0x00000000, 0x42B40000, 0x00000000,
    (u32)str_nozzle_r_802f4390, 0x00030004, 0x42F00000, 0x00000000, 0x42B40000, 0x00000000,
};



/* Exact US small-data stage-object descriptors.
 * Complete extents are also declared in BattleStageObjectInit so MWCC can
 * preserve SDA21/r13-relative addressing while emitting full target objects. */
extern char str_escape_wall_802f439c[];
extern char str_spotlight_802f43a8[];
extern u8 unit_object_tree[];
extern u8 unit_object_switch[];

u32 _object_wall_data[6] = {
    (u32)str_escape_wall_802f439c,
    0x00040005,
    0xC32F0000,
    0x00000000,
    0x00000000,
    0x00000000,
};

u32 _object_spot_light_data[12] = {
    (u32)str_spotlight_802f43a8,
    0x00060007,
    0x00000000,
    0x43700000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00060007,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
};

u32 _entry_tree_data[12] = {
    (u32)unit_object_tree,
    0x02000000,
    0x04000005,
    0xC47A0000,
    0xC47A0000,
    0xC47A0000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
};

u32 _entry_switch_data[12] = {
    (u32)unit_object_switch,
    0x02000000,
    0x04000005,
    0xC47A0000,
    0xC47A0000,
    0xC47A0000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
};


/* Recovered battle_stage_object event bytecode from exact US target words/relocations. */
extern s32 evt_btl_camera_shake_h();
extern s32 btlevtcmd_check_battleflag();
extern s32 btlevtcmd_GetEnemyBelong();
extern s32 btlevtcmd_SamplingEnemy();
extern s32 btlevtcmd_GetSelectEnemy();
extern s32 btlevtcmd_GetACPossibility();
extern s32 btlevtcmd_ResultACDefence();
extern s32 btlevtcmd_PreCheckDamage();
extern s32 btlevtcmd_StartAvoid();
extern s32 btlevtcmd_CheckDamage();
extern s32 btlevtcmd_GetSelectNextEnemy();
extern s32 btlevtcmd_WaitAttackEnd();
extern s32 btlevtcmd_AnnounceMessage();

const char str_btl_msg_escape_wall__802f43c0[] = "btl_msg_escape_wall_close2";

u32 _object_turn_event_core[] = {
    0x0002005B, (u32)_set_mobj_shake_init, 0xFE363C89, 0x0002005B, (u32)_set_mobj_shake,
    0xFE363C89, 0x0002005B, (u32)_set_mobj_rotate_x, 0xFE363C89, 0x00020019,
    0xFE363C89, 0x00000001, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000,
    0x00000004, 0x00000000, 0x0000001E, 0x0000000D, 0x00000021,
    0x0002005B, (u32)_bgset_kemuri_effect, 0xFE363C89, 0x00020019, 0xFE363C89,
    0x00000001, 0x0001005B, (u32)_bgset_audience_bgset_access, 0x00000021, 0x0003005B,
    (u32)btlevtcmd_check_battleflag, 0xFE363C80, 0x00000100, 0x00020019, 0xFE363C80,
    0x00000000, 0x00010004, 0x00000014, 0x00000021, 0x0003005B,
    (u32)_get_bgset_attack_data, 0xFE363C89, 0xFE363C8C, 0x00020018, 0xFE363C8C,
    0x00000000, 0x00010004, 0x00000014, 0x00000021, 0x0003005B,
    (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFB, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy,
    0xFFFFFFFB, 0xFE363C80, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy,
    0xFE363C8A, 0xFE363C8B, 0x00020032, 0xFE363C8F, 0x00000000,
    0x00010003, 0x0000000A, 0x00020018, 0xFE363C8A, 0xFFFFFFFF,
    0x00010004, 0x00000014, 0x00000021, 0x00020018, 0xFE363C8F,
    0x00000000, 0x0003005B, (u32)btlevtcmd_GetACPossibility, 0xFE363C8A, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x00020035, 0xFE363C8F,
    0x00000001, 0x0003005B, (u32)btlevtcmd_ResultACDefence, 0xFE363C8A, 0xFE363C8C,
    0x00000021, 0x00000021, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFB,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85,
    0x00010022, 0xFE363C85, 0x00010024, 0x00000004, 0x00010024,
    0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000028,
    0x00010024, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A,
    0x00000026, 0x00010024, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C8A, 0x00000027, 0x00010024, 0x00000001, 0x0007005B,
    (u32)btlevtcmd_CheckDamage, 0xFFFFFFFB, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000100, 0xFE363C85, 0x0000002A, 0x00000031, 0x0003005B,
    (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C8A, 0xFE363C8B, 0x00010004, 0x0000000A,
    0x00010003, 0x00000014, 0x00010009, 0x00000032, 0x00020032,
    0xFE363C80, 0x000000FF, 0x00020032, 0xFE363C81, 0x00000033,
    0x00010003, 0x00000032, 0x00020036, 0xFE363C80, 0x00000005,
    0x0003005B, (u32)_mobj_set_alpha, 0xFE363C89, 0xFE363C80, 0x00020036,
    0xFE363C81, 0x00000001, 0x0002001B, 0xFE363C81, 0x00000000,
    0x00010009, 0x00000001, 0x00010004, 0x00000032, 0x00000021,
    0x0002005B, (u32)_mobj_destroy, 0xFE363C89, 0x00000002, 0x00000001,
};

u32 _fall_frame_event[] = {
    0x0001005B, (u32)_fall_frame_wait, 0x0001005B, (u32)_fall_frame_wait_unit_ready, 0x0002005B,
    (u32)_fall_frame, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFB,
    0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFB, 0xFE363C80,
    0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B,
    0x00020032, 0xFE363C8F, 0x00000000, 0x00010003, 0x0000000A,
    0x00020018, 0xFE363C8A, 0xFFFFFFFF, 0x00010004, 0x00000014,
    0x00000021, 0x00020018, 0xFE363C8F, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_GetACPossibility, 0xFE363C8A, 0xFE363C80, 0x00020019, 0xFE363C80,
    0x00000000, 0x00020035, 0xFE363C8F, 0x00000001, 0x0003005B,
    (u32)btlevtcmd_ResultACDefence, 0xFE363C8A, 0xFE363C8C, 0x00000021, 0x00000021,
    0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFB, 0xFE363C8A, 0xFE363C8B,
    0xFE363C8C, 0x00000100, 0xFE363C85, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy,
    0xFE363C8A, 0xFE363C8B, 0x00010004, 0x0000000A, 0x00010003,
    0x00000014, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000, 0x0000000A,
    0x00000000, 0x00000014, 0x0000000D, 0x0001005B, (u32)_fall_frame_delete,
    0x00000002, 0x00000001,
};

u32 _nozzle_work_event_core[] = {
    0x0001005B, (u32)_nozzle_work_wait, 0x0003005B, (u32)_nozzle_work_effect, 0xFE363C8A,
    0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000, 0x00000002,
    0x00000021, 0x0001005B, (u32)_nozzle_work_attack_set, 0x0003005B, (u32)_nozzle_work_get_weapon_address,
    0x00000000, 0xFE363C8C, 0x00020018, 0xFE363C8C, 0x00000000,
    0x00010004, 0x00000013, 0x00000021, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong,
    0xFFFFFFFB, 0xFE363C80, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFB,
    0xFE363C80, 0xFE363C8C, 0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A,
    0xFE363C8B, 0x00020032, 0xFE363C8F, 0x00000000, 0x00010003,
    0x0000000A, 0x00020018, 0xFE363C8A, 0xFFFFFFFF, 0x00010004,
    0x00000013, 0x00000021, 0x00020018, 0xFE363C8F, 0x00000000,
    0x0003005B, (u32)btlevtcmd_GetACPossibility, 0xFE363C8A, 0xFE363C80, 0x00020019,
    0xFE363C80, 0x00000000, 0x00020035, 0xFE363C8F, 0x00000001,
    0x0003005B, (u32)btlevtcmd_ResultACDefence, 0xFE363C8A, 0xFE363C8C, 0x00000021,
    0x00000021, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFB, 0xFE363C8A,
    0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85, 0x00010022,
    0xFE363C85, 0x00010024, 0x00000004, 0x00010024, 0x00000002,
    0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000028, 0x00010024,
    0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000026,
    0x00010024, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A,
    0x00000027, 0x00010024, 0x00000001, 0x0007005B, (u32)btlevtcmd_CheckDamage,
    0xFFFFFFFB, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100,
    0xFE363C85, 0x0000002A, 0x00000031, 0x0003005B, (u32)btlevtcmd_GetSelectNextEnemy,
    0xFE363C8A, 0xFE363C8B, 0x00010004, 0x0000000A, 0x00010003,
    0x00000013, 0x0003005B, (u32)_nozzle_work_get_weapon_address, 0x00000001, 0xFE363C8C,
    0x00020018, 0xFE363C8C, 0x00000000, 0x00010004, 0x0000001D,
    0x00000021, 0x0003005B, (u32)btlevtcmd_GetEnemyBelong, 0xFFFFFFFB, 0xFE363C80,
    0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFB, 0xFE363C80, 0xFE363C8C,
    0x0003005B, (u32)btlevtcmd_GetSelectEnemy, 0xFE363C8A, 0xFE363C8B, 0x00010003,
    0x00000014, 0x00020018, 0xFE363C8A, 0xFFFFFFFF, 0x00010004,
    0x0000001D, 0x00000021, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFB,
    0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00000100, 0xFE363C85,
    0x00010022, 0xFE363C85, 0x00010024, 0x00000004, 0x00010024,
    0x00000002, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A, 0x00000028,
    0x00010024, 0x00000003, 0x0003005B, (u32)btlevtcmd_StartAvoid, 0xFE363C8A,
    0x00000026, 0x00010024, 0x00000006, 0x0003005B, (u32)btlevtcmd_StartAvoid,
    0xFE363C8A, 0x00000027, 0x00010024, 0x00000001, 0x0007005B,
    (u32)btlevtcmd_CheckDamage, 0xFFFFFFFB, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000100, 0xFE363C85, 0x0000002A, 0x00000031, 0x0003005B,
    (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C8A, 0xFE363C8B, 0x00010004, 0x00000014,
    0x00010003, 0x0000001D, 0x00000002, 0x00000001,
};

u32 _object_fall_event[] = {
    0x0001005B, (u32)_object_fall_wait, 0x0002005B, (u32)_object_fall_attack, 0xFE363C80,
    0x00020018, 0xFE363C80, 0x00000001, 0x0001000A, 0x000003E8,
    0x00000021, 0x00000002, 0x00000001,
};

u32 _object_turn_event_A1[] = {
    0x00020032, 0xFE363C89, 0x00000000, 0x0001005E, (u32)_object_turn_event_core,
    0x00000002, 0x00000001,
};

u32 _object_turn_event_A2[7] = {
    0x00020032, 0xFE363C89, 0x00000001, 0x0001005E, (u32)_object_turn_event_core,
    0x00000002, 0x00000001,
};

u32 _object_turn_event_B[] = {
    0x00020032, 0xFE363C89, 0x00000002, 0x0001005E, (u32)_object_turn_event_core,
    0x00000002, 0x00000001,
};

u32 _nozzle_turn_event[] = {
    0x0001005B, (u32)_nozzle_turn, 0x00000002, 0x00000001,
};

u32 _close_wall_event[11] = {
    0x0001005B, (u32)btlevtcmd_WaitAttackEnd,
    0x0006005B, (u32)btlevtcmd_AnnounceMessage,
    0x00000002, 0x00000000, 0xFFFFFF1F,
    (u32)(char*)str_btl_msg_escape_wall__802f43c0,
    0x000000B4, 0x00000002, 0x00000001,
};

u32 _nozzle_work_event_0[] = {
    0x00020032, 0xFE363C8A, 0x00000000, 0x0001005E, (u32)_nozzle_work_event_core,
    0x00000002, 0x00000001,
};

u32 _nozzle_work_event_1[] = {
    0x00020032, 0xFE363C8A, 0x00000001, 0x0001005E, (u32)_nozzle_work_event_core,
    0x00000002, 0x00000001,
};

/* Recovered battle_stage_object .sdata2 scalar catalog.
 * Late definitions preserve the existing consumer declaration shapes while
 * restoring the exact US target values and source order. */
__declspec(section ".sdata2") u32 dat_80422fb8 = 0xFFFFFF00;
__declspec(section ".sdata2") u32 dat_80422fbc = 0xFFFFFF00;
__declspec(section ".sdata2") u32 dat_80422fc0 = 0xFFFFFF00;
__declspec(section ".sdata2") u32 dat_80422fc4 = 0xFFFFFF00;
__declspec(section ".sdata2") u32 dat_80422fc8 = 0xFFFFFF00;
__declspec(section ".sdata2") f32 float_0p1_80422fcc = 0.1f;
__declspec(section ".sdata2") const f32 float_0_80422fd0 = 0.0f;
__declspec(section ".sdata2") f32 float_90_80422fd4 = 90.0f;
__declspec(section ".sdata2") f32 float_270_80422fd8 = 270.0f;
__declspec(section ".sdata2") f32 float_5_80422fdc = 5.0f;
__declspec(section ".sdata2") f32 float_neg300_80422fe0 = -300.0f;
__declspec(section ".sdata2") f32 float_15_80422fe4 = 15.0f;
__declspec(section ".sdata2") f32 float_300_80422fe8 = 300.0f;
__declspec(section ".sdata2") f32 float_85_80422fec = 85.0f;
__declspec(section ".sdata2") f32 float_50_80422ff0 = 50.0f;
__declspec(section ".sdata2") f32 float_neg50_80422ff4 = -50.0f;
__declspec(section ".sdata2") f32 float_45_80422ff8 = 45.0f;
__declspec(section ".sdata2") f32 float_0p75_80422ffc = 0.75f;
__declspec(section ".sdata2") f32 float_10_80423000 = 10.0f;
__declspec(section ".sdata2") f32 float_12_80423004 = 12.0f;
__declspec(section ".sdata2") f32 float_neg80_80423008 = -80.0f;
__declspec(section ".sdata2") f32 float_20_8042300c = 20.0f;
__declspec(section ".sdata2") f32 float_8_80423010 = 8.0f;
__declspec(section ".sdata2") f32 float_1_80423014 = 1.0f;
__declspec(section ".sdata2") f32 float_neg45_80423018 = -45.0f;
__declspec(section ".sdata2") f32 float_neg135_8042301c = -135.0f;
__declspec(section ".sdata2") f32 float_neg90_80423020 = -90.0f;
__declspec(section ".sdata2") f32 float_30_80423024 = 30.0f;

/* Small-data stage object names.  The first four were already proven by A7r
 * to retain SDA21 addressing.  str_den has a complete 4-byte declaration
 * above so MWCC can classify that consumer the same way while these real
 * string definitions preserve the exact target .sdata2 layout. */
__declspec(section ".sdata2") const char str_A1_80423028[] = "A1";
__declspec(section ".sdata2") const char str_A2_8042302c[] = "A2";
__declspec(section ".sdata2") const char str_B_80423030[] = "B";
__declspec(section ".sdata2") const char str_C_80423034[] = "C";
__declspec(section ".sdata2") const char str_den_80423038[4] = "den";
