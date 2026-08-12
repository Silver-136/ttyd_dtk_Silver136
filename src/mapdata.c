#include "mapdata.h"

extern void* world_data[];
extern int strcmp(const char*, const char*);

char** mapDataPtr(char* name) {
    void** world;
    void** outer;
    void** area_scan;
    void* area;
    void* map;
    int outer_i;
    int outer_count;
    int map_i;
    int map_count;

    world = world_data;
    area_scan = world;
    outer_count = 0;
    while (area_scan[2] != 0) {
        area_scan++;
        outer_count++;
    }

    outer_i = 0;
    outer = world;
    while (outer_i < outer_count) {
        area = outer[2];
        map_count = 0;
        while (*(void**)((s32)area + 0x10 + map_count * 4) != 0) {
            map_count++;
        }
        map_i = 0;
        while (map_i < map_count) {
            map = *(void**)((s32)area + 0x10 + map_i * 4);
            if (strcmp(*(const char**)map, name) == 0) {
                return map;
            }
            map_i++;
        }
        outer++;
        outer_i++;
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
    void* found;
    char* areaName;
    s32 worldCount;
    s32 areaIndex;
    s32 lookupCount;
    s32 lookupIndex;
    s32 mapCount;
    s32 mapIndex;

    base = world_data;
    found = 0;
    areaIndex = 0;
    areaCursor = base;
    while (1) {
        cursor = base;
        worldCount = 0;
        while (cursor[2] != 0) {
            cursor++;
            worldCount++;
        }
        if (areaIndex >= worldCount) {
            break;
        }
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
        while (*(void**)((s32)canonicalArea + 0x10 + mapCount * 4) != 0) {
            mapCount++;
        }
        mapIndex = 0;
        while (mapIndex < mapCount) {
            map = *(void**)((s32)area + 0x10 + mapIndex * 4);
            if (strcmp(*(const char**)map, name) == 0) {
                found = map;
                break;
            }
            mapIndex++;
        }
        if (found != 0) {
            break;
        }
        areaCursor++;
        areaIndex++;
    }

    if (found != 0) {
        *(void**)((s32)found + 4) = evt;
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

