#include "mapdata.h"

extern void* world_data[];
extern int strcmp(const char*, const char*);

char** mapDataPtr(char* name) {
    void** outer;
    void** outerArea;
    void** scan;
    void** base;
    void** lookupBase;
    void** countBase;
    void** outerCountBase;
    void* area;
    void* map;
    char* areaName;
    s32 outerIndex;
    s32 mapIndex;
    s32 mapOffset;
    s32 lookupCount;
    s32 lookupIndex;
    s32 mapCount;
    s32 outerCount;

    outer = world_data;
    base = outer;
    lookupBase = outer;
    countBase = outer;
    outerCountBase = outer;
    outerIndex = 0;
    goto outer_check;

outer_body:
    outerArea = outer + 2;
    mapIndex = 0;
    mapOffset = 0;
    goto map_check;

map_body:
    map = *(void**)((s32)*outerArea + 0x10 + mapOffset);
    if (strcmp(*(char**)map, name) == 0) {
        return map;
    }
    mapOffset += 4;
    mapIndex++;

map_check:
    areaName = *(char**)*outerArea;
    scan = countBase;
    lookupCount = 0;
    while (scan[2] != 0) {
        scan++;
        lookupCount++;
    }
    scan = lookupBase;
    lookupIndex = 0;
    area = 0;
    while (lookupIndex < lookupCount) {
        if (strcmp(*(char**)scan[2], areaName) == 0) {
            area = base[lookupIndex + 2];
            break;
        }
        scan++;
        lookupIndex++;
    }
    mapCount = 0;
    while (*(void**)((s32)area + 0x10) != 0) {
        area = (void*)((s32)area + 4);
        mapCount++;
    }
    if (mapIndex < mapCount) {
        goto map_body;
    }
    outer++;
    outerIndex++;

outer_check:
    scan = outerCountBase;
    outerCount = 0;
    while (scan[2] != 0) {
        scan++;
        outerCount++;
    }
    if (outerIndex < outerCount) {
        goto outer_body;
    }
    return 0;
}

void* areaDataPtr(char* name) {
    char* key;
    void** base;
    void** scan;
    int count;
    int i;

    key = name;
    base = &world_data[2];
    scan = base;
    count = 0;
    while (*scan != 0) {
        scan++;
        count++;
    }

    scan = base;
    i = 0;
    while (i < count) {
        if (strcmp(*(const char**)*scan, key) == 0) {
            return base[i];
        }
        scan++;
        i++;
    }

    return 0;
}


u8 relSetEvtAddr(char* name, void* evt) {
    void** base;
    void** cursor;
    void** areaCursor;
    void* area;
    void* canonicalArea;
    void* map;
    char* areaName;
    s32 worldCount;
    s32 areaIndex;
    s32 lookupCount;
    s32 lookupIndex;
    s32 mapCount;
    s32 mapIndex;
    s32 mapOffset;

    base = world_data;
    areaIndex = 0;
    areaCursor = base;
    goto outer_check;

outer_body:
    mapIndex = 0;
    mapOffset = 0;
    goto map_check;

map_body:
    area = areaCursor[2];
    map = *(void**)((s32)area + 0x10 + mapOffset);
    if (strcmp(*(const char**)map, name) == 0) {
        goto done;
    }
    mapOffset += 4;
    mapIndex++;

map_check:
    area = areaCursor[2];
    areaName = *(char**)area;
    cursor = base;
    lookupCount = 0;
    while (cursor[2] != 0) {
        cursor++;
        lookupCount++;
    }
    canonicalArea = 0;
    cursor = base;
    lookupIndex = 0;
    while (lookupIndex < lookupCount) {
        if (strcmp(*(const char**)cursor[2], areaName) == 0) {
            canonicalArea = base[lookupIndex + 2];
            break;
        }
        cursor++;
        lookupIndex++;
    }
    mapCount = 0;
    while (*(void**)((s32)canonicalArea + 0x10) != 0) {
        canonicalArea = (void*)((s32)canonicalArea + 4);
        mapCount++;
    }
    if (mapIndex < mapCount) {
        goto map_body;
    }
    areaCursor++;
    areaIndex++;

outer_check:
    cursor = base;
    worldCount = 0;
    while (cursor[2] != 0) {
        cursor++;
        worldCount++;
    }
    if (areaIndex < worldCount) {
        goto outer_body;
    }
    map = 0;

done:
    if (map != 0) {
        *(void**)((s32)map + 4) = evt;
    }
}

void relSetBtlAddr(char* name, int btl1, int btl2) {
    void** base;
    void** scan;
    void* data;
    int count;
    int i;

    base = world_data;
    scan = base;
    count = 0;
    while (scan[2] != 0) {
        scan++;
        count++;
    }

    scan = base;
    data = 0;
    i = 0;
    while (i < count) {
        if (strcmp(*(const char**)scan[2], name) == 0) {
            data = base[i + 2];
            break;
        }
        scan++;
        i++;
    }

    if (data != 0) {
        *(int*)((int)data + 8) = btl1;
        *(int*)((int)data + 0xC) = btl2;
    }
}

