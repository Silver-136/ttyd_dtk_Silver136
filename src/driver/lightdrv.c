#include "driver/lightdrv.h"

typedef struct LightWorkSet {
    s32 count;
    LightEntry* entries;
} LightWorkSet;

extern LightWorkSet work[];
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
    f32 lhs = *(f32*)((s32)a + 4);
    f32 rhs = *(f32*)((s32)b + 4);
    if (lhs > rhs) {
        return 1;
    }
    if (lhs < rhs) {
        return -1;
    }
    return 0;
}

LightEntry* lightNumberToPtr(s32 index) {
    LightEntry* entry = &currentLightWork()->entries[index];
    if (entry->flags & LIGHT_ENABLED) {
        return entry;
    }
    return 0;
}

s32 lightGetEntryNum(void) {
    LightWorkSet* set = currentLightWork();
    LightEntry* entry = set->entries;
    s32 i;
    s32 count = 0;
    for (i = 0; i < set->count; i++, entry++) {
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
    if (camGetCurNo() == 4) {
        return &paperLight3D;
    }
    return &paperLight3DImg;
}

s32 lightCheckCharaLight(void) {
    LightWorkSet* set = currentLightWork();
    LightEntry* entry = set->entries;
    s32 i;
    for (i = 0; i < set->count; i++, entry++) {
        if ((entry->flags & LIGHT_ENABLED) && (entry->flags & LIGHT_CHARA)) {
            return 1;
        }
    }
    return 0;
}

LightEntry* lightGetCharaAmbient(void) {
    LightWorkSet* set = currentLightWork();
    LightEntry* entry = set->entries;
    s32 i;
    for (i = 0; i < set->count; i++, entry++) {
        if ((entry->flags & LIGHT_ENABLED) && (entry->flags & LIGHT_CHARA) && (entry->flags & LIGHT_AMBIENT)) {
            return entry;
        }
    }
    return 0;
}

LightEntry* lightGetAmbient(void) {
    LightWorkSet* set = currentLightWork();
    LightEntry* entry = set->entries;
    s32 i;
    for (i = 0; i < set->count; i++, entry++) {
        if ((entry->flags & LIGHT_ENABLED) && !(entry->flags & LIGHT_CHARA) && (entry->flags & LIGHT_AMBIENT)) {
            return entry;
        }
    }
    return 0;
}

void lightReInit(void) {
    LightWorkSet* set = currentLightWork();
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
