#ifndef _MATH_H_
#define _MATH_H_

/* The game was built with the MSL headers of its own CodeWarrior (GC/2.6); the Dolphin SDK libraries were built with
 * an older one, whose math.h is libs/dolsdk2004/include/libc/math.h. Game flags search this directory first.
 * Differences: NAN is the library's __float_nan, __fpclassifyf is inline, and sqrtf handles negative and NaN input. */

#define M_PI 3.14159265358979323846f

extern int __float_nan[];
#define NAN (*(float*)__float_nan)
#define HUGE_VALF (1.0f / 0.0f)
#define INFINITY (1.0f / 0.0f)

double fabs(double x);
double sin(double x);
double cos(double x);

float sinf(float x);
float cosf(float x);
float tanf(float x);
float acosf(float x);
float powf(float base, float exponent);

double ldexp(double x, int exp);

double scalbn(double x, int n);

double copysign(double x, double y);

double floor(double x);
double fmod(double x, double y);
double pow(double x, double y);

#ifdef __MWERKS__
#define fabs(x) __fabs(x)
#define fabsf(x) __fabsf(x)
#else
double fabs(double x);
float fabsf(float x);
#endif

#define FP_NAN 1
#define FP_INFINITE 2
#define FP_ZERO 3
#define FP_NORMAL 4
#define FP_SUBNORMAL 5

static inline int __fpclassifyf(float x) {
    switch (*(int*)&x & 0x7F800000) {
        case 0x7F800000:
            if (*(int*)&x & 0x007FFFFF) {
                return FP_NAN;
            } else {
                return FP_INFINITE;
            }
            break;
        case 0:
            if (*(int*)&x & 0x007FFFFF) {
                return FP_SUBNORMAL;
            } else {
                return FP_ZERO;
            }
            break;
        default:
            return FP_NORMAL;
    }
}

#define fpclassify(x) __fpclassifyf(x)
#define isnan(x) (fpclassify(x) == FP_NAN)
#define isfinite(x) ((fpclassify(x) > FP_INFINITE))

static inline float sqrtf(float x) {
    const double _half = .5;
    const double _three = 3.0;

    if (x > 0.0f) {
        double guess = __frsqrte(x);
        guess = _half * guess * (_three - guess * guess * x); // now have 12 sig bits
        guess = _half * guess * (_three - guess * guess * x); // now have 24 sig bits
        return x * (_half * guess * (_three - guess * guess * x)); // now have 32 sig bits
    } else if (x < 0.0) {
        return NAN;
    } else if (isnan(x)) {
        return NAN;
    }
    return x;
}

static inline float fmodf(float x, float y) {
    return fmod(x, y);
}

static inline float floorf(float x) {
    return floor(x);
}

#endif
