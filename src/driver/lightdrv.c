#include "driver/lightdrv.h"

typedef struct LightWorkSet {
    s32 count;
    LightEntry* entries;
} LightWorkSet;

extern LightWorkSet work[2];
extern LightEntry paperCraft;
extern LightEntry paperLight3D;
extern LightEntry paperLight3DImg;
s32 mapGetActiveGroup(void);
s32 camGetCurNo(void);
s32 strcmp(const char* str1, const char* str2);

void lightInit(void) {
    extern void* __memAlloc(s32 heap, u32 size);
    extern void strcpy(char* dst, const char* src);
    extern const char str_PAPER_8041f8c4[6];
    extern f32 float_0_8041f8cc;
    extern f32 float_1_8041f8d0;
    extern f32 float_3_8041f8d4;
    extern f32 float_10_8041f8d8;
    extern LightEntry booLight;
    f32 scaleMtx[3][4];

    work[0].count = 0x20;
    work[0].entries = __memAlloc(0, work[0].count * sizeof(LightEntry));
    work[1].count = 0x20;
    work[1].entries = __memAlloc(0, work[1].count * sizeof(LightEntry));

    paperLight3D.flags = LIGHT_POINT | LIGHT_NO_DIST_ATTEN;
    strcpy(paperLight3D.name, str_PAPER_8041f8c4);
    paperLight3D.position = (Vec){ 0.0f, 0.0f, 0.0f };
    paperLight3D.rotation = (Vec){ 0.0f, 0.0f, 0.0f };
    paperLight3D.color = (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };
    paperLight3D.spotAngle = float_0_8041f8cc;
    paperLight3D.attenAngle = float_1_8041f8d0;

    paperLight3DImg.flags = LIGHT_POINT | LIGHT_NO_DIST_ATTEN;
    strcpy(paperLight3DImg.name, "PAPERI");
    paperLight3DImg.position = (Vec){ 0.0f, 0.0f, 1048576.0f };
    paperLight3DImg.rotation = (Vec){ 0.0f, 0.0f, 0.0f };
    paperLight3DImg.color = (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };
    paperLight3DImg.spotAngle = float_0_8041f8cc;
    paperLight3DImg.attenAngle = float_1_8041f8d0;

    paperCraft.flags = LIGHT_POINT | LIGHT_NO_DIST_ATTEN;
    strcpy(paperCraft.name, "PAPERCRAFT");
    paperCraft.position = (Vec){ -200.0f, 100.0f, 100.0f };
    paperCraft.rotation = (Vec){ 0.0f, 0.0f, 0.0f };
    paperCraft.color = (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };
    paperCraft.spotAngle = float_0_8041f8cc;
    paperCraft.attenAngle = float_1_8041f8d0;
    PSMTXScale(scaleMtx, float_3_8041f8d4, float_3_8041f8d4, float_3_8041f8d4);
    PSMTXMultVec(scaleMtx, &paperCraft.position, &paperCraft.position);

    booLight.flags = LIGHT_POINT | LIGHT_NO_DIST_ATTEN;
    strcpy(booLight.name, "BooBoo");
    booLight.position = (Vec){ 0.0f, 0.0f, 0.0f };
    booLight.rotation = (Vec){ 0.0f, 0.0f, 0.0f };
    booLight.color = (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };
    booLight.spotAngle = float_0_8041f8cc;
    booLight.attenAngle = float_10_8041f8d8;
}

void lightReInit(void) {
    s32 group = mapGetActiveGroup();
    LightWorkSet* set = work;
    if (group == 1) {
        set++;
    }
    memset(set->entries, 0, set->count * sizeof(LightEntry));
}

void lightMain(void) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32 cameraId);
    extern void PSVECNormalize(Vec* src, Vec* dst);
    extern f32 float_1p0486E06_8041f8c0;
    extern LightEntry booLight;
    void* cam;
    Vec direction;
    Mtx view;
    Vec out;
    Vec normalized;
    Vec paperPos;
    Vec booPos;

    direction = (Vec){ 0.0f, 0.0f, 1.0f };
    cam = camGetPtr(4);

    *(Vec*)&view[1][0] = *(Vec*)((u8*)cam + 0x12C);
    *(Vec*)&view[0][0] = *(Vec*)((u8*)cam + 0x11C);
    *(Vec*)&view[2][0] = *(Vec*)((u8*)cam + 0x13C);

    out = (Vec){ 0.0f, 0.0f, 0.0f };
    out.x = view[1][0] * direction.z
        + view[0][0] * direction.x
        + view[2][0] * direction.y;
    out.y = view[1][1] * direction.z
        + view[0][1] * direction.x
        + view[2][1] * direction.y;
    out.z = view[1][2] * direction.z
        + view[0][2] * direction.x
        + view[2][2] * direction.y;

    normalized = out;
    PSVECNormalize(&normalized, &normalized);

    paperPos = (Vec){ 0.0f, 0.0f, 0.0f };
    paperPos.x = float_1p0486E06_8041f8c0 * normalized.x;
    paperPos.y = float_1p0486E06_8041f8c0 * normalized.y;
    paperPos.z = float_1p0486E06_8041f8c0 * normalized.z;
    paperLight3D.position = paperPos;

    booPos = (Vec){ 0.0f, 1000.0f, 0.0f };
    booLight.position = booPos;
}

