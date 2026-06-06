#include "battle/battle_monosiri.h"

extern void* _battleWorkPointer;
extern s32* battle_monosiri_same_tbl[];
void swSet(s32 id);
s32 swGet(s32 id);

void* battleGetUnitMonosiriPtr(s32 id) {
    extern u8 monosiriMessageTable[];

    return &monosiriMessageTable[id * 0x18];
}


void battleSetUnitMonosiriFlag(void* unit) {
    void* battleWork;
    s32 id;
    s32** group;
    s32* list;
    s32 i;
    s32 word;
    s32 bit;

    id = *(s32*)((s32)unit + 8);
    battleWork = _battleWorkPointer;
    group = battle_monosiri_same_tbl;

    while ((list = *group) != 0) {
        i = 0;
        while (list[i] != 0) {
            if (id == list[i]) {
                i = 0;
                while ((id = list[i]) != 0) {
                    swSet(id + 0x117A);
                    word = id / 32;
                    bit = id % 32;
                    *(u32*)((s32)battleWork + 0x163D4 + word * 4) |= 1 << bit;
                    i++;
                }
                return;
            }
            i++;
        }
        group++;
    }

    swSet(id + 0x117A);
    word = id / 32;
    bit = id % 32;
    *(u32*)((s32)battleWork + 0x163D4 + word * 4) |= 1 << bit;
}


s32 battleCheckUnitMonosiriFlag(void* unit) {
    void* battleWork;
    s32 id;
    s32 flags;
    s32 result;
    s32 word;
    s32 bit;
    s32 sameId;

    battleWork = _battleWorkPointer;
    id = *(s32*)((s32)unit + 8);

    if ((*(u32*)((s32)battleWork + 0x163F4) & 2) != 0) {
        return 1;
    }

    result = swGet(id + 0x117A);
    word = id / 32;
    bit = id % 32;
    result |= *(u32*)((s32)battleWork + 0x163D4 + word * 4) & (1 << bit);

    flags = *(s32*)((s32)unit + 0x1C);
    if ((flags & 4) != 0) {
        sameId = *(s32*)((s32)unit + 4);
        word = sameId / 32;
        bit = sameId % 32;
        result |= *(u32*)((s32)battleWork + 0x163D4 + word * 4) & (1 << bit);
    }

    if (result != 0) {
        return 1;
    }

    return 0;
}
