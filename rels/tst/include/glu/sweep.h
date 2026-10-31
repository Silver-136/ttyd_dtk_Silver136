/*
 * Copyright (C) 1991-2000 Silicon Graphics, Inc. All Rights Reserved.
 * SGI Free Software License B (Version 2.0, Sept. 18, 2008), http://oss.sgi.com/projects/FreeB/;
 * the full notice is at the top of src/glu/tess.c.
 */

#pragma once

/* libtess plane sweep (sweep.c). */

#include "glu/mesh.h"

/* __gl_computeInterior( tess ) computes the planar arrangement specified
 * by the given contours, and further subdivides this arrangement
 * into regions. Each region is marked "inside" if it belongs
 * to the polygon, according to the rule given by tess->windingRule.
 * Each interior region is guaranteed be monotone.
 */
int __gl_computeInterior(GLUtesselator* tess);

/* The following is here *only* for access by debugging routines */

#include "glu/dict.h"

/* For each pair of adjacent edges crossing the sweep line, there is
 * an ActiveRegion to represent the region between them. The active
 * regions are kept in sorted order in a dynamic dictionary. As the
 * sweep line crosses each vertex, we update the affected regions.
 */

struct ActiveRegion {
    GLUhalfEdge* eUp;       /* upper edge, directed right to left */
    DictNode* nodeUp;       /* dictionary node corresponding to eUp */
    int windingNumber;      /* used to determine which regions are
                             * inside the polygon */
    GLboolean inside;       /* is this region inside the polygon? */
    GLboolean sentinel;     /* marks fake edges at t = +/-infinity */
    GLboolean dirty;        /* marks regions where the upper or lower
                             * edge has changed, but we haven't checked
                             * whether they intersect yet */
    GLboolean fixUpperEdge; /* marks temporary edges introduced when
                             * we process a "right vertex" (one without
                             * any edges leaving to the right) */
};

#define RegionBelow(r) ((ActiveRegion*)dictKey(dictPred((r)->nodeUp)))
#define RegionAbove(r) ((ActiveRegion*)dictKey(dictSucc((r)->nodeUp)))
