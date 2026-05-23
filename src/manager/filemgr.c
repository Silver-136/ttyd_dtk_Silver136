#include "manager/filemgr.h"

extern void _fileAlloc();

void fileAlloc(void* name, unsigned long size) {
    _fileAlloc(name, size, 0);
}

void fileSetCurrentArchiveType(s32 type) {
    extern void* afp;
    ((s32*)afp)[1] = type;
}
