/*
 * Copyright (C) 1991-2000 Silicon Graphics, Inc. All Rights Reserved.
 * SGI Free Software License B (Version 2.0, Sept. 18, 2008), http://oss.sgi.com/projects/FreeB/;
 * the full notice is at the top of src/glu/tess.c.
 */

#pragma once

/* libtess memory allocation: the tessellator allocates from the map heap (memory.c) instead of malloc/free, and
 * memalloc.c supplies the realloc the sorted priority queue grows its arrays with. */

#include <stddef.h>
#include "memory.h"

#define memRealloc __gl_memRealloc
#define memAlloc(size) _mapAlloc(size)
#define memFree(ptr) _mapFree(ptr)

#define memInit __gl_memInit

extern int __gl_memInit(size_t maxFast);
extern void* __gl_memRealloc(void* ptr, size_t size);
