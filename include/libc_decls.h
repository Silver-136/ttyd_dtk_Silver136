// Declarations of the C library and compiler runtime routines (math, string, sprintf, rand) the code calls.

#pragma once

#include "libc/stdarg.h"

extern void __div2u();
extern int __float_nan[];
extern unsigned char __lower_map[];
extern void _restfpr_28(void);
extern void _savefpr_28(void);
extern double asin(double);
extern double atan(double x);
extern double atan2(double y, double x);
extern double cos(double);
extern double floor(double);
extern double fmod(double x, double y);
extern double hypot(double x, double y);
extern void* memcpy(void*, const void*, unsigned long);
extern void* memset(void*, int, unsigned long);
extern double pow(double x, double y);
extern int rand(void);
extern double sin(double);
extern double sqrt(double); // MSL's out-of-line double sqrt (math.h only has the inline sqrtf)
extern int sprintf(char*, const char*, ...);
extern int sscanf(const char* str, const char* format, ...);
extern int strcmp(const char*, const char*);
extern char* strcpy(char*, const char*);
extern char* strncpy(char* dst, const char* src, unsigned long n);
extern char* strstr(const char* str, const char* sub);
extern double tan(double arg);
extern int vsprintf(char* string, const char* format, va_list arg);
