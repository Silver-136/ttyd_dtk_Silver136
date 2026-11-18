double tan(double x);
double sin(double x);
double cos(double x);
double acos(double x);
double acosh(double x);
double asin(double x);
double asinh(double x);
double atan(double x);
double atan2(double x, double y);
double atanh(double x);
double ceil(double x);
double copysign(double x, double y);
double cosh(double x);
double erf(double x);
double erfc(double x);
double exp(double x);
double expm1(double x);
double fdim(double x, double y);
double floor(double x);
double fmax(double x, double y);
double fmin(double x, double y);
double fmod(double x, double y);
double frexp(double x, int* exp);
double gamma(double x);
double hypot(double x, double y);
double ldexp(double x, int n);
double lgamma(double x);
long long llrint(double x);
long long llround(double x);
double log(double x);
double log10(double x);
double log1p(double x);
double logb(double x);
long lrint(double x);
double nearbyint(double x);
double nextafter(double x, double y);
double pow(double x, double y);
double remainder(double x, double y);
double remquo(double x, double y, int* quo);
double rint(double x);
double scalbln(double x, long n);
double scalbn(double x, int n);
double sinh(double x);
double sqrt(double x);
double tanh(double x);
double trunc(double x);

extern unsigned long __float_nan[];

/* The bodies of fabs, sqrtf and __fpclassifyf are the inline ones of libs/msl/include/math.h (the game's MSL header):
 * compiled out of line here they are exactly the sizes the JP demo map gives this unit (fabs 0x8, sqrtf 0xe4 with
 * __fpclassifyf inlined, __fpclassifyf 0x64). The map's out-of-line __fpclassifyf (after scalbn, at the end of the
 * unit) is not emitted here: nothing in this file refers to it without inlining it. */
inline int __fpclassifyf(float x) {
    switch (*(int*)&x & 0x7F800000) {
        case 0x7F800000:
            if (*(int*)&x & 0x007FFFFF) {
                return 1; // FP_NAN
            } else {
                return 2; // FP_INFINITE
            }
            break;
        case 0:
            if (*(int*)&x & 0x007FFFFF) {
                return 5; // FP_SUBNORMAL
            } else {
                return 3; // FP_ZERO
            }
            break;
        default:
            return 4; // FP_NORMAL
    }
}

__declspec(weak) float fminf(float x, float y) {
    return fmin(x, y);
}

__declspec(weak) float fmaxf(float x, float y) {
    return fmax(x, y);
}

__declspec(weak) float fdimf(float x, float y) {
    return fdim(x, y);
}

__declspec(weak) float remquof(float x, float y, int* quo) {
    return remquo(x, y, quo);
}

__declspec(weak) float copysignf(float x, float y) {
    return copysign(x, y);
}

__declspec(weak) float remainderf(float x, float y) {
    return remainder(x, y);
}

__declspec(weak) float truncf(float x) {
    return trunc(x);
}

__declspec(weak) long long llrintf(float x) {
    return llrint(x);
}

__declspec(weak) long long llroundf(float x) {
    return llround(x);
}

__declspec(weak) long lrintf(float x) {
    return lrint(x);
}

__declspec(weak) float rintf(float x) {
    return rint(x);
}

__declspec(weak) float nearbyintf(float x) {
    return nearbyint(x);
}

__declspec(weak) float nextafterf(float x, float y) {
    return nextafter(x, y);
}

__declspec(weak) float lgammaf(float x) {
    return lgamma(x);
}

__declspec(weak) float gammaf(float x) {
    return gamma(x);
}

__declspec(weak) float erfcf(float x) {
    return erfc(x);
}

__declspec(weak) float erff(float x) {
    return erf(x);
}

__declspec(weak) float hypotf(float x, float y) {
    return hypot(x, y);
}

__declspec(weak) float scalblnf(float x, long n) {
    return scalbln(x, n);
}

__declspec(weak) float scalbnf(float x, int n) {
    return scalbn(x, n);
}

__declspec(weak) float logbf(float x) {
    return logb(x);
}

__declspec(weak) float log1pf(float x) {
    return log1p(x);
}

__declspec(weak) float expm1f(float x) {
    return expm1(x);
}

__declspec(weak) float atanhf(float x) {
    return atanh(x);
}

__declspec(weak) float asinhf(float x) {
    return asinh(x);
}

__declspec(weak) float acoshf(float x) {
    return acosh(x);
}

__declspec(weak) double fabs(double x) {
    return __fabs(x);
}

__declspec(weak) float sqrtf(float x) {
    static const double _half = .5;
    static const double _three = 3.0;

    if (x > 0.0f) {
        double guess = __frsqrte(x);
        guess = _half * guess * (_three - guess * guess * x); // now have 12 sig bits
        guess = _half * guess * (_three - guess * guess * x); // now have 24 sig bits
        return x * (_half * guess * (_three - guess * guess * x)); // now have 32 sig bits
    } else if (x < 0.0) {
        return *(float*)__float_nan;
    } else if (__fpclassifyf(x) == 1) {
        return *(float*)__float_nan;
    }
    return x;
}

__declspec(weak) float fmodf(float x, float y) {
    return fmod(x, y);
}

