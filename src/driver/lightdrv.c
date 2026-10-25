#include "driver/lightdrv.h"
#include "driver/camdrv.h"
#include "driver/mapdrv.h"
#include "memory.h"
#include <string.h>
#include "libc_decls.h"

void qqsort(void* base, u32 n, u32 size, s32 (*compare)(const void*, const void*));

//.sbss
static LightWork work[2];

//.bss
LightEntry paperLight3D;
LightEntry paperLight3DImg;
LightEntry paperCraft;
LightEntry booLight;

//local prototypes
static s32 _sort(const void* a, const void* b);

#define lightGetWork() (mapGetActiveGroup() == 1 ? &work[1] : &work[0])

void lightInit(void) {
    Mtx scale;
    LightWork* wp;

    work[0].count = LIGHT_ENTRY_COUNT;
    work[0].entries = __memAlloc(HEAP_DEFAULT, work[0].count * sizeof(LightEntry));

    wp = &work[1];
    wp->count = LIGHT_ENTRY_COUNT;
    wp->entries = __memAlloc(HEAP_DEFAULT, wp->count * sizeof(LightEntry));

    paperLight3D.flags = LIGHT_NO_DIST_ATTEN | LIGHT_POINT;
    strcpy(paperLight3D.name, "PAPER");
    paperLight3D.position = (Vec){0.0f, 0.0f, 0.0f};
    paperLight3D.rotation = (Vec){0.0f, 0.0f, 0.0f};
    paperLight3D.color = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    paperLight3D.spotAngle = 0.0f;
    paperLight3D.attenAngle = 1.0f;

    paperLight3DImg.flags = LIGHT_NO_DIST_ATTEN | LIGHT_POINT;
    strcpy(paperLight3DImg.name, "PAPERI");
    paperLight3DImg.position = (Vec){0.0f, 0.0f, 1048576.0f};
    paperLight3DImg.rotation = (Vec){0.0f, 0.0f, 0.0f};
    paperLight3DImg.color = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    paperLight3DImg.spotAngle = 0.0f;
    paperLight3DImg.attenAngle = 1.0f;

    paperCraft.flags = LIGHT_NO_DIST_ATTEN | LIGHT_POINT;
    strcpy(paperCraft.name, "PAPERCRAFT");
    paperCraft.position = (Vec){-200.0f, 100.0f, 100.0f};
    paperCraft.rotation = (Vec){0.0f, 0.0f, 0.0f};
    paperCraft.color = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    paperCraft.spotAngle = 0.0f;
    paperCraft.attenAngle = 1.0f;
    MTXScale(scale, 3.0f, 3.0f, 3.0f);
    MTXMultVec(scale, &paperCraft.position, &paperCraft.position);

    booLight.flags = LIGHT_NO_DIST_ATTEN | LIGHT_POINT;
    strcpy(booLight.name, "BooBoo");
    booLight.position = (Vec){0.0f, 0.0f, 0.0f};
    booLight.rotation = (Vec){0.0f, 0.0f, 0.0f};
    booLight.color = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
    booLight.spotAngle = 0.0f;
    booLight.attenAngle = 10.0f;
}

void lightReInit(void) {
    LightWork* wp = lightGetWork();
    memset(wp->entries, 0, sizeof(LightEntry) * wp->count);
}

static const Vec camDir = {0.0f, 0.0f, 1.0f};


void lightMain(void) {
    CameraEntry* camera;
    Vec w = camDir;
    Vec a, b, c;
    Vec position;

    camera = camGetPtr(CAMERA_3D);
    a = *(Vec*)camera->view[0];
    b = *(Vec*)camera->view[1];
    c = *(Vec*)camera->view[2];
    {
        Vec r = {0.0f, 0.0f, 0.0f};
        r.x = a.x * w.x + b.x * w.y + c.x * w.z;
        r.y = a.y * w.x + b.y * w.y + c.y * w.z;
        r.z = a.z * w.x + b.z * w.y + c.z * w.z;
        position = r;
    }
    VECNormalize(&position, &position);
    {
        Vec tmp = {0.0f, 0.0f, 0.0f};

        tmp.x = 1048576.0f * position.x;
        tmp.y = 1048576.0f * position.y;
        tmp.z = 1048576.0f * position.z;
        paperLight3D.position = tmp;
    }
    booLight.position = (Vec){0.0f, 1000.0f, 0.0f};
}



