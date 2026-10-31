// Barriern (Yux, the shield unit guarded by Mini-Yux): the satellite flight paths shared by every copy of the unit
// (aji unit_barriern / unit_barriern_custom / unit_barriern_z, moo unit_barriern_z, mri unit_barriern
// and the tst copies), and Sir Grodus (rels/las unit_boss_batten_leader, satellites in unit->work[13 + satellite]).
#pragma once

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
