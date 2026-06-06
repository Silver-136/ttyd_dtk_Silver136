#include "sdk/arc.h"

BOOL ARCClose(ARCFileInfo* info) {
    return TRUE;
}

u32 ARCGetLength(ARCFileInfo* info) {
    return info->location;
}

void* ARCGetStartAddrInMem(ARCFileInfo* info) {
    return (void*)((s32)info->handle->archiveStartAddr + info->entryNum);
}

BOOL ARCConvertEntrynumToPath(ARCHandle* handle, u32 entryNum, char* path, u32 maxlen);

void ARCGetCurrentDir(ARCHandle* handle, char* path, u32 maxlen) {
    ARCConvertEntrynumToPath(handle, handle->currDir, path, maxlen);
}

u32 myStrncpy(char* dst, char* src, u32 maxlen) {
    u32 len = maxlen;

    while (len != 0 && *(s8*)src != 0) {
        char value = *src;
        len--;
        src++;
        *dst = value;
        dst++;
    }
    return maxlen - len;
}

u32 ARCConvertPathToEntrynum(int param_1, char* param_2) {
    return 0;
}

int entryToPath(ARCHandle* handle, u32 entryNum, char* path, u32 maxlen) {
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
BOOL ARCConvertEntrynumToPath(ARCHandle* handle, u32 entryNum, char* path, u32 maxlen) {
    typedef struct ARCEntry {
        u32 flags;
        u32 location;
        u32 next;
    } ARCEntry;
    ARCEntry* entries;
    u32 len;

    entries = (ARCEntry*)handle->FSTStart;
    len = entryToPath(handle, entryNum, path, maxlen);
    if (len == maxlen) {
        path[maxlen - 1] = 0;
        return FALSE;
    }
    if (entries[entryNum].flags & 0xFF000000) {
        if (len == maxlen - 1) {
            path[len] = 0;
            return FALSE;
        }
        path[len++] = '/';
    }
    path[len] = 0;
    return TRUE;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
BOOL ARCOpen(ARCHandle* handle, const char* filename, ARCFileInfo* info) {
    typedef struct ARCEntry {
        u32 flags;
        u32 location;
        u32 next;
    } ARCEntry;
    ARCEntry* entries;
    s32 entryNum;
    char path[128];

    entries = (ARCEntry*)handle->FSTStart;
    entryNum = ARCConvertPathToEntrynum((int)handle, (char*)filename);
    if (entryNum < 0) {
        ARCGetCurrentDir(handle, path, sizeof(path));
        return FALSE;
    }
    if (entryNum < 0 || (entries[entryNum].flags & 0xFF000000)) {
        return FALSE;
    }
    info->handle = handle;
    info->entryNum = entries[entryNum].location;
    info->location = entries[entryNum].next;
    return TRUE;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
BOOL ARCInitHandle(void* data, ARCHandle* handle) {
    extern void OSPanic(const char* file, int line, const char* fmt, ...);
    void* fst;

    if (*(u32*)data != 0x55AA382D) {
        OSPanic("arc.c", 0x46, "ARCInitHandle: bad archive format");
    }
    handle->archiveStartAddr = data;
    fst = (void*)((u32)data + *(u32*)((u32)data + 4));
    handle->FSTStart = fst;
    handle->fileStart = (void*)((u32)data + *(u32*)((u32)data + 0xC));
    handle->entryNum = *(u32*)((u32)fst + 8);
    handle->FSTStringStart = (char*)((u32)fst + handle->entryNum * 0xC);
    handle->FSTLength = *(u32*)((u32)data + 8);
    handle->currDir = 0;
    return TRUE;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
