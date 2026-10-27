#pragma once

#include "manager/dvdmgr.h"
#include <dolphin/types.h>

typedef void (*FileCallback)(struct FileEntry* file);

// FileEntry.type, the fileAlloc/fileAsync type: which pointers fileGarbageDataAdrSet relocates after loading
// (types 3 and 6..10 relocate one to three pointers of a file layout with no other use in the code)
typedef enum FileType {
    FILE_TYPE_RAW = 0,              // nothing to relocate (rel, .bin, .dat files)
    FILE_TYPE_POSE_UNUSED = 1,      // AnimationPoseData without its animation table; no caller asks for it
    FILE_TYPE_ANIM_DATA_UNUSED = 2, // one AnimData; no caller asks for it
    FILE_TYPE_TPL = 4,              // texture palette (TEXPalette)
    FILE_TYPE_ANIM = 5,             // animation model in a/: AnimationPoseData and its AnimData table
    FILE_TYPE_CANCELED = 11,        // the DVD read was canceled (dvdReadDoneCallBack)
} FileType;

typedef struct FileEntry {
    u8 state; //0x0
    s8 type; //0x1, FileType
    s16 references; //0x2
    void* dataSlot; //0x4, data points here unless the data is in a smart block
    u8 pad_8[0x20 - 0x8]; //0x8
    char filename[64]; //0x20
    u8 pad_60[0xA0 - 0x60]; //0x60
    void** data; //0xA0
    struct FileEntry* next; //0xA4
    FileCallback callback; //0xA8
    DVDEntry* entry; //0xAC
} FileEntry;

typedef struct FileManagerWork {
    FileEntry* entries; //0x0
    s32 archiveType; //0x4
    FileEntry* firstused; //0x8
    FileEntry* lastused; //0xC
    FileEntry* firstavailable; //0x10
    FileEntry* lastavailable; //0x14
} FileManagerWork;

void fileInit(void);
void fileGarbageMoveMem(void* data, FileEntry* file);
void _fileGarbage(BOOL a1);
FileEntry* fileAllocf(s32 type, const char* format, ...);
FileEntry* fileAlloc(const char* filename, s32 type);
void fileFree(FileEntry* handle);
s32 fileAsyncf(s32 type, FileCallback callback, const char* format, ...);
void fileSetCurrentArchiveType(s32 type);
