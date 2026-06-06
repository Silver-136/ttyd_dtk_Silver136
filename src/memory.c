#include "memory.h"

#include <dolphin/os.h>

extern void* mapalloc_base_ptr;
extern void* R_battlemapalloc_base_ptr;

void _mapFree(void* base, void* ptr);
void GXInitTexObjData(void* texObj, void* data);
void OSFreeToHeap(OSHeapHandle heap, void* ptr);
void* OSAllocFromHeap(OSHeapHandle heap, u32 size);
void DCFlushRange(void* ptr, u32 size);
void* memset(void* ptr, int value, u32 size);

//.sdata
s32 size_table[6][2] = {
	{1, 0x1C84}, //0x721000, 7300 KiB/7.13 MiB
	{1, 0xD90}, //0x364000, 3472 KiB/3.39 MiB
	{1, 0xE0}, //0x38000, 224 KiB
	{1, 0x80}, //0x20000, 128 KiB
	{0, 0x64}
};

//.sbss
u32 mapalloc_size;
void* heapEnd[5];
void* heapStart[5];
OSHeapHandle heapHandle[5];

#pragma optimize_for_size off
void memInit(void) {
    s32(*var_r27)[2];
    s32 temp_r0_2;
    s32 temp_r24;
    s32 temp_r31;
    s32 temp_r3_3;
    s32 temp_r3_6;
    s32 temp_r4_3;
    s32 temp_r6;
    s32 temp_r6_2;
    s32 temp_r6_3;
    s32 temp_r6_4;
    s32 temp_r7;
    s32 var_r22;
    s32 var_r23;
    s32 var_r24;
    s32 var_r29_2;
    s32* temp_r3_4;
    s32* temp_r3_5;
    s32* var_r27_2;
    s32* var_r28;
    u32 temp_r23;
    u32 temp_r4_4;
    void* temp_r0;
    void* lo;
    void* hi;
    void* alloc;
    void* temp_r3_7;
    void** temp_r4;
    void** temp_r4_2;
    void** temp_r5;
    void** temp_r5_2;
    void** var_r22_2;
    void** var_r23_2;
    void** var_r25;
    void** var_r26;
    void** var_r29;
    void** var_r30;

    u32 address;
    int i;

    lo = OSGetArenaLo();
    hi = OSGetArenaHi();
    alloc = OSInitAlloc(lo, hi, 5);
    OSSetArenaLo(alloc);
    address = OSRoundUp32B(alloc);

    for (i = 0; i < 5; i++) {
        if (size_table[i][0] == 1) {
            heapStart[i] = (void*)address;
            heapEnd[i] = (void*)(address + (size_table[i][1] << 10));
            address += (size_table[i][1] << 10);
        }
    }
    /*var_r27 = size_table;
    temp_r3_3 = (s32)(alloc + 0x1F) & 0xFFFFFFE0;
    var_r22 = temp_r3_3;
    temp_r24 = (s32)hi & 0xFFFFFFE0;
    var_r26 = heapStart;
    var_r25 = heapEnd;
    if ((s32)size_table[0][0] == 1) {
        heapStart[0] = (void*)temp_r3_3;
        temp_r0 = temp_r3_3 + (size_table[0][1] << 0xA);
        heapEnd[0] = temp_r0;
        var_r22 = (s32)temp_r0;
    }
    if ((s32)size_table[1][0] == 1) {
        heapStart[1] = (void*)var_r22;
        temp_r6 = size_table[1][0].unk4 << 0xA;
        heapEnd[1] = var_r22 + temp_r6;
        var_r22 += temp_r6;
    }
    temp_r3_4 = size_table[1] + 8;
    temp_r4 = &heapStart[1] + 4;
    temp_r5 = &heapEnd[1] + 4;
    if ((s32)size_table[2][0] == 1) {
        *temp_r4 = (void*)var_r22;
        temp_r6_2 = temp_r3_4->unk4 << 0xA;
        *temp_r5 = var_r22 + temp_r6_2;
        var_r22 += temp_r6_2;
    }
    temp_r3_5 = temp_r3_4 + 8;
    temp_r4_2 = temp_r4 + 4;
    temp_r5_2 = temp_r5 + 4;
    if ((s32)size_table[3][0] == 1) {
        *temp_r4_2 = (void*)var_r22;
        temp_r6_3 = temp_r3_5->unk4 << 0xA;
        *temp_r5_2 = var_r22 + temp_r6_3;
        var_r22 += temp_r6_3;
    }
    if ((s32)size_table[4][0] == 1) {
        *(temp_r4_2 + 4) = (void*)var_r22;
        temp_r6_4 = (temp_r3_5 + 8)->unk4 << 0xA;
        *(temp_r5_2 + 4) = var_r22 + temp_r6_4;
        var_r22 += temp_r6_4;
    }*/


    /*var_r23 = 0;
    var_r30 = heapStart;
    var_r29 = heapEnd;
    temp_r31 = temp_r24 - var_r22;
    do {
        if ((s32)var_r27[0][0] == 0) {
            temp_r4_3 = var_r27[0][1];
            temp_r7 = temp_r4_3 >> 0x1F;
            temp_r4_4 = (u32)__div2u(MULTU_HI(temp_r31, temp_r4_3) + (0 * temp_r4_3) + (temp_r31 * temp_r7),
                                     temp_r31 * temp_r4_3, 0, 0x64, temp_r7);
            temp_r0_2 = temp_r4_4 & 0x1F;
            *var_r30 = (void*)var_r22;
            temp_r3_6 = temp_r4_4 - temp_r0_2;
            *var_r29 = var_r22 + temp_r3_6;
            var_r22 += temp_r3_6;
        }
        var_r23 += 1;
        var_r30 += 4;
        var_r29 += 4;
        var_r27 += 8;
    }
    while (var_r23 < 5);
    var_r27_2 = heapHandle;
    var_r22_2 = heapEnd;
    var_r23_2 = heapStart;
    var_r29_2 = 0;
    var_r28 = heapHandle;
    do {
        var_r29_2 += 1;
        *var_r28 = OSCreateHeap(*var_r23_2, *var_r22_2);
        var_r22_2 += 4;
        var_r23_2 += 4;
        var_r28 += 4;
    }
    while (var_r29_2 < 5);
    OSSetArenaLo((void*)temp_r24);
    var_r24 = 0;
    do {
        OSDestroyHeap(*var_r27_2);
        OSCreateHeap(*var_r26, *var_r25);
        if (var_r24 == 1) {
            temp_r23 = (heapEnd[1] - heapStart[1]) - 0x20;
            mapalloc_size = temp_r23;
            temp_r3_7 = OSAllocFromHeap(heapHandle[1], temp_r23);
            if (temp_r3_7 != NULL) {
                memset(temp_r3_7, 0, temp_r23);
                DCFlushRange(temp_r3_7, temp_r23);
            }
            mapalloc_base_ptr = temp_r3_7;
            temp_r3_7->unk0 = 0;
            temp_r3_7->unk4 = (s32)(mapalloc_size - 0x20);
            temp_r3_7->unk8 = 0;
        }
        var_r24 += 1;
        var_r25 += 4;
        var_r26 += 4;
        var_r27_2 += 4;
    }
    while (var_r24 < 5);*/
}

