#ifndef _DTRIGF_H_
#define _DTRIGF_H_

/* MSL's paired-single sine/cosine helpers (dtrigf.h), as far as the game binaries show them. Their constants are
 * const locals that inline assembly addresses, so the compiler gives each one storage when the header is compiled
 * into a precompiled header, even though no game code calls the helpers: these are the negone_one, neg_one,
 * __local_PI, __local_3PIO2, one_negone, __local_PIO2 and one objects in every REL's .data (names in the JP demo
 * maps). The names, types and values are the original's. The bodies are NOT: no binary contains their code, so they
 * only address each constant once, in the order that gives the original's storage order (a precompiled header
 * emits the constants in reverse order of first use). */

static inline void sincosf_f(void) {
    const long long one_negone = 0x3F800000BF800000;
    const long long negone_one = 0xBF8000003F800000;
    const float one = 1.0f;
    const float neg_one = -1.0f;
    const float __local_PI = 3.14159265358979323846f;
    const float __local_PIO2 = 1.57079632679489661923f;
    const float __local_3PIO2 = 3.0f * 1.57079632679489661923f;
    register float f;
    register const void* p;

    asm {
        lfs f, one
        lfs f, __local_PIO2
        la p, one_negone
        lfs f, __local_3PIO2
        lfs f, __local_PI
        lfs f, neg_one
        la p, negone_one
    }
}

static inline void sincosf_a(void) {
    const long long one_negone = 0x3F800000BF800000;
    const long long negone_one = 0xBF8000003F800000;
    const float one = 1.0f;
    const float neg_one = -1.0f;
    const float __local_PI = 3.14159265358979323846f;
    const float __local_PIO2 = 1.57079632679489661923f;
    const float __local_3PIO2 = 3.0f * 1.57079632679489661923f;
    register float f;
    register const void* p;

    asm {
        lfs f, one
        lfs f, __local_PIO2
        la p, one_negone
        lfs f, __local_3PIO2
        lfs f, __local_PI
        lfs f, neg_one
        la p, negone_one
    }
}

#endif
