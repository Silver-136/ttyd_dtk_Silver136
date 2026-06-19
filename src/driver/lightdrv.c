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
s32 strcmp(const char* str1, const char* str2);

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
    extern s32 sprintf(char* buffer, const char* format, ...);
    extern void strcpy(char* dst, const char* src);
    extern const char str_PCTs_PCT04d_802bf930[];
    extern Vec decay_tbl[4];
    LightWorkSet* set;
    LightEntry* entry;
    s32 i;
    s32 j;
    char nameBuf[264];

    set = work;
    if (mapGetActiveGroup() == 1) {
        set++;
    }

    entry = set->entries;
    i = 0;
    while (i < set->count) {
        if ((entry->flags & LIGHT_ENABLED) != 0) {
            if (strcmp(entry->name, pName) == 0) {
                break;
            }
        }
        i++;
        entry++;
    }

    if (i < 0x20) {
        j = 0;
        while (1) {
            sprintf(nameBuf, str_PCTs_PCT04d_802bf930, pName, j);
            pName = nameBuf;
            entry = set->entries;
            i = 0;
            while (i < set->count) {
                if ((entry->flags & LIGHT_ENABLED) != 0) {
                    if (strcmp(entry->name, pName) == 0) {
                        break;
                    }
                }
                i++;
                entry++;
            }
            if (i >= 0x20) {
                break;
            }
            j++;
        }
    }

    i = 0;
    entry = set->entries;
    if (set->count > 0) {
        do {
            if ((entry->flags & LIGHT_ENABLED) == 0) {
                break;
            }
            i++;
            entry++;
        } while (i < set->count);
    }

    memset(entry, 0, sizeof(LightEntry));
    strcpy(entry->name, pName);

    entry->position = *(Vec*)pos;
    entry->rotation = *(Vec*)rot;
    entry->field_0x3C = *(Vec*)pos;
    entry->color = *color;
    entry->spotAngle = (f32)spotAngle;
    entry->attenAngle = (f32)angleAtten;
    entry->attenDistCoef = decay_tbl[distanceAttenType];

    entry->flags |= LIGHT_ENABLED;
    if (lightType == 2) {
        entry->flags |= LIGHT_SPOT;
    } else if (lightType < 2) {
        if (lightType == 0) {
            entry->flags |= LIGHT_AMBIENT;
        } else if (lightType > -1) {
            entry->flags |= LIGHT_POINT;
        }
    } else if (lightType < 4) {
        entry->flags |= LIGHT_DIRECTIONAL;
    }

    if ((flags & 2) != 0) {
        entry->flags |= LIGHT_CHARA;
    }

    return i;
}

void lightInit(void) {
    extern void* __memAlloc(s32 heap, u32 size);
    extern void strcpy(char* dst, const char* src);
    extern const char str_PAPER_8041f8c4[];
    extern const char str_PAPERI_802bf938[];
    extern const char str_PAPERCRAFT_802bf940[];
    extern const char str_BooBoo_802bf94c[];
    extern Vec vec3_802bf870;
    extern Vec vec3_802bf87c;
    extern Vec vec3_802bf888;
    extern Vec vec3_802bf894;
    extern Vec vec3_802bf8a0;
    extern Vec vec3_802bf8ac;
    extern Vec vec3_802bf8b8;
    extern Vec vec3_802bf8c4;
    extern GXColor dat_8041f8b0;
    extern GXColor dat_8041f8b4;
    extern GXColor dat_8041f8b8;
    extern GXColor dat_8041f8bc;
    extern f32 float_0_8041f8cc;
    extern f32 float_1_8041f8d0;
    extern f32 float_3_8041f8d4;
    extern LightEntry booLight;
    f32 scaleMtx[3][4];

    work[0].count = 0x20;
    work[0].entries = __memAlloc(0, work[0].count * sizeof(LightEntry));
    work[1].count = 0x20;
    work[1].entries = __memAlloc(0, work[1].count * sizeof(LightEntry));

    paperLight3D.flags = LIGHT_POINT | LIGHT_NO_DIST_ATTEN;
    strcpy(paperLight3D.name, str_PAPER_8041f8c4);
    paperLight3D.position = vec3_802bf870;
    paperLight3D.rotation = vec3_802bf87c;
    paperLight3D.color = dat_8041f8b0;
    paperLight3D.spotAngle = float_0_8041f8cc;
    paperLight3D.attenAngle = float_1_8041f8d0;

    paperLight3DImg.flags = LIGHT_POINT | LIGHT_NO_DIST_ATTEN;
    strcpy(paperLight3DImg.name, str_PAPERI_802bf938);
    paperLight3DImg.position = vec3_802bf888;
    paperLight3DImg.rotation = vec3_802bf894;
    paperLight3DImg.color = dat_8041f8b4;
    paperLight3DImg.spotAngle = float_0_8041f8cc;
    paperLight3DImg.attenAngle = float_1_8041f8d0;

    paperCraft.flags = LIGHT_POINT | LIGHT_NO_DIST_ATTEN;
    strcpy(paperCraft.name, str_PAPERCRAFT_802bf940);
    paperCraft.position = vec3_802bf8a0;
    paperCraft.rotation = vec3_802bf8ac;
    paperCraft.color = dat_8041f8b8;
    paperCraft.spotAngle = float_0_8041f8cc;
    paperCraft.attenAngle = float_1_8041f8d0;
    PSMTXScale(scaleMtx, float_3_8041f8d4, float_3_8041f8d4, float_3_8041f8d4);
    PSMTXMultVec(scaleMtx, &paperCraft.position, &paperCraft.position);

    booLight.flags = LIGHT_POINT | LIGHT_NO_DIST_ATTEN;
    strcpy(booLight.name, str_BooBoo_802bf94c);
    booLight.position = vec3_802bf8b8;
    booLight.rotation = vec3_802bf8c4;
    booLight.color = dat_8041f8bc;
    booLight.spotAngle = float_0_8041f8cc;
    booLight.attenAngle = float_0_8041f8cc;
}

