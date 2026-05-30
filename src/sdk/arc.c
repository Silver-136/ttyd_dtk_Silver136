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

void ARCConvertEntrynumToPath(ARCHandle* handle, u32 entryNum, char* path, u32 maxlen);

void ARCGetCurrentDir(ARCHandle* handle, char* path, u32 maxlen) {
    ARCConvertEntrynumToPath(handle, handle->currDir, path, maxlen);
}

u32 myStrncpy(char* dst, char* src, u32 maxlen) {
    u32 len = maxlen;

    while (len != 0 && *src != 0) {
        *dst = *src;
        len--;
        src++;
        dst++;
    }
    return maxlen - len;
}


u32 ARCConvertPathToEntrynum(int param_1, char* param_2) {
    return 0;
}


int entryToPath(int param_1, int param_2, int param_3, int param_4) {
    return 0;
}


void ARCConvertEntrynumToPath(ARCHandle* handle, u32 entryNum, char* path, u32 maxlen) {
}


BOOL ARCOpen(ARCHandle* handle, const char* filename, ARCFileInfo* info) {
    return 0;
}


BOOL ARCInitHandle(void* data, ARCHandle* handle) {
    return 0;
}
