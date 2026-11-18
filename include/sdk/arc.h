// src/sdk/arc.c: the SDK ARC archive reader (ARCHeader, ARCHandle).

#pragma once

#include <dolphin/types.h>

typedef struct ARCHeader {
    u32 magic;
    s32 fstStart;
    u32 fstSize;
    s32 fileStart;
    s32 reserved[4];
} ARCHeader;

typedef struct ARCHandle {
    void* archiveStartAddr;
    void* FSTStart;
    void* fileStart;
    u32 entryNum;
    char* FSTStringStart;
    u32 FSTLength;
    u32 currDir;
} ARCHandle;

typedef struct ARCFileInfo {
    ARCHandle* handle;
    u32 startOffset;
    u32 length;
} ARCFileInfo;

// ARCOpenDir / ARCReadDir (stripped from every build): DVDDir and DVDDirEntry with the archive handle first, as in
// ARCFileInfo (ARCReadDir has no handle parameter and reads the file table through dir->handle). The order is fixed
// only by those two stripped functions' sizes, not by any live code.
typedef struct ARCDir {
    ARCHandle* handle;
    u32 entryNum;
    u32 location;
    u32 next;
} ARCDir;

typedef struct ARCDirEntry {
    ARCHandle* handle;
    u32 entryNum;
    BOOL isDir;
    char* name;
} ARCDirEntry;

s32 ARCConvertPathToEntrynum(ARCHandle* handle, const char* path);
BOOL ARCOpen(ARCHandle* handle, const char* filename, ARCFileInfo* info);
BOOL ARCFastOpen(ARCHandle* handle, s32 entrynum, ARCFileInfo* info);
BOOL ARCInitHandle(void* arcStart, ARCHandle* handle);
BOOL ARCGetCurrentDir(ARCHandle* handle, char* path, u32 maxlen);

void* ARCGetStartAddrInMem(ARCFileInfo* info);
u32 ARCGetStartOffset(ARCFileInfo* info);
u32 ARCGetLength(ARCFileInfo* info);
BOOL ARCClose(ARCFileInfo* info);
BOOL ARCChangeDir(ARCHandle* handle, const char* dirName);
BOOL ARCOpenDir(ARCHandle* handle, const char* dirName, ARCDir* dir);
BOOL ARCReadDir(ARCDir* dir, ARCDirEntry* dirent);
BOOL ARCCloseDir(ARCDir* dir);