void lightGetNearObj(void* pPos, LightEntry** pDst, int count, LightEntryFlags flags) {
    typedef struct LightSortEntry {
        LightEntry* entry;
        f32 dist;
    } LightSortEntry;
    extern f32 PSVECDistance(void* a, void* b);
    extern void qqsort(void* base, s32 num, s32 size, void* cmp);
    LightWorkSet* set;
    LightWorkSet* countSet;
    LightEntry* entry;
    LightSortEntry sortEntries[64];
    LightSortEntry* sort;
    s32 enabledCount;
    s32 num;
    s32 i;
    LightEntryFlags entryFlags;

    set = work;
    if (mapGetActiveGroup() == 1) {
        set++;
    }

    countSet = work;
    if (mapGetActiveGroup() == 1) {
        countSet++;
    }

    enabledCount = 0;
    entry = countSet->entries;
    for (i = 0; i < countSet->count; i++, entry++) {
        if ((entry->flags & LIGHT_ENABLED) != 0) {
            enabledCount++;
        }
    }

    sort = sortEntries;
    num = 0;
    entry = set->entries;
    if (enabledCount > 8) {
        for (i = 0; i < set->count; i++, entry++) {
            if ((entry->flags & LIGHT_ENABLED) != 0) {
                sort->entry = entry;
                sort->dist = PSVECDistance(pPos, &entry->position);
                sort++;
                num++;
            }
        }
        qqsort(sortEntries, num, sizeof(LightSortEntry), _sort);
    } else {
        for (i = 0; i < set->count; i++, entry++) {
            if ((entry->flags & LIGHT_ENABLED) != 0) {
                sort->entry = entry;
                sort++;
                num++;
            }
        }
    }

    sort = sortEntries;
    i = 0;
    while (num != 0) {
        entry = sort->entry;
        entryFlags = entry->flags;
        if ((entryFlags & LIGHT_AMBIENT) == 0 && (flags == 0 || ((flags & entryFlags) != 0))) {
            i++;
            *pDst = entry;
            pDst++;
            if (count <= i) {
                break;
            }
        }
        sort++;
        num--;
    }

    while (i < count) {
        *pDst = NULL;
        pDst++;
        i++;
    }
}

void lightMain(void) {
    extern void* camGetPtr(s32 cameraId);
    extern void PSVECNormalize(Vec* src, Vec* dst);
    extern Vec vec3_802bf8d0;
    extern Vec vec3_802bf8f4;
    extern f32 float_1p0486E06_8041f8c0;
    extern LightEntry booLight;
    void* cam;
    Vec out;
    f32 x;
    f32 y;
    f32 z;

    x = vec3_802bf8d0.x;
    y = vec3_802bf8d0.y;
    z = vec3_802bf8d0.z;

    cam = camGetPtr(4);

    out.x = (*(f32*)((s32)cam + 0x11C) * x)
        + (*(f32*)((s32)cam + 0x12C) * y)
        + (*(f32*)((s32)cam + 0x13C) * z);
    out.y = (*(f32*)((s32)cam + 0x120) * x)
        + (*(f32*)((s32)cam + 0x130) * y)
        + (*(f32*)((s32)cam + 0x140) * z);
    out.z = (*(f32*)((s32)cam + 0x124) * x)
        + (*(f32*)((s32)cam + 0x134) * y)
        + (*(f32*)((s32)cam + 0x144) * z);

    PSVECNormalize(&out, &out);

    paperLight3D.position.x = float_1p0486E06_8041f8c0 * out.x;
    paperLight3D.position.y = float_1p0486E06_8041f8c0 * out.y;
    paperLight3D.position.z = float_1p0486E06_8041f8c0 * out.z;

    booLight.position = vec3_802bf8f4;
}

void* lightNameToPtr(char* name) {
    LightWorkSet* set;
    void* entry;
    s32 i;
    s32 group;

    group = mapGetActiveGroup();
    set = work;
    if (group == 1) {
        set++;
    }

    entry = set->entries;
    i = 0;
    goto check;

loop:
    if ((*(u16*)entry & 1) != 0) {
        if (strcmp(name, (const char*)((s32)entry + 2)) == 0) {
            return entry;
        }
    }
    i++;
    entry = (void*)((s32)entry + 0x60);

check:
    if (i < set->count) {
        goto loop;
    }

    return 0;
}
