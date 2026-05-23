#include "mail_data.h"

void* mailGetPtr(s32 id) {
    extern u8 mail_data[];
    u8* entry = mail_data;
    s32 i;

    for (i = 0; i < 11; i++) {
        if (id == *(s32*)(entry + 4)) {
            break;
        }
        entry += 0x14;
        if (id == *(s32*)(entry + 4)) {
            break;
        }
        entry += 0x14;
        if (id == *(s32*)(entry + 4)) {
            break;
        }
        entry += 0x14;
        if (id == *(s32*)(entry + 4)) {
            break;
        }
        entry += 0x14;
    }
    return entry;
}
