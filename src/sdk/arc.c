#include "sdk/arc.h"

u32 ARCConvertPathToEntrynum(int param_1, char* param_2);
void ARCGetCurrentDir(ARCHandle* handle, char* path, u32 maxlen);
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
    u32 isDir;
    char path[128];

    entries = (ARCEntry*)handle->FSTStart;
    entryNum = ARCConvertPathToEntrynum((int)handle, (char*)filename);
    if (entryNum < 0) {
        ARCGetCurrentDir(handle, path, sizeof(path));
        return FALSE;
    }
    isDir = entries[entryNum].flags & 0xFF000000;
    if (entryNum < 0 || (((u32)(-isDir) | isDir) >> 31) != 0) {
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
u32 ARCConvertPathToEntrynum(int param_1, char* param_2) {
    typedef struct ARCEntry {
        u32 flags;
        u32 location;
        u32 next;
    } ARCEntry;
    extern u8 __lower_map[];
    ARCHandle* handle;
    u32 dir;
    ARCEntry* entries;
    u32 i;
    u8* path;
    u8* ptr;
    u8* p;
    u8* name;
    u32 length;
    BOOL isDir;
    BOOL match;
    u8 c;
    s32 a;
    s32 b;

    handle = (ARCHandle*)param_1;
    dir = handle->currDir;
    entries = (ARCEntry*)handle->FSTStart;
    path = (u8*)param_2;

    while (TRUE) {
        if ((s8)*path == 0) {
            return dir;
        } else if ((s8)*path == '/') {
            dir = 0;
            path++;
            continue;
        } else if ((s8)*path == '.') {
            if ((s8)path[1] == '.') {
                if (path[2] == '/') {
                    dir = entries[dir].location;
                    path += 3;
                    continue;
                } else if ((s8)path[2] == 0) {
                    return entries[dir].location;
                }
            } else if ((s8)path[1] == '/') {
                path += 2;
                continue;
            } else if ((s8)path[1] == 0) {
                return dir;
            }
        }

        for (ptr = path; (s8)*ptr != 0 && (s8)*ptr != '/'; ptr++) {
        }
        isDir = ((u32)(-(s32)(s8)*ptr) | (u32)(s8)*ptr) >> 31;
        length = ptr - path;
        p = path;

        for (i = dir + 1; i < entries[dir].next;
             i = (entries[i].flags & 0xFF000000) ? entries[i].next : i + 1) {
            if ((entries[i].flags & 0xFF000000) == 0 && isDir) {
                continue;
            }
            name = (u8*)(handle->FSTStringStart + (entries[i].flags & 0xFFFFFF));
            p = path;
            while (TRUE) {
                c = *name;
                if ((s8)c == 0) {
                    c = *p;
                    match = (c == '/' || (s8)c == 0) ? TRUE : FALSE;
                    break;
                }
                name++;
                a = ((s8)c == -1) ? -1 : __lower_map[c];
                c = *p++;
                b = ((s8)c == -1) ? -1 : __lower_map[c];
                if (b != a) {
                    match = FALSE;
                    break;
                }
            }
            if (match == TRUE) {
                goto next_hier;
            }
        }
        return 0xFFFFFFFF;

    next_hier:
        if (!isDir) {
            return i;
        }
        dir = i;
        path += length + 1;
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


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
#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
int entryToPath(ARCHandle* handle, u32 entryNum, char* path, u32 maxlen) {
    typedef struct ARCEntry {
        u32 flags;
        u32 location;
        u32 next;
    } ARCEntry;
    ARCEntry* entries;
    char* stringStart;
    char* name1;
    char* name2;
    char* name0;
    char* name3;
    u32 parent;
    u32 len;
    u32 remaining;
    char* out;

    entries = (ARCEntry*)handle->FSTStart;
    if (entryNum == 0) {
        return 0;
    }
    stringStart = handle->FSTStringStart;

    name0 = stringStart + (entries[entryNum].flags & 0xFFFFFF);
    parent = entries[entryNum].location;
    if (parent == 0) {
        len = 0;
    } else {
        name1 = stringStart + (entries[parent].flags & 0xFFFFFF);
        parent = entries[parent].location;
        if (parent == 0) {
            len = 0;
        } else {
            name2 = stringStart + (entries[parent].flags & 0xFFFFFF);
            parent = entries[parent].location;
            if (parent == 0) {
                len = 0;
            } else {
                name3 = stringStart + (entries[parent].flags & 0xFFFFFF);
                len = entryToPath(handle, entries[parent].location, path, maxlen);
                if (len != maxlen) {
                    path[len] = '/';
                    len++;
                    len += myStrncpy(path + len, name3, maxlen - len);
                }
            }
            if (len != maxlen) {
                path[len] = '/';
                out = path + len + 1;
                remaining = maxlen - (len + 1);
                while (remaining != 0 && *(s8*)name2 != 0) {
                    char c = *name2;
                    remaining--;
                    name2++;
                    *out = c;
                    out++;
                }
                len = (len + 1) + ((maxlen - (len + 1)) - remaining);
            }
        }
        if (len != maxlen) {
            path[len] = '/';
            out = path + len + 1;
            remaining = maxlen - (len + 1);
            while (remaining != 0 && *(s8*)name1 != 0) {
                char c = *name1;
                remaining--;
                name1++;
                *out = c;
                out++;
            }
            len = (len + 1) + ((maxlen - (len + 1)) - remaining);
        }
    }
    if (len != maxlen) {
        path[len] = '/';
        out = path + len + 1;
        remaining = maxlen - (len + 1);
        while (remaining != 0 && *(s8*)name0 != 0) {
            char c = *name0;
            remaining--;
            name0++;
            *out = c;
            out++;
        }
        len = (len + 1) + ((maxlen - (len + 1)) - remaining);
    }
    return len;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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


void ARCGetCurrentDir(ARCHandle* handle, char* path, u32 maxlen) {
    ARCConvertEntrynumToPath(handle, handle->currDir, path, maxlen);
}

void* ARCGetStartAddrInMem(ARCFileInfo* info) {
    return (void*)((s32)info->handle->archiveStartAddr + info->entryNum);
}

u32 ARCGetLength(ARCFileInfo* info) {
    return info->location;
}

BOOL ARCClose(ARCFileInfo* info) {
    return TRUE;
}
