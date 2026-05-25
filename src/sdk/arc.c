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
