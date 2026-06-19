#include "mapdata.h"


u8 relSetEvtAddr(char* param_1, void* param_2) {
    return 0;
}


char** mapDataPtr(char* map) {
    return 0;
}


void relSetBtlAddr(char* name, int btl1, int btl2) {
    extern void* world_data[];
    extern int strcmp(const char*, const char*);
    char* key;
    void** base;
    void** scan;
    void* data;
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

    data = 0;
    scan = base;
    i = 0;
    while (i < count) {
        if (strcmp(*(const char**)*scan, key) == 0) {
            data = base[i];
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

