#include "sdk/arc.h"
#include <dolphin/os.h>
#include "libc_decls.h"

#pragma unsigned_char off

typedef struct FSTEntry {
    u32 entryInfo; //top byte is 0 file 1 dir, u24 string offset
    u32 entryOffsetOrParent; //file = offset, dir = parent index
    u32 entrySizeOrNext; //file = size, dir = next entry
} FSTEntry;

static inline int tolower(int c) { return (c == -1) ? -1 : (int)__lower_map[c & 0xFF]; }

#define entryIsDir(i) (((FstStart[i].entryInfo & 0xff000000) == 0) ? FALSE : TRUE)
#define stringOff(i) (FstStart[i].entryInfo & ~0xff000000)
#define parentDir(i) (FstStart[i].entryOffsetOrParent)
#define nextDir(i) (FstStart[i].entrySizeOrNext)
#define filePosition(i) (FstStart[i].entryOffsetOrParent)
#define fileLength(i) (FstStart[i].entrySizeOrNext)

BOOL ARCGetCurrentDir(ARCHandle* handle, char* path, u32 maxlen);

// NOTE: functions are emitted in reverse source order (target starts with ARCClose)

BOOL ARCInitHandle(void* arcStart, ARCHandle* handle) {
    FSTEntry* entry;
    ARCHeader* header;

    header = (ARCHeader*)arcStart;
    if (header->magic != 0x55AA382D) {
        OSPanic(__FILE__, 70, "ARCInitHandle: bad archive format\0\0");
    }
    handle->archiveStartAddr = arcStart;
    entry = (FSTEntry*)((u32)arcStart + header->fstStart);
    handle->FSTStart = (void*)entry; //relative->absolute
    handle->fileStart = (void*)((u32)arcStart + header->fileStart); //relative->absolute
    handle->entryNum = entry->entrySizeOrNext; //entry 0 size is entry count
    handle->FSTStringStart = (char*)&entry[handle->entryNum];
    handle->FSTLength = header->fstSize;
    handle->currDir = 0;
    return TRUE;
}

BOOL ARCOpen(ARCHandle* handle, const char* filename, ARCFileInfo* info) {
    s32 entry;
    char currentDir[128];
    FSTEntry* FstStart = (FSTEntry*)handle->FSTStart;

    entry = ARCConvertPathToEntrynum(handle, filename);
    if (0 > entry) {
        ARCGetCurrentDir(handle, currentDir, 128);
        return FALSE;
    }
    if ((0 > entry) || entryIsDir(entry)) {
        return FALSE;
    }
    info->handle = handle;
    info->startOffset = filePosition(entry);
    info->length = fileLength(entry);
    return TRUE;
}

static BOOL isSame(const char* path, const char* string) {
    while (*string != '\0') {
        if (tolower(*path++) != tolower(*string++)) {
            return FALSE;
        }
    }
    if ((*path == '/') || (*path == '\0')) {
        return TRUE;
    }
    return FALSE;
}

s32 ARCConvertPathToEntrynum(ARCHandle* handle, const char* pathPtr) {
    const char* ptr;
    char* stringPtr;
    BOOL isDir;
    s32 length;
    u32 dirLookAt;
    u32 i;
    FSTEntry* FstStart;

    dirLookAt = handle->currDir;
    FstStart = (FSTEntry*)handle->FSTStart;

    while (1) {
        if (*pathPtr == '\0') {
            return (s32)dirLookAt;
        } else if (*pathPtr == '/') {
            dirLookAt = 0;
            pathPtr++;
            continue;
        } else if (*pathPtr == '.') {
            if (*(pathPtr + 1) == '.') {
                if (*(pathPtr + 2) == '/') {
                    dirLookAt = parentDir(dirLookAt);
                    pathPtr += 3;
                    continue;
                } else if (*(pathPtr + 2) == '\0') {
                    return (s32)parentDir(dirLookAt);
                }
            } else if (*(pathPtr + 1) == '/') {
                pathPtr += 2;
                continue;
            } else if (*(pathPtr + 1) == '\0') {
                return (s32)dirLookAt;
            }
        }

        for (ptr = pathPtr; (*ptr != '\0') && (*ptr != '/'); ptr++) {
            ;
        }

        isDir = (*ptr == '\0') ? FALSE : TRUE;
        length = (s32)(ptr - pathPtr);
        ptr = pathPtr;

        for (i = dirLookAt + 1; i < nextDir(dirLookAt); i = entryIsDir(i) ? nextDir(i) : (i + 1)) {
            if ((entryIsDir(i) == FALSE) && (isDir == TRUE)) {
                continue;
            }
            stringPtr = handle->FSTStringStart + stringOff(i);
            if (isSame(ptr, stringPtr) == TRUE) {
                goto next_hier;
            }
        }
        return -1;

    next_hier:
        if (isDir == FALSE) {
            return (s32)i;
        }
        dirLookAt = i;
        pathPtr += length + 1;
    }
}

u32 myStrncpy(char* dest, char* src, u32 maxlen) {
    u32 i = maxlen;
    while ((i > 0) && (*src != 0)) {
        *dest++ = *src++;
        i--;
    }
    return maxlen - i;
}

#pragma inline_depth(3)
static u32 entryToPath(ARCHandle* handle, u32 entry, char* path, u32 maxlen) {
    char* name;
    u32 loc;
    FSTEntry* FstStart = (FSTEntry*)handle->FSTStart;

    if (entry == 0) {
        return 0;
    }
    name = handle->FSTStringStart + stringOff(entry);
    loc = entryToPath(handle, parentDir(entry), path, maxlen);
    if (loc == maxlen) {
        return loc;
    }
    *(path + loc++) = '/';
    loc += myStrncpy(path + loc, name, maxlen - loc);
    return loc;
}

BOOL ARCConvertEntrynumToPath(ARCHandle* handle, s32 entrynum, char* path, u32 maxlen) {
    u32 loc;
    FSTEntry* FstStart = (FSTEntry*)handle->FSTStart;

    loc = entryToPath(handle, (u32)entrynum, path, maxlen);
    if (loc == maxlen) {
        path[maxlen - 1] = '\0';
        return FALSE;
    }
    if (entryIsDir(entrynum)) {
        if (loc == maxlen - 1) {
            path[loc] = '\0';
            return FALSE;
        }
        path[loc++] = '/';
    }
    path[loc] = '\0';
    return TRUE;
}

BOOL ARCGetCurrentDir(ARCHandle* handle, char* path, u32 maxlen) {
    return ARCConvertEntrynumToPath(handle, (s32)handle->currDir, path, maxlen);
}

void* ARCGetStartAddrInMem(ARCFileInfo* info) {
    ARCHandle* handle = info->handle;
    return (void*)((u32)handle->archiveStartAddr + info->startOffset);
}

u32 ARCGetLength(ARCFileInfo* info) {
    return info->length;
}

BOOL ARCClose(ARCFileInfo* info) {
    return TRUE;
}
