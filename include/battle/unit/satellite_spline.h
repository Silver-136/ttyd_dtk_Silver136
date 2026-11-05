#pragma once

// The satellite flight paths of the Yux units (unit_barriern, unit_barriern_custom, unit_barriern_z in aji, moo, mri
// and tst) and of Sir Grodus (las unit_boss_batten_leader).

#include <dolphin/mtx.h>
#include <dolphin/types.h>

// One satellite moving along a 5-point cubic spline (spline_maketable / spline_getvalue).
// Allocated with BTL_ALLOC and kept in the owner's unit work (Barriern: work[10 + satellite], Grodus:
// work[13 + satellite]).
typedef struct SatelliteSpline {
    s32 numPoints;    // 0x00, always 5
    Vec* points;      // 0x04, control points (splineTableIn / splineTableOut)
    f32 knots[5];     // 0x08, spline parameter of each point, 0..1
    Vec curvature[5]; // 0x1C, second-derivative table, one Vec per point
    s32 frame;        // 0x58, 0..60, the move takes 60 frames
} SatelliteSpline;