void __memFree(s32 heap, void* ptr) {
    OSFreeToHeap(heapHandle[heap], ptr);
}

void N_battleMapFree(void) {
    _mapFree(mapalloc_base_ptr, R_battlemapalloc_base_ptr);
    R_battlemapalloc_base_ptr = 0;
}

void* smartTexObj(void* texObj, void** data) {
    if (data != 0) {
        GXInitTexObjData(texObj, *data);
    }
    return texObj;
}


void* smartAlloc(u32 param_1, u8 param_2) {
    return 0;
}


u8 smartInit(void) {
    return 0;
}


u8 smartGarbage(void) {
    return 0;
}


u8 L_smartReInit(void) {
    return 0;
}


void smartFree(void*) {
}


int** _mapAllocTail(int** param_1, int param_2) {
    return 0;
}


void* _mapAlloc(void*, u32) {
    return 0;
}


void memClear(s32 heap) {
    u32 size;
    void* ptr;

    OSDestroyHeap(heapHandle[heap]);
    OSCreateHeap(heapStart[heap], heapEnd[heap]);

    if (heap == 1) {
        size = (u32)heapEnd[1] - (u32)heapStart[1] - 0x20;
        mapalloc_size = size;
        ptr = OSAllocFromHeap(heapHandle[1], size);
        if (ptr != NULL) {
            memset(ptr, 0, size);
            DCFlushRange(ptr, size);
        }
        mapalloc_base_ptr = ptr;
        *(s32*)ptr = 0;
        *(u32*)((s32)ptr + 4) = mapalloc_size - 0x20;
        *(u16*)((s32)ptr + 8) = 0;
    }
}


void smartAutoFree(s32 heap) {
}


void _mapFree(void*, void*) {
}


void* __memAlloc(s32, u32) {
    return 0;
}


u8 N_battleMapAlloc(void) {
    return 0;
}
