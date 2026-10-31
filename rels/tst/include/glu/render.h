/*
 * Copyright (C) 1991-2000 Silicon Graphics, Inc. All Rights Reserved.
 * SGI Free Software License B (Version 2.0, Sept. 18, 2008), http://oss.sgi.com/projects/FreeB/;
 * the full notice is at the top of src/glu/tess.c.
 */

#pragma once

/* libtess output (render.c). */

#include "glu/mesh.h"

/* __gl_renderMesh( tess, mesh ) takes a mesh and breaks it into triangle
 * fans, strips, and separate triangles. A substantial effort is made
 * to use as few rendering primitives as possible (ie. to make the fans
 * and strips as large as possible).
 *
 * The rendering output is provided as callbacks (see the api).
 */
void __gl_renderMesh(GLUtesselator* tess, GLUmesh* mesh);
void __gl_renderBoundary(GLUtesselator* tess, GLUmesh* mesh);

GLboolean __gl_renderCache(GLUtesselator* tess);
