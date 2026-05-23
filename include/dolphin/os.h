#pragma once

#include <dolphin/types.h>

typedef s64 OSTime;
typedef u32 OSTick;
typedef int OSHeapHandle;

#define OSRoundUp32B(x)   (((u32)(x) + 32 - 1) & ~(32 - 1))
#define OSRoundDown32B(x) (((u32)(x)) & ~(32 - 1))

OSTick OSGetTick(void);
OSTime OSGetTime(void);

void* OSGetArenaHi(void);
void* OSGetArenaLo(void);
void OSSetArenaHi(void* newHi);
void OSSetArenaLo(void* newLo);
void* OSInitAlloc(void* arenaStart, void* arenaEnd, int maxHeaps);
void* OSAllocFromHeap(OSHeapHandle heap, u32 size);
OSHeapHandle OSCreateHeap(void* start, void* end);
void OSDestroyHeap(OSHeapHandle heap);
void DCFlushRange(void* addr, u32 nBytes);