s32 lightEntry(const char* name, s32 type, Vec* position, Vec* rotation, const GXColor* color, f32 spotAngle, f32 attenAngle, s32 attenType, s32 flags) {
    static const Vec decay_tbl[] = {
    {1.0f, 0.0f, 0.0f},
    {0.0f, 0.1f, 0.0f},
    {0.0f, 0.0f, 0.01f}, 
    {0.0f, 0.0f, 0.01f}
};
    LightWork* wp = lightGetWork();
    LightEntry* entry;
    s32 i, j, k, n;
    char v41[256];

    entry = wp->entries;
    for (i = 0; i < wp->count; i++, entry++) {
        if (entry->flags & LIGHT_ENABLED && !strcmp(entry->name, name)) {
            break;
        }
    }
    //seems like it will break
    if (i < LIGHT_ENTRY_COUNT) {
        for (j = 0;; j++) {
            sprintf(v41, "%s_%04d", name, j);
            entry = wp->entries;
            name = v41; //will keep getting longer
            for (i = 0; i < wp->count; i++, entry++) {
                if (entry->flags & LIGHT_ENABLED && !strcmp(entry->name, name)) {
                    break;
                }
            }
            if (i >= LIGHT_ENTRY_COUNT) {
                break;
            }
        }
    }

    entry = wp->entries;
    for (n = 0; n < wp->count; n++, entry++) {
        if (!(entry->flags & LIGHT_ENABLED)) {
            break;
        }
    }
    memset(entry, 0, sizeof(LightEntry));
    strcpy(entry->name, name);
    entry->position = *position;
    entry->rotation = *rotation;
    entry->basePosition = *position;
    entry->color = *color;
    entry->spotAngle = spotAngle;
    entry->attenAngle = attenAngle;
    entry->attenDistCoef.x = decay_tbl[attenType].x;
    entry->attenDistCoef.y = decay_tbl[attenType].y;
    entry->attenDistCoef.z = decay_tbl[attenType].z;
    entry->flags |= LIGHT_ENABLED;
    switch (type) {
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
    if (flags & 2) {
        entry->flags |= LIGHT_CHARA;
    }
    return n;
}

LightEntry* lightNameToPtr(const char* name) {
    LightWork* wp = lightGetWork();
    LightEntry* entry = wp->entries;
    s32 i;

    for (i = 0; i < wp->count; i++, entry++) {
        if (entry->flags & LIGHT_ENABLED && !strcmp(name, entry->name)) {
            return entry;
        }
    }
    return NULL;
}

typedef struct NearWork {
    LightEntry* entry;
    f32 dist;
} NearWork;

static s32 _sort(const void* a, const void* b) {
    f32 value2, value1;

    value1 = ((NearWork*)a)->dist;
    value2 = ((NearWork*)b)->dist;

    if (value1 > value2) {
        return 1;
    }
    if (value1 < value2) {
        return -1;
    }
    return 0;
}

s32 lightGetNearObj(const Vec* position, LightEntry** out, s32 count, LightEntryFlags flags) {
    s32 found;
    LightWork* wp = lightGetWork();
    NearWork tmp[32];
    LightEntry** op;
    NearWork* tp;
    LightEntry* entry;
    s32 i;
    u32 n;
    u32 mask;

    if (lightGetEntryNum() > 8) {
        entry = wp->entries;
        tp = tmp;
        n = 0;
        for (i = 0; i < wp->count; i++, entry++) {
            if (entry->flags & LIGHT_ENABLED) {
                tp->entry = entry;
                tp->dist = VECDistance(position, &entry->position);
                tp++;
                n++;
            }
        }
        qqsort(tmp, n, 8, _sort);
    } else {
        entry = wp->entries;
        tp = tmp;
        n = 0;
        for (i = 0; i < wp->count; i++, entry++) {
            if (entry->flags & LIGHT_ENABLED) {
                tmp[n].entry = entry;
                n++;
            }
        }
    }

    op = out;
    tp = tmp;
    mask = flags;
    found = 0;
    for (i = 0; i < n; i++, tp++) {
        u32 fl;
        LightEntry* e = tp->entry;
        fl = e->flags;
        if (!(fl & LIGHT_AMBIENT) && (!mask || (mask & fl))) {
            *op++ = e;
            found++;
            if (found >= count) {
                break;
            }
        }
    }
    for (i = found; i < count; i++) {
        out[i] = NULL;
    }
    return found;
}

LightEntry* lightGetAmbient(void) {
    LightWork* wp = lightGetWork();
    LightEntry* entry = wp->entries;
    s32 i;

    for (i = 0; i < wp->count; i++, entry++) {
        if ((entry->flags & LIGHT_ENABLED) && !(entry->flags & LIGHT_CHARA) && (entry->flags & LIGHT_AMBIENT)) {
            return entry;
        }
    }
    return NULL;
}

LightEntry* lightGetCharaAmbient(void) {
    LightWork* wp = lightGetWork();
    LightEntry* entry = wp->entries;
    s32 i;

    for (i = 0; i < wp->count; i++, entry++) {
        if ((entry->flags & LIGHT_ENABLED) && (entry->flags & LIGHT_CHARA) && (entry->flags & LIGHT_AMBIENT)) {
            return entry;
        }
    }
    return NULL;
}

s32 lightCheckCharaLight(void) {
    LightWork* wp = lightGetWork();
    LightEntry* entry = wp->entries;
    s32 i;

    for (i = 0; i < wp->count; i++, entry++) {
        if ((entry->flags & LIGHT_ENABLED) && (entry->flags & LIGHT_CHARA)) {
            return 1;
        }
    }
    return 0;
}

LightEntry* lightGetPaper(void) {
    s32 cam = camGetCurNo();

    if (cam == CAMERA_3D) {
        return &paperLight3D;
    }
    if (cam == CAMERA_3D_IMAGE) {
        return &paperLight3DImg;
    }
    return &paperLight3DImg;
}

LightEntry* lightGetPaperCraft(void) {
    return &paperCraft;
}

s32 lightGetEntryNum(void) {
    LightWork* wp = lightGetWork();
    LightEntry* entry = wp->entries;
    s32 i, n = 0;

    for (i = 0; i < wp->count; i++, entry++) {
        if (entry->flags & LIGHT_ENABLED) {
            n++;
        }
    }
    return n;
}

LightEntry* lightNumberToPtr(s32 index) {
    LightWork* wp = lightGetWork();
    LightEntry* entry = &wp->entries[index];

    if (entry->flags & LIGHT_ENABLED) {
        return entry;
    }
    return NULL;
}
