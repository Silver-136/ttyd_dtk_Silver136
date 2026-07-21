#include "parse.h"


u8 parseInit(s32 param_1, s32 param_2) {
    extern char parse[];

    *(s32*)(parse + 0) = param_1;
    *(s32*)(parse + 0x54) = 0;
    *(s32*)(parse + 4) = 0;
    *(s32*)(parse + 0x2C) = param_2;
}

u32 parsePush(char* tagName) {
    extern char parse[];
    extern char buf_405[];
    extern s32 strcmp(const char*, const char*);
    s32 depth = *(s32*)(parse + 0x54);
    s32* starts = (s32*)(parse + 4);
    s32* ends = (s32*)(parse + 0x2C);
    s32 begin = starts[depth];
    s32 end = ends[depth];
    s32 nesting = 0;
    s32 state = 0;
    s32 childStart = 0;
    s32 childEnd = 0;
    s32 i;

    for (i = begin; i < end; i++) {
        if (parse[i] != '<') {
            continue;
        }
        if (parse[i + 1] == '/') {
            nesting--;
            if (state == 1 && nesting == 0) {
                childEnd = i;
                while (i < end && parse[i] != '>') {
                    i++;
                }
                childEnd = i + 1;
                state = 2;
                break;
            }
        } else {
            if (state == 0 && nesting == 0) {
                s32 length = 0;
                s32 j = i;
                while (j < end) {
                    char c = parse[j];
                    if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                        buf_405[length++] = c;
                    }
                    if (length > 0x7E || c == '>') {
                        break;
                    }
                    j++;
                }
                buf_405[length] = 0;
                if (strcmp(buf_405, tagName) == 0) {
                    while (i < end && parse[i] != '>') {
                        i++;
                    }
                    state = 1;
                    childStart = i + 1;
                }
            }
            nesting++;
        }
    }
    if (state == 2) {
        depth++;
        *(s32*)(parse + 0x54) = depth;
        starts[depth] = childStart;
        ends[depth] = childEnd;
    }
    return state == 2;
}

void parsePop(void) {
    extern char parse[];
    s32 count;

    count = *(s32*)(parse + 0x54);
    *(s32*)(parse + 0x54) = count - 1;
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


s32 parseGet1Next(s32 type, char* out, s32 unused, s32 tokenStart) {
    extern char parse[];
    extern s32 sscanf(const char*, const char*, ...);
    extern char str_PCTd_804207a0[];
    extern char str_PCTf_804207a4[];
    s32 depth = *(s32*)(parse + 0x54);
    s32* starts = (s32*)(parse + 4);
    s32* ends = (s32*)(parse + 0x2C);
    s32 start = starts[depth];
    s32 end = ends[depth];
    s32 found = 0;
    s32 tokenEnd = 0;
    s32 i;

    (void)unused;
    if (type == 3) {
        s32 x = parseGet1Next(2, out, unused, tokenStart);
        s32 y = parseGet1Next(2, out + 4, unused, tokenStart);
        s32 z = parseGet1Next(2, out + 8, unused, tokenStart);
        return x * y * z;
    }

    for (i = start; i < end; i++) {
        char c = parse[i];
        if (c == '/' && parse[i + 1] == '/') {
            while (i < end && parse[i] != '\n') {
                i++;
            }
        } else if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
            tokenStart = i;
            found = 1;
            for (i = i + 1; i < end; i++) {
                c = parse[i];
                if (c == ' ' || c == '\t' || c == '\n' || c == '\r' ||
                    (c == '/' && parse[i + 1] == '/')) {
                    tokenEnd = i;
                    found = 2;
                    break;
                }
            }
            break;
        }
    }
    if (found < 2) {
        return 0;
    }
    if (type == 1) {
        sscanf(parse + tokenStart, str_PCTd_804207a0, out);
        starts[depth] = tokenEnd;
    } else if (type == 0) {
        s32 count = 0;
        char* src = parse + tokenStart;
        for (i = tokenStart; i < end; i++, src++) {
            if (*src == '"') {
                i++;
                src++;
                while (i < end && *src != '"') {
                    out[count++] = *src++;
                    i++;
                }
            }
        }
        out[count] = 0;
        starts[depth] = i;
    } else if (type == 2) {
        sscanf(parse + tokenStart, str_PCTf_804207a4, out);
        starts[depth] = tokenEnd;
    }
    return 1;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