int lightEntry(char* pName, int lightType, void* pos, void* rot, GXColor* color, int distanceAttenType, u32 flags, f32 spotAngle, f32 angleAtten) {
    extern s32 sprintf(char* buffer, const char* format, ...);
    extern void strcpy(char* dst, const char* src);
    extern const char str_PCTs_PCT04d_802bf930[];
    extern const Vec dat_802bf900[4];
    LightWorkSet* set;
    LightEntry* entry;
    s32 i;
    s32 j;
    s32 group;
    char nameBuf[264];

    group = mapGetActiveGroup();
    set = work;
    if (group == 1) {
        set++;
    }

    entry = set->entries;
    for (i = 0; i < set->count; i++, entry++) {
        if ((entry->flags & LIGHT_ENABLED) != 0 && strcmp(entry->name, pName) == 0) {
            break;
        }
    }

    if (i < 0x20) {
        for (j = 0;; j++) {
            sprintf(nameBuf, str_PCTs_PCT04d_802bf930, pName, j);
            pName = nameBuf;
            entry = set->entries;
            for (i = 0; i < set->count; i++, entry++) {
                if ((entry->flags & LIGHT_ENABLED) != 0 && strcmp(entry->name, pName) == 0) {
                    break;
                }
            }
            if (i >= 0x20) {
                break;
            }
        }
    }

    entry = set->entries;
    for (i = 0; i < set->count; i++, entry++) {
        if ((entry->flags & LIGHT_ENABLED) == 0) {
            break;
        }
    }

    memset(entry, 0, sizeof(LightEntry));
    strcpy(entry->name, pName);

    entry->position = *(Vec*)pos;
    entry->rotation = *(Vec*)rot;
    entry->field_0x3C = *(Vec*)pos;
    entry->color = *color;
    entry->spotAngle = spotAngle;
    entry->attenAngle = angleAtten;
    entry->attenDistCoef.x = dat_802bf900[distanceAttenType].x;
    entry->attenDistCoef.y = dat_802bf900[distanceAttenType].y;
    entry->attenDistCoef.z = dat_802bf900[distanceAttenType].z;

    entry->flags |= LIGHT_ENABLED;
    switch (lightType) {
        case 0:
            entry->flags |= LIGHT_AMBIENT;
            break;
        case 1:
            entry->flags |= LIGHT_POINT;
            break;
        case 2:
            entry->flags |= LIGHT_SPOT;
            break;
        case 3:
            entry->flags |= LIGHT_DIRECTIONAL;
            break;
    }

    if ((flags & 2) != 0) {
        entry->flags |= LIGHT_CHARA;
    }

    return i;
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

s32 _sort(void* a, void* b) {
    if (*(f32*)((s32)a + 4) > *(f32*)((s32)b + 4)) {
        return 1;
    }
    if (*(f32*)((s32)a + 4) < *(f32*)((s32)b + 4)) {
        return -1;
    }
    return 0;
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
    LightSortEntry sortEntries[32];
    LightSortEntry* sort;
    s32 enabledCount;
    s32 num;
    s32 i;
    s32 group;
    LightEntryFlags entryFlags;

    group = mapGetActiveGroup();
    set = work;
    if (group == 1) {
        set++;
    }

    group = mapGetActiveGroup();
    countSet = work;
    if (group == 1) {
        countSet++;
    }

    enabledCount = 0;
    entry = countSet->entries;
    for (i = 0; i < countSet->count; i++, entry++) {
        if ((entry->flags & LIGHT_ENABLED) != 0) {
            enabledCount++;
        }
    }

    if (enabledCount > 8) {
        entry = set->entries;
        sort = sortEntries;
        num = 0;
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
        entry = set->entries;
        sort = sortEntries;
        num = 0;
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

LightEntry* lightGetPaperCraft(void) {
    return &paperCraft;
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

const Vec dat_802bf900[4] = {
    { 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.1f, 0.0f },
    { 0.0f, 0.0f, 0.01f },
    { 0.0f, 0.0f, 0.01f },
};
const char str_PCTs_PCT04d_802bf930[] = "%s_%04d";

LightEntry paperLight3D;
LightEntry paperLight3DImg;
LightEntry paperCraft;
LightEntry booLight;

LightWorkSet work[2];

const f32 float_1p0486E06_8041f8c0 = 1048576.0f;
const char str_PAPER_8041f8c4[] = "PAPER";
const u16 gap_09_8041F8CA_sdata2 = 0;
const f32 float_0_8041f8cc = 0.0f;
const f32 float_1_8041f8d0 = 1.0f;
const f32 float_3_8041f8d4 = 3.0f;
const f32 float_10_8041f8d8 = 10.0f;
const u32 gap_09_8041F8DC_sdata2 = 0;
