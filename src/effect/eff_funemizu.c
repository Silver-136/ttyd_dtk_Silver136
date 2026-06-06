#include "effect/eff_funemizu.h"

typedef struct FunemizuColorEntry {
    const char* name;
    u32 color;
} FunemizuColorEntry;

u32 unk_802294a4(void) {
    extern FunemizuColorEntry unk_803b3b00[];
    extern void* gp;
    extern u32 dat_804279c0;
    extern s32 strcmp(const char* a, const char* b);
    extern s32 strncmp(const char* a, const char* b, s32 n);

    FunemizuColorEntry* entry = unk_803b3b00;

    while (entry != 0) {
        if (entry->name == 0) {
            break;
        }
        if (strcmp(entry->name, (const char*)((s32)gp + 0x12C)) == 0) {
            return entry->color;
        }
        entry++;
    }

    entry = unk_803b3b00;
    while (entry != 0) {
        if (entry->name == 0) {
            break;
        }
        if (strncmp(entry->name, (const char*)((s32)gp + 0x12C), 3) == 0) {
            return entry->color;
        }
        entry++;
    }
    return dat_804279c0;
}
