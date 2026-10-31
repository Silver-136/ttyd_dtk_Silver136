// Platform glue (assert) for the SGI GLU tessellator (libtess) that tst.rel carries.
#pragma once

/* Platform glue for libtess. In PAL the tessellator was built with its assertions on (the game's NDEBUG build turns
 * MSL's assert off), each failing one calling the game's __assert (system.c, PAL only) with the bare file name. The
 * Japanese demo's copy has them off. */

#include "dolphin/types.h"
#include "version.h"

BOOL __assert(const char* filename, s32 line, const char* assertion);

#undef assert
#if VERSION <= VERSION_DEMO_US
#define assert(expr) ((void)0)
#else
#define assert(expr) ((void)((expr) || __assert(__FILE__, __LINE__, #expr)))
#endif
