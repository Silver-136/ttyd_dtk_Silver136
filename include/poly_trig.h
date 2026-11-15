#pragma once

#include <dolphin/types.h>

// Polynomial sine and cosine for an angle in radians in [0, 2pi), each quadrant folded onto [0, pi/2]:
// sin x ~ x (1 - 0.16605 x^2 + 0.00761 x^4), cos x ~ 1 - 0.4967 x^2 + 0.03705 x^4.
// Many units carried private copies of these two inlines under their own names; the names here are descriptive
// (see docs/NAME_PROVENANCE.md).
// FAKE: inferred original form: each quadrant's polynomial is inline asm on register variables. IRO propagates
// every `var = constant` into its uses but never a constant into an asm operand, so a, b and c are loaded once
// before the quadrant tests as in the target, with the callers' optimisation settings unchanged; no plain-C form
// does that. The exact asm is unknown (several blocks give the same bytes). The private copies (hei_10, bom_01,
// envdrv, eki_00, rsh_06, eff_starpoint) use the same block (cleanup_notes/bf/Q0/log.md, Q6/findings.md, S0/log.md).

static inline f32 polySinf(register f32 x) {
    register f32 a = 0.00761f;
    register f32 b = 0.16605f;
    register f32 c = 1.0f;
    register f32 x2;
    if (x <= 3.1415927f) {
        if (x <= 1.5707964f) {
            asm {
                fmuls x2, x, x
                fmsubs a, a, x2, b
                fmadds a, a, x2, c
                fmuls a, a, x
            }
        } else {
            x = 1.5707964f - (x - 1.5707964f);
            asm {
                fmuls x2, x, x
                fmsubs a, a, x2, b
                fmadds a, a, x2, c
                fmuls a, a, x
            }
        }
    } else if (x < 4.712389f) {
        x = x - 3.1415927f;
        asm {
            fmuls x2, x, x
            fmsubs a, a, x2, b
            fmadds a, a, x2, c
            fmuls a, a, x
        }
        a = -a;
    } else {
        x = 1.5707964f - (x - 4.712389f);
        asm {
            fmuls x2, x, x
            fmsubs a, a, x2, b
            fmadds a, a, x2, c
            fmuls a, a, x
        }
        a = -a;
    }
    return a;
}

static inline f32 polyCosf(register f32 x) {
    register f32 a = 0.03705f;
    register f32 b = 0.4967f;
    register f32 c = 1.0f;
    register f32 x2;
    if (x <= 3.1415927f) {
        if (x <= 1.5707964f) {
            asm {
                fmuls x2, x, x
                fmsubs a, a, x2, b
                fmadds a, a, x2, c
            }
        } else {
            x = 1.5707964f - (x - 1.5707964f);
            asm {
                fmuls x2, x, x
                fmsubs a, a, x2, b
                fmadds a, a, x2, c
            }
            a = -a;
        }
    } else if (x < 4.712389f) {
        x = x - 3.1415927f;
        asm {
            fmuls x2, x, x
            fmsubs a, a, x2, b
            fmadds a, a, x2, c
        }
        a = -a;
    } else {
        x = 1.5707964f - (x - 4.712389f);
        asm {
            fmuls x2, x, x
            fmsubs a, a, x2, b
            fmadds a, a, x2, c
        }
    }
    return a;
}
