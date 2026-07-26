#pragma once

#include <dolphin/types.h>

typedef struct EffEntry {
    u32 flags;
    s32 unk4;
    void* unk8;
    void* data;
    void* unk10;
    void* unk14;
    char name[0x10];
} EffEntry;

void effInit(void);
void effTexSetup(void);
void effGetTexObj(s32 id, void* texObj);
void effAutoRelease(s32 value);
EffEntry* effEntry(void);
void effSetName(EffEntry* entry, char* name);
void effMain(void);
void effDelete(EffEntry* entry);
void effSoftDelete(EffEntry* entry);
void* effNameToPtr(char* name);
void* effGetSet(char* name);
int* effMayaAnimAlloc(void* effData);
void effDeleteMayaAnim(void* ptr);
void effPlayMayaAnim(void* anim);
u8 effCalcMayaAnim(void* anim);
void effDrawMayaPoly(void* data);
