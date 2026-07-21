#include "database.h"

void setupDataLoad(char* name) {
    extern void* ptr;
    extern u32 size;
    extern char* getMarioStDvdRoot(void);
    extern s32 sprintf(char* str, const char* fmt, ...);
    extern s32 strcmp(const char* a, const char* b);
    extern s32 evtGetValue(void* event, s32 arg);
    extern void* arcOpen(const char* filename, void** address, u32* length);
    extern void* DVDMgrOpen(const char* path, s32 mode, s32 unk);
    extern u32 DVDMgrGetLength(void* entry);
    extern void DVDMgrRead(void* entry, void* dst, u32 size, u32 offset);
    extern void DVDMgrClose(void* entry);
    extern void* _mapAlloc(void* heap, u32 size);
    extern void* mapalloc_base_ptr;
    extern const char str_PCTs_m_PCTs_s_802c3b20[];
    extern const char str_muj_20_802c3b2c[];
    char path[64];
    void* entry;

    ptr = NULL;
    sprintf(path, str_PCTs_m_PCTs_s_802c3b20, getMarioStDvdRoot(), name);
    if (strcmp(name, str_muj_20_802c3b2c) == 0 && evtGetValue(NULL, 0xF5DE0180) == 2) {
        return;
    }
    arcOpen(path, &ptr, &size);
    if (ptr != NULL) {
        return;
    }
    entry = DVDMgrOpen(path, 2, 0);
    if (entry != NULL) {
        size = DVDMgrGetLength(entry);
        if (size != 0) {
            ptr = _mapAlloc(mapalloc_base_ptr, (size + 0x1F) & ~0x1F);
            DVDMgrRead(entry, ptr, (size + 0x1F) & ~0x1F, 0);
        }
        DVDMgrClose(entry);
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 setupDataCheck(void) {
    extern void* ptr;
    extern u32 size;
    extern void parseInit(void* data, u32 size);
    extern s32 parsePush(const char* tag);
    extern void parsePop(void);
    extern void parseTagGet1(const char* tag, s32 index, char* out);
    extern void parsePopNext(void);
    extern void* npcGetTribe(char* name);
    extern s32 animGroupBaseAsync(const char* name, s32 a, s32 b);
    extern void* gp;
    extern const char str_tribe_802c3a50[];
    extern char str_Npc_804207b8[];
    s32 useLang;
    const char* tribeTag = str_tribe_802c3a50;
    char name[64];

    if (ptr == NULL) {
        return 1;
    }
    parseInit(ptr, size);
    while (parsePush(str_Npc_804207b8) != 0) {
        parseTagGet1(tribeTag, 0, name);
        parsePopNext();
        useLang = (s32)((u32)(*(s32*)((s32)gp + 0x14) | -*(s32*)((s32)gp + 0x14)) >> 31);
        if (animGroupBaseAsync(*(char**)((s32)npcGetTribe(name) + 4), useLang, 0) == 0) {
            return 0;
        }
    }
    parsePop();
    return 1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



void setupDataBase(char* area, char* map) {
    extern void* fbatGetPointer(void);
    extern s32 strcmp(const char* a, const char* b);
    extern char* strcpy(char* dst, const char* src);
    extern void* memset(void* dst, s32 value, u32 size);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern void parseInit(void* data, u32 size);
    extern u32 parsePush(const char* tag);
    extern void parseTagGet1(const char* tag, s32 type, void* out, ...);
    extern void parsePopNext(void);
    extern void parsePop(void);
    extern void* npcGetTribe(char* name);
    extern s32 npcEntry(char* name, char* model);
    extern void* npcNameToPtr(char* name);
    extern void* ptr;
    extern u32 size;
    extern void* _map_attr_data[];
    extern char str_Npc_804207b8[];
    extern char str_dir_804207ac[];

    void* fbat = fbatGetPointer();
    u8* dead = (u8*)fbat + 0x4C;
    void** attr = _map_attr_data;
    void** currentAttr;
    void** previousAttr;
    void* tribe;
    void* npc;
    u32 index;
    u32 attrFlags;
    char name[76];
    char tribeName[64];
    char aiType[64];
    char territoryType[64];
    char battleSetup[64];
    f32 position[3];
    f32 direction;
    f32 territoryBase[3];
    f32 territoryLoiter[3];
    f32 territoryHoming[3];
    f32 searchRange;
    f32 searchAngle;
    f32 homingRange;
    f32 homingAngle;

    while (attr[0] != 0 && strcmp((char*)attr[0], map) != 0) {
        attr += 3;
    }
    currentAttr = attr;

    for (index = 0; index < 64; index++) {
        u8* entry = dead + index * 0x14;
        if (entry[0] == 0) {
            memset(entry, 0, 0x14);
            strcpy((char*)entry, map);
            break;
        }
        if (strcmp((char*)entry, map) == 0) {
            break;
        }
    }

    if (index > 0 && index < 64) {
        u8 saved[20];
        memcpy(saved, dead + index * 0x14, 0x14);
        while (index > 0) {
            memcpy(dead + index * 0x14, dead + (index - 1) * 0x14, 0x14);
            index--;
        }
        memcpy(dead, saved, 0x14);
    }

    attrFlags = (u32)currentAttr[2];
    if (attrFlags & 1) {
        previousAttr = _map_attr_data;
        while (previousAttr[0] != 0 &&
               strcmp((char*)previousAttr[0], (char*)(dead + 0x14)) != 0) {
            previousAttr += 3;
        }
        if (currentAttr[1] == previousAttr[1]) {
            if ((attrFlags & 2) == 0 && (attrFlags & 4)) {
                memset(dead + 0x28, 0, 0x4D8);
            }
        } else {
            memset(dead, 0, 0x500);
            strcpy((char*)dead, map);
        }
    }

    index = 0;
    if (ptr != 0) {
        parseInit(ptr, size);
        while (parsePush(str_Npc_804207b8) != 0) {
            parseTagGet1("<name>", 0, name, 0);
            parseTagGet1("<tribe>", 0, tribeName, 0);
            parseTagGet1("<position>", 3, position, 0);
            parseTagGet1(str_dir_804207ac, 2, &direction, 0);
            parseTagGet1("<aitype>", 0, aiType, 0);
            parseTagGet1("<territory_type>", 0, territoryType, 0);
            parseTagGet1("<territory_base>", 3, territoryBase, 0);
            parseTagGet1("<territory_loiter>", 3, territoryLoiter, 0);
            parseTagGet1("<territory_homing>", 3, territoryHoming, 0);
            parseTagGet1("<search_range>", 2, &searchRange, 0);
            parseTagGet1("<search_angle>", 2, &searchAngle, 0);
            parseTagGet1("<homing_range>", 2, &homingRange, 0);
            parseTagGet1("<homing_angle>", 2, &homingAngle, 0);
            parseTagGet1("<btl_setup_no>", 0, battleSetup, 0);
            parsePopNext();

            if ((attrFlags & 1) == 0 ||
                ((*(u32*)(dead + 0x10) & (1U << (index & 31))) == 0)) {
                tribe = npcGetTribe(tribeName);
                npcEntry(name, *(char**)((s32)tribe + 4));
                npc = npcNameToPtr(name);
                *(u8*)((s32)npc + 0x314) = (u8)index;
                *(u32*)npc |= 4;
                *(f32*)((s32)npc + 0x8C) = position[0];
                *(f32*)((s32)npc + 0x90) = position[1];
                *(f32*)((s32)npc + 0x94) = position[2];
                *(f32*)((s32)npc + 0x144) = direction;
                *(void**)((s32)npc + 0x28) = tribe;
                *(f32*)((s32)npc + 0x14C) = *(f32*)((s32)tribe + 0x28);
                *(f32*)((s32)npc + 0x150) = *(f32*)((s32)tribe + 0x2C);
                *(u32*)npc &= ~2U;
            }
            index++;
        }
        parsePop();
    }
}