__declspec(weak) float floorf(float x) {
    return floor(x);
}

__declspec(weak) float ceilf(float x) {
    return ceil(x);
}

__declspec(weak) float powf(float x, float y) {
    return pow(x, y);
}

__declspec(weak) float log10f(float x) {
    return log10(x);
}

__declspec(weak) float logf(float x) {
    return log(x);
}

__declspec(weak) float ldexpf(float x, int n) {
    return ldexp(x, n);
}

__declspec(weak) float frexpf(float x, int* exp) {
    return frexp(x, exp);
}

__declspec(weak) float expf(float x) {
    return exp(x);
}

__declspec(weak) float tanhf(float x) {
    return tanh(x);
}

__declspec(weak) float sinhf(float x) {
    return sinh(x);
}

__declspec(weak) float coshf(float x) {
    return cosh(x);
}

__declspec(weak) float tanf(float x) {
    return tan(x);
}

__declspec(weak) float sinf(float x) {
    return sin(x);
}

__declspec(weak) float cosf(float x) {
    return cos(x);
}

__declspec(weak) float atan2f(float x, float y) {
    return atan2(x, y);
}

__declspec(weak) float atanf(float x) {
    return atan(x);
}

__declspec(weak) float asinf(float x) {
    return asin(x);
}

__declspec(weak) float acosf(float x) {
    return acos(x);
}

__declspec(weak) long double fminl(long double x, long double y) {
    return fmin(x, y);
}

__declspec(weak) long double fmaxl(long double x, long double y) {
    return fmax(x, y);
}

__declspec(weak) long double fdiml(long double x, long double y) {
    return fdim(x, y);
}

__declspec(weak) long double remquol(long double x, long double y, int* quo) {
    return remquo(x, y, quo);
}

__declspec(weak) long double copysignl(long double x, long double y) {
    return copysign(x, y);
}

__declspec(weak) long double remainderl(long double x, long double y) {
    return remainder(x, y);
}

__declspec(weak) long double fmodl(long double x, long double y) {
    return fmod(x, y);
}

__declspec(weak) long double truncl(long double x) {
    return trunc(x);
}

__declspec(weak) long long llrintl(long double x) {
    return llrint(x);
}

__declspec(weak) long lrintl(long double x) {
    return lrint(x);
}

__declspec(weak) long double rintl(long double x) {
    return rint(x);
}

__declspec(weak) long double nearbyintl(long double x) {
    return nearbyint(x);
}

__declspec(weak) long double floorl(long double x) {
    return floor(x);
}

__declspec(weak) long double ceill(long double x) {
    return ceil(x);
}

__declspec(weak) long double nextafterl(long double x, long double y) {
    return nextafter(x, y);
}

__declspec(weak) long double lgammal(long double x) {
    return lgamma(x);
}

__declspec(weak) long double gammal(long double x) {
    return gamma(x);
}

__declspec(weak) long double erfcl(long double x) {
    return erfc(x);
}

__declspec(weak) long double erfl(long double x) {
    return erf(x);
}

__declspec(weak) long double hypotl(long double x, long double y) {
    return hypot(x, y);
}

__declspec(weak) long double sqrtl(long double x) {
    return sqrt(x);
}

__declspec(weak) long double powl(long double x, long double y) {
    return pow(x, y);
}

__declspec(weak) long double scalblnl(long double x, long n) {
    return scalbln(x, n);
}

__declspec(weak) long double scalbnl(long double x, int n) {
    return scalbn(x, n);
}

__declspec(weak) long double logbl(long double x) {
    return logb(x);
}

__declspec(weak) long double log1pl(long double x) {
    return log1p(x);
}

__declspec(weak) long double expm1l(long double x) {
    return expm1(x);
}

__declspec(weak) long double log10l(long double x) {
    return log10(x);
}

__declspec(weak) long double logl(long double x) {
    return log(x);
}

__declspec(weak) long double ldexpl(long double x, int n) {
    return ldexp(x, n);
}

__declspec(weak) long double frexpl(long double x, int* exp) {
    return frexp(x, exp);
}

__declspec(weak) long double expl(long double x) {
    return exp(x);
}

__declspec(weak) long double atanhl(long double x) {
    return atanh(x);
}

__declspec(weak) long double asinhl(long double x) {
    return asinh(x);
}

__declspec(weak) long double acoshl(long double x) {
    return acosh(x);
}

__declspec(weak) long double tanhl(long double x) {
    return tanh(x);
}

__declspec(weak) long double sinhl(long double x) {
    return sinh(x);
}

__declspec(weak) long double coshl(long double x) {
    return cosh(x);
}

__declspec(weak) long double tanl(long double x) {
    return tan(x);
}

__declspec(weak) long double sinl(long double x) {
    return sin(x);
}

__declspec(weak) long double cosl(long double x) {
    return cos(x);
}

__declspec(weak) long double atan2l(long double x, long double y) {
    return atan2(x, y);
}

__declspec(weak) long double atanl(long double x) {
    return atan(x);
}

__declspec(weak) long double asinl(long double x) {
    return asin(x);
}

__declspec(weak) long double acosl(long double x) {
    return acos(x);
}
