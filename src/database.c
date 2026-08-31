#include "database.h"
#include "driver/arcdrv.h"

void* ptr;
u32 size;

void setupDataLoad(char* name) {
    extern void* ptr;
    extern u32 size;
    extern char* getMarioStDvdRoot(void);
    extern s32 sprintf(char* str, const char* fmt, ...);
    extern s32 strcmp(const char* a, const char* b);
    extern s32 evtGetValue(void* event, s32 arg);
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
    typedef struct DeadInfo {
        u32 word0;
        u32 word1;
        u32 word2;
        u32 word3;
        u32 word4;
    } DeadInfo;
    typedef struct NpcAiTypeTable {
        char* name;
        u32 flags;
        void* initEvt;
        void* moveEvt;
        void* deadEvt;
        void* findEvt;
        void* lostEvt;
        void* returnEvt;
        void* blowEvt;
    } NpcAiTypeTable;
    extern void* fbatGetPointer(void);
    extern s32 strcmp(const char* a, const char* b);
    extern char* strcpy(char* dst, const char* src);
    extern void* memset(void* dst, s32 value, u32 size);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern void parseInit(void* data, u32 size);
    extern u32 parsePush(const char* tag);
    extern void parseTagGet1(const char* tag, s32 type, void* out);
    extern void parsePopNext(void);
    extern void parsePop(void);
    extern void* npcGetTribe(char* name);
    extern s32 npcEntry(char* name, char* model);
    extern void* npcNameToPtr(char* name);
    extern void* dbGetDefData(void* table, char* name);
    extern void* areaDataPtr(char* area);
    extern void npcSetBattleInfo(void* npc, s32 info);
    extern void animPoseSetAnim(s32 poseId, char* anim, s32 force);
    extern void* ptr;
    extern u32 size;
    extern void* _map_attr_data[];
    extern NpcAiTypeTable npc_ai_type_table[];
    extern void* npc_define_territory_type[];
    extern char str_Npc_804207b8[];
    extern char str_dir_804207ac[];
    extern char str_BTLNO_INVALID_802c3b10[];
    extern char zero_804207a8;
    extern f32 float_0_804207b4;

    void* fbat = fbatGetPointer();
    u8* dead = (u8*)fbat + 0x4C;
    void** attr = _map_attr_data;
    void** currentAttr;
    void** previousAttr;
    void* tribe;
    void* npc;
    NpcAiTypeTable* ai;
    void* areaData;
    s32 battleInfo;
    s32 index;
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

    {
        u8* deadScan = (u8*)fbat;

        index = 0;
        do {
            if (strcmp((char*)deadScan + 0x4C, &zero_804207a8) == 0) {
                u8* entry = dead + index * 0x14;
                memset(entry, 0, 0x14);
                strcpy((char*)entry, map);
                break;
            }
            if (strcmp((char*)deadScan + 0x4C, map) == 0) {
                break;
            }
            index++;
            deadScan += 0x14;
        } while (index < 64);
    }

    if (index > 0) {
        DeadInfo* entry = (DeadInfo*)(dead + index * 0x14);
        DeadInfo saved;
        saved.word0 = entry->word0;
        saved.word1 = entry->word1;
        saved.word2 = entry->word2;
        saved.word3 = entry->word3;
        saved.word4 = entry->word4;
        while (index >= 4) {
            entry->word0 = (entry - 1)->word0;
            entry->word1 = (entry - 1)->word1;
            entry->word2 = (entry - 1)->word2;
            entry->word3 = (entry - 1)->word3;
            entry->word4 = (entry - 1)->word4;
            entry--;
            entry->word0 = (entry - 1)->word0;
            entry->word1 = (entry - 1)->word1;
            entry->word2 = (entry - 1)->word2;
            entry->word3 = (entry - 1)->word3;
            entry->word4 = (entry - 1)->word4;
            entry--;
            entry->word0 = (entry - 1)->word0;
            entry->word1 = (entry - 1)->word1;
            entry->word2 = (entry - 1)->word2;
            entry->word3 = (entry - 1)->word3;
            entry->word4 = (entry - 1)->word4;
            entry--;
            entry->word0 = (entry - 1)->word0;
            entry->word1 = (entry - 1)->word1;
            entry->word2 = (entry - 1)->word2;
            entry->word3 = (entry - 1)->word3;
            entry->word4 = (entry - 1)->word4;
            entry--;
            index -= 4;
        }
        while (index > 0) {
            entry->word0 = (entry - 1)->word0;
            entry->word1 = (entry - 1)->word1;
            entry->word2 = (entry - 1)->word2;
            entry->word3 = (entry - 1)->word3;
            entry->word4 = (entry - 1)->word4;
            entry--;
            index--;
        }
        ((DeadInfo*)dead)->word0 = saved.word0;
        ((DeadInfo*)dead)->word1 = saved.word1;
        ((DeadInfo*)dead)->word2 = saved.word2;
        ((DeadInfo*)dead)->word3 = saved.word3;
        ((DeadInfo*)dead)->word4 = saved.word4;
    }

    attrFlags = (u32)currentAttr[2];
    if (attrFlags & 1) {
        void** checkAttr = _map_attr_data;
        while (checkAttr[0] != 0 &&
               strcmp((char*)checkAttr[0], map) != 0) {
            checkAttr += 3;
        }
        previousAttr = _map_attr_data;
        while (previousAttr[0] != 0 &&
               strcmp((char*)previousAttr[0], (char*)(dead + 0x14)) != 0) {
            previousAttr += 3;
        }
        if (checkAttr[1] == previousAttr[1]) {
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
            parseTagGet1("<name>", 0, name);
            parseTagGet1("<tribe>", 0, tribeName);
            parseTagGet1("<position>", 3, position);
            parseTagGet1(str_dir_804207ac, 2, &direction);
            parseTagGet1("<aitype>", 0, aiType);
            parseTagGet1("<territory_type>", 0, territoryType);
            parseTagGet1("<territory_base>", 3, territoryBase);
            parseTagGet1("<territory_loiter>", 3, territoryLoiter);
            parseTagGet1("<territory_homing>", 3, territoryHoming);
            parseTagGet1("<search_range>", 2, &searchRange);
            parseTagGet1("<search_angle>", 2, &searchAngle);
            parseTagGet1("<homing_range>", 2, &homingRange);
            parseTagGet1("<homing_angle>", 2, &homingAngle);
            parseTagGet1("<btl_setup_no>", 0, battleSetup);
            parsePopNext();

            if ((attrFlags & 1) == 0 ||
                ((*(u32*)(dead + 0x10) & (1U << (index & 63))) == 0)) {
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

                for (ai = npc_ai_type_table; ai->name != 0; ai++) {
                    if (strcmp(ai->name, aiType) == 0) {
                        *(u32*)npc |= ai->flags;
                        *(void**)((u8*)npc + 0x120) = ai->initEvt;
                        *(void**)((u8*)npc + 0x124) = ai->moveEvt;
                        *(void**)((u8*)npc + 0x12C) = ai->deadEvt;
                        *(void**)((u8*)npc + 0x130) = ai->findEvt;
                        *(void**)((u8*)npc + 0x134) = ai->lostEvt;
                        *(void**)((u8*)npc + 0x138) = ai->returnEvt;
                        *(void**)((u8*)npc + 0x13C) = ai->blowEvt;
                        break;
                    }
                }

                *(s32*)((u8*)npc + 0x1F8) = (s32)dbGetDefData(npc_define_territory_type, territoryType);
                *(f32*)((u8*)npc + 0x1FC) = territoryBase[0];
                *(f32*)((u8*)npc + 0x200) = territoryBase[1];
                *(f32*)((u8*)npc + 0x204) = territoryBase[2];
                *(f32*)((u8*)npc + 0x208) = territoryLoiter[0];
                *(f32*)((u8*)npc + 0x20C) = territoryLoiter[1];
                *(f32*)((u8*)npc + 0x210) = territoryLoiter[2];
                *(f32*)((u8*)npc + 0x214) = territoryHoming[0];
                *(f32*)((u8*)npc + 0x218) = territoryHoming[1];
                *(f32*)((u8*)npc + 0x21C) = territoryHoming[2];
                *(f32*)((u8*)npc + 0x220) = searchRange;
                *(f32*)((u8*)npc + 0x224) = searchAngle;
                *(f32*)((u8*)npc + 0x228) = homingRange;
                *(f32*)((u8*)npc + 0x22C) = homingAngle;
                *(void**)((u8*)npc + 0x1E0) = *(void**)((u8*)tribe + 0x40);
                *(void**)((u8*)npc + 0x1E4) = *(void**)((u8*)tribe + 0x44);
                *(u16*)((u8*)npc + 0x1E8) = *(u16*)((u8*)tribe + 0x48);
                *(void**)((u8*)npc + 0x1EC) = *(void**)((u8*)tribe + 0x4C);
                *(void**)((u8*)npc + 0x1F0) = *(void**)((u8*)tribe + 0x50);
                *(u16*)((u8*)npc + 0x1D8) = 1;
                *(f32*)((u8*)npc + 0x1DC) = float_0_804207b4;

                areaData = areaDataPtr(area);
                battleInfo = -1;
                if (strcmp(battleSetup, str_BTLNO_INVALID_802c3b10) != 0) {
                    battleInfo = (s32)dbGetDefData(*(void***)((u8*)areaData + 0xC), battleSetup);
                }
                npcSetBattleInfo(npc, battleInfo);
                if (*(char**)((u8*)tribe + 0x10) != 0) {
                    strcpy((char*)npc + 0x4C, *(char**)((u8*)tribe + 0x10));
                }
                if (*(char**)((u8*)tribe + 0x14) != 0) {
                    strcpy((char*)npc + 0x6C, *(char**)((u8*)tribe + 0x14));
                }
                if (*(char**)((u8*)tribe + 8) != 0) {
                    strcpy((char*)npc + 0x2C, *(char**)((u8*)tribe + 8));
                    animPoseSetAnim(*(s32*)((u8*)npc + 0x104), (char*)npc + 0x2C, 1);
                }
            }
            index++;
        }
        parsePop();
    }
}

const char zero_804207a8 = 0;
const char str_dir_804207ac[] = "<dir>";
const f32 float_0_804207b4 = 0.0f;
const char str_Npc_804207b8[] = "<Npc>";

const char str_PCTs_m_PCTs_s_802c3b20[] = "%s/m/%s/s";
const char str_muj_20_802c3b2c[] = "muj_20";
