#include "parse.h"


int parseGet1Next(int param_1, char* param_2, s32 param_3, int param_4) {
    return 0;
}


u32 parsePush(char* param_1) {
    return 0;
}


int parseTagGet1(char* param_1, int param_2, char* param_3, int param_4) {
    extern char parse[];
    int ret;

    if ((int)parsePush(param_1) != 0) {
        ret = ((int (*)())parseGet1Next)(param_2, param_3);
        *(s32*)(parse + 0x54) = *(s32*)(parse + 0x54) - 1;
    } else {
        ret = 0;
    }

    return ret;
}


void parsePopNext(void) {
    extern char parse[];
    char* base;
    s32 index;

    base = parse;
    index = *(s32*)(base + 0x54) - 1;
    *(s32*)(base + 0x54) = index;
    *(s32*)(base + index * 4 + 4) = *(s32*)(base + index * 4 + 0x30);
}


u8 parseInit(s32 param_1, s32 param_2) {
    extern char parse[];

    *(s32*)(parse + 0) = param_1;
    *(s32*)(parse + 0x54) = 0;
    *(s32*)(parse + 4) = 0;
    *(s32*)(parse + 0x2C) = param_2;
}

void parsePop(void) {
    extern char parse[];
    s32 count;

    count = *(s32*)(parse + 0x54);
    *(s32*)(parse + 0x54) = count - 1;
}
