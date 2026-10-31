/*
 * Copyright (C) 1991-2000 Silicon Graphics, Inc. All Rights Reserved.
 * SGI Free Software License B (Version 2.0, Sept. 18, 2008), http://oss.sgi.com/projects/FreeB/;
 * the full notice is at the top of src/glu/tess.c.
 */

#pragma once

/* libtess sweep-plane projection (normal.c). */

#include "glu/tess.h"

/* __gl_projectPolygon( tess ) determines the polygon normal
 * and project vertices onto the plane of the polygon.
 */
void __gl_projectPolygon(GLUtesselator* tess);
