#include "database.h"


void setupDataBase(char* area, char* map) {
    ;
}


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

