#include "mapdata.h"


u8 relSetEvtAddr(char* name, void* evt) {
    extern void* world_data[];
    extern int strcmp(const char*, const char*);
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
                *(void**)((s32)map + 4) = evt;
                return;
            }
            map_i++;
        }
        outer++;
        outer_i++;
    }
}

char** mapDataPtr(char* name) {
    extern void* world_data[];
    extern int strcmp(const char*, const char*);
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

void relSetBtlAddr(char* name, int btl1, int btl2) {
    extern void* world_data[];
    extern int strcmp(const char*, const char*);
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

void* areaDataPtr(char* name) {
    extern void* world_data[];
    extern int strcmp(const char*, const char*);
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

