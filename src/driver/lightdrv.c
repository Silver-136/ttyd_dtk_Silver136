#include "driver/lightdrv.h"

typedef struct LightWorkSet {
    s32 count;
    LightEntry* entries;
} LightWorkSet;

extern LightWorkSet work[2];
extern LightEntry paperCraft;
extern LightEntry paperLight3D;
extern LightEntry paperLight3DImg;
void* memset(void* dst, int value, u32 size);
s32 mapGetActiveGroup(void);
s32 camGetCurNo(void);

static LightWorkSet* currentLightWork(void) {
    if (mapGetActiveGroup() == 1) {
        return &work[1];
    }
    return &work[0];
}

s32 _sort(void* a, void* b) {
    if (*(f32*)((s32)a + 4) > *(f32*)((s32)b + 4)) {
        return 1;
    }
    if (*(f32*)((s32)a + 4) < *(f32*)((s32)b + 4)) {
        return -1;
    }
    return 0;
}

LightEntry* lightNumberToPtr(s32 index) {
    s32 group = mapGetActiveGroup();
    LightWorkSet* set = work;
    LightEntry* entry;
    if (group == 1) {
        set++;
    }
    entry = &set->entries[index];
    if (entry->flags & LIGHT_ENABLED) {
        return entry;
    }
    return 0;
}

s32 lightGetEntryNum(void) {
    s32 group = mapGetActiveGroup();
    LightWorkSet* set = work;
    LightEntry* entry;
    s32 i;
    s32 entryCount;
    s32 count;
    if (group == 1) {
        set++;
    }
    entryCount = set->count;
    count = 0;
    entry = set->entries;
    for (i = 0; i < entryCount; i++, entry++) {
        if (entry->flags & LIGHT_ENABLED) {
            count++;
        }
    }
    return count;
}

LightEntry* lightGetPaperCraft(void) {
    return &paperCraft;
}

LightEntry* lightGetPaper(void) {
    s32 cam = camGetCurNo();
    if (cam == 4) {
        return &paperLight3D;
    }
    if (cam == 6) {
        return &paperLight3DImg;
    }
    return &paperLight3DImg;
}

s32 lightCheckCharaLight(void) {
    s32 group = mapGetActiveGroup();
    LightWorkSet* set = work;
    LightEntry* entry;
    s32 i;
    if (group == 1) {
        set++;
    }
    entry = set->entries;
    for (i = 0; i < set->count; i++, entry++) {
        if ((entry->flags & LIGHT_ENABLED) && (entry->flags & LIGHT_CHARA)) {
            return 1;
        }
    }
    return 0;
}

LightEntry* lightGetCharaAmbient(void) {
    s32 group = mapGetActiveGroup();
    LightWorkSet* set = work;
    LightEntry* entry;
    s32 i;
    if (group == 1) {
        set++;
    }
    entry = set->entries;
    for (i = 0; i < set->count; i++, entry++) {
        if ((entry->flags & LIGHT_ENABLED) && (entry->flags & LIGHT_CHARA) && (entry->flags & LIGHT_AMBIENT)) {
            return entry;
        }
    }
    return 0;
}

LightEntry* lightGetAmbient(void) {
    s32 group = mapGetActiveGroup();
    LightWorkSet* set = work;
    LightEntry* entry;
    s32 i;
    if (group == 1) {
        set++;
    }
    entry = set->entries;
    for (i = 0; i < set->count; i++, entry++) {
        if ((entry->flags & LIGHT_ENABLED) && !(entry->flags & LIGHT_CHARA) && (entry->flags & LIGHT_AMBIENT)) {
            return entry;
        }
    }
    return 0;
}

void lightReInit(void) {
    s32 group = mapGetActiveGroup();
    LightWorkSet* set = work;
    if (group == 1) {
        set++;
    }
    memset(set->entries, 0, set->count * sizeof(LightEntry));
}

int lightEntry(char* pName, int lightType, void* pos, void* rot, GXColor* color, int distanceAttenType, u32 flags, double spotAngle, double angleAtten) {
    return 0;
}


u8 lightInit(void) {
    return 0;
}


u8 lightGetNearObj(void* pPos, LightEntry** pDst, int count, LightEntryFlags flags) {
    return 0;
}


u8 lightMain(void) {
    return 0;
}


void* lightNameToPtr(char* name) {
    return 0;
}
