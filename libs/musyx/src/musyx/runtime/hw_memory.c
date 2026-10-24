#include "musyx/hardware.h"

void* salMalloc(u32 len) { return salHooks.malloc(len); }

#if MUSY_VERSION >= MUSY_VERSION_CHECK(2, 0, 3)
void* salMallocPhysical(u32 len) { return salHooks.mallocPhysical(len); }
#endif

void salFree(void* addr) { salHooks.free(addr); }
