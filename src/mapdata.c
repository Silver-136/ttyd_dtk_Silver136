#include "mapdata.h"


u8 relSetEvtAddr(char* param_1, void* param_2) {
    return 0;
}


char** mapDataPtr(char* map) {
    return 0;
}


void relSetBtlAddr(char* param_1, s32 param_2, s32 param_3) {
    extern s32 world_data[];
    extern s32 strcmp(const char*, const char*);
    void* data;
    s32 count;
    s32 i;

    count = 0;
    while (world_data[count + 2] != 0) {
        count++;
    }

    data = 0;
    i = 0;
    while (i < count) {
        if (strcmp(**(const char***)(world_data + i + 2), param_1) == 0) {
            data = *(void**)(world_data + i + 2);
            break;
        }
        i++;
    }

    if (data != 0) {
        *(s32*)((s32)data + 8) = param_2;
        *(s32*)((s32)data + 0xC) = param_3;
    }
}


void* areaDataPtr(char* name) {
    extern s32 world_data[];
    extern s32 strcmp(const char*, const char*);
    s32 count;
    s32 i;

    count = 0;
    while (world_data[count + 2] != 0) {
        count++;
    }

    i = 0;
    while (i < count) {
        if (strcmp(**(const char***)(world_data + i + 2), name) == 0) {
            return *(void**)(world_data + i + 2);
        }
        i++;
    }

    return 0;
}
