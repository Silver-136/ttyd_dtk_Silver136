#ifndef _FDLIBM_H
#define _FDLIBM_H

/* Shared by the fdlibm-derived math files (e_*.c kernels behind the w_*.c wrappers, k_*.c helpers, s_*.c).
   __HI/__LO access the high and low 32-bit words of a double; the long constants in those files are the
   polynomial coefficients and split constants of the fdlibm algorithms. */

#define __HI(x) *(int*)&x
#define __LO(x) *(1 + (int*)&x)
#define __HIp(x) *(int*)x
#define __LOp(x) *(1 + (int*)x)

double __ieee754_sqrt(double);
double __ieee754_acos(double);
double __ieee754_asin(double);
double __ieee754_atan2(double, double);
double __ieee754_fmod(double, double);
double __ieee754_hypot(double, double);
double __ieee754_log(double);
double __ieee754_pow(double, double);
int __ieee754_rem_pio2(double, double*);
double __kernel_sin(double, double, int);
double __kernel_cos(double, double);
double __kernel_tan(double, double, int);
int __kernel_rem_pio2(double*, double*, int, int, int, const int*);
inline double fabs(double x) {
    return __fabs(x);
}
double floor(double);
double scalbn(double, int);
double copysign(double, double);
double sqrt(double);
double atan(double);

extern int __float_nan[];
#define NAN (*(float*)__float_nan)
extern int errno;
#define EDOM 33
#define ERANGE 34

#endif
