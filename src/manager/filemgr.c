#include "manager/filemgr.h"

extern void _fileAlloc();

void fileAlloc(void* name, unsigned long size) {
    _fileAlloc(name, size, 0);
}

void fileSetCurrentArchiveType(s32 type) {
    extern void* afp;
    ((s32*)afp)[1] = type;
}


u8 fileGarbageDataAdrClear(int param_1) {
    return 0;
}


u8 fileGarbageDataAdrSet(char* pFileData, int fileKind) {
    return 0;
}


char* fileAsync(char* param_1, s32 param_2, s32 param_3) {
    return 0;
}


void _fileAlloc() {
}


u8 _fileGarbage(int param_1) {
    return 0;
}


u8 fileInit(void) {
    return 0;
}


void* fileAllocf(s32, const char*, ...) {
    return 0;
}


u8 fileGarbageMoveMem(void* param_1, char* param_2) {
    return 0;
}


u8 dvdReadDoneCallBack(int param_1, int param_2) {
    return 0;
}


s32 fileAsyncf(s32, s32, const char*, ...) {
    return 0;
}


void fileFree(void*) {
}
